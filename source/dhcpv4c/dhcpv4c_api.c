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
   Copyright [2014] [Cisco Systems, Inc.]
 
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

    module: dhcpv4c_api.c

        For CCSP Component: DHCPV4-Client Status

    ---------------------------------------------------------------

    description:

        This header file gives the function call prototypes and 
        structure definitions used for the RDK-Broadband 
        DHCPv4 Client Status abstraction layer

        NOTE:
        THIS VERSION IS AN EARLY DRAFT INTENDED TO GET COMMENTS FROM COMCAST.
        TESTING HAS NOT YET BEEN COMPLETED.  
       
    ---------------------------------------------------------------

    environment:

    ---------------------------------------------------------------

    author:

        Cisco

**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dhcp4cApi.h"
#include "dhcpv4c_api.h"
/**********************************************************************************
 *
 *  DHCPV4-Client Subsystem level function definitions
 *
**********************************************************************************/

#ifdef DEBUG_QUERY_ALL
void query_all();
static int query_all_in_progress = 0;
#endif

#define UPTIME_FILE_PATH        "/proc/uptime"
#define MAX_LINE_SIZE           64
static int dhcpv4c_get_up_time(unsigned int *up_time)
{
    FILE *fp;
    char line[MAX_LINE_SIZE];
    char *ret_val;
    unsigned int upTime = 0;
   
   /* This file contains two numbers:
    * the uptime of the system (seconds), and the amount of time spent in idle process (seconds). 
    * We care only for the first one */
    fp = fopen( UPTIME_FILE_PATH, "r");
    if (fp == NULL)
    {
        return -1;
    }
  
    ret_val = fgets(line,MAX_LINE_SIZE,fp);
    fclose(fp);

    if (ret_val == NULL)
    {
        return -1;
    }

    /* Extracting the first token (number of up-time in seconds). */
    ret_val = strtok (line," .");

    /* we need only the number of seconds */
    upTime += atoi(ret_val);

    *up_time = upTime;

    return 0;
}

/* dhcpv4c_get_ert_lease_time() function */
/**
* @description Gets the E-Router Offered Lease Time
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_lease_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
	char buf[256] = {0};
        FILE *fr=NULL;
        fr= popen("sysevent get ipv4_erouter0_lease_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
	*pValue = atoi(buf);
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_lease_time(pValue);
    }
}
 
/* dhcpv4c_get_ert_remain_lease_time() function */
/**
* @description Gets the E-Router Remaining Lease Time
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_remain_lease_time(UINT *pValue)
{       
    if(pValue==NULL)
    {
       return(STATUS_FAILURE);
    }
    else
    {
	char buf[256] = {0};
        FILE *fr=NULL;
	unsigned lease_time = 0, start_time = 0, up_time = 0, remain_lease_time = 0;
        fr= popen("sysevent get ipv4_erouter0_lease_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
	lease_time=atoi(buf);
	memset(buf,0,sizeof(buf));
        fr= popen("sysevent get ipv4_erouter0_start_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
	start_time=atoi(buf);
	dhcpv4c_get_up_time(&up_time);
	remain_lease_time = lease_time - (up_time - start_time);
	*pValue = remain_lease_time;
        return STATUS_SUCCESS;
       //return dhcp4c_get_ert_remain_lease_time(pValue);
    }
}

/* dhcpv4c_get_ert_remain_renew_time() function */
/**
* @description Gets the E-Router Interface Remaining Time to Renew
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_remain_renew_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
	char buf[256] = {0};
        FILE *fr=NULL;
        unsigned lease_time = 0, start_time = 0, up_time = 0, remain_renew_time = 0,renew_time = 0;
        fr= popen("sysevent get ipv4_erouter0_lease_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
        lease_time=atoi(buf);
        renew_time = lease_time/2;
        memset(buf,0,sizeof(buf));
        fr= popen("sysevent get ipv4_erouter0_start_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
        start_time=atoi(buf);
        dhcpv4c_get_up_time(&up_time);
        remain_renew_time = renew_time - (up_time - start_time);
        *pValue = remain_renew_time;
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_remain_renew_time(pValue);
    }
}

/* dhcpv4c_get_ert_remain_rebind_time() function */
/**
* @description Gets the E-Router Interface Remaining Time to Rebind
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_remain_rebind_time(UINT *pValue)
{
    if (NULL == pValue) 
    { 
        return STATUS_FAILURE;
    } 
    else 
    {
	char buf[256] = {0};
        FILE *fr=NULL;
        unsigned lease_time = 0, start_time = 0, up_time = 0, remain_rebind_time = 0,rebind_time = 0;
        fr= popen("sysevent get ipv4_erouter0_lease_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
        lease_time=atoi(buf);
	rebind_time = lease_time*7/8;
        memset(buf,0,sizeof(buf));
        fr= popen("sysevent get ipv4_erouter0_start_time","r");
        fgets(buf,sizeof(buf),fr);
        pclose(fr);
        start_time=atoi(buf);
        dhcpv4c_get_up_time(&up_time);
	remain_rebind_time = rebind_time - (up_time - start_time);
        *pValue = remain_rebind_time;
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_remain_rebind_time(pValue);
    }
}

/* dhcpv4c_get_ert_config_attempts() function */
/**
* @description Gets the E-Router Number of Attemts to Configure.
* @param
*    pValue - Count.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_config_attempts(INT *pValue)
{
    if (NULL == pValue) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
	*pValue = 100;
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_config_attempts(pValue);
    }
}

/* dhcpv4c_get_ert_ifname() function */
/**
* @description Gets the E-Router Interface Name.
* @param
*    pName - Interface Name (e.g. ert0)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_ifname(CHAR *pName)
{
    if (NULL == pName) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
	FILE *fr = NULL;
	char buf[128] = {0},temp_buf[128] = {0};
	int count=0;
        fr= popen("sysevent get current_wan_ifname","r");
        fgets(buf,sizeof(buf),fr);
	for(count=0;buf[count]!='\n';count++)
		temp_buf[count]=buf[count];
	temp_buf[count]='\0';
	strcpy(pName,temp_buf);
	pclose(fr);
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_ifname(pName);
    }
}

/* dhcpv4c_get_ert_fsm_state() function */
/**
* @description Gets the E-Router DHCP State
* @param
*    pValue - State of the DHCP (RENEW/ACQUIRED etc.)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
/*
Retrieves the status of client,The ip address are assigned to the client by dhcp-server and puts the state in bound, if the IP is not assigned, by default it is in Init state 

Init                  = 1
Selecting             = 2
Bound                 = 5

*/
INT dhcpv4c_get_ert_fsm_state(INT *pValue)
{
    if(pValue==NULL)
    {    
           *pValue = 1;
           return STATUS_FAILURE;
    }
    else
    {
           *pValue = 5; //5 indicates that status is bound. Client gets Ip addrs and changes its state to bound.
            return STATUS_SUCCESS;
       //return dhcp4c_get_ert_fsm_state(pValue);
    }
}

/* dhcpv4c_get_ert_ip_addr() function */
/**
* @description Gets the E-Router Interface IP Address
* @param
*    pValue - IP Address (of the Interface)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_ip_addr(UINT *pValue)
{
    if (NULL == pValue ) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
	char buf[256] = {0};
        FILE *fr=NULL;
	struct in_addr addr;
        fr= popen("sysevent get ipv4_erouter0_ipaddr","r");
        fgets(buf,sizeof(buf),fr);
	pclose(fr);
	inet_aton(buf, &addr);
	*pValue = addr.s_addr;
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_ip_addr(pValue);
    }
}

/* dhcpv4c_get_ert_mask() function */
/**
* @description Gets the E-Router Subnet Mask.
* @param
*    pValue - Subnet Mask (bitmask)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_mask(UINT *pValue)
{
    if (NULL == pValue) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
        FILE *fr;
        char buf[256] = {0};
	unsigned mask = 0;
        fr= popen("sysevent get ipv4_erouter0_subnet","r");
        fgets(buf,sizeof(buf),fr);
	pclose(fr);
	mask=atoi(buf);
	mask = (0xFFFFFFFF << (32 - mask)) & 0xFFFFFFFF;
        *pValue = htonl(mask);
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_mask(pValue);
    }
}

/* dhcpv4c_get_ert_gw() function */
/**
* @description Gets the E-Router Gateway IP Address
* @param
*    pValue - IP Address (of the Gateway)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_gw(UINT *pValue)
{
    if(pValue==NULL)
    {    
       return(STATUS_FAILURE);
    }
    else
    {
        char buf[256] = {0};
        FILE *fr;
	struct in_addr addr;
        fr= popen("sysevent get default_router","r");
        fgets(buf,sizeof(buf),fr);
	pclose(fr);
	inet_aton(buf, &addr);
        *pValue = addr.s_addr;
        return STATUS_SUCCESS;
       //return dhcp4c_get_ert_gw(pValue);
    }
}

/* dhcpv4c_get_ert_dns_svrs() function */
/**
* @description Gets the E-Router List of DNS Servers
* @param
*    pList - List of IP Address (of DNS Servers)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_dns_svrs(dhcpv4c_ip_list_t *pList)
{
	if (NULL == pList) 
	{    
		return STATUS_FAILURE;
	} 
	else 
	{
		char buf[256] = {0};;
		struct in_addr addr;
		FILE *fr=NULL;
		unsigned dns_num = 0;

		fr= popen("sysevent get ipv4_erouter0_dns_number","r");
		fgets(buf,sizeof(buf),fr);
		pclose(fr);

		dns_num = atoi(buf);

		if (dns_num >= 1) {
			int i = 0;

			if (dns_num >4)
				dns_num = 4;

			for (i=0; i<dns_num; i++) {
				char gw_str[32];
				memset(gw_str, 0, sizeof(gw_str));
				sprintf(gw_str,"sysevent get ipv4_erouter0_dns_%d",i);
				memset(buf,0,sizeof(buf));
				fr= popen(gw_str,"r");
				fgets(buf,sizeof(buf),fr);
				pclose(fr);
				inet_aton(buf, &addr);
				pList->addrs[i] = addr.s_addr;
			}
			pList->number = dns_num;
		} else {
			pList->number = 0;
		}
		return STATUS_SUCCESS;
		//return dhcp4c_get_ert_dns_svrs((ipv4AddrList_t*) pList);
	}
}

/* dhcpv4c_get_ert_dhcp_svr() function */
/**
* @description Gets the E-Router DHCP Server IP Address
* @param
*    pValue - IP Address (of DHCP Server)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ert_dhcp_svr(UINT *pValue)
{
    if (NULL == pValue) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
        FILE *fr;
        char buf[256] = {0};
	struct in_addr addr;
        fr= popen("sysevent get ipv4_erouter0_dhcp_server","r");
        fgets(buf,sizeof(buf),fr);
	pclose(fr);
	inet_aton(buf, &addr);
	*pValue = addr.s_addr;
        return STATUS_SUCCESS;
        //return dhcp4c_get_ert_dhcp_svr(pValue);
    }
}

/* dhcpv4c_get_ecm_lease_time() function */
/**
* @description Gets the ECM Offered Lease Time.
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_lease_time(UINT *pValue)
{
    if (NULL == pValue) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_lease_time(pValue);
    }
}

/* dhcpv4c_get_ecm_remain_lease_time() function */
/**
* @description Gets the ECM Remaining Lease Time
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_remain_lease_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_remain_lease_time(pValue);
    }
}

/* dhcpv4c_get_ecm_remain_renew_time() function */
/**
* @description Gets the ECM Interface Remaining time to Renew.
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_remain_renew_time(UINT *pValue)
{
    if (NULL == pValue) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_remain_renew_time(pValue);
    }
}

/* dhcpv4c_get_ecm_remain_rebind_time() function */
/**
* @description Gets the ECM Interface Remaining time to Rebind.
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_remain_rebind_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_remain_rebind_time(pValue);
    }
}

/* dhcpv4c_get_ecm_config_attempts() function */
/**
* @description Gets the ECM Configuration Number of Attemts.
* @param
*    pValue - Count.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_config_attempts(INT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_config_attempts(pValue);
    }
}

/* dhcpv4c_get_ecm_ifname() function */
/**
* @description Gets the ECM Interface Name.
* @param
*    pName - Name of the Interface (e.g doc0)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_ifname(CHAR *pName)
{
    if (NULL == pName) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_ifname(pName);;        
    }
}

/* dhcpv4c_get_ecm_fsm_state() function */
/**
* @description Gets the ECM DHCP State
* @param
*    pValue - State of the DHCP (RENEW/ACQUIRED etc)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_fsm_state(INT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_fsm_state(pValue);
    }
}

/* dhcpv4c_get_ecm_ip_addr() function */
/**
* @description Gets the ECM Interface IP Address
* @param
*    pValue - IP Address of the Interface.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_ip_addr(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_ip_addr(pValue);
    }
}

/* dhcpv4c_get_ecm_mask() function */
/**
* @description Gets the ECM Interface Subnet Mask.
* @param
*    pValue - Subnet Mask (bitmask).
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_mask(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_mask(pValue);
    }
}

/* dhcpv4c_get_ecm_gw() function */
/**
* @description Gets the ECM Gateway IP Address
* @param
*    pValue - IP Address of Gateway
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_gw(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_gw(pValue);
    }
}

/* dhcpv4c_get_ecm_dns_svrs() function */
/**
* @description Gets the ECM List of DNS Servers
* @param
*    pList - List of IP Addresses (of DNS Servers)
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_dns_svrs(dhcpv4c_ip_list_t *pList)
{
    if (NULL == pList) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_dns_svrs((ipv4AddrList_t*) pList);
    }
}

/* dhcpv4c_get_ecm_dhcp_svr() function */
/**
* @description Gets the ECM DHCP Server IP Address
* @param
*    pValue - IP Address 
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_ecm_dhcp_svr(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_ecm_dhcp_svr(pValue);
    }
}


/* dhcpv4c_get_emta_remain_lease_time() function */
/**
* @description Gets the E-MTA interface Least Time
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_emta_remain_lease_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_emta_remain_lease_time(pValue);
    }
}

/* dhcpv4c_get_emta_remain_renew_time() function */
/**
* @description Gets the E-MTA interface Remaining Time to Renew
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_emta_remain_renew_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_emta_remain_renew_time(pValue);
    }
}

/* dhcpv4c_get_emta_remain_rebind_time() function */
/**
* @description Gets the E-MTA interface Remaining Time to Rebind
* @param
*    pValue - Value in Seconds.
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
INT dhcpv4c_get_emta_remain_rebind_time(UINT *pValue)
{
    if (NULL == pValue) 
    {
        return STATUS_FAILURE;
    } 
    else 
    {
        return STATUS_SUCCESS;
        //return dhcp4c_get_emta_remain_rebind_time(pValue);
    }
}

bool AnscValidStringCheck(char *pString)
{
	int i =0;

	/* check if pstring doesn't hold NULL or whitespaces */
	if((pString == NULL) || (*pString=='\0'))
	{
		return FALSE;
	}
	while(pString[i] != '\0')
	{
		if ((pString[i] == ' ') || (pString[i] == '<') || (pString[i] == '>') || (pString[i] == '&') || (pString[i] == '\'') || (pString[i] == '\"') || (pString[i] == '|'))
		{
			return FALSE;
		}
		i++;
	}
	return TRUE;

}

#if 0
//#ifdef DEBUG_QUERY_ALL
void query_all()
{
   int i;

   unsigned int Value;
   int iValue;
   char Name[100];
   dhcpv4c_ip_list_t List;
   
   unsigned int* pValue = &Value;
   int* piValue = &iValue;
   char* pName = &Name[0];
   dhcpv4c_ip_list_t*  pList = &List;
  
   int result;
   
   query_all_in_progress = 1;
   
   printf("Query all start\n");
   
   result = dhcpv4c_get_ert_lease_time(&Value);
   printf("dhcpv4_get_ert_lease_time - result=%d pValue = %d\n",  result, *pValue);
    
   result = dhcp4c_get_ert_remain_lease_time(pValue); 
   printf("dhcpv4_get_ert_remain_lease_time - result=%d pValue = %d\n",  result, *pValue);
    
   result = dhcpv4c_get_ert_remain_renew_time(pValue);
   printf("dhcpv4_get_ert_remain_renew_time - result=%d pValue = %d\n",  result, *pValue);
    
   result = dhcpv4c_get_ert_remain_rebind_time(pValue);
   printf("dhcpv4_get_ert_remain_rebind_time - result=%d pValue = %d\n",  result, *pValue);
    
   result = dhcpv4c_get_ert_config_attempts(piValue);
   printf("dhcpv4_get_ert_config_attempts - result=%d piValue = %d\n",  result, *piValue);
    
   result = dhcpv4c_get_ert_ifname(pName);
   printf("dhcpv4_get_ert_ifname - result=%d pName = [%s]\n",  result, pName);
    
   result = dhcpv4c_get_ert_fsm_state(piValue);
   printf("dhcpv4_get_ert_fsm_state - result=%d piValue = %d\n",  result, *piValue);
    
   result = dhcpv4c_get_ert_ip_addr(pValue);
   printf("dhcpv4_get_ert_ip_addr - result=%d pValue = %04X\n",  result, *pValue);
    
   result = dhcpv4c_get_ert_mask(pValue);
   printf("dhcpv4_get_ert_mask - result=%d pValue = %04X\n",  result, *pValue);
    
   result = dhcpv4c_get_ert_gw(pValue);
   printf("dhcpv4_get_ert_gw - result=%d pValue = %04X\n",  result, *pValue);
    
   result = dhcpv4c_get_ert_dns_svrs(pList);
   printf("dhcpv4_get_ert_dns_svrs - result=%d num_servers = %d\n",  result, pList->number);
   for (i=0;i<pList->number;i++)
   {
      printf("    server [%d] = %04X\n", i, pList->addrs[i]);
   }
   
   result = dhcpv4c_get_ert_dhcp_svr(pValue);
   printf("dhcpv4_get_ert_dhcp_svr - result=%d pValue = %04X\n",  result, *pValue);
 
   result = dhcpv4c_get_ecm_lease_time(pValue);
   printf("dhcpv4_get_ecm_lease_time - result=%d pValue = %d\n",  result, *pValue); 
    
   result = dhcpv4c_get_ecm_remain_lease_time(pValue);
   printf("dhcpv4_get_ecm_remain_lease_time - result=%d pValue = %d\n",  result, *pValue);  
    
   result = dhcpv4c_get_ecm_remain_renew_time(pValue);
   printf("dhcpv4_get_ecm_remain_renew_time - result=%d pValue = %d\n",  result, *pValue);  
    
   result = dhcpv4c_get_ecm_remain_rebind_time(pValue);
   printf("dhcpv4_get_ecm_remain_rebind_time - result=%d pValue = %d\n",  result, *pValue);  
    
   result = dhcpv4c_get_ecm_config_attempts(piValue);
   printf("dhcpv4_get_ecm_config_attempts - result=%d piValue = %d\n",  result, *piValue);
    
   result = dhcpv4c_get_ecm_ifname(pName);
   printf("dhcpv4_get_ecm_ifname - result=%d pName = [%s]\n",  result, pName);
    
   result = dhcpv4c_get_ecm_fsm_state(piValue);
   printf("dhcpv4_get_ecm_fsm_state - result=%d piValue = %d\n",  result, *piValue);
    
   result = dhcpv4c_get_ecm_ip_addr(pValue);
   printf("dhcpv4_get_ecm_ip_addr - result=%d pValue = %04X\n",  result, *pValue);
    
   result = dhcpv4c_get_ecm_mask(pValue);
   printf("dhcpv4_get_ecm_mask - result=%d pValue = %04X\n",  result, *pValue);
    
   result = dhcpv4c_get_ecm_gw(pValue);
   printf("dhcpv4_get_ecm_gw - result=%d pValue = %04X\n",  result, *pValue);
    
   result = dhcpv4c_get_ecm_dns_svrs(pList); 
   printf("dhcpv4_get_ecm_dns_svrs - result=%d num_servers = %d\n",  result, pList->number);
   for (i=0;i<pList->number;i++)
   {
      printf("    server [%d] = %04X\n", i, pList->addrs[i]);
   }
   
   result = dhcpv4c_get_ecm_dhcp_svr(pValue);
   printf("dhcpv4_get_ecm_dhcp_svr - result=%d pValue = %04X\n",  result, *pValue);
 
   result = dhcpv4c_get_emta_remain_lease_time(pValue);
   printf("dhcpv4_get_emta_remain_lease_time - result=%d pValue = %d\n",  result, *pValue);  
    
   result = dhcpv4c_get_emta_remain_renew_time(pValue);
   printf("dhcpv4_get_ecm_remain_renew_time - result=%d pValue = %d\n",  result, *pValue);  
    
   result = dhcpv4c_get_emta_remain_rebind_time(pValue);
   printf("dhcpv4_get_ecm_remain_rebind_time - result=%d pValue = %d\n",  result, *pValue);  
    
   printf("Query all end\n");
   
   query_all_in_progress = 0;
}

#endif



