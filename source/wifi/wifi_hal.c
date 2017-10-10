/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
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

/**********************************************************************
   Copyright [2015] [Comcast, Corp.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/**********************************************************************

    module: wifi_hal.c

        For CCSP Component:  Wifi_Provisioning_and_management

    ---------------------------------------------------------------

    copyright:

        Comcast, Corp., 2015
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This sample implementation file gives the function call prototypes and 
        structure definitions used for the RDK-Broadband 
        Wifi hardware abstraction layer

     
    ---------------------------------------------------------------

    environment:

        This HAL layer is intended to support Wifi drivers 
        through an open API.  

    ---------------------------------------------------------------

    author:

        zhicheng_qiu@cable.comcast.com 

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "wifi_hal.h"

#ifndef AP_PREFIX
#define AP_PREFIX	"ath"
#endif

#ifndef RADIO_PREFIX
#define RADIO_PREFIX	"wifi"
#endif

#define MAX_BUF_SIZE 128
#define MAX_CMD_SIZE 1024

#define WIFI_DEBUG 

#ifdef WIFI_DEBUG
#define wifi_dbg_printf printf
#else
#define wifi_dbg_printf(format,args...) printf("")
#endif

#define HOSTAPD_CONF_0 "/nvram/hostapd0.conf"
#define HOSTAPD_CONF_1 "/nvram/hostapd1.conf"
#define DEF_HOSTAPD_CONF_0 "/usr/ccsp/wifi/hostapd0.conf"
#define DEF_HOSTAPD_CONF_1 "/usr/ccsp/wifi/hostapd1.conf"
#define DEF_RADIO_PARAM_CONF "/usr/ccsp/wifi/radio_param_def.cfg"

//wifi_setApBeaconRate(1, beaconRate);

int _syscmd(char *cmd, char *retBuf, int retBufSize)
{
    FILE *f;
    char *ptr = retBuf;
    int bufSize=retBufSize, bufbytes=0, readbytes=0;

    if((f = popen(cmd, "r")) == NULL) {
        fprintf(stderr,"\npopen %s error\n", cmd);
        return RETURN_ERR;
    }

    while(!feof(f))
    {
        *ptr = 0;
	if(bufSize>=128) {
	   bufbytes=128;
	} else {
	   bufbytes=bufSize-1;
	}
		
        fgets(ptr,bufbytes,f); 
	readbytes=strlen(ptr);
        if( readbytes== 0)        
            break;
        bufSize-=readbytes;
        ptr += readbytes;
    }
    pclose(f);
    retBuf[retBufSize-1]=0;
    return RETURN_OK;
}
static int writeBandWidth(int radioIndex,char *bw_value)
{
        char buf[MAX_BUF_SIZE];
        char cmd[MAX_CMD_SIZE];
        sprintf(cmd,"sed -i 's/^SET_BW%d=.*$/SET_BW%d=%s/' %s",radioIndex,radioIndex,bw_value,BW_FNAME);
        _syscmd(cmd,buf,sizeof(buf));
        return RETURN_OK;
}

static int readBandWidth(int radioIndex,char *bw_value)
{
        char buf[MAX_BUF_SIZE];
        char cmd[MAX_CMD_SIZE];
        sprintf(cmd,"grep 'SET_BW%d=' %s | sed 's/^.*=//'",radioIndex,BW_FNAME);
        _syscmd(cmd,buf,sizeof(buf));
        strcpy(bw_value,buf);
        return RETURN_OK;
}
/**************************************************************************/
/*! \fn void add_ifnames_in_bridge()
 **************************************************************************
 *  \brief This function add given interfaces in given bridges
 *  \param[in] bridge name,interface list seperated by comma',' or space' '
 *  \return void
 **************************************************************************/
static INT add_ifnames_in_bridge(char *bridge,char *ifnames_list)
{
    char *list = ifnames_list;
    char command[MAX_BUF_SIZE] = "";
    char out[MAX_BUF_SIZE] = "";
    char *temp;

    sprintf(command,"ifconfig %s",bridge);

    if(RETURN_ERR == _syscmd(command,out,MAX_BUF_SIZE))
    {
        return RETURN_ERR;
    }
    if(strlen(out) == 0)
    {
      fprintf(stderr,"\nbridge interface is not Ready!!!\n");
      return RETURN_ERR;
    }

    temp = strtok(list,", ");
    while(temp != NULL)
    {
       /*adding interface in bridge*/
        sprintf(command, "brctl addif %s %s", bridge,temp);
        system(command);
        temp = strtok(NULL,", ");
    }
    return RETURN_OK;
}

/**************************************************************************/
/*! \fn static INT list_add_param(param_list_t *list,struct params params)
 **************************************************************************
 *  \brief This function will add params in list and increment count
 *  \param[in] *list - pointer to list
 *  \param[in] param - parameter that we need to add in list
 *  \return (RETURN_OK/RETURN_ERR)
 **************************************************************************/
static INT list_add_param(param_list_t *list,struct params params)
{
    if(list->parameter_list==NULL)
    {

        if(list->parameter_list=(struct params *)calloc(1,(sizeof(struct params))))
        {
            list->count=1;
            memcpy(&(list->parameter_list[list->count-1]),&params,sizeof(params));
            wifi_dbg_printf("\n[%s]:inside calloc\n",__func__);
        }
        else
        {
            wifi_dbg_printf("\n[%s]:memmory allocation failed!!\n",__func__);
            return RETURN_ERR;
        }
    }
    else
    {
        if(list->parameter_list=(struct params *)realloc(list->parameter_list,((list->count +1) * sizeof(struct params))))
        {
            list->count = list->count + 1;
            memcpy(&(list->parameter_list[list->count-1]),&params,sizeof(params));
            wifi_dbg_printf("\n[%s]:inside realloc\n",__func__);
        }
        else
        {
            wifi_dbg_printf("\n[%s]:memmory allocation failed!!\n",__func__);
            return RETURN_ERR;
        }
    }
return RETURN_OK;
}

/**************************************************************************/
/*! \fn static void list_free_param(param_list_t *list)
 **************************************************************************
 *  \brief This function will free memory allocated by param list
 *  \param[in] *list - pointer to list
 *  \return none
 **************************************************************************/
static void list_free_param(param_list_t *list)
{
	if(list->parameter_list)
		free(list->parameter_list);
}
/**************************************************************************/
/*! \fn static INT get_param_value(char *parameter, char *output)
 **************************************************************************
 *  \brief This function will get parameter value for passed parameter name
 *  \param[in] parameter- name of parameter
 *  \param[out] value- value of passed parameter
 *  \return (RETURN_OK/RETURN_ERR)
 **************************************************************************/
static INT get_param_value(char *parameter, char *output)
{
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *param,*value;
    f = fopen(DEF_RADIO_PARAM_CONF, "r");
    if (f == NULL) {
        perror("fopen");
        wifi_dbg_printf("\n[%s]:Failed to open file %s\n",__func__,DEF_RADIO_PARAM_CONF);
        return RETURN_ERR;
    }
    while ((nread = getline(&line, &len, f)) != -1) {
        param = strtok(line,"=");
        value = strtok(NULL,"=");
        if( strcmp( parameter,param ) == 0 )
        {
            value[strlen(value)-1]='\0';
            strcpy(output,value);
        }
     }
     free(line);
     fclose(f);
    return RETURN_OK;
}

/**************************************************************************/
/*! \fn statis INT prepare_hostapd_conf()
 **************************************************************************
 *  \brief This function will prepare hostapd conf in nvram from default conf
 *  \return (RETURN_OK/RETURN_ERR)
 **************************************************************************/
static INT prepare_hostapd_conf()
{
	char cmd[128];
    /* check  /usr/ccsp/wifi/hostapd0.conf and /usr/ccsp/wifi/hostapd0.conf exists or not */
	if(( access(DEF_HOSTAPD_CONF_0, F_OK) != -1 ) && ( access(DEF_HOSTAPD_CONF_1, F_OK) != -1 ))
	{
		wifi_dbg_printf("\n[%s]: Default files %s and %s presents!!\n",__func__,DEF_HOSTAPD_CONF_0,DEF_HOSTAPD_CONF_1);
	}
	else
	{
		wifi_dbg_printf("\n[%s]: Default files %s and %s not presents!!\n",__func__,DEF_HOSTAPD_CONF_0,DEF_HOSTAPD_CONF_1);
		return RETURN_ERR;
	}
    /* check  /nvram/hostapd0.conf exists or not */
	if( access(HOSTAPD_CONF_0, F_OK) != -1 )
	{
		wifi_dbg_printf("\n[%s]: %s file allready exits!!\n",__func__,HOSTAPD_CONF_0);
	}
	else
	{
		wifi_dbg_printf("\n[%s]: %s file does not exits. Preparing from %s file\n",__func__,HOSTAPD_CONF_0,DEF_HOSTAPD_CONF_0);
		sprintf(cmd, "cp %s %s",DEF_HOSTAPD_CONF_0,HOSTAPD_CONF_0);
		system(cmd);
	}

    /* check  /nvram/hostapd1.conf exists or not */
	if( access(HOSTAPD_CONF_1, F_OK) != -1 )
	{
		wifi_dbg_printf("\n[%s]: %s file allready exits!!\n",__func__,HOSTAPD_CONF_1);
	}
	else
	{
		wifi_dbg_printf("\n[%s]: %s file does not exits. Preparing from %s file\n",__func__,HOSTAPD_CONF_1,DEF_HOSTAPD_CONF_1);
		sprintf(cmd, "cp %s %s",DEF_HOSTAPD_CONF_1,HOSTAPD_CONF_1);
		system(cmd);
	}
	return RETURN_OK;
}
void wifi_newApAssociatedDevice_callback_register(wifi_newApAssociatedDevice_callback callback_proc)
{
}

INT wifi_setApBeaconRate(INT radioIndex,CHAR *beaconRate)
{
	return 0;
}

INT wifi_getApBeaconRate(INT radioIndex, CHAR *beaconRate)
{
	return 0;
}

INT wifi_setLED(INT radioIndex, BOOL enable)
{
   return 0;
}

/**********************************************************************************
 *
 *  Wifi Subsystem level function prototypes 
 *
**********************************************************************************/
//---------------------------------------------------------------------------------------------------
//Wifi system api
//Get the wifi hal version in string, eg "2.0.0".  WIFI_HAL_MAJOR_VERSION.WIFI_HAL_MINOR_VERSION.WIFI_HAL_MAINTENANCE_VERSION
INT wifi_getHalVersion(CHAR *output_string)   //RDKB   
{
	snprintf(output_string, 64, "%d.%d.%d", WIFI_HAL_MAJOR_VERSION, WIFI_HAL_MINOR_VERSION, WIFI_HAL_MAINTENANCE_VERSION);
	return RETURN_OK;
}


/* wifi_factoryReset() function */
/**
* @description Clears internal variables to implement a factory reset of the Wi-Fi 
* subsystem. Resets Implementation specifics may dictate some functionality since different hardware implementations may have different requirements.
*
* @param None
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous
* @sideeffect None
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_factoryReset()
{
	char cmd[128];
	/*delete running hostapd conf files*/
	wifi_dbg_printf("\n[%s]: deleting hostapd conf file %s and %s",__func__,HOSTAPD_CONF_0,HOSTAPD_CONF_1);
	sprintf(cmd, "rm -rf %s %s",HOSTAPD_CONF_0,HOSTAPD_CONF_1);
	system(cmd);
	/*create new configuraion file from default configuration*/
	if(RETURN_ERR == prepare_hostapd_conf())
	{
		return RETURN_ERR;
	}
	return RETURN_OK;
}

/* wifi_factoryResetRadios() function */
/**
* @description Restore all radio parameters without touching access point parameters. Resets Implementation specifics may dictate some functionality since different hardware implementations may have different requirements.
*
* @param None
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous
*
* @sideeffect None
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_factoryResetRadios()
{	
	if((RETURN_OK == wifi_factoryResetRadio(0)) && (RETURN_OK == wifi_factoryResetRadio(1)))
	{
		return RETURN_OK;
	}
	return RETURN_ERR;
}


/* wifi_factoryResetRadio() function */
/**
* @description Restore selected radio parameters without touching access point parameters
*
* @param radioIndex - Index of Wi-Fi Radio channel
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_factoryResetRadio(int radioIndex) 	//RDKB
{
	char param_name[64];
	struct params params={0};
	param_list_t list;
	ULONG channel=0;
	char output[64];

	memset(&list,0,sizeof(list));

    sprintf(param_name,"BASIC_RATES_%d",radioIndex);
    if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"basic_rates");
		strcpy( params.value,output);
		if(RETURN_ERR == list_add_param(&list,params))
		{
			return RETURN_ERR;
		}
	}
    else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		return RETURN_ERR;
	}

    sprintf(param_name,"SUPPORTED_RATES_%d",radioIndex);
    if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"supported_rates");
		strcpy( params.value,output);
		if(RETURN_ERR == list_add_param(&list,params))
		{
			return RETURN_ERR;
		}

	}
    else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		return RETURN_ERR;
	}


	sprintf(param_name,"RADIO_CHANNEL_%d",radioIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"channel");
		strcpy( params.value,output);
		if(RETURN_ERR == list_add_param(&list,params))
		{
			return RETURN_ERR;
		}

	}
	else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		return RETURN_ERR;
	}

	sprintf(param_name,"CHANNEL_MODE_%d",radioIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"vht_oper_chwidth");
		if(strstr (output,"20") != NULL )
		{
			writeBandWidth(radioIndex,"20MHz");
			strcpy( params.value,"0");
		}
		else if (strstr (output,"40") != NULL)
		{	
			writeBandWidth(radioIndex,"40MHz");
			strcpy( params.value,"0");
		}
		else if (strstr (output,"80") != NULL)
		{
			strcpy( params.value,"1");
		}
                else if (strstr (output,"160") != NULL)
                {
                        strcpy( params.value,"2");
                }

		if(RETURN_ERR == list_add_param(&list,params))
		{
			list_free_param(&list);
			return RETURN_ERR;
		}
	}
	else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		list_free_param(&list);
		return RETURN_ERR;
	}
	wifi_hostapdWrite(radioIndex,&list);
	list_free_param(&list);

	return RETURN_OK;
}

/* wifi_initRadio() function */
/**
* Description: This function call initializes the specified radio.
*  Implementation specifics may dictate the functionality since 
*  different hardware implementations may have different initilization requirements.
* Parameters : radioIndex - The index of the radio. First radio is index 0. 2nd radio is index 1   - type INT
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_initRadio(INT radioIndex)
{
  //TODO: Initializes the wifi subsystem (for specified radio)

  return RETURN_OK;
}

// Initializes the wifi subsystem (all radios)
INT wifi_init()                            //RDKB
{
    char interface[MAX_BUF_SIZE]={'\0'};
    char bridge_name[MAX_BUF_SIZE]={'\0'};
    INT len=0;
	/* preparing hostapd configuration*/
	if(RETURN_ERR == prepare_hostapd_conf())
	{
		return RETURN_ERR;
	}

    if( ( RETURN_ERR == _syscmd("syscfg get lan_ifname",bridge_name,sizeof(bridge_name)) ) || 
        ( RETURN_ERR == _syscmd("iwconfig | grep -r \"IEEE 802.11\" | cut -d \" \" -f1 | tr '\n' ' '",interface,sizeof(interface)) ) )
    {
		return RETURN_ERR;
    }

    system("/usr/sbin/iw reg set US");
    system("systemctl start hostapd.service");
    sleep(2);//sleep to wait for hostapd to start

    if((strlen(bridge_name) > 0) && (strlen(interface) > 0) )
    {
          /* Removing '\n' from bridge_name because syscfg get lan_ifname returns bridge name terminating with '\n'*/
          len=strlen(bridge_name);
          if(bridge_name[len-1]=='\n')
          bridge_name[len-1]='\0';

          if(RETURN_ERR == add_ifnames_in_bridge(bridge_name,interface))
          {
              fprintf(stderr,"\nFailed to radio add interface in bridge\n");
              return RETURN_ERR;
          }
    }
    else
    {
        fprintf(stderr,"\n***Either bridge or Radio interfaces list are Empty***\n");
        return RETURN_ERR;
    }
    #ifdef USE_HOSTAPD_STRUCT
    read_hostapd_all_aps();
    #endif
    return RETURN_OK;
}

/* wifi_reset() function */
/**
* Description: Resets the Wifi subsystem.  This includes reset of all AP varibles.
*  Implementation specifics may dictate what is actualy reset since 
*  different hardware implementations may have different requirements.
* Parameters : None
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_reset()
{
	//TODO: resets the wifi subsystem, deletes all APs
  return RETURN_OK;
}

/* wifi_down() function */
/**
* @description Turns off transmit power for the entire Wifi subsystem, for all radios.
* Implementation specifics may dictate some functionality since 
* different hardware implementations may have different requirements.
*
* @param None
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous
* @sideeffect None
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_down()
{
	//TODO: turns off transmit power for the entire Wifi subsystem, for all radios
  return RETURN_OK;
}


/* wifi_createInitialConfigFiles() function */
/**
* @description This function creates wifi configuration files. The format
* and content of these files are implementation dependent.  This function call is 
* used to trigger this task if necessary. Some implementations may not need this 
* function. If an implementation does not need to create config files the function call can 
* do nothing and return RETURN_OK.
*
* @param None
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous
* @sideeffect None
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT wifi_createInitialConfigFiles()
{
	//TODO: creates initial implementation dependent configuration files that are later used for variable storage.  Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0)

	return RETURN_OK;
}

// outputs the country code to a max 64 character string
INT wifi_getRadioCountryCode(INT radioIndex, CHAR *output_string)
{
	if (NULL == output_string) {
		return RETURN_ERR;
	} else {
		snprintf(output_string, 64, "841");
		return RETURN_OK;
	}
}

INT wifi_setRadioCountryCode(INT radioIndex, CHAR *CountryCode)
{
	//Set wifi config. Wait for wifi reset to apply
	return RETURN_OK;
}

/**********************************************************************************
 *
 *  Wifi radio level function prototypes
 *
**********************************************************************************/

//Get the total number of radios in this wifi subsystem
INT wifi_getRadioNumberOfEntries(ULONG *output) //Tr181
{
	*output=2;
	return RETURN_OK;
}

//Get the total number of SSID entries in this wifi subsystem 
INT wifi_getSSIDNumberOfEntries(ULONG *output) //Tr181
{
	*output=2;
	return RETURN_OK;
}

//Get the Radio enable config parameter
INT wifi_getRadioEnable(INT radioIndex, BOOL *output_bool)	//RDKB
{
    char cmd[MAX_CMD_SIZE]={'\0'};
    char buf[MAX_BUF_SIZE]={'\0'};
    INT  wlanIndex;

    if( 0 == radioIndex ) // For 2.4 GHz
        wlanIndex = wifi_getApIndexForWiFiBand(band_2_4);
    else
        wlanIndex = wifi_getApIndexForWiFiBand(band_5);

    if (NULL == output_bool) 
    {
        return RETURN_ERR;
    } else {
        sprintf(cmd,"ifconfig|grep wlan%d", wlanIndex);
        _syscmd(cmd,buf,sizeof(buf));
        if(strlen(buf)>0)
            *output_bool=1;
        else
            *output_bool=0;
        return RETURN_OK;
    }
}

//Set the Radio enable config parameter
INT wifi_setRadioEnable(INT radioIndex, BOOL enable)		//RDKB
{
    char cmd[MAX_CMD_SIZE]={'\0'};
    char buf[MAX_BUF_SIZE]={'\0'};
    INT  wlanIndex;

    if( 0 == radioIndex ) // For 2.4 GHz
        wlanIndex = wifi_getApIndexForWiFiBand(band_2_4);
    else
        wlanIndex = wifi_getApIndexForWiFiBand(band_5);

    if(enable == TRUE)
        sprintf(cmd,"ifconfig wlan%d up", wlanIndex);
    else
        sprintf(cmd,"ifconfig wlan%d down", wlanIndex);
    
    wifi_dbg_printf("\ncmd=%s",cmd);
    _syscmd(cmd,buf,sizeof(buf));
    //Set wifi config. Wait for wifi reset to apply
    return RETURN_OK;
}

//Get the Radio enable status
INT wifi_getRadioStatus(INT radioIndex, BOOL *output_bool)	//RDKB
{
    INT  retValue;
    INT  wlanIndex;

    if (NULL == output_bool) {
        return RETURN_ERR;
    } else {
        retValue = wifi_getRadioEnable(wlanIndex, output_bool);
        return retValue;
    }
}

//Get the Radio Interface name from platform, eg "wifi0"
INT wifi_getRadioIfName(INT radioIndex, CHAR *output_string) //Tr181
{
	if (NULL == output_string) 
		return RETURN_ERR;
	snprintf(output_string, 64, "%s%d", RADIO_PREFIX, radioIndex);
	return RETURN_OK;
}

//Get the maximum PHY bit rate supported by this interface. eg: "216.7 Mb/s", "1.3 Gb/s"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioMaxBitRate(INT radioIndex, CHAR *output_string)	//RDKB
{
	char cmd[64];
    char buf[1024];
	int apIndex;
	
    if (NULL == output_string) 
		return RETURN_ERR;
	
    apIndex=(radioIndex==0)?0:1;

    snprintf(cmd, sizeof(cmd), "iwconfig %s%d | grep \"Bit Rate\" | cut -d':' -f2 | cut -d' ' -f1,2", AP_PREFIX, apIndex);
    _syscmd(cmd,buf, sizeof(buf));

    snprintf(output_string, 64, "%s", buf);
	
	return RETURN_OK;
}


//Get Supported frequency bands at which the radio can operate. eg: "2.4GHz,5GHz"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioSupportedFrequencyBands(INT radioIndex, CHAR *output_string)	//RDKB
{
        char buf[MAX_BUF_SIZE]={'\0'};
        char str[MAX_BUF_SIZE]={'\0'};
        char cmd[MAX_CMD_SIZE]={'\0'};
        char *ch=NULL;
        char *ch2=NULL;

		sprintf(cmd,"grep 'channel=' %s%d.conf",HOSTAPD_FNAME,radioIndex);
        
   		if(_syscmd(cmd,buf,sizeof(buf)) == RETURN_ERR)
	    {
    	    printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
	        return RETURN_ERR;
	    }
        ch=strchr(buf,'\n');
        *ch='\0';
        ch=strchr(buf,'=');
        if(ch==NULL)
          return RETURN_ERR;

	
        ch++;
		sprintf(cmd,"grep 'interface=' %s%d.conf",HOSTAPD_FNAME,radioIndex);

        if(_syscmd(cmd,str,64) ==  RETURN_ERR)
        {
                wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
                return RETURN_ERR;
		}

		
		ch2=strchr(str,'\n');
		//replace \n with \0
		*ch2='\0';
        ch2=strchr(str,'=');
        if(ch2==NULL)
        {
        	wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
       		return RETURN_ERR;
        }
        else
         wifi_dbg_printf("%s",ch2+1);

		
        ch2++;


        sprintf(cmd,"iwlist %s frequency|grep 'Channel %s'",ch2,ch);

        memset(buf,'\0',sizeof(buf));
        if(_syscmd(cmd,buf,sizeof(buf))==RETURN_ERR)
		{
			wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
			return RETURN_ERR;
		}
		if (strstr(buf,"2.4") != NULL )
			strcpy(output_string,"2.4GHz");
		else if(strstr(buf,"5.") != NULL )
			strcpy(output_string,"5GHz");
		return RETURN_OK;
}

//Get the frequency band at which the radio is operating, eg: "2.4GHz"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioOperatingFrequencyBand(INT radioIndex, CHAR *output_string) //Tr181
{
	
        char buf[MAX_BUF_SIZE]={'\0'};
        char str[MAX_BUF_SIZE]={'\0'};
        char cmd[MAX_CMD_SIZE]={'\0'};
        char *ch=NULL;
        char *ch2=NULL;

		sprintf(cmd,"grep 'channel=' %s%d.conf",HOSTAPD_FNAME,radioIndex);
        
   		if(_syscmd(cmd,buf,sizeof(buf)) == RETURN_ERR)
	    {
    	    printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
	        return RETURN_ERR;
	    }
        ch=strchr(buf,'\n');
        *ch='\0';
        ch=strchr(buf,'=');
        if(ch==NULL)
          return RETURN_ERR;

	
        ch++;
		sprintf(cmd,"grep 'interface=' %s%d.conf",HOSTAPD_FNAME,radioIndex);

        if(_syscmd(cmd,str,64) ==  RETURN_ERR)
        {
                wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
                return RETURN_ERR;
		}

		
		ch2=strchr(str,'\n');
		//replace \n with \0
		*ch2='\0';
        ch2=strchr(str,'=');
        if(ch2==NULL)
        {
        	wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
       		return RETURN_ERR;
        }
        else
         wifi_dbg_printf("%s",ch2+1);

		
        ch2++;


        sprintf(cmd,"iwlist %s frequency|grep 'Channel %s'",ch2,ch);

        memset(buf,'\0',sizeof(buf));
        if(_syscmd(cmd,buf,sizeof(buf))==RETURN_ERR)
		{
			wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
			return RETURN_ERR;
		}

		strcpy(output_string,buf);
		return RETURN_OK;
}

//Get the Supported Radio Mode. eg: "b,g,n"; "n,ac"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioSupportedStandards(INT radioIndex, CHAR *output_string) //Tr181
{
	if (NULL == output_string) 
		return RETURN_ERR;
	snprintf(output_string, 64, (radioIndex==0)?"b,g,n":"n,ac");
	return RETURN_OK;
}

//Get the radio operating mode, and pure mode flag. eg: "ac"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioStandard(INT radioIndex, CHAR *output_string, BOOL *gOnly, BOOL *nOnly, BOOL *acOnly)	//RDKB
{
    struct params params={"hw_mode",""};
    if ((NULL == output_string) && (NULL == gOnly) && (NULL == nOnly) && (NULL == acOnly)) 
        return RETURN_ERR;
    
    memset(output_string,'\0',4);
    wifi_hostapdRead(radioIndex,&params,output_string);

    wifi_dbg_printf("\noutput_string=%s\n",output_string);
    if (NULL == output_string) 
    {
        wifi_dbg_printf("\nwifi_hostapdRead returned NULL\n");
        return RETURN_ERR;
    }
    if(strcmp(output_string,"g")==0)
    {
        wifi_dbg_printf("\nG\n");
        *gOnly=TRUE;
        *nOnly=FALSE;
        *acOnly=FALSE;
    }
    else if(strcmp(output_string,"n")==0)
    {
        wifi_dbg_printf("\nN\n");
        *gOnly=FALSE;
        *nOnly=TRUE;
        *acOnly=FALSE;
    }
    else if(strcmp(output_string,"ac")==0)
    {
        wifi_dbg_printf("\nReturning from getRadioStandard\n");
        *gOnly=FALSE;
        *nOnly=FALSE;
        *acOnly=TRUE;
    }
    else
        wifi_dbg_printf("\nInvalid Mode\n");

        wifi_dbg_printf("\nReturning from getRadioStandard\n");
#if 0
    if(radioIndex==0) {
        snprintf(output_string, 64, "n");        //"ht" needs to be translated to "n" or others
        *gOnly=FALSE;
        *nOnly=TRUE;
        *acOnly=FALSE;
    } else {
        snprintf(output_string, 64, "ac");        //"vht" needs to be translated to "ac"
        *gOnly=FALSE;
        *nOnly=FALSE;
        *acOnly=FALSE;    
    }
#endif
return RETURN_OK;

}

//Set the radio operating mode, and pure mode flag. 
INT wifi_setRadioChannelMode(INT radioIndex, CHAR *channelMode, BOOL gOnlyFlag, BOOL nOnlyFlag, BOOL acOnlyFlag)	//RDKB
{
          
        if (strcmp (channelMode,"11A") == 0)
        {
		writeBandWidth(radioIndex,"20MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11a (5GHz)\n");
        }
        else if (strcmp (channelMode,"11NAHT20") == 0)
        {
                writeBandWidth(radioIndex,"20MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11n-20MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11NAHT40PLUS") == 0)
        {
                writeBandWidth(radioIndex,"40MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11n-40MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11NAHT40MINUS") == 0)
        {
                writeBandWidth(radioIndex,"40MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11n-40MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11ACVHT20") == 0)
        {
                writeBandWidth(radioIndex,"20MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11ac-20MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11ACVHT40PLUS") == 0)
        {
                writeBandWidth(radioIndex,"40MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11ac-40MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11ACVHT40MINUS") == 0)
        {
                writeBandWidth(radioIndex,"40MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11ac-40MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11ACVHT80") == 0)
        {
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"1");
                printf("\nChannel Mode is 802.11ac-80MHz(5GHz)\n");
        }
        else if (strcmp (channelMode,"11ACVHT160") == 0)
        {
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"2");
                printf("\nChannel Mode is 802.11ac-160MHz(5GHz)\n");
        }      
        else if (strcmp (channelMode,"11B") == 0)
	{
 	        writeBandWidth(radioIndex,"20MHz");
          	wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
		printf("\nChannel Mode is 802.11b(2.4GHz)\n");
	}
	else if (strcmp (channelMode,"11G") == 0)
	{
 	        writeBandWidth(radioIndex,"20MHz");
          	wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
		printf("\nChannel Mode is 802.11g(2.4GHz)\n");
	}
	else if (strcmp (channelMode,"11NGHT20") == 0)
	{
 		writeBandWidth(radioIndex,"20MHz");
 		wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
		printf("\nChannel Mode is 802.11n-20MHz(2.4GHz)\n");
	}
        else if (strcmp (channelMode,"11NGHT40PLUS") == 0)
        {
                writeBandWidth(radioIndex,"40MHz");
	        wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11n-40MHz(2.4GHz)\n");
        }
        else if (strcmp (channelMode,"11NGHT40MINUS") == 0)
        {
                writeBandWidth(radioIndex,"40MHz");
                wifi_setRadioOperatingChannelBandwidth(radioIndex,"0");
                printf("\nChannel Mode is 802.11n-40MHz(2.4GHz)\n");
        }
        else 
        {
                return RETURN_ERR;
        }

	return RETURN_OK;
}


//Get the list of supported channel. eg: "1-11"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioPossibleChannels(INT radioIndex, CHAR *output_string)	//RDKB
{
	if (NULL == output_string) 
		return RETURN_ERR;
	snprintf(output_string, 64, (radioIndex==0)?"1,2,3,4,6,7,8,9,10,11":"36,40");
	return RETURN_OK;
}

//Get the list for used channel. eg: "1,6,9,11"
//The output_string is a max length 256 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioChannelsInUse(INT radioIndex, CHAR *output_string)	//RDKB
{
    if (NULL == output_string)
       return RETURN_ERR;

	snprintf(output_string, 256, (radioIndex==0)?"1,6,11":"36,40");
	return RETURN_OK;
}

//Get the running channel number 
INT wifi_getRadioChannel(INT radioIndex,ULONG *output_ulong)	//RDKB
{
  struct params params={"channel",""};
  char output[3]={'\0'};
  
  wifi_hostapdRead(radioIndex,&params,output);
  if(output!=NULL)
  {
    *output_ulong=atol(output);
  }
  wifi_dbg_printf("\n*output_long=%ld output from hal=%s\n",*output_ulong,output);
  
  return RETURN_OK;
}

//Set the running channel number 
INT wifi_setRadioChannel(INT radioIndex, ULONG channel)	//RDKB	//AP only
{
  struct params params={'\0'};
  char str_channel[3]={'\0'};
  param_list_t list;
  strncpy(params.name,"channel",strlen("channel"));
  sprintf(str_channel,"%d",channel);
  strncpy(params.value,str_channel,strlen(str_channel));
  memset(&list,0,sizeof(list));
  if(RETURN_ERR == list_add_param(&list,params))
  {
     return RETURN_ERR;
  }
  wifi_hostapdWrite(radioIndex,&list);
  list_free_param(&list);

  //Set to wifi config only. Wait for wifi reset or wifi_pushRadioChannel to apply.
  return RETURN_OK;
}

//Enables or disables a driver level variable to indicate if auto channel selection is enabled on this radio
//This "auto channel" means the auto channel selection when radio is up. (which is different from the dynamic channel/frequency selection (DFC/DCS))
INT wifi_setRadioAutoChannelEnable(INT radioIndex, BOOL enable) //RDKB
{
	//Set to wifi config only. Wait for wifi reset to apply.
	return RETURN_ERR;
}

INT wifi_getRadioDCSSupported(INT radioIndex, BOOL *output_bool) 	//RDKB
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=FALSE;
	return RETURN_OK;
}

INT wifi_getRadioDCSEnable(INT radioIndex, BOOL *output_bool)		//RDKB
{	
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=FALSE;
	return RETURN_OK;
}

INT wifi_setRadioDCSEnable(INT radioIndex, BOOL enable)            //RDKB
{    
    //Set to wifi config only. Wait for wifi reset to apply.
    return RETURN_OK;
}

INT wifi_setApEnableOnLine(ULONG wlanIndex,BOOL enable)
{
   return RETURN_OK;
}

INT wifi_factoryResetAP(int apIndex)
{
	char param_name[64];
	ULONG channel=0;
	char output[64];
	char beaconType[64];
	struct params params={0};
	param_list_t list;

	memset(&list,0,sizeof(list));
	sprintf(param_name,"SSID_NAME_%d",apIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"ssid");
		strcpy( params.value,output);
		if(RETURN_ERR == list_add_param(&list,params))
		{
			return RETURN_ERR;
		}
	}
	else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		return RETURN_ERR;
	}

	sprintf(param_name,"PRESHAREDKEY_%d",apIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"wpa_passphrase");
		strcpy( params.value,output);
		if(RETURN_ERR==list_add_param(&list,params))
		{
			list_free_param(&list);
			return RETURN_ERR;
		}
	}
	else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		list_free_param(&list);
		return RETURN_ERR;
	}

	sprintf(param_name,"BEACONTYPE_%d",apIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy( params.name,"beaconType");
		strcpy( params.value,output);
		if(RETURN_ERR == list_add_param(&list,params))
		{
			list_free_param(&list);
			return RETURN_ERR;
		}
		strcpy(beaconType,output);
	}
	else
	{
		list_free_param(&list);
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		return RETURN_ERR;
	}
	sprintf(param_name,"WPAENCRYPTIONMODE_%d",apIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		if ( strcmp(output, "TKIPEncryption") == 0)
		{
			strcpy(params.value, "TKIP");
		} else if ( strcmp(output,"AESEncryption") == 0)
		{
			strcpy(params.value, "CCMP");
		} else if (strcmp(output,"TKIPandAESEncryption") == 0)
		{
			strcpy(params.value,"TKIP CCMP");
		}

		if(strcmp(beaconType,"WPAand11i") == 0)
		{
			strcpy(params.name,"wpa_pairwise");
			if(RETURN_ERR == list_add_param(&list,params))
			{
				list_free_param(&list);
				return RETURN_ERR;
			}
			strcpy(params.name,"rsn_pairwise");
			if(RETURN_ERR == list_add_param(&list,params))
			{
				list_free_param(&list);
				return RETURN_ERR;
			}
		}
		else if(strcmp(beaconType,"11i") == 0)
		{
			strcpy(params.name,"rsn_pairwise");
			if(RETURN_ERR == list_add_param(&list,params))
			{
				list_free_param(&list);
				return RETURN_ERR;
			}
		}
		else if(strcmp(beaconType,"WPA") == 0)
		{
			strcpy(params.name,"wpa_pairwise");
			if(RETURN_ERR == list_add_param(&list,params))
			{
				list_free_param(&list);
				return RETURN_ERR;
			}
		}
		else
		{
			wifi_dbg_printf("\n[%s]:Invalid beaconType\n",__func__);
			list_free_param(&list);
			return RETURN_ERR;
		}
	}
	else
	{
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		list_free_param(&list);
		return RETURN_ERR;
	}
	sprintf(param_name,"AUTHENTICATIONMODE_%d",apIndex);
	if(get_param_value(param_name,output)==0)
	{
		wifi_dbg_printf("\n[%s]:%s=%s\n",__func__,param_name,output);
		strcpy(params.name,"wpa_key_mgmt");
		if(strcmp(output,"PSKAuthentication") == 0)
		{
			strcpy(params.value,"WPA-PSK");
			if(RETURN_ERR == list_add_param(&list,params))
			{
				list_free_param(&list);
				return RETURN_ERR;
			}
		}
		else if(strcmp(output,"EAPAuthentication") == 0)
		{
			strcpy(params.value,"WPA-EAP");
			if(RETURN_ERR == list_add_param(&list,params))
			{
				list_free_param(&list);
				return RETURN_ERR;
			}
		}
	}
	else
	{
		list_free_param(&list);
		wifi_dbg_printf("\n[%s]:Failed to get parameter %s\n",__func__,param_name);
		return RETURN_ERR;
	}

	wifi_hostapdWrite(apIndex,&list);
	list_free_param(&list);
	return RETURN_OK;
}

INT wifi_setApDTIMInterval(INT apIndex, INT dtimInterval)
{
   return RETURN_OK;
}

//Check if the driver support the Dfs
INT wifi_getRadioDfsSupport(INT radioIndex, BOOL *output_bool) //Tr181
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=FALSE;	
	return RETURN_OK;
}

//The output_string is a max length 256 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
//The value of this parameter is a comma seperated list of channel number
INT wifi_getRadioDCSChannelPool(INT radioIndex, CHAR *output_pool)			//RDKB
{
	if (NULL == output_pool) 
		return RETURN_ERR;
	snprintf(output_pool, 256, "1,2,3,4,5,6,7,8,9,10,11");
	return RETURN_OK;
}

INT wifi_setRadioDCSChannelPool(INT radioIndex, CHAR *pool)			//RDKB
{
	//Set to wifi config. And apply instantly.
	return RETURN_OK;
}

INT wifi_getRadioDCSScanTime(INT radioIndex, INT *output_interval_seconds, INT *output_dwell_milliseconds)
{
	if (NULL == output_interval_seconds || NULL == output_dwell_milliseconds) 
		return RETURN_ERR;
	*output_interval_seconds=1800;
	*output_dwell_milliseconds=40;
	return RETURN_OK;
}

INT wifi_setRadioDCSScanTime(INT radioIndex, INT interval_seconds, INT dwell_milliseconds)
{
	//Set to wifi config. And apply instantly.
	return RETURN_OK;
}

//Get the Dfs enable status
INT wifi_getRadioDfsEnable(INT radioIndex, BOOL *output_bool)	//Tr181
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=FALSE;	
	return RETURN_OK;
}

//Set the Dfs enable status
INT wifi_setRadioDfsEnable(INT radioIndex, BOOL enable)	//Tr181
{
	return RETURN_ERR;
}

//Check if the driver support the AutoChannelRefreshPeriod
INT wifi_getRadioAutoChannelRefreshPeriodSupported(INT radioIndex, BOOL *output_bool) //Tr181
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=FALSE;		//not support
	return RETURN_OK;
}

//Get the ACS refresh period in seconds
INT wifi_getRadioAutoChannelRefreshPeriod(INT radioIndex, ULONG *output_ulong) //Tr181
{
	if (NULL == output_ulong) 
		return RETURN_ERR;
	*output_ulong=300;	
	return RETURN_OK;
}

//Set the ACS refresh period in seconds
INT wifi_setRadioDfsRefreshPeriod(INT radioIndex, ULONG seconds) //Tr181
{
	return RETURN_ERR;
}

//Get the Operating Channel Bandwidth. eg "20MHz", "40MHz", "80MHz", "80+80", "160"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioOperatingChannelBandwidth(INT radioIndex, CHAR *output_string) //Tr181
{
	struct params params={"vht_oper_chwidth",""};
	char output_buf[8]={0};
        char bw_value[10];	
	wifi_hostapdRead(radioIndex,&params,output_buf);
	readBandWidth(radioIndex,bw_value);
	if (NULL == output_string) 
			return RETURN_ERR;
        
	if(strstr (output_buf,"0") != NULL )
	{
		strcpy(output_string,bw_value);
	}
	else if (strstr (output_buf,"1") != NULL)
	{
		strcpy(output_string,"80MHz");
	}
	else if (strstr (output_buf,"2") != NULL)
	{
		strcpy(output_string,"160MHz");
	}
	else if (strstr (output_buf,"3") != NULL)
	{
		strcpy(output_string,"80+80");
	}
        else
        {
                strcpy(output_string,"Auto");
        }

	return RETURN_OK;
}

//Set the Operating Channel Bandwidth.
INT wifi_setRadioOperatingChannelBandwidth(INT radioIndex, CHAR *output_string) //Tr181	//AP only
{
  
  	struct params params={'\0'};
 	param_list_t list;
  	strcpy(params.name,"vht_oper_chwidth");
  	if (NULL == output_string)
         	return RETURN_ERR;
       	
        else{
        strncpy(params.value,output_string,1);
        }
  	wifi_dbg_printf("\n%s:",__func__);
  	wifi_dbg_printf(params.value);
  	memset(&list,0,sizeof(list));
  	if(RETURN_ERR == list_add_param(&list,params))
  	{
		return RETURN_ERR;
  	}
  	wifi_hostapdWrite(radioIndex,&list);
  	list_free_param(&list);
  	return RETURN_OK;
}

//Get the secondary extension channel position, "AboveControlChannel" or "BelowControlChannel". (this is for 40MHz and 80MHz bandwith only)
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioExtChannel(INT radioIndex, CHAR *output_string) //Tr181
{
	if (NULL == output_string) 
		return RETURN_ERR;
	snprintf(output_string, 64, (radioIndex==0)?"":"BelowControlChannel");
	return RETURN_OK;
}

//Set the extension channel.
INT wifi_setRadioExtChannel(INT radioIndex, CHAR *string) //Tr181	//AP only
{
	return RETURN_ERR;
}

//Get the guard interval value. eg "400nsec" or "800nsec" 
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioGuardInterval(INT radioIndex, CHAR *output_string)	//Tr181
{
	//save config and apply instantly
	if (NULL == output_string) 
		return RETURN_ERR;
	snprintf(output_string, 64, (radioIndex==0)?"400nsec":"400nsec");
	return RETURN_OK;
}

//Set the guard interval value. 
INT wifi_setRadioGuardInterval(INT radioIndex, CHAR *string)	//Tr181
{
	//Apply setting instantly
	return RETURN_ERR;
}

//Get the Modulation Coding Scheme index, eg: "-1", "1", "15"
INT wifi_getRadioMCS(INT radioIndex, INT *output_int) //Tr181
{
	if (NULL == output_int) 
		return RETURN_ERR;
	if (radioIndex==0)	
		*output_int=1;
	else
		*output_int=3;
	return RETURN_OK;
}

//Set the Modulation Coding Scheme index
INT wifi_setRadioMCS(INT radioIndex, INT MCS) //Tr181
{
	return RETURN_ERR;
}



//Get current Transmit Power, eg "75", "100"
//The transmite power level is in units of full power for this radio.
INT wifi_getRadioTransmitPower(INT radioIndex, ULONG *output_ulong)	//RDKB
{
	char cmd[128]={0};
	char buf[256]={0};
	INT apIndex;
	//save config and apply instantly
	
	if (NULL == output_ulong) 
		return RETURN_ERR;
	
	//zqiu:TODO:save config
	apIndex=(radioIndex==0)?0:1;
	
	snprintf(cmd, sizeof(cmd),  "iwlist %s%d txpower | grep Tx-Power | cut -d'=' -f2", AP_PREFIX, index);
	_syscmd(cmd, buf, sizeof(buf));
	*output_ulong = atol(buf);
	
	return RETURN_OK;
}

//Set Transmit Power
//The transmite power level is in units of full power for this radio.
INT wifi_setRadioTransmitPower(INT radioIndex, ULONG TransmitPower)	//RDKB
{
	char cmd[128]={0};
	char buf[256]={0};
	INT apIndex;	
		
	apIndex=(radioIndex==0)?0:1;
	
	snprintf(cmd, sizeof(cmd),  "iwconfig %s%d txpower %lu", AP_PREFIX, apIndex, TransmitPower);
	_syscmd(cmd, buf, sizeof(buf));	
	
	return RETURN_OK;
}

//get 80211h Supported.  80211h solves interference with satellites and radar using the same 5 GHz frequency band
INT wifi_getRadioIEEE80211hSupported(INT radioIndex, BOOL *Supported)  //Tr181
{
	if (NULL == Supported) 
		return RETURN_ERR;
	*Supported=FALSE;	
	return RETURN_OK;
}

//Get 80211h feature enable
INT wifi_getRadioIEEE80211hEnabled(INT radioIndex, BOOL *enable) //Tr181
{
	if (NULL == enable) 
		return RETURN_ERR;
	*enable=FALSE;	
	return RETURN_OK;
}

//Set 80211h feature enable
INT wifi_setRadioIEEE80211hEnabled(INT radioIndex, BOOL enable)  //Tr181
{
	return RETURN_ERR;
}

//Indicates the Carrier Sense ranges supported by the radio. It is measured in dBm. Refer section A.2.3.2 of CableLabs Wi-Fi MGMT Specification.
INT wifi_getRadioCarrierSenseThresholdRange(INT radioIndex, INT *output)  //P3
{
	if (NULL == output) 
		return RETURN_ERR;
	*output=100;	
	return RETURN_OK;
}

//The RSSI signal level at which CS/CCA detects a busy condition. This attribute enables APs to increase minimum sensitivity to avoid detecting busy condition from multiple/weak Wi-Fi sources in dense Wi-Fi environments. It is measured in dBm. Refer section A.2.3.2 of CableLabs Wi-Fi MGMT Specification.
INT wifi_getRadioCarrierSenseThresholdInUse(INT radioIndex, INT *output)	//P3
{
	if (NULL == output) 
		return RETURN_ERR;
	*output=-99;	
	return RETURN_OK;
}

INT wifi_setRadioCarrierSenseThresholdInUse(INT radioIndex, INT threshold)	//P3
{
	return RETURN_ERR;
}


//Time interval between transmitting beacons (expressed in milliseconds). This parameter is based ondot11BeaconPeriod from [802.11-2012].
INT wifi_getRadioBeaconPeriod(INT radioIndex, UINT *output)
{
	if (NULL == output) 
		return RETURN_ERR;
	*output=100;	
	return RETURN_OK;
}
 
INT wifi_setRadioBeaconPeriod(INT radioIndex, UINT BeaconPeriod)
{
	return RETURN_ERR;
}

//Comma-separated list of strings. The set of data rates, in Mbps, that have to be supported by all stations that desire to join this BSS. The stations have to be able to receive and transmit at each of the data rates listed inBasicDataTransmitRates. For example, a value of "1,2", indicates that stations support 1 Mbps and 2 Mbps. Most control packets use a data rate in BasicDataTransmitRates.	
INT wifi_getRadioBasicDataTransmitRates(INT radioIndex, CHAR *output)
{
	char *temp;
	char temp_output[128];
	char temp_TransmitRates[512];
	struct params params={"basic_rates",""};

	if (NULL == output)
		return RETURN_ERR;

	wifi_hostapdRead(radioIndex,&params,output);

	strcpy(temp_TransmitRates,output);
	strcpy(temp_output,"");
	temp = strtok(temp_TransmitRates," ");
	while(temp!=NULL)
	{
		temp[strlen(temp)-1]=0;
		if((temp[0]=='5') && (temp[1]=='\0'))
		{
			temp="5.5";
		}
		strcat(temp_output,temp);
		temp = strtok(NULL," ");
		if(temp!=NULL)
		{
			strcat(temp_output,",");
		}
	}
	strcpy(output,temp_output);
	return RETURN_OK;
}

INT wifi_setRadioBasicDataTransmitRates(INT radioIndex, CHAR *TransmitRates)
{
	int i=0;
	char *temp;
	char temp1[128];
	char temp_output[128];
	char temp_TransmitRates[128];

        if(NULL == TransmitRates)
            return RETURN_ERR;

	strcpy(temp_TransmitRates,TransmitRates);
	for(i=0;i<strlen(temp_TransmitRates);i++)
	{
		//if (((temp_TransmitRates[i]>=48) && (temp_TransmitRates[i]<=57)) | (temp_TransmitRates[i]==32))
		if (((temp_TransmitRates[i]>='0') && (temp_TransmitRates[i]<='9')) | (temp_TransmitRates[i]==' ') | (temp_TransmitRates[i]=='.'))
		{
			continue;
		}
		else
		{
			return RETURN_ERR;
		}
	}

	strcpy(temp_output,"");
	temp = strtok(temp_TransmitRates," ");
	while(temp!=NULL)
	{
		strcpy(temp1,temp);
		if(radioIndex==1)
		{
			if((strcmp(temp,"1")==0) | (strcmp(temp,"2")==0) | (strcmp(temp,"5.5")==0))
			{
				return RETURN_ERR;
			}
		}

		if(strcmp(temp,"5.5")==0)
		{
			strcpy(temp1,"55");
		}
		else
		{
			strcat(temp1,"0");
		}
		strcat(temp_output,temp1);
		temp = strtok(NULL," ");
		if(temp!=NULL)
		{
			strcat(temp_output," ");
		}
	}
	strcpy(TransmitRates,temp_output);
	char buf[127]={'\0'};
	struct params params={'\0'};
	param_list_t list;
	strcpy(params.name,"basic_rates");
	strncpy(params.value,TransmitRates,strlen(TransmitRates));
	wifi_dbg_printf("\n%s:",__func__);
	wifi_dbg_printf("\nparams.value=%s\n",params.value);
	wifi_dbg_printf("\n******************Transmit rates=%s\n",TransmitRates);

	memset(&list,0,sizeof(list));
	if(RETURN_ERR == list_add_param(&list,params))
	{
		return RETURN_ERR;
	}
	wifi_hostapdWrite(radioIndex,&list);
	list_free_param(&list);
	return RETURN_OK;
}



//Get detail radio traffic static info
INT wifi_getRadioTrafficStats2(INT radioIndex, wifi_radioTrafficStats2_t *output_struct) //Tr181
{
	if (NULL == output_struct) 
		return RETURN_ERR;
		
	//ifconfig radio_x	
	output_struct->radio_BytesSent=250;	//The total number of bytes transmitted out of the interface, including framing characters.
	output_struct->radio_BytesReceived=168;	//The total number of bytes received on the interface, including framing characters.
	output_struct->radio_PacketsSent=25;	//The total number of packets transmitted out of the interface.
	output_struct->radio_PacketsReceived=20; //The total number of packets received on the interface.

	output_struct->radio_ErrorsSent=0;	//The total number of outbound packets that could not be transmitted because of errors.
	output_struct->radio_ErrorsReceived=0;    //The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol.
	output_struct->radio_DiscardPacketsSent=0; //The total number of outbound packets which were chosen to be discarded even though no errors had been detected to prevent their being transmitted. One possible reason for discarding such a packet could be to free up buffer space.
	output_struct->radio_DiscardPacketsReceived=0; //The total number of inbound packets which were chosen to be discarded even though no errors had been detected to prevent their being delivered. One possible reason for discarding such a packet could be to free up buffer space.
	output_struct->radio_PLCPErrorCount=0;	//The number of packets that were received with a detected Physical Layer Convergence Protocol (PLCP) header error.	
	output_struct->radio_FCSErrorCount=0;	//The number of packets that were received with a detected FCS error. This parameter is based on dot11FCSErrorCount from [Annex C/802.11-2012].
	output_struct->radio_InvalidMACCount=0;	//The number of packets that were received with a detected invalid MAC header error.
	output_struct->radio_PacketsOtherReceived=0;	//The number of packets that were received, but which were destined for a MAC address that is not associated with this interface.
	output_struct->radio_NoiseFloor=-99; 	//The noise floor for this radio channel where a recoverable signal can be obtained. Expressed as a signed integer in the range (-110:0).  Measurement should capture all energy (in dBm) from sources other than Wi-Fi devices as well as interference from Wi-Fi devices too weak to be decoded. Measured in dBm
	output_struct->radio_ChannelUtilization=35; //Percentage of time the channel was occupied by the radios own activity (Activity Factor) or the activity of other radios.  Channel utilization MUST cover all user traffic, management traffic, and time the radio was unavailable for CSMA activities, including DIFS intervals, etc.  The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected from the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1.  Units in Percentage
	output_struct->radio_ActivityFactor=2; //Percentage of time that the radio was transmitting or receiving Wi-Fi packets to/from associated clients. Activity factor MUST include all traffic that deals with communication between the radio and clients associated to the radio as well as management overhead for the radio, including NAV timers, beacons, probe responses,time for receiving devices to send an ACK, SIFC intervals, etc.  The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected from the just completed interval.   If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage
	output_struct->radio_CarrierSenseThreshold_Exceeded=20; //Percentage of time that the radio was unable to transmit or receive Wi-Fi packets to/from associated clients due to energy detection (ED) on the channel or clear channel assessment (CCA). The metric is calculated and updated in this Parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected from the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage
	output_struct->radio_RetransmissionMetirc=0; //Percentage of packets that had to be re-transmitted. Multiple re-transmissions of the same packet count as one.  The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".   The calculation of this metric MUST only use the data collected from the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units  in percentage
	
	output_struct->radio_MaximumNoiseFloorOnChannel=-1; //Maximum Noise on the channel during the measuring interval.  The metric is updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected in the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1.  Units in dBm
	output_struct->radio_MinimumNoiseFloorOnChannel=-1; //Minimum Noise on the channel. The metric is updated in this Parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected in the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm
	output_struct->radio_MedianNoiseFloorOnChannel=-1;  //Median Noise on the channel during the measuring interval.   The metric is updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected in the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm
	output_struct->radio_StatisticsStartTime=0; 	    //The date and time at which the collection of the current set of statistics started.  This time must be updated whenever the radio statistics are reset.
	
	return RETURN_OK;	
}


//Set radio traffic static Measureing rules
INT wifi_setRadioTrafficStatsMeasure(INT radioIndex, wifi_radioTrafficStatsMeasure_t *input_struct) //Tr181
{
	//zqiu:  If the RadioTrafficStats process running, and the new value is different from old value, the process needs to be reset. The Statistics date, such as MaximumNoiseFloorOnChannel, MinimumNoiseFloorOnChannel and MedianNoiseFloorOnChannel need to be reset. And the "StatisticsStartTime" must be reset to the current time. Units in Seconds
	//       Else, save the MeasuringRate and MeasuringInterval for future usage
	
	return RETURN_OK;
}

//To start or stop RadioTrafficStats
INT wifi_setRadioTrafficStatsRadioStatisticsEnable(INT radioIndex, BOOL enable) 
{
	//zqiu:  If the RadioTrafficStats process running
	//          	if(enable)
	//					return RETURN_OK.
	//				else
	//					Stop RadioTrafficStats process
	//       Else 
	//				if(enable)
	//					Start RadioTrafficStats process with MeasuringRate and MeasuringInterval, and reset "StatisticsStartTime" to the current time, Units in Seconds
	//				else
	//					return RETURN_OK.
		
	return RETURN_OK;
}


//Clients associated with the AP over a specific interval.  The histogram MUST have a range from -110to 0 dBm and MUST be divided in bins of 3 dBM, with bins aligning on the -110 dBm end of the range.  Received signal levels equal to or greater than the smaller boundary of a bin and less than the larger boundary are included in the respective bin.  The bin associated with the client?s current received signal level MUST be incremented when a client associates with the AP.   Additionally, the respective bins associated with each connected client?s current received signal level MUST be incremented at the interval defined by "Radio Statistics Measuring Rate".  The histogram?s bins MUST NOT be incremented at any other time.  The histogram data collected during the interval MUST be published to the parameter only at the end of the interval defined by "Radio Statistics Measuring Interval".  The underlying histogram data MUST be cleared at the start of each interval defined by "Radio Statistics Measuring Interval?. If any of the parameter's representing this histogram is queried before the histogram has been updated with an initial set of data, it MUST return -1. Units dBm
INT wifi_getRadioStatsReceivedSignalLevel(INT radioIndex, INT signalIndex, INT *SignalLevel) //Tr181
{
	//zqiu: Please ignor signalIndex.
	if (NULL == SignalLevel) 
		return RETURN_ERR;
	*SignalLevel=(radioIndex==0)?-19:-19;
	return RETURN_OK;
}

//Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0)
INT wifi_applyRadioSettings(INT radioIndex) 
{
	return RETURN_OK;
}

//Get the radio index assocated with this SSID entry
INT wifi_getSSIDRadioIndex(INT ssidIndex, INT *radioIndex) 
{
	if (NULL == radioIndex) 
		return RETURN_ERR;
	*radioIndex=ssidIndex%2;
	return RETURN_OK;
}

//Get SSID enable configuration parameters (not the SSID enable status)
INT wifi_getSSIDEnable(INT ssidIndex, BOOL *output_bool) //Tr181
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	return wifi_getApEnable(ssidIndex, output_bool);
}

//Set SSID enable configuration parameters
INT wifi_setSSIDEnable(INT ssidIndex, BOOL enable) //Tr181
{
	return wifi_setApEnable(ssidIndex, enable);
}

//Get the SSID enable status
INT wifi_getSSIDStatus(INT ssidIndex, CHAR *output_string) //Tr181
{
    char cmd[MAX_CMD_SIZE]={0};
    char buf[MAX_BUF_SIZE]={0};
    if (NULL == output_string) 
        return RETURN_ERR;
    snprintf(cmd, sizeof(cmd), "ifconfig %s%d | grep %s%d", AP_PREFIX, ssidIndex, AP_PREFIX, ssidIndex);    
    _syscmd(cmd, buf, sizeof(buf));
    
    snprintf(output_string, 64, (strlen(buf)> 5)?"Enabled":"Disabled");

	return RETURN_OK;
}

// Outputs a 32 byte or less string indicating the SSID name.  Sring buffer must be preallocated by the caller.
INT wifi_getSSIDName(INT apIndex, CHAR *output)
{
    struct params params={"ssid",""};
    if (NULL == output) 
        return RETURN_ERR;
    
    wifi_hostapdRead(apIndex,&params,output);
    wifi_dbg_printf("\n[%s]: SSID Name is : %s",__func__,output); 
    if(output==NULL)
        return RETURN_ERR;
    else
        return RETURN_OK;

}
        
// Set a max 32 byte string and sets an internal variable to the SSID name          
INT wifi_setSSIDName(INT apIndex, CHAR *ssid_string)
{

  char str[MAX_BUF_SIZE]={'\0'};
  char cmd[MAX_CMD_SIZE]={'\0'};
  char *ch;
  struct params params;
  param_list_t list;

  if(NULL == ssid_string)
      return RETURN_ERR;

  strcpy(params.name,"ssid");
  strcpy(params.value,ssid_string);
  printf("\n%s\n",__func__);
  memset(&list,0,sizeof(list));
  if(RETURN_ERR == list_add_param(&list,params))
  {
      return RETURN_ERR;
  }
  wifi_hostapdWrite(apIndex,&list);
  list_free_param(&list);
  return RETURN_OK;
}


//Get the BSSID 
INT wifi_getBaseBSSID(INT ssidIndex, CHAR *output_string)	//RDKB
{
        char cmd[MAX_CMD_SIZE]={0};
        char str[MAX_BUF_SIZE]={'\0'};
        char *ch={'\0'};
        char *ch2={'\0'};
        char buf[1024]={'\0'};
        char *pos;
        if (NULL == output_string)
                return RETURN_ERR;

        
        sprintf(cmd,"grep 'interface=' %s%d.conf",HOSTAPD_FNAME,ssidIndex);
        wifi_dbg_printf("\n%s\n",__func__);
        printf("\ncmd=%s\n",cmd);
        if(_syscmd(cmd,str,sizeof(str)) == RETURN_ERR)
        {
            wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
            return RETURN_ERR;
        }
        
        ch=strchr(str,'=');
        if(ch==NULL)
        {
        	wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
	        return RETURN_ERR;
        }
        else
          wifi_dbg_printf("%s",ch+1);

        sprintf(cmd, "ifconfig -a %s | grep HWaddr", (ch+1));
        if (_syscmd(cmd, buf,sizeof(buf))==RETURN_ERR)
        {
                return RETURN_ERR;
        }
        pos = buf;
        if ((pos = strstr(pos, "HWaddr")) != NULL) 
        {
            char *dash = strchr(pos, '-');
            while (dash != NULL) 
            {
                *dash = ':';
                dash = strchr(dash, '-');
            }
            pos += 7;
            memcpy(output_string, pos, 17);
            output_string[17] = 0;
            printf("\n%s\n",output_string);
        }
        return RETURN_OK;
}

//Get the MAC address associated with this Wifi SSID
INT wifi_getSSIDMACAddress(INT ssidIndex, CHAR *output_string) //Tr181
{
    char cmd[128]={0};
        
    if (NULL == output_string) 
        return RETURN_ERR;
        
    sprintf(cmd, "ifconfig -a %s%d | grep HWaddr | tr -s " " | cut -d' ' -f5", AP_PREFIX, ssidIndex);
    _syscmd(cmd, output_string, 64);
    
    return RETURN_OK;
}

//Get the basic SSID traffic static info
INT wifi_getSSIDTrafficStats2(INT ssidIndex, wifi_ssidTrafficStats2_t *output_struct) //Tr181
{
    char cmd[128]={0};
    char buf[1024]={0};
    
    sprintf(cmd, "ifconfig %s%d ", AP_PREFIX, ssidIndex);
    _syscmd(cmd, buf, sizeof(buf));
    
    output_struct->ssid_BytesSent        =2048;    //The total number of bytes transmitted out of the interface, including framing characters.
    output_struct->ssid_BytesReceived    =4096;    //The total number of bytes received on the interface, including framing characters.
    output_struct->ssid_PacketsSent        =128;    //The total number of packets transmitted out of the interface.
    output_struct->ssid_PacketsReceived    =128; //The total number of packets received on the interface.

	output_struct->ssid_RetransCount	=0;	//The total number of transmitted packets which were retransmissions. Two retransmissions of the same packet results in this counter incrementing by two.
	output_struct->ssid_FailedRetransCount=0; //The number of packets that were not transmitted successfully due to the number of retransmission attempts exceeding an 802.11 retry limit. This parameter is based on dot11FailedCount from [802.11-2012].	
	output_struct->ssid_RetryCount		=0;  //The number of packets that were successfully transmitted after one or more retransmissions. This parameter is based on dot11RetryCount from [802.11-2012].	
	output_struct->ssid_MultipleRetryCount=0; //The number of packets that were successfully transmitted after more than one retransmission. This parameter is based on dot11MultipleRetryCount from [802.11-2012].	
	output_struct->ssid_ACKFailureCount	=0;  //The number of expected ACKs that were never received. This parameter is based on dot11ACKFailureCount from [802.11-2012].	
	output_struct->ssid_AggregatedPacketCount=0; //The number of aggregated packets that were transmitted. This applies only to 802.11n and 802.11ac.	

	output_struct->ssid_ErrorsSent		=0;	//The total number of outbound packets that could not be transmitted because of errors.
	output_struct->ssid_ErrorsReceived	=0;    //The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol.
	output_struct->ssid_UnicastPacketsSent=2;	//The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol.
	output_struct->ssid_UnicastPacketsReceived=2;  //The total number of received packets, delivered by this layer to a higher layer, which were not addressed to a multicast or broadcast address at this layer.
	output_struct->ssid_DiscardedPacketsSent=1; //The total number of outbound packets which were chosen to be discarded even though no errors had been detected to prevent their being transmitted. One possible reason for discarding such a packet could be to free up buffer space.
	output_struct->ssid_DiscardedPacketsReceived=1; //The total number of inbound packets which were chosen to be discarded even though no errors had been detected to prevent their being delivered. One possible reason for discarding such a packet could be to free up buffer space.
	output_struct->ssid_MulticastPacketsSent=10; //The total number of packets that higher-level protocols requested for transmission and which were addressed to a multicast address at this layer, including those that were discarded or not sent.
	output_struct->ssid_MulticastPacketsReceived=0; //The total number of received packets, delivered by this layer to a higher layer, which were addressed to a multicast address at this layer.  
	output_struct->ssid_BroadcastPacketsSent=0;  //The total number of packets that higher-level protocols requested for transmission and which were addressed to a broadcast address at this layer, including those that were discarded or not sent.
	output_struct->ssid_BroadcastPacketsRecevied=1; //The total number of packets that higher-level protocols requested for transmission and which were addressed to a broadcast address at this layer, including those that were discarded or not sent.
	output_struct->ssid_UnknownPacketsReceived=0;  //The total number of packets received via the interface which were discarded because of an unknown or unsupported protocol.
	return RETURN_OK;
}

//Apply SSID and AP (in the case of Acess Point devices) to the hardware
//Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0)
INT wifi_applySSIDSettings(INT ssidIndex)
{
	return RETURN_OK;
}



//Start the wifi scan and get the result into output buffer for RDKB to parser. The result will be used to manage endpoint list
//HAL funciton should allocate an data structure array, and return to caller with "neighbor_ap_array"
INT wifi_getNeighboringWiFiDiagnosticResult2(INT radioIndex, wifi_neighbor_ap2_t **neighbor_ap_array, UINT *output_array_size) //Tr181	
{
	INT status = RETURN_ERR;
	UINT index;
	wifi_neighbor_ap2_t *pt=NULL;
	char cmd[128]={0};
	char buf[8192]={0};
	
	sprintf(cmd, "iwlist %s%d scan",AP_PREFIX,(radioIndex==0)?0:1);	//suppose ap0 mapping to radio0
    _syscmd(cmd, buf, sizeof(buf));
	
	
	*output_array_size=2;
	//zqiu: HAL alloc the array and return to caller. Caller response to free it.
	*neighbor_ap_array=(wifi_neighbor_ap2_t *)calloc(sizeof(wifi_neighbor_ap2_t), *output_array_size);
	for (index = 0, pt=*neighbor_ap_array; index < *output_array_size; index++, pt++) {
		strcpy(pt->ap_SSID,"");
		strcpy(pt->ap_BSSID,"");
		strcpy(pt->ap_Mode,"");
		pt->ap_Channel=1;
		pt->ap_SignalStrength=0;
		strcpy(pt->ap_SecurityModeEnabled,"");
		strcpy(pt->ap_EncryptionMode,"");
		strcpy(pt->ap_OperatingFrequencyBand,"");
		strcpy(pt->ap_SupportedStandards,"");
		strcpy(pt->ap_OperatingStandards,"");
		strcpy(pt->ap_OperatingChannelBandwidth,"");
		pt->ap_BeaconPeriod=1;
		pt->ap_Noise=0;
		strcpy(pt->ap_BasicDataTransferRates,"");
		strcpy(pt->ap_SupportedDataTransferRates,"");
		pt->ap_DTIMPeriod=1;
		pt->ap_ChannelUtilization=0;
	}

	status = RETURN_OK;
	return status;
}

//>> Deprecated: used for old RDKB code. 
INT wifi_getRadioWifiTrafficStats(INT radioIndex, wifi_radioTrafficStats_t *output_struct)
{
  INT status = RETURN_ERR;
  output_struct->wifi_PLCPErrorCount = 0;
  output_struct->wifi_FCSErrorCount = 0;
  output_struct->wifi_InvalidMACCount = 0;
  output_struct->wifi_PacketsOtherReceived = 0;
  output_struct->wifi_Noise = 0;
  status = RETURN_OK;
  return status;
}

INT wifi_getBasicTrafficStats(INT apIndex, wifi_basicTrafficStats_t *output_struct)
{
    char cmd[128];  
	char buf[1280];
	char *pos=NULL;
	
	if (NULL == output_struct) {
		return RETURN_ERR;
	} 
	

    memset(output_struct, 0, sizeof(wifi_basicTrafficStats_t));

    snprintf(cmd, sizeof(cmd), "ifconfig %s%d", AP_PREFIX, apIndex);
    _syscmd(cmd,buf, sizeof(buf));

    pos = buf;
    if((pos=strstr(pos,"RX packets:"))==NULL)
        return RETURN_ERR;
    output_struct->wifi_PacketsReceived = atoi(pos+strlen("RX packets:"));
    
    if((pos=strstr(pos,"TX packets:"))==NULL)
        return RETURN_ERR;
    output_struct->wifi_PacketsSent = atoi(pos+strlen("TX packets:"));

    if((pos=strstr(pos,"RX bytes:"))==NULL)
		return RETURN_ERR;    
    output_struct->wifi_BytesReceived = atoi(pos+strlen("RX bytes:"));

    if((pos=strstr(pos,"TX bytes:"))==NULL)
		return RETURN_ERR; 
    output_struct->wifi_BytesSent = atoi(pos+strlen("TX bytes:"));
        
    sprintf(cmd, "wlanconfig %s%d list sta | grep -v HTCAP | wc -l", AP_PREFIX, apIndex);
	_syscmd(cmd, buf, sizeof(buf));
	sscanf(buf,"%lu", &output_struct->wifi_Associations);
	
	return RETURN_OK;
}

INT wifi_getWifiTrafficStats(INT apIndex, wifi_trafficStats_t *output_struct)
{
	if (NULL == output_struct) {
		return RETURN_ERR;
	} else {
		memset(output_struct, 0, sizeof(wifi_trafficStats_t));
		return RETURN_OK;
	}
}

INT wifi_getSSIDTrafficStats(INT apIndex, wifi_ssidTrafficStats_t *output_struct)
{
	INT status = RETURN_ERR;
	//Below values should get updated from hal 
	output_struct->wifi_RetransCount=0;
	output_struct->wifi_FailedRetransCount=0;
	output_struct->wifi_RetryCount=0;
	output_struct->wifi_MultipleRetryCount=0;
	output_struct->wifi_ACKFailureCount=0;
	output_struct->wifi_AggregatedPacketCount=0;

	status = RETURN_OK;
	return status;
}

INT wifi_getNeighboringWiFiDiagnosticResult(wifi_neighbor_ap_t **neighbor_ap_array, UINT *output_array_size)
{
	INT status = RETURN_ERR;
	UINT index;
	wifi_neighbor_ap_t *pt=NULL;
	
	*output_array_size=2;
	//zqiu: HAL alloc the array and return to caller. Caller response to free it.
	*neighbor_ap_array=(wifi_neighbor_ap_t *)calloc(sizeof(wifi_neighbor_ap_t), *output_array_size);
	for (index = 0, pt=*neighbor_ap_array; index < *output_array_size; index++, pt++) {
		strcpy(pt->ap_Radio,"");
		strcpy(pt->ap_SSID,"");
		strcpy(pt->ap_BSSID,"");
		strcpy(pt->ap_Mode,"");
		pt->ap_Channel=1;
		pt->ap_SignalStrength=0;
		strcpy(pt->ap_SecurityModeEnabled,"");
		strcpy(pt->ap_EncryptionMode,"");
		strcpy(pt->ap_OperatingFrequencyBand,"");
		strcpy(pt->ap_SupportedStandards,"");
		strcpy(pt->ap_OperatingStandards,"");
		strcpy(pt->ap_OperatingChannelBandwidth,"");
		pt->ap_BeaconPeriod=1;
		pt->ap_Noise=0;
		strcpy(pt->ap_BasicDataTransferRates,"");
		strcpy(pt->ap_SupportedDataTransferRates,"");
		pt->ap_DTIMPeriod=1;
		pt->ap_ChannelUtilization = 1;
	}

	status = RETURN_OK;
	return status;
}

//----------------- AP HAL -------------------------------

//>> Deprecated: used for old RDKB code.
INT wifi_getAllAssociatedDeviceDetail(INT apIndex, ULONG *output_ulong, wifi_device_t **output_struct)
{
	if (NULL == output_ulong || NULL == output_struct) {
		return RETURN_ERR;
	} else {
		*output_ulong = 0;
		*output_struct = NULL;
		return RETURN_OK;
	}
}

INT wifi_getAssociatedDeviceDetail(INT apIndex, INT devIndex, wifi_device_t *output_struct)
{
	if (NULL == output_struct) {
		return RETURN_ERR;
	} else {
		memset(output_struct, 0, sizeof(wifi_device_t));
		return RETURN_OK;
	}
}

INT wifi_kickAssociatedDevice(INT apIndex, wifi_device_t *device)
{
	if (NULL == device) {
		return RETURN_ERR;
	} else {
		return RETURN_OK;
	}
}
//<<


//--------------wifi_ap_hal-----------------------------
//enables CTS protection for the radio used by this AP
INT wifi_setRadioCtsProtectionEnable(INT apIndex, BOOL enable)
{
	//save config and Apply instantly
	return RETURN_ERR;
}

// enables OBSS Coexistence - fall back to 20MHz if necessary for the radio used by this ap
INT wifi_setRadioObssCoexistenceEnable(INT apIndex, BOOL enable)
{
	//save config and Apply instantly
	return RETURN_ERR;
}

//P3 // sets the fragmentation threshold in bytes for the radio used by this ap
INT wifi_setRadioFragmentationThreshold(INT apIndex, UINT threshold)
{
	char cmd[64];
	char buf[512];
	//save config and apply instantly
	
    //zqiu:TODO: save config
    if (threshold > 0)  {
        snprintf(cmd, sizeof(cmd),  "iwconfig %s%d frag %d", AP_PREFIX, apIndex, threshold);
    } else {
        snprintf(cmd, sizeof(cmd),  "iwconfig %s%d frag off", AP_PREFIX, apIndex );
    }
    _syscmd(cmd,buf, sizeof(buf));
	
	return RETURN_OK;
}

// enable STBC mode in the hardwarwe, 0 == not enabled, 1 == enabled 
INT wifi_setRadioSTBCEnable(INT radioIndex, BOOL STBC_Enable)
{
	//Save config and Apply instantly
	return RETURN_ERR;
}

// outputs A-MSDU enable status, 0 == not enabled, 1 == enabled 
INT wifi_getRadioAMSDUEnable(INT radioIndex, BOOL *output_bool)
{
	return RETURN_ERR;
}

// enables A-MSDU in the hardware, 0 == not enabled, 1 == enabled           
INT wifi_setRadioAMSDUEnable(INT radioIndex, BOOL amsduEnable)
{
	//Apply instantly
	return RETURN_ERR;
}

//P2  // outputs the number of Tx streams
INT wifi_getRadioTxChainMask(INT radioIndex, INT *output_int)
{
	return RETURN_ERR;
}

//P2  // sets the number of Tx streams to an enviornment variable
INT wifi_setRadioTxChainMask(INT radioIndex, INT numStreams)  
{
	//save to wifi config, wait for wifi reset or wifi_pushTxChainMask to apply
	return RETURN_ERR;
}

//P2  // outputs the number of Rx streams
INT wifi_getRadioRxChainMask(INT radioIndex, INT *output_int)
{
	if (NULL == output_int) 
		return RETURN_ERR;
	*output_int=1;		
	return RETURN_OK;	
}

//P2  // sets the number of Rx streams to an enviornment variable
INT wifi_setRadioRxChainMask(INT radioIndex, INT numStreams)
{
	//save to wifi config, wait for wifi reset or wifi_pushRxChainMask to apply
	return RETURN_ERR;
}

//Get radio RDG enable setting
INT wifi_getRadioReverseDirectionGrantSupported(INT radioIndex, BOOL *output_bool)    
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Get radio RDG enable setting
INT wifi_getRadioReverseDirectionGrantEnable(INT radioIndex, BOOL *output_bool)    
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Set radio RDG enable setting
INT wifi_setRadioReverseDirectionGrantEnable(INT radioIndex, BOOL enable)
{
	return RETURN_ERR;
}

//Get radio ADDBA enable setting
INT wifi_getRadioDeclineBARequestEnable(INT radioIndex, BOOL *output_bool)	
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Set radio ADDBA enable setting
INT wifi_setRadioDeclineBARequestEnable(INT radioIndex, BOOL enable)
{
	return RETURN_ERR;
}

//Get radio auto block ack enable setting
INT wifi_getRadioAutoBlockAckEnable(INT radioIndex, BOOL *output_bool)	
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Set radio auto block ack enable setting
INT wifi_setRadioAutoBlockAckEnable(INT radioIndex, BOOL enable)
{
	return RETURN_ERR;
}

//Get radio 11n pure mode enable support
INT wifi_getRadio11nGreenfieldSupported(INT radioIndex, BOOL *output_bool)
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Get radio 11n pure mode enable setting
INT wifi_getRadio11nGreenfieldEnable(INT radioIndex, BOOL *output_bool)
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Set radio 11n pure mode enable setting
INT wifi_setRadio11nGreenfieldEnable(INT radioIndex, BOOL enable)
{
	return RETURN_ERR;
}

//Get radio IGMP snooping enable setting
INT wifi_getRadioIGMPSnoopingEnable(INT radioIndex, BOOL *output_bool)		
{
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=TRUE;		
	return RETURN_OK;	
}

//Set radio IGMP snooping enable setting
INT wifi_setRadioIGMPSnoopingEnable(INT radioIndex, BOOL enable)	
{
	return RETURN_ERR;
}

//Get the Reset count of radio
INT wifi_getRadioResetCount(INT radioIndex, ULONG *output_int) 
{

	if (NULL == output_int) 
		return RETURN_ERR;
	if (radioIndex==0)	
		*output_int=1;
	else
		*output_int=3;
	return RETURN_OK;
}


//---------------------------------------------------------------------------------------------------
//
// Additional Wifi AP level APIs used for Access Point devices
//
//---------------------------------------------------------------------------------------------------

// creates a new ap and pushes these parameters to the hardware
INT wifi_createAp(INT apIndex, INT radioIndex, CHAR *essid, BOOL hideSsid)
{
	char buf[1024];
    char cmd[128];
    
	if (NULL == essid) {
		return RETURN_ERR;
	} 
		
    snprintf(cmd,sizeof(cmd), "wlanconfig %s%d create wlandev %s%d wlanmode ap", AP_PREFIX, apIndex, RADIO_PREFIX, radioIndex);
    _syscmd(cmd, buf, sizeof(buf));

    snprintf(cmd,sizeof(cmd), "iwconfig %s%d essid %s mode master", AP_PREFIX, apIndex, essid);
    _syscmd(cmd, buf, sizeof(buf));

	wifi_pushSsidAdvertisementEnable(apIndex, !hideSsid);    
    
    snprintf(cmd,sizeof(cmd), "ifconfig %s%d txqueuelen 1000", AP_PREFIX, apIndex);
    _syscmd(cmd, buf, sizeof(buf));
		
	return RETURN_OK;
	
}

// deletes this ap entry on the hardware, clears all internal variables associaated with this ap
INT wifi_deleteAp(INT apIndex)
{	
	char buf[1024];
    char cmd[128];
    
    snprintf(cmd,sizeof(cmd),  "wlanconfig %s%d destroy", AP_PREFIX, apIndex);
    _syscmd(cmd, buf, sizeof(buf));

	wifi_removeApSecVaribles(apIndex);

	return RETURN_OK;
}

// Outputs a 16 byte or less name assocated with the AP.  String buffer must be pre-allocated by the caller
INT wifi_getApName(INT apIndex, CHAR *output_string)
{
	if (NULL == output_string) 
		return RETURN_ERR;
	snprintf(output_string, 16, "%s%d", AP_PREFIX, apIndex);
	return RETURN_OK;
}     
       
// Outputs the index number in that corresponds to the SSID string
INT wifi_getIndexFromName(CHAR *inputSsidString, INT *ouput_int)
{
    CHAR *pos=NULL;

    *ouput_int = -1;
	pos=strstr(inputSsidString, AP_PREFIX);
	if(pos) 
	{
		sscanf(pos+strlen(AP_PREFIX),"%d", ouput_int);
		return RETURN_OK;
	} 
	return RETURN_ERR;
}

// Outputs a 32 byte or less string indicating the beacon type as "None", "Basic", "WPA", "11i", "WPAand11i"
INT wifi_getApBeaconType(INT apIndex, CHAR *output_string)
{
    struct params params={"beaconType",""};
    

    if (NULL == output_string)
            return RETURN_ERR;

    
    wifi_hostapdRead(apIndex,&params,output_string);
    wifi_dbg_printf("\n%s: output_string=%s\n",__func__,output_string);
    if (NULL == output_string) 
        return RETURN_ERR;
    
    return RETURN_OK;

}

// Sets the beacon type enviornment variable. Allowed input strings are "None", "Basic", "WPA, "11i", "WPAand11i"
INT wifi_setApBeaconType(INT apIndex, CHAR *beaconTypeString)
{
	struct params params={"beaconType",""};
	param_list_t list;
	if (NULL == beaconTypeString)
		return RETURN_ERR;
	printf("\nbeaconTypeString=%s",beaconTypeString);
	strncpy(params.value,beaconTypeString,strlen(beaconTypeString));
	memset(&list,0,sizeof(list));
	if(RETURN_ERR == list_add_param(&list,params))
	{
		return RETURN_ERR;
	}
	wifi_hostapdWrite(apIndex,&list);
	list_free_param(&list);
	//save the beaconTypeString to wifi config and hostapd config file. Wait for wifi reset or hostapd restart to apply
	return RETURN_OK;
}

// sets the beacon interval on the hardware for this AP
INT wifi_setApBeaconInterval(INT apIndex, INT beaconInterval) 
{
	//save config and apply instantly
	return RETURN_ERR;
}

INT wifi_setDTIMInterval(INT apIndex, INT dtimInterval)
{
	//save config and apply instantly
	return RETURN_ERR;
}

// Get the packet size threshold supported.
INT wifi_getApRtsThresholdSupported(INT apIndex, BOOL *output_bool)
{
	//save config and apply instantly
	if (NULL == output_bool) 
		return RETURN_ERR;
	*output_bool=FALSE;
	return RETURN_OK;
}

// sets the packet size threshold in bytes to apply RTS/CTS backoff rules. 
INT wifi_setApRtsThreshold(INT apIndex, UINT threshold)
{
	char cmd[128];
	char buf[512];
    
    if (threshold > 0) {
        snprintf(cmd, sizeof(cmd), "iwconfig %s%d rts %d", AP_PREFIX, apIndex, threshold);
    } else {
        snprintf(cmd, sizeof(cmd), "iwconfig %s%d rts off", AP_PREFIX, apIndex);
    }
    _syscmd(cmd,buf, sizeof(buf));
	
	return RETURN_OK;
}

// ouputs up to a 32 byte string as either "TKIPEncryption", "AESEncryption", or "TKIPandAESEncryption"
INT wifi_getApWpaEncryptionMode(INT apIndex, CHAR *output_string)
{
	struct params beacon={"beaconType",""};
	struct params params={"wpa_pairwise",""};
	char buf[32];

	if (NULL == output_string)
		return RETURN_ERR;

	memset(buf,'\0',32);
	wifi_hostapdRead(apIndex,&beacon,buf);

	if((strcmp(buf,"WPAand11i")==0))
	{
		strcpy(params.name,"rsn_pairwise");
	}
	else if((strcmp(buf,"11i")==0))
	{
		strcpy(params.name,"rsn_pairwise");
	}
	else if((strcmp(buf,"WPA")==0))
	{
		strcpy(params.name,"wpa_pairwise");
	}
	memset(output_string,'\0',32);
	wifi_hostapdRead(apIndex,&params,output_string);
	wifi_dbg_printf("\n%s output_string=%s",__func__,output_string);

	if (strcmp(output_string,"TKIP") == 0)
		strncpy(output_string,"TKIPEncryption", strlen("TKIPEncryption"));
	else if(strcmp(output_string,"CCMP") == 0)
		strncpy(output_string,"AESEncryption", strlen("AESEncryption"));
	else if(strcmp(output_string,"TKIP CCMP") == 0)
		strncpy(output_string,"TKIPandAESEncryption", strlen("TKIPandAESEncryption"));

	return RETURN_OK;
}

// sets the encyption mode enviornment variable.  Valid string format is "TKIPEncryption", "AESEncryption", or "TKIPandAESEncryption"
INT wifi_setApWpaEncryptionMode(INT apIndex, CHAR *encMode)
{
	struct params beacon={"beaconType",""};
	struct params params={'\0'};
	param_list_t list;
	char output_string[32];
	int ret;

	memset(&list,0,sizeof(list));
	memset(output_string,'\0',32);
	wifi_hostapdRead(apIndex,&beacon,output_string);

	if ( strcmp(encMode, "TKIPEncryption") == 0)
	{
		strncpy(params.value, "TKIP", strlen("TKIP"));
	} else if ( strcmp(encMode,"AESEncryption") == 0)
	{
		strncpy(params.value, "CCMP", strlen("CCMP"));
	} else if (strcmp(encMode,"TKIPandAESEncryption") == 0)
	{
		strncpy(params.value,"TKIP CCMP",strlen("TKIP CCMP"));
	}

	if((strcmp(output_string,"WPAand11i")==0))
	{
		strcpy(params.name,"wpa_pairwise");
		if(RETURN_ERR == list_add_param(&list,params))
		{
			return RETURN_ERR;
		}
		strcpy(params.name,"rsn_pairwise");
		if(RETURN_ERR == list_add_param(&list,params))
		{
			list_free_param(&list);
			return RETURN_ERR;
		}
	}
	else if((strcmp(output_string,"11i")==0))
	{
		strcpy(params.name,"rsn_pairwise");
		if(RETURN_ERR == list_add_param(&list,params))
		{
			list_free_param(&list);
			return RETURN_ERR;
		}
	}
	else if((strcmp(output_string,"WPA")==0))
	{
		strcpy(params.name,"wpa_pairwise");
		if(RETURN_ERR == list_add_param(&list,params))
		{
			list_free_param(&list);
			return RETURN_ERR;
		}
	}
	ret=wifi_hostapdWrite(apIndex,&list);
	list_free_param(&list);
	return RETURN_OK;
}

// deletes internal security varable settings for this ap
INT wifi_removeApSecVaribles(INT apIndex)
{
	//TODO: remove the entry in hostapd config file
    //snprintf(cmd,sizeof(cmd), "sed -i 's/\\/nvram\\/etc\\/wpa2\\/WSC_%s%d.conf//g' /tmp/conf_filename", AP_PREFIX, apIndex);
    //_syscmd(cmd, buf, sizeof(buf));
	
    //snprintf(cmd,sizeof(cmd), "sed -i 's/\\/tmp\\//sec%s%d//g' /tmp/conf_filename", AP_PREFIX, apIndex);
    //_syscmd(cmd, buf, sizeof(buf));
	return RETURN_ERR;
}

// changes the hardware settings to disable encryption on this ap 
INT wifi_disableApEncryption(INT apIndex) 
{
	//Apply instantly
	return RETURN_ERR;
}

// set the authorization mode on this ap
// mode mapping as: 1: open, 2: shared, 4:auto
INT wifi_setApAuthMode(INT apIndex, INT mode)
{
	//Apply instantly
	return RETURN_ERR;
}

// sets an enviornment variable for the authMode. Valid strings are "None", "EAPAuthentication" or "SharedAuthentication"                     
INT wifi_setApBasicAuthenticationMode(INT apIndex, CHAR *authMode)
{
	//save to wifi config, and wait for wifi restart to apply
	struct params params={'\0'};
	int ret;
	param_list_t list;
	if(authMode ==  NULL)
		return RETURN_ERR;
	memset(&list,0,sizeof(list));
	wifi_dbg_printf("\n%s AuthMode=%s",__func__,authMode);
	strncpy(params.name,"wpa_key_mgmt",strlen("wpa_key_mgmt"));
	if(strcmp(authMode,"PSKAuthentication") == 0)
		strcpy(params.value,"WPA-PSK");
	else if(strcmp(authMode,"EAPAuthentication") == 0)
		strcpy(params.value,"WPA-EAP");
	else if(strcmp(authMode,"None") == 0) //Donot change in case the authMode is None
		return RETURN_OK;			  //This is taken careof in beaconType
	if(RETURN_ERR == list_add_param(&list,params))
	{
		return RETURN_ERR;
	}
	ret=wifi_hostapdWrite(apIndex,&list);
	list_free_param(&list);
	return ret;
}

// sets an enviornment variable for the authMode. Valid strings are "None", "EAPAuthentication" or "SharedAuthentication"
INT wifi_getApBasicAuthenticationMode(INT apIndex, CHAR *authMode)
{
    //save to wifi config, and wait for wifi restart to apply
    struct params params={'\0'};
    int ret;

    if(authMode ==  NULL)
        return RETURN_ERR;
    strcpy(authMode,"SharedAuthentication");
    return RETURN_OK;
}

// Outputs the number of stations associated per AP
INT wifi_getApNumDevicesAssociated(INT apIndex, ULONG *output_ulong)
{
	char cmd[128]={0};
	char buf[128]={0};
		
	sprintf(cmd, "wlanconfig %s%d list sta | grep -v HTCAP | wc -l", AP_PREFIX, apIndex);
	_syscmd(cmd, buf, sizeof(buf));
	sscanf(buf,"%lu", output_ulong);
	return RETURN_OK;
}

// manually removes any active wi-fi association with the device specified on this ap
INT wifi_kickApAssociatedDevice(INT apIndex, CHAR *client_mac) 	
{
	return RETURN_ERR;
}

// outputs the radio index for the specified ap. similar as wifi_getSsidRadioIndex
INT wifi_getApRadioIndex(INT apIndex, INT *output_int) 
{
	*output_int=apIndex%2;
	return RETURN_OK;
}

// sets the radio index for the specific ap            
INT wifi_setApRadioIndex(INT apIndex, INT radioIndex)                
{
	//set to config only and wait for wifi reset to apply settings
	return RETURN_ERR;
}

// Get the ACL MAC list per AP
INT wifi_getApAclDevices(INT apIndex, CHAR *macArray, UINT buf_size) 
{		
	snprintf(macArray, buf_size, "11:22:33:44:55:66\n11:22:33:44:55:67\n");		
	return RETURN_OK;
}
	
// Get the list of stations assocated per AP
INT wifi_getApDevicesAssociated(INT apIndex, CHAR *macArray, UINT buf_size) 
{
	char cmd[128];
		
	sprintf(cmd, "wlanconfig %s%d list sta | grep -v HTCAP | cut -d' ' -f1", AP_PREFIX, apIndex);
	_syscmd(cmd, macArray, buf_size);
		
	return RETURN_OK;
}

// adds the mac address to the filter list
//DeviceMacAddress is in XX:XX:XX:XX:XX:XX format
INT wifi_addApAclDevice(INT apIndex, CHAR *DeviceMacAddress) 
{
	//Apply instantly		
	return RETURN_ERR;
}

// deletes the mac address from the filter list
//DeviceMacAddress is in XX:XX:XX:XX:XX:XX format
INT wifi_delApAclDevice(INT apIndex, CHAR *DeviceMacAddress)        
{
	//Apply instantly
	return RETURN_ERR;
}

// outputs the number of devices in the filter list
INT wifi_getApAclDeviceNum(INT apIndex, UINT *output_uint)   
{
	if (NULL == output_uint) 
		return RETURN_ERR;
	*output_uint=0;
	return RETURN_ERR;   
}

// enable kick for devices on acl black list    
INT wifi_kickApAclAssociatedDevices(INT apIndex, BOOL enable)    
{
	char aclArray[512]={0}, *acl=NULL;
	char assocArray[512]={0}, *asso=NULL;
	
	wifi_getApAclDevices( apIndex, aclArray, sizeof(aclArray));
    wifi_getApDevicesAssociated( apIndex, assocArray, sizeof(assocArray));

	// if there are no devices connected there is nothing to do
    if (strlen(assocArray) < 17) {
        return RETURN_OK;
    }
   
    if ( enable == TRUE ) {
		//kick off the MAC which is in ACL array (deny list)
		acl = strtok (aclArray,"\r\n");
		while (acl != NULL) {
			if(strlen(acl)>=17 && strcasestr(assocArray, acl)) {
				wifi_kickApAssociatedDevice(apIndex, acl); 
			}
			acl = strtok (NULL, "\r\n");
		}
    } else {
		//kick off the MAC which is not in ACL array (allow list)
		asso = strtok (assocArray,"\r\n");
		while (asso != NULL) {
			if(strlen(asso)>=17 && !strcasestr(aclArray, asso)) {
				wifi_kickApAssociatedDevice(apIndex, asso); 
			}
			asso = strtok (NULL, "\r\n");
		}
	}	    

    return RETURN_OK;
}

// sets the mac address filter control mode.  0 == filter disabled, 1 == filter as whitelist, 2 == filter as blacklist
INT wifi_setApMacAddressControlMode(INT apIndex, INT filterMode)
{
	//apply instantly
	return RETURN_ERR;
}

// enables internal gateway VLAN mode.  In this mode a Vlan tag is added to upstream (received) data packets before exiting the Wifi driver.  VLAN tags in downstream data are stripped from data packets before transmission.  Default is FALSE. 
INT wifi_setApVlanEnable(INT apIndex, BOOL VlanEnabled)
{
	return RETURN_ERR;
}      

// gets the vlan ID for this ap from an internal enviornment variable
INT wifi_getApVlanID(INT apIndex, INT *output_int)
{
	if(apIndex=0) {
		*output_int=100;
		return RETURN_OK;
	}
	return RETURN_ERR;
}   

// sets the vlan ID for this ap to an internal enviornment variable
INT wifi_setApVlanID(INT apIndex, INT vlanId)
{
	//save the vlanID to config and wait for wifi reset to apply (wifi up module would read this parameters and tag the AP with vlan id)
	return RETURN_ERR;
}   

// gets bridgeName, IP address and Subnet. bridgeName is a maximum of 32 characters,
INT wifi_getApBridgeInfo(INT index, CHAR *bridgeName, CHAR *IP, CHAR *subnet)
{	
	snprintf(bridgeName, 32, "br0");
	snprintf(IP, 64, "10.0.0.2");
	snprintf(subnet, 64, "255.255.255.0");
	
	return RETURN_ERR;
}

//sets bridgeName, IP address and Subnet to internal enviornment variables. bridgeName is a maximum of 32 characters, 
INT wifi_setApBridgeInfo(INT apIndex, CHAR *bridgeName, CHAR *IP, CHAR *subnet)
{	
	//save settings, wait for wifi reset or wifi_pushBridgeInfo to apply. 
	return RETURN_ERR;
}

// reset the vlan configuration for this ap
INT wifi_resetApVlanCfg(INT apIndex)
{
	//TODO: remove existing vlan for this ap
    
    //Reapply vlan settings
    wifi_pushBridgeInfo(apIndex);
	
	return RETURN_ERR;
}

// creates configuration variables needed for WPA/WPS.  These variables are implementation dependent and in some implementations these variables are used by hostapd when it is started.  Specific variables that are needed are dependent on the hostapd implementation. These variables are set by WPA/WPS security functions in this wifi HAL.  If not needed for a particular implementation this function may simply return no error.
INT wifi_createHostApdConfig(INT apIndex, BOOL createWpsCfg)
{	
	return RETURN_ERR;
}

// starts hostapd, uses the variables in the hostapd config with format compatible with the specific hostapd implementation
INT wifi_startHostApd()
{
	char cmd[128] = {0};
	char buf[1028] = {0};

	_syscmd("iwconfig wlan0|grep 802.11a",buf,sizeof(buf));
	if(strlen(buf) > 0)
	{
		system("sed -i 's/interface=wlan0/interface=wlan1/g' /nvram/hostapd0.conf");
		system("sed -i 's/interface=wlan1/interface=wlan0/g' /nvram/hostapd1.conf");
		_syscmd("hostapd -B /nvram/hostapd0.conf /nvram/hostapd1.conf",buf,sizeof(buf));
	}
	else
	{
		system("sed -i 's/interface=wlan1/interface=wlan0/g' /nvram/hostapd0.conf");
		system("sed -i 's/interface=wlan0/interface=wlan1/g' /nvram/hostapd1.conf");
		memset(buf,'\0',sizeof(buf));
		_syscmd("iwconfig wlan1",buf,sizeof(buf));
		if(strlen(buf)== 0)
		{	
			_syscmd("hostapd -B /nvram/hostapd0.conf",buf,sizeof(buf));
		}
		else
		{
			_syscmd("hostapd -B /nvram/hostapd0.conf /nvram/hostapd1.conf",buf,sizeof(buf));
		}
	}
	return RETURN_OK;
}


 // stops hostapd	
INT wifi_stopHostApd()                                        
{
	char cmd[128] = {0};
	char buf[128] = {0};
	
	sprintf(cmd,"killall hostapd");
	_syscmd(cmd, buf, sizeof(buf));
	
	return RETURN_OK;	
}

// restart hostapd dummy function
INT wifi_restartHostApd()
{
        return RETURN_OK;
}

// sets the AP enable status variable for the specified ap.
INT wifi_setApEnable(INT apIndex, BOOL enable)
{
    INT retValue;

    //Store the AP enable settings and wait for wifi up to apply
    retValue = wifi_setRadioEnable(apIndex, enable);
    return retValue;
}

// Outputs the setting of the internal variable that is set by wifi_setEnable().  
INT wifi_getApEnable(INT apIndex, BOOL *output_bool)
{
    INT retValue;

    if(!output_bool)
        return RETURN_ERR;

    retValue = wifi_getRadioEnable(apIndex, output_bool);
    return retValue;
}

// Outputs the AP "Enabled" "Disabled" status from driver 
INT wifi_getApStatus(INT apIndex, CHAR *output_string) 
{
	char cmd[128] = {0};
	char buf[128] = {0};
	INT  wlanIndex;


	if( 0 == apIndex ) // For 2.4 GHz
		wlanIndex = wifi_getApIndexForWiFiBand(band_2_4);
	else
		wlanIndex = wifi_getApIndexForWiFiBand(band_5);

	sprintf(cmd,"iwconfig  | grep wlan%d", wlanIndex);
	_syscmd(cmd, buf, sizeof(buf));
	
	if(strlen(buf)>3)
		snprintf(output_string, 32, "Up");
	else
		snprintf(output_string, 32, "Disable");
	return RETURN_OK;
}

//Indicates whether or not beacons include the SSID name.
// outputs a 1 if SSID on the AP is enabled, else ouputs 0
INT wifi_getApSsidAdvertisementEnable(INT apIndex, BOOL *output_bool) 
{
	//get the running status
	if(!output_bool)
		return RETURN_ERR;
	*output_bool=TRUE;	
	return RETURN_OK;	
}

// sets an internal variable for ssid advertisement.  Set to 1 to enable, set to 0 to disable
INT wifi_setApSsidAdvertisementEnable(INT apIndex, BOOL enable) 
{
	//store the config, apply instantly
	return RETURN_ERR;
}

//The maximum number of retransmission for a packet. This corresponds to IEEE 802.11 parameter dot11ShortRetryLimit.
INT wifi_getApRetryLimit(INT apIndex, UINT *output_uint)
{
	//get the running status
	if(!output_uint)
		return RETURN_ERR;
	*output_uint=15;	
	return RETURN_OK;	
}

INT wifi_setApRetryLimit(INT apIndex, UINT number)
{
	//apply instantly
	return RETURN_ERR;
}

//Indicates whether this access point supports WiFi Multimedia (WMM) Access Categories (AC).
INT wifi_getApWMMCapability(INT apIndex, BOOL *output)
{
	if(!output)
		return RETURN_ERR;
	*output=TRUE;	
	return RETURN_OK;	
}

//Indicates whether this access point supports WMM Unscheduled Automatic Power Save Delivery (U-APSD). Note: U-APSD support implies WMM support.
INT wifi_getApUAPSDCapability(INT apIndex, BOOL *output)
{
	//get the running status from driver
	if(!output)
		return RETURN_ERR;
	*output=TRUE;	
	return RETURN_OK;
}

//Whether WMM support is currently enabled. When enabled, this is indicated in beacon frames.
INT wifi_getApWmmEnable(INT apIndex, BOOL *output)
{
	//get the running status from driver
	if(!output)
		return RETURN_ERR;
	*output=TRUE;	
	return RETURN_OK;
}

// enables/disables WMM on the hardwawre for this AP.  enable==1, disable == 0    
INT wifi_setApWmmEnable(INT apIndex, BOOL enable)
{
	//Save config and apply instantly. 
	return RETURN_ERR;
}

//Whether U-APSD support is currently enabled. When enabled, this is indicated in beacon frames. Note: U-APSD can only be enabled if WMM is also enabled.
INT wifi_getApWmmUapsdEnable(INT apIndex, BOOL *output)
{
	//get the running status from driver
	if(!output)
		return RETURN_ERR;
	*output=TRUE;	
	return RETURN_OK;
}

// enables/disables Automatic Power Save Delivery on the hardwarwe for this AP
INT wifi_setApWmmUapsdEnable(INT apIndex, BOOL enable)
{
	//save config and apply instantly. 
	return RETURN_ERR;
}   

// Sets the WMM ACK polity on the hardware. AckPolicy false means do not acknowledge, true means acknowledge
INT wifi_setApWmmOgAckPolicy(INT apIndex, INT class, BOOL ackPolicy)  //RDKB
{
	//save config and apply instantly. 
	return RETURN_ERR;
}

//Enables or disables device isolation.	A value of true means that the devices connected to the Access Point are isolated from all other devices within the home network (as is typically the case for a Wireless Hotspot).	
INT wifi_getApIsolationEnable(INT apIndex, BOOL *output)
{
	//get the running status from driver
	if(!output)
		return RETURN_ERR;
	*output=TRUE;	
	return RETURN_OK;
}
	
INT wifi_setApIsolationEnable(INT apIndex, BOOL enable)
{
	//store the config, apply instantly
	return RETURN_ERR;
}

//The maximum number of devices that can simultaneously be connected to the access point. A value of 0 means that there is no specific limit.			
INT wifi_getApMaxAssociatedDevices(INT apIndex, UINT *output_uint)
{
	//get the running status from driver
	if(!output_uint)
		return RETURN_ERR;
	*output_uint=5;	
	return RETURN_OK;
}

INT wifi_setApMaxAssociatedDevices(INT apIndex, UINT number)
{
	//store to wifi config, apply instantly
	return RETURN_ERR;
}

//The HighWatermarkThreshold value that is lesser than or equal to MaxAssociatedDevices. Setting this parameter does not actually limit the number of clients that can associate with this access point as that is controlled by MaxAssociatedDevices.	MaxAssociatedDevices or 50. The default value of this parameter should be equal to MaxAssociatedDevices. In case MaxAssociatedDevices is 0 (zero), the default value of this parameter should be 50. A value of 0 means that there is no specific limit and Watermark calculation algorithm should be turned off.			
INT wifi_getApAssociatedDevicesHighWatermarkThreshold(INT apIndex, UINT *output_uint)
{
	//get the current threshold
	if(!output_uint)
		return RETURN_ERR;
	*output_uint=50;	
	return RETURN_OK;
}

INT wifi_setApAssociatedDevicesHighWatermarkThreshold(INT apIndex, UINT Threshold)
{
	//store the config, reset threshold, reset AssociatedDevicesHighWatermarkThresholdReached, reset AssociatedDevicesHighWatermarkDate to current time
	return RETURN_ERR;
}		

//Number of times the current total number of associated device has reached the HighWatermarkThreshold value. This calculation can be based on the parameter AssociatedDeviceNumberOfEntries as well. Implementation specifics about this parameter are left to the product group and the device vendors. It can be updated whenever there is a new client association request to the access point.	
INT wifi_getApAssociatedDevicesHighWatermarkThresholdReached(INT apIndex, UINT *output_uint)
{
	if(!output_uint)
		return RETURN_ERR;
	*output_uint=3;	
	return RETURN_OK;
}

//Maximum number of associated devices that have ever associated with the access point concurrently since the last reset of the device or WiFi module.	
INT wifi_getApAssociatedDevicesHighWatermark(INT apIndex, UINT *output_uint)
{
	if(!output_uint)
		return RETURN_ERR;
	*output_uint=3;	
	return RETURN_OK;
}

//Date and Time at which the maximum number of associated devices ever associated with the access point concurrenlty since the last reset of the device or WiFi module (or in short when was X_COMCAST-COM_AssociatedDevicesHighWatermark updated). This dateTime value is in UTC.	
INT wifi_getApAssociatedDevicesHighWatermarkDate(INT apIndex, ULONG *output_in_seconds)
{
	if(!output_in_seconds)
		return RETURN_ERR;
	*output_in_seconds=0;	
	return RETURN_OK;
}

//Comma-separated list of strings. Indicates which security modes this AccessPoint instance is capable of supporting. Each list item is an enumeration of: None,WEP-64,WEP-128,WPA-Personal,WPA2-Personal,WPA-WPA2-Personal,WPA-Enterprise,WPA2-Enterprise,WPA-WPA2-Enterprise
INT wifi_getApSecurityModesSupported(INT apIndex, CHAR *output)
{
	if(!output)
		return RETURN_ERR;
	snprintf(output, 128, "None,WPA-Personal,WPA2-Personal,WPA-WPA2-Personal,WPA-Enterprise,WPA2-Enterprise,WPA-WPA2-Enterprise");
	return RETURN_OK;
}		

//The value MUST be a member of the list reported by the ModesSupported parameter. Indicates which security mode is enabled.
INT wifi_getApSecurityModeEnabled(INT apIndex, CHAR *output)
{
	if(!output)
		return RETURN_ERR;
	snprintf(output, 128, "WPA-WPA2-Personal");
	return RETURN_OK;
}
  
INT wifi_setApSecurityModeEnabled(INT apIndex, CHAR *encMode)
{
	//store settings and wait for wifi up to apply
	return RETURN_ERR;
}   


//A literal PreSharedKey (PSK) expressed as a hexadecimal string.
// output_string must be pre-allocated as 64 character string by caller
// PSK Key of 8 to 63 characters is considered an ASCII string, and 64 characters are considered as HEX value
INT wifi_getApSecurityPreSharedKey(INT apIndex, CHAR *output_string)
{	
	if(!output_string)
		return RETURN_ERR;
	snprintf(output_string, 64, "E4A7A43C99DFFA57");
	return RETURN_OK;
}

// sets an enviornment variable for the psk. Input string preSharedKey must be a maximum of 64 characters
// PSK Key of 8 to 63 characters is considered an ASCII string, and 64 characters are considered as HEX value
INT wifi_setApSecurityPreSharedKey(INT apIndex, CHAR *preSharedKey)        
{	
	//save to wifi config and hotapd config. wait for wifi reset or hostapd restet to apply
	struct params params={'\0'};
	int ret;
	param_list_t list;

        if(NULL == preSharedKey)
            return RETURN_ERR;
	strcpy(params.name,"wpa_passphrase");
	strcpy(params.value,preSharedKey);
	if(strlen(preSharedKey)<8 || strlen(preSharedKey)>63)
	{
		wifi_dbg_printf("\nCannot Set Preshared Key length of preshared key should be 8 to 63 chars\n");
		return RETURN_ERR;
	}
	else
	{
		memset(&list,0,sizeof(list));
		if(RETURN_ERR == list_add_param(&list,params))
		{
			return RETURN_ERR;
		}
		ret=wifi_hostapdWrite(apIndex,&list);
		list_free_param(&list);
		return ret;
	}
}

//A passphrase from which the PreSharedKey is to be generated, for WPA-Personal or WPA2-Personal or WPA-WPA2-Personal security modes.
// outputs the passphrase, maximum 63 characters
INT wifi_getApSecurityKeyPassphrase(INT apIndex, CHAR *output_string)
{	
    struct params params={"wpa_passphrase",""};
    wifi_dbg_printf("\nFunc=%s\n",__func__);
    if (NULL == output_string)
    return RETURN_ERR;
    wifi_hostapdRead(apIndex,&params,output_string);
    wifi_dbg_printf("\noutput_string=%s\n",output_string);

    if(output_string==NULL)
        return RETURN_ERR;
    else
        return RETURN_OK;
}

// sets the passphrase enviornment variable, max 63 characters
INT wifi_setApSecurityKeyPassphrase(INT apIndex, CHAR *passPhrase)
{	
	//save to wifi config and hotapd config. wait for wifi reset or hostapd restet to apply
	return RETURN_ERR;
}

//When set to true, this AccessPoint instance's WiFi security settings are reset to their factory default values. The affected settings include ModeEnabled, WEPKey, PreSharedKey and KeyPassphrase.
INT wifi_setApSecurityReset(INT apIndex)
{
	//apply instantly
	return RETURN_ERR;
}

//The IP Address and port number of the RADIUS server used for WLAN security. RadiusServerIPAddr is only applicable when ModeEnabled is an Enterprise type (i.e. WPA-Enterprise, WPA2-Enterprise or WPA-WPA2-Enterprise).
INT wifi_getApSecurityRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusSecret_output)
{
	if(!IP_output || !Port_output || !RadiusSecret_output)
		return RETURN_ERR;
	snprintf(IP_output, 64, "75.56.77.78");
	*Port_output=123;
	snprintf(RadiusSecret_output, 64, "12345678");
	return RETURN_OK;
}

INT wifi_setApSecurityRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusSecret)
{
	//store the paramters, and apply instantly
	return RETURN_ERR;
}

INT wifi_getApSecuritySecondaryRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusSecret_output)
{
	if(!IP_output || !Port_output || !RadiusSecret_output)
		return RETURN_ERR;
	snprintf(IP_output, 64, "75.56.77.78");
	*Port_output=123;
	snprintf(RadiusSecret_output, 64, "12345678");
	return RETURN_OK;
}

INT wifi_setApSecuritySecondaryRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusSecret)
{
	//store the paramters, and apply instantly
	return RETURN_ERR;
}

//RadiusSettings
INT wifi_getApSecurityRadiusSettings(INT apIndex, wifi_radius_setting_t *output)
{
	if(!output)
		return RETURN_ERR;
	
	output->RadiusServerRetries=3; 				//Number of retries for Radius requests.
	output->RadiusServerRequestTimeout=5; 		//Radius request timeout in seconds after which the request must be retransmitted for the # of retries available.	
	output->PMKLifetime=28800; 					//Default time in seconds after which a Wi-Fi client is forced to ReAuthenticate (def 8 hrs).	
	output->PMKCaching=FALSE; 					//Enable or disable caching of PMK.	
	output->PMKCacheInterval=300; 				//Time interval in seconds after which the PMKSA (Pairwise Master Key Security Association) cache is purged (def 5 minutes).	
	output->MaxAuthenticationAttempts=3; 		//Indicates the # of time, a client can attempt to login with incorrect credentials. When this limit is reached, the client is blacklisted and not allowed to attempt loging into the network. Settings this parameter to 0 (zero) disables the blacklisting feature.
	output->BlacklistTableTimeout=600; 			//Time interval in seconds for which a client will continue to be blacklisted once it is marked so.	
	output->IdentityRequestRetryInterval=5; 	//Time Interval in seconds between identity requests retries. A value of 0 (zero) disables it.	
	output->QuietPeriodAfterFailedAuthentication=5;  	//The enforced quiet period (time interval) in seconds following failed authentication. A value of 0 (zero) disables it.	
	//snprintf(output->RadiusSecret, 64, "12345678");		//The secret used for handshaking with the RADIUS server [RFC2865]. When read, this parameter returns an empty string, regardless of the actual value.
	
	return RETURN_OK;
}

INT wifi_setApSecurityRadiusSettings(INT apIndex, wifi_radius_setting_t *input)	
{
	//store the paramters, and apply instantly
	return RETURN_ERR;
}

//Enables or disables WPS functionality for this access point.
// outputs the WPS enable state of this ap in output_bool 
INT wifi_getApWpsEnable(INT apIndex, BOOL *output_bool)
{
	if(!output_bool)
		return RETURN_ERR;
	*output_bool=FALSE;
	return RETURN_OK;
}        

// sets the WPS enable enviornment variable for this ap to the value of enableValue, 1==enabled, 0==disabled     
INT wifi_setApWpsEnable(INT apIndex, BOOL enableValue)
{
	//store the paramters, and wait for wifi up to apply
	return RETURN_ERR;
}        

//Comma-separated list of strings. Indicates WPS configuration methods supported by the device. Each list item is an enumeration of: USBFlashDrive,Ethernet,ExternalNFCToken,IntegratedNFCToken,NFCInterface,PushButton,PIN
INT wifi_getApWpsConfigMethodsSupported(INT apIndex, CHAR *output)
{
	if(!output)
		return RETURN_ERR;
	snprintf(output, 128, "PushButton,PIN");
	return RETURN_OK;
}			

//Comma-separated list of strings. Each list item MUST be a member of the list reported by the ConfigMethodsSupported parameter. Indicates WPS configuration methods enabled on the device.
// Outputs a common separated list of the enabled WPS config methods, 64 bytes max
INT wifi_getApWpsConfigMethodsEnabled(INT apIndex, CHAR *output)
{
	if(!output)
		return RETURN_ERR;
	snprintf(output, 128, "PushButton,PIN");
	return RETURN_OK;
}

// sets an enviornment variable that specifies the WPS configuration method(s).  methodString is a comma separated list of methods USBFlashDrive,Ethernet,ExternalNFCToken,IntegratedNFCToken,NFCInterface,PushButton,PIN
INT wifi_setApWpsConfigMethodsEnabled(INT apIndex, CHAR *methodString)
{
	//apply instantly. No setting need to be stored. 
	return RETURN_ERR;
}

// outputs the pin value, ulong_pin must be allocated by the caller
INT wifi_getApWpsDevicePIN(INT apIndex, ULONG *output_ulong)
{
	if(!output_ulong)
		return RETURN_ERR;
	*output_ulong=45276457;
	return RETURN_OK;
}

// set an enviornment variable for the WPS pin for the selected AP. Normally, Device PIN should not be changed.
INT wifi_setApWpsDevicePIN(INT apIndex, ULONG pin)
{
	//set the pin to wifi config and hostpad config. wait for wifi reset or hostapd reset to apply 
	return RETURN_ERR;
}    

// Output string is either Not configured or Configured, max 32 characters
INT wifi_getApWpsConfigurationState(INT apIndex, CHAR *output_string)
{
	char cmd[64];
	char buf[512]={0};
	char *pos=NULL;

	snprintf(output_string, 64, "Not configured");

	sprintf(cmd, "hostapd_cli -i %s%d get_config", AP_PREFIX, apIndex);
	_syscmd(cmd,buf, sizeof(buf));
	
	if((pos=strstr(buf, "wps_state="))!=NULL) {
		if (strstr(pos, "configured")!=NULL)
			snprintf(output_string, 64, "Configured");
	}
	return RETURN_OK;
}

// sets the WPS pin for this AP
INT wifi_setApWpsEnrolleePin(INT apIndex, CHAR *pin)
{
	char cmd[64];
	char buf[256]={0};
	BOOL enable;

	wifi_getApEnable(apIndex, &enable);
	if (!enable) 
		return RETURN_ERR; 

	wifi_getApWpsEnable(apIndex, &enable);
	if (!enable) 
		return RETURN_ERR; 

	snprintf(cmd, 64, "hostapd_cli -i%s%d wps_pin any %s", AP_PREFIX, apIndex, pin);
	_syscmd(cmd,buf, sizeof(buf));
	
	if((strstr(buf, "OK"))!=NULL) 
		return RETURN_OK;
	else
		return RETURN_ERR;
}

// This function is called when the WPS push button has been pressed for this AP
INT wifi_setApWpsButtonPush(INT apIndex)
{
	char cmd[64];
	char buf[256]={0};
	BOOL enable;

	wifi_getApEnable(apIndex, &enable);
	if (!enable) 
		return RETURN_ERR; 

	wifi_getApWpsEnable(apIndex, &enable);
	if (!enable) 
		return RETURN_ERR; 

	snprintf(cmd, 64, "hostapd_cli -i%s%d wps_cancel; hostapd_cli -i%s%d wps_pbc", AP_PREFIX, apIndex, AP_PREFIX, apIndex);
	_syscmd(cmd,buf, sizeof(buf));
	
	if((strstr(buf, "OK"))!=NULL) 
		return RETURN_OK;
	else
		return RETURN_ERR;
}

// cancels WPS mode for this AP
INT wifi_cancelApWPS(INT apIndex)
{
	char cmd[64];
	char buf[256]={0};

	snprintf(cmd, 64, "hostapd_cli -i%s%d wps_cancel", AP_PREFIX, apIndex);
	_syscmd(cmd,buf, sizeof(buf));
	
	if((strstr(buf, "OK"))!=NULL) 
		return RETURN_OK;
	else
		return RETURN_ERR;
}                                 

//Device.WiFi.AccessPoint.{i}.AssociatedDevice.*
//HAL funciton should allocate an data structure array, and return to caller with "associated_dev_array"
INT wifi_getApAssociatedDeviceDiagnosticResult(INT apIndex, wifi_associated_dev_t **associated_dev_array, UINT *output_array_size)
{
	FILE *f;
	int read_flag=0, auth_temp=0, mac_temp=0,i=0;
	char cmd[256], buf[2048];
	char *param , *value, *line=NULL;
	size_t len = 0;
	ssize_t nread;
	wifi_associated_dev_t *dev=NULL;
	*associated_dev_array = NULL;
	sprintf(cmd, "hostapd_cli -p /var/run/hostapd%d all_sta | grep AUTHORIZED | wc -l" , apIndex);
	_syscmd(cmd,buf,sizeof(buf));
	*output_array_size = atoi(buf);

	if (*output_array_size <= 0)
		return RETURN_OK;

	dev=(wifi_associated_dev_t *) calloc (*output_array_size, sizeof(wifi_associated_dev_t));
	*associated_dev_array = dev;
	sprintf(cmd, "hostapd_cli -p /var/run/hostapd%d all_sta > /tmp/connected_devices.txt" , apIndex);
	_syscmd(cmd,buf,sizeof(buf));
	f = fopen("/tmp/connected_devices.txt", "r");
	if (f==NULL)
	{
		*output_array_size=0;
		return RETURN_ERR;
	}
	while ((nread = getline(&line, &len, f)) != -1)
	{
		param = strtok(line,"=");
		value = strtok(NULL,"=");

		if( strcmp("flags",param) == 0 )
		{
			value[strlen(value)-1]='\0';
			if(strstr (value,"AUTHORIZED") != NULL )
			{
				dev[auth_temp].cli_AuthenticationState = 1;
				dev[auth_temp].cli_Active = 1;
				auth_temp++;
				read_flag=1;
			}
		}
		if(read_flag==1)
		{
			if( strcmp("dot11RSNAStatsSTAAddress",param) == 0 )
			{
				value[strlen(value)-1]='\0';
				sscanf(value, "%x:%x:%x:%x:%x:%x",
						(unsigned int *)&dev[mac_temp].cli_MACAddress[0],
						(unsigned int *)&dev[mac_temp].cli_MACAddress[1],
						(unsigned int *)&dev[mac_temp].cli_MACAddress[2],
						(unsigned int *)&dev[mac_temp].cli_MACAddress[3],
						(unsigned int *)&dev[mac_temp].cli_MACAddress[4],
						(unsigned int *)&dev[mac_temp].cli_MACAddress[5] );
				mac_temp++;
				read_flag=0;
			}
		}
	}
	*output_array_size = auth_temp;
	auth_temp=0;
	mac_temp=0;
	free(line);
	fclose(f);
	return RETURN_OK;
}

//Device.WiFi.X_RDKCENTRAL-COM_BandSteering object
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.Capability bool r/o
//To get Band Steering Capability
INT wifi_getBandSteeringCapability(BOOL *support) {
	*support=FALSE;
	return RETURN_OK;
}


//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.Enable bool r/w
//To get Band Steering enable status
INT wifi_getBandSteeringEnable(BOOL *enable) {
	*enable=FALSE;
	return RETURN_OK;
}

//To turn on/off Band steering
INT wifi_setBandSteeringEnable(BOOL enable) {
	
	return RETURN_OK;
}


//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.UtilizationThreshold int r/w
//to set and read the band steering BandUtilizationThreshold parameters 
INT wifi_getBandSteeringBandUtilizationThreshold (INT radioIndex, INT *pBuThreshold){
	
	return RETURN_ERR;
}

INT wifi_setBandSteeringBandUtilizationThreshold (INT radioIndex, INT buThreshold){
	
	return RETURN_ERR;
}

//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.RSSIThreshold int r/w
//to set and read the band steering RSSIThreshold parameters 
INT wifi_getBandSteeringRSSIThreshold (INT radioIndex, INT *pRssiThreshold){
	
	return RETURN_ERR;
}

INT wifi_setBandSteeringRSSIThreshold (INT radioIndex, INT rssiThreshold){
	
	return RETURN_ERR;
}


//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.PhyRateThreshold int r/w
//to set and read the band steering physical modulation rate threshold parameters 
INT wifi_getBandSteeringPhyRateThreshold (INT radioIndex, INT *pPrThreshold) { //If chip is not support, return -1
	
	return RETURN_ERR;
}

INT wifi_setBandSteeringPhyRateThreshold (INT radioIndex, INT prThreshold) { //If chip is not support, return -1
	
	return RETURN_ERR;
}


//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.History string r/o
//pClientMAC[64]
//pSourceSSIDIndex[64]
//pDestSSIDIndex[64]
//pSteeringReason[256]
INT wifi_getBandSteeringLog(INT record_index, ULONG *pSteeringTime, CHAR *pClientMAC, INT *pSourceSSIDIndex, INT *pDestSSIDIndex, INT *pSteeringReason) { //if no steering or redord_index is out of boundary, return -1. pSteeringTime returns the UTC time in seconds. pClientMAC is pre allocated as 64bytes. pSteeringReason returns the predefined steering trigger reason 
	*pSteeringTime=1454685924;
	strcpy(pClientMAC, "14:CF:E2:13:CD:AE");
	strcpy(pSourceSSIDIndex, "ath0");
	strcpy(pSourceSSIDIndex, "ath1");
	snprintf(pSteeringReason, 256, "RSSIThreshold=%d; RSSI=%d", 30, 35);
	return RETURN_OK;
}

INT wifi_ifConfigUp(INT apIndex)
{
    char cmd[128];
	char buf[1024];  

    snprintf(cmd, sizeof(cmd), "ifconfig %s%d up 2>/dev/null", AP_PREFIX, apIndex);
    _syscmd(cmd, buf, sizeof(buf));
    return 0;
}

//>> Deprecated. Replace with wifi_applyRadioSettings
INT wifi_pushBridgeInfo(INT apIndex)
{
    char ip[32]; 
    char subnet[32]; 
    char bridge[32];
    int vlanId;
    char cmd[128];
    char buf[1024];

    wifi_getApBridgeInfo(apIndex,bridge,ip,subnet);
    wifi_getApVlanID(apIndex,&vlanId);

    snprintf(cmd, sizeof(cmd), "cfgVlan %s%d %s %d %s ", AP_PREFIX, apIndex, bridge, vlanId, ip);
    _syscmd(cmd,buf, sizeof(buf));

    return 0;
}

INT wifi_pushRadioChannel(INT radioIndex,UINT channel)
{
      //Apply wifi_pushRadioChannel() instantly
     return RETURN_OK;
}

INT wifi_pushChannel(INT radioIndex, UINT channel)
{
    char cmd[128];
    char buf[1024];
	int  apIndex;
	
	apIndex=(radioIndex==0)?0:1;	
	snprintf(cmd, sizeof(cmd), "iwconfig %s%d freq %d",AP_PREFIX, apIndex,channel);
	_syscmd(cmd,buf, sizeof(buf));

    return 0;
}

INT wifi_pushChannelMode(INT radioIndex)
{
	//Apply Channel mode, pure mode, etc that been set by wifi_setRadioChannelMode() instantly
	return RETURN_ERR;
}

INT wifi_pushDefaultValues(INT radioIndex)
{
    //Apply Comcast specified default radio settings instantly
	//AMPDU=1
	//AMPDUFrames=32
	//AMPDULim=50000
	//txqueuelen=1000
    
    return RETURN_ERR;
}

INT wifi_pushTxChainMask(INT radioIndex)
{
	//Apply default TxChainMask instantly
	return RETURN_ERR;
}

INT wifi_pushRxChainMask(INT radioIndex)
{
	//Apply default RxChainMask instantly
	return RETURN_ERR;
}
//<<

INT wifi_pushSSID(INT apIndex, CHAR *ssid)
{
    char cmd[128];
    char buf[1024];
    
	snprintf(cmd, sizeof(cmd), "iwconfig %s%d essid \"%s\"",AP_PREFIX, apIndex, ssid);
    _syscmd(cmd, buf, sizeof(buf));

    return RETURN_OK;
}

INT wifi_pushSsidAdvertisementEnable(INT apIndex, BOOL enable)
{
    //Apply default Ssid Advertisement instantly

    return RETURN_ERR;
}

INT wifi_getRadioUpTime(INT radioIndex, ULONG *output)
{
	INT status = RETURN_ERR;
	*output = 0;
	return RETURN_ERR;
}

INT wifi_getApEnableOnLine(INT wlanIndex, BOOL *enabled)
{
   return RETURN_OK;
}

INT wifi_getApSecurityWpaRekeyInterval(INT apIndex, INT *output_int)
{
   return RETURN_OK;
}

INT wifi_getRadioAutoChannelEnable(INT radioIndex, BOOL *output_bool)
{
   return RETURN_OK;
}

INT wifi_getRouterEnable(INT wlanIndex, BOOL *enabled)
{
   return RETURN_OK;
}

INT wifi_setApSecurityWpaRekeyInterval(INT apIndex, INT *rekeyInterval)
{
   return RETURN_OK;
}

INT wifi_setRouterEnable(INT wlanIndex, INT *RouterEnabled)
{
   return RETURN_OK;
}

INT wifi_getApIndexForWiFiBand(wifi_band band)
{
    char cmd[128] = {0};
    char buf[1028] = {0};
    int  apIndex = -1;

    _syscmd("iwconfig wlan0|grep 802.11a", buf, sizeof(buf));
    if( strlen(buf) > 0 )
    {
        apIndex = ( band_2_4 == band ) ? 1: 0;
    }
    else
    {
        apIndex = ( band_2_4 == band ) ? 0 : 1;
    }

    wifi_dbg_printf("AP Index for band %d is %d", band, apIndex );
    return apIndex;
}

INT wifi_getRadioSupportedDataTransmitRates(INT wlanIndex,CHAR *output)
{
	struct params params={"hw_mode",""};

	if (NULL == output)
		return RETURN_ERR;

	wifi_hostapdRead(wlanIndex,&params,output);

	if(strcmp(output,"b")==0)
	{
		sprintf(output, "%s", "1,2,5.5,11");
	}
	else if (strcmp(output,"a")==0)
	{
		sprintf(output, "%s", "6,9,11,12,18,24,36,48,54");
	}
	else if ((strcmp(output,"n")==0) | (strcmp(output,"g")==0))
	{
		sprintf(output, "%s", "1,2,5.5,6,9,11,12,18,24,36,48,54");
	}
	return RETURN_OK;
}


INT wifi_getRadioOperationalDataTransmitRates(INT wlanIndex,CHAR *output)
{
	char *temp;
	char temp_output[128];
	char temp_TransmitRates[128];
	struct params params={"supported_rates",""};

	if (NULL == output)
		return RETURN_ERR;

	wifi_hostapdRead(wlanIndex,&params,output);

	strcpy(temp_TransmitRates,output);
	strcpy(temp_output,"");
	temp = strtok(temp_TransmitRates," ");
	while(temp!=NULL)
	{
		temp[strlen(temp)-1]=0;
		if((temp[0]=='5') && (temp[1]=='\0'))
		{
			temp="5.5";
		}
		strcat(temp_output,temp);
		temp = strtok(NULL," ");
		if(temp!=NULL)
		{
			strcat(temp_output,",");
		}
	}
	strcpy(output,temp_output);
	return RETURN_OK;
}

INT wifi_setRadioSupportedDataTransmitRates(INT wlanIndex,CHAR *output)
{
        return RETURN_OK;
}


INT wifi_setRadioOperationalDataTransmitRates(INT wlanIndex,CHAR *output)
{
	int i=0;
	char *temp;
	char temp1[128];
	char temp_output[128];
	char temp_TransmitRates[128];

        if(NULL == output)
            return RETURN_ERR;

	strcpy(temp_TransmitRates,output);

	for(i=0;i<strlen(temp_TransmitRates);i++)
	{
		if (((temp_TransmitRates[i]>='0') && (temp_TransmitRates[i]<='9')) | (temp_TransmitRates[i]==' ') | (temp_TransmitRates[i]=='.'))
		{
			continue;
		}
		else
		{
			return RETURN_ERR;
		}
	}
	strcpy(temp_output,"");
	temp = strtok(temp_TransmitRates," ");
	while(temp!=NULL)
	{
		strcpy(temp1,temp);
		if(wlanIndex==1)
		{
			if((strcmp(temp,"1")==0) | (strcmp(temp,"2")==0) | (strcmp(temp,"5.5")==0))
			{
				return RETURN_ERR;
			}
		}

		if(strcmp(temp,"5.5")==0)
		{
			strcpy(temp1,"55");
		}
		else
		{
			strcat(temp1,"0");
		}
		strcat(temp_output,temp1);
		temp = strtok(NULL," ");
		if(temp!=NULL)
		{
			strcat(temp_output," ");
		}
	}
	strcpy(output,temp_output);

	char buf[127]={'\0'};
	struct params params={'\0'};
	param_list_t list;
	strcpy(params.name,"supported_rates");
	strncpy(params.value,output,strlen(output));
	wifi_dbg_printf("\n%s:",__func__);
	wifi_dbg_printf("params.value=%s\n",params.value);
	memset(&list,0,sizeof(list));
	if (RETURN_ERR == list_add_param(&list,params))
	{
		return RETURN_ERR;
	}
	wifi_hostapdWrite(wlanIndex,&list);
	list_free_param(&list);
	return RETURN_OK;
}

INT wifi_getApManagementFramePowerControl(INT wlanIndex, INT *ManagementFramePowerControl)
{
   return RETURN_OK;
}

INT wifi_setApManagementFramePowerControl(INT wlanIndex, INT *ManagementFramePowerControl)
{
   return RETURN_OK;
}
INT wifi_getRadioDcsChannelMetrics(INT radioIndex,wifi_channelMetrics_t channel_matrix[],size_t size)
{
   return RETURN_OK;
}
INT wifi_setRadioDcsDwelltime(INT radioIndex, INT ms)
{
	return RETURN_OK;
}
INT wifi_getRadioDcsDwelltime(INT radioIndex, INT *ms)
{
	return RETURN_OK;
}
#ifdef _WIFI_HAL_TEST_
int main(int argc,char **argv)
{
	int index;
	INT ret=0;
    if(argc <= 1) {
        printf("help\n");
        //fprintf(stderr,"%s", commands_help);

        exit(-1);
    } 

    if(strstr(argv[1], "init")!=NULL) {
        return wifi_init();
    }
    else if(strstr(argv[1], "reset")!=NULL) {
        return wifi_reset();
    }    
	
	index = atoi(argv[2]);
    if(strstr(argv[1], "getApEnable")!=NULL) {
        BOOL enable;
		ret=wifi_getApEnable(index, &enable);
        printf("%s %d: %d, returns %d\n", argv[1], index, enable, ret);
    }
    else if(strstr(argv[1], "setApEnable")!=NULL) {
        BOOL enable = atoi(argv[3]);
        ret=wifi_setApEnable(index, enable);
        printf("%s %d: %d, returns %d\n", argv[1], index, enable, ret);
    }
    else if(strstr(argv[1], "getApStatus")!=NULL) {
        char status[64]; 
        ret=wifi_getApStatus(index, status);
        printf("%s %d: %s, returns %d\n", argv[1], index, status, ret);
    }
    else if(strstr(argv[1], "getSSIDTrafficStats2")!=NULL) {
		wifi_ssidTrafficStats2_t stats={0};
		ret=wifi_getSSIDTrafficStats2(index, &stats); //Tr181
		printf("%s %d: returns %d\n", argv[1], index, ret);
		printf("     ssid_BytesSent             =%lu\n", stats.ssid_BytesSent);
		printf("     ssid_BytesReceived         =%lu\n", stats.ssid_BytesReceived);
		printf("     ssid_PacketsSent           =%lu\n", stats.ssid_PacketsSent);
		printf("     ssid_PacketsReceived       =%lu\n", stats.ssid_PacketsReceived);
		printf("     ssid_RetransCount          =%lu\n", stats.ssid_RetransCount);
		printf("     ssid_FailedRetransCount    =%lu\n", stats.ssid_FailedRetransCount);
		printf("     ssid_RetryCount            =%lu\n", stats.ssid_RetryCount);
		printf("     ssid_MultipleRetryCount    =%lu\n", stats.ssid_MultipleRetryCount);
		printf("     ssid_ACKFailureCount       =%lu\n", stats.ssid_ACKFailureCount);
		printf("     ssid_AggregatedPacketCount =%lu\n", stats.ssid_AggregatedPacketCount);
		printf("     ssid_ErrorsSent            =%lu\n", stats.ssid_ErrorsSent);
		printf("     ssid_ErrorsReceived        =%lu\n", stats.ssid_ErrorsReceived);
		printf("     ssid_UnicastPacketsSent    =%lu\n", stats.ssid_UnicastPacketsSent);
		printf("     ssid_UnicastPacketsReceived    =%lu\n", stats.ssid_UnicastPacketsReceived);
		printf("     ssid_DiscardedPacketsSent      =%lu\n", stats.ssid_DiscardedPacketsSent);
		printf("     ssid_DiscardedPacketsReceived  =%lu\n", stats.ssid_DiscardedPacketsReceived);
		printf("     ssid_MulticastPacketsSent      =%lu\n", stats.ssid_MulticastPacketsSent);
		printf("     ssid_MulticastPacketsReceived  =%lu\n", stats.ssid_MulticastPacketsReceived);
		printf("     ssid_BroadcastPacketsSent      =%lu\n", stats.ssid_BroadcastPacketsSent);
		printf("     ssid_BroadcastPacketsRecevied  =%lu\n", stats.ssid_BroadcastPacketsRecevied);
		printf("     ssid_UnknownPacketsReceived    =%lu\n", stats.ssid_UnknownPacketsReceived);
	}
	else if(strstr(argv[1], "getNeighboringWiFiDiagnosticResult2")!=NULL) {
		wifi_neighbor_ap2_t *neighbor_ap_array=NULL, *pt=NULL;
		UINT array_size=0;
		UINT i=0;
		ret=wifi_getNeighboringWiFiDiagnosticResult2(index, &neighbor_ap_array, &array_size);
		printf("%s %d: array_size=%d, returns %d\n", argv[1], index, array_size, ret);
		for(i=0, pt=neighbor_ap_array; i<array_size; i++, pt++) {	
			printf("  neighbor %d:\n", i);
			printf("     ap_SSID                =%s\n", pt->ap_SSID);
			printf("     ap_BSSID               =%s\n", pt->ap_BSSID);
			printf("     ap_Mode                =%s\n", pt->ap_Mode);
			printf("     ap_Channel             =%d\n", pt->ap_Channel);
			printf("     ap_SignalStrength      =%d\n", pt->ap_SignalStrength);
			printf("     ap_SecurityModeEnabled =%s\n", pt->ap_SecurityModeEnabled);
			printf("     ap_EncryptionMode      =%s\n", pt->ap_EncryptionMode);
			printf("     ap_SupportedStandards  =%s\n", pt->ap_SupportedStandards);
			printf("     ap_OperatingStandards  =%s\n", pt->ap_OperatingStandards);
			printf("     ap_OperatingChannelBandwidth   =%s\n", pt->ap_OperatingChannelBandwidth);
			printf("     ap_SecurityModeEnabled         =%s\n", pt->ap_SecurityModeEnabled);
			printf("     ap_BeaconPeriod                =%d\n", pt->ap_BeaconPeriod);
			printf("     ap_Noise                       =%d\n", pt->ap_Noise);
			printf("     ap_BasicDataTransferRates      =%s\n", pt->ap_BasicDataTransferRates);
			printf("     ap_SupportedDataTransferRates  =%s\n", pt->ap_SupportedDataTransferRates);
			printf("     ap_DTIMPeriod                  =%d\n", pt->ap_DTIMPeriod);
			printf("     ap_ChannelUtilization          =%d\n", pt->ap_ChannelUtilization);			
		}
		if(neighbor_ap_array)
			free(neighbor_ap_array); //make sure to free the list
	}
	else if(strstr(argv[1], "getApAssociatedDeviceDiagnosticResult")!=NULL) {
		wifi_associated_dev_t *associated_dev_array=NULL, *pt=NULL;
		UINT array_size=0;
		UINT i=0;
		ret=wifi_getApAssociatedDeviceDiagnosticResult(index, &associated_dev_array, &array_size);
		printf("%s %d: array_size=%d, returns %d\n", argv[1], index, array_size, ret);
		for(i=0, pt=associated_dev_array; i<array_size; i++, pt++) {	
			printf("  associated_dev %d:\n", i);
			printf("     cli_OperatingStandard      =%s\n", pt->cli_OperatingStandard);
			printf("     cli_OperatingChannelBandwidth  =%s\n", pt->cli_OperatingChannelBandwidth);
			printf("     cli_SNR                    =%d\n", pt->cli_SNR);
			printf("     cli_InterferenceSources    =%s\n", pt->cli_InterferenceSources);
			printf("     cli_DataFramesSentAck      =%lu\n", pt->cli_DataFramesSentAck);
			printf("     cli_DataFramesSentNoAck    =%lu\n", pt->cli_DataFramesSentNoAck);
			printf("     cli_BytesSent              =%lu\n", pt->cli_BytesSent);
			printf("     cli_BytesReceived          =%lu\n", pt->cli_BytesReceived);
			printf("     cli_RSSI                   =%d\n", pt->cli_RSSI);
			printf("     cli_MinRSSI                =%d\n", pt->cli_MinRSSI);
			printf("     cli_MaxRSSI                =%d\n", pt->cli_MaxRSSI);
			printf("     cli_Disassociations        =%d\n", pt->cli_Disassociations);
			printf("     cli_AuthenticationFailures =%d\n", pt->cli_AuthenticationFailures);
		}
		if(associated_dev_array)
			free(associated_dev_array); //make sure to free the list
	}
	return 0;
}
#endif
//<<
