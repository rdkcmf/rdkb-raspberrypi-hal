/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2021 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include <stdio.h>
#include "json_hal_server.h"
#include <pthread.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "json_rpc_common.h"
#include "ccsp_hal_ethsw.h"
#define ETH_LINK_EVENT "Device.Ethernet.X_RDK_Interface.1.Status"
#define LINK_UP "up"
#define LINK_DOWN "down"
#define ETH_JSON_CONF_PATH "/etc/rdk/conf/eth_manager_conf.json"

bool eth_event_started = FALSE;
int evt_subs_index = -1;
char schemaPath[256] = {'\0'};
#define  CcspHalEthSwTrace(msg)                     printf("%s - ", __FUNCTION__); printf msg;
#define  LINK_VALUE_SIZE 50
#define  ETH_INITIALIZE  "/tmp/ethagent_initialized"
/**
 * @brief global list to store event susbscriptions.
 */
static hal_subscribe_event_request_t gsubs_event_list[5];
/**
 * RPC callback handler - Event subsctiption.
 */
static int subs_event_cb(const json_object *inJson, int param_count, json_object *jreply);

appCallBack ethWanCallbacks;

void *ethsw_thread_main(void *context __attribute__((unused)))
{
   FILE *fp = NULL;
   char command[128] = {0};
   char* buff = NULL, *pLink;
   char previousLinkDetected[10]="no";
   int timeout = 0;
   int file = 0;

   buff = malloc(sizeof(char)*50);
   if(buff == NULL)
    {
        return (void *) 1;
    }

   while(timeout != 180)
    {
       if (file == access(ETH_INITIALIZE, R_OK))
       {
	    CcspHalEthSwTrace(("Eth agent initialized \n"));
	    break;
       }
       else
       {
	   timeout = timeout+1;
	   sleep(1);
       }
    }

   while(1)
    {
        memset(buff,0,sizeof(buff));
        snprintf(command,128, "ethtool erouter0 | grep \"Link detected\" | cut -d ':' -f2 | cut -d ' ' -f2");
        fp = popen(command, "r");
          if (fp == NULL)
          {
                continue;
          }
          while (fgets(buff, LINK_VALUE_SIZE, fp) != NULL)
          {
                pLink = strchr(buff, '\n');
                if(pLink)
                    *pLink = '\0';
          }
          pclose(fp);
        if (strcmp(buff, (const char *)previousLinkDetected))
	{
            if (strcmp(buff, "yes") == 0)
	    {
	        json_hal_server_publish_event(ETH_LINK_EVENT, LINK_UP);
	        LOGINFO("Link Up event sent to json-client");
            }
            else
	    {
	        json_hal_server_publish_event(ETH_LINK_EVENT, LINK_DOWN);
	        LOGINFO("Link Down event sent to json-client");
	    }
            memset(previousLinkDetected, 0, sizeof(previousLinkDetected));
            strcpy((char *)previousLinkDetected, buff);
        }
        sleep(5);
    }
    if(buff != NULL)
        free(buff);
    return NULL;
}

void GWP_RegisterEthWan_Callback(appCallBack *obj)
{
   if (obj != NULL)
   {
      ethWanCallbacks.pGWP_act_EthWanLinkUP = obj->pGWP_act_EthWanLinkUP;
      ethWanCallbacks.pGWP_act_EthWanLinkDown = obj->pGWP_act_EthWanLinkDown;
   }
   return;
}

/**
 * Signal handler.
 * Do cleanup and exit.
 */
static void sig_handler(int signo)
{
    if (signo == SIGUSR1 || signo == SIGKILL || signo == SIGINT)
    {
        int rc = RETURN_OK;
        rc = json_hal_server_terminate();
        assert(rc == RETURN_OK);
        LOGINFO("Cleanup done, exiting \n");
        exit(0);
    }
}


static int GWP_EthWanLinkDown_callback(){
   json_hal_server_publish_event(ETH_LINK_EVENT, LINK_DOWN);
   return 0;
}

static int GWP_EthWanLinkUp_callback(){
   json_hal_server_publish_event(ETH_LINK_EVENT, LINK_UP);
   return 0;
}

static int subs_event_cb(const json_object *jmsg, int param_count, json_object *jreply)
{
    if (jmsg == NULL || jreply == NULL)
    {
        LOGINFO("Invalid memory");
        return RETURN_ERR;
    }
    int ret = RETURN_OK;
    int param_index = 0;
    appCallBack *obj     =    NULL;
    hal_subscribe_event_request_t param_request;
    memset(&param_request, 0, sizeof(param_request));
    for (param_index = 0; param_index < param_count; param_index++)
    {
        /**
         * Unpack the request parameter and store it into param_request structure.
         */
        if (json_hal_get_subscribe_event_request(jmsg, param_index, &param_request) != RETURN_OK)
        {
            LOGINFO("Failed to get subscription data from the server");
            return RETURN_ERR;
        }
        /**
         * Store subscription data into global list.
         */
        evt_subs_index += 1;
        strncpy(gsubs_event_list[evt_subs_index].name, param_request.name, sizeof(gsubs_event_list[evt_subs_index].name));
        gsubs_event_list[evt_subs_index].type = param_request.type;
        LOGINFO("Subscribed [%s][%d]", gsubs_event_list[evt_subs_index].name, gsubs_event_list[evt_subs_index].type);
    }
        ret = json_hal_add_result_status(jreply, RESULT_SUCCESS);
        LOGINFO("[%s] Replly msg = %s", __FUNCTION__, json_object_to_json_string_ext(jreply, JSON_C_TO_STRING_PRETTY));

        if (!eth_event_started) {
             if (CcspHalEthSwInit() != RETURN_OK) {
                  LOGINFO("HAL INIT failed");
                  return RETURN_ERR;
              }
      obj = ( appCallBack* ) malloc(sizeof(appCallBack));
      obj->pGWP_act_EthWanLinkDown =  GWP_EthWanLinkDown_callback;
      obj->pGWP_act_EthWanLinkUP =  GWP_EthWanLinkUp_callback;
      GWP_RegisterEthWan_Callback(obj);
         }
    return ret;
}

int main(int argc, char **argv)
{
    int rc = RETURN_ERR;
    int ret_value = RETURN_ERR;
    pthread_t ethsw_tid;
    /**
     * RPC Server Initialisation.
     * Entry point for the server. Initialise the server software.
     */
    rc = json_hal_server_init(ETH_JSON_CONF_PATH);
    assert(rc == RETURN_OK);
    LOGINFO("Server init successful  \n");
    /**
     *  Register RPC methods and its callback function.
     *  Register all the supported methods and its action callback with rpc server library.
     */
    json_hal_server_register_action_callback("subscribeEvent", subs_event_cb);
    /**
     *  Started the server socket.
     *  Server socket created and listen for the client connections at this point.
     **/

    rc = json_hal_server_run();
    assert(rc == RETURN_OK);

    //TODO:Temporary fix for LinkState, Will need to revisit once the issue with driver is fixed.
    ret_value = pthread_create(&ethsw_tid, NULL, ethsw_thread_main, NULL);
    if (ret_value != 0)
	LOGINFO("Error in pthread_create");

    /* Signal Handling. */
    signal(SIGINT, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGUSR1, sig_handler);
    while (1)
    {
        sleep(5);
    }
    return 0;
}

