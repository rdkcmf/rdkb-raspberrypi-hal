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

    copyright:

        Cisco Systems, Inc., 2014
        All Rights Reserved.

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
#include <string.h>
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

/* Dnsmasq.leases file is generated and have all information of client leases. We check for clients whether connected ,
 by checking dnsmaq.leases file  */

static int check_client_connected()
{
   int connected_client=0;
   FILE *fp;
   char buf[256] = {0};
   fp=popen("cat /nvram/dnsmasq.leases | awk '/10.0.0./ {print $3}' | wc -l","r");
   if(fgets(buf,sizeof(buf),fp)!= NULL)
      connected_client = atoi (buf);
   return connected_client;
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
  int cli_connected=0;
   cli_connected = check_client_connected();
    if(pValue==NULL || cli_connected == 0)
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
  int cli_connected = 0;
  cli_connected = check_client_connected();
    if (NULL == pValue || cli_connected == 0) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
	FILE *fr;
	char *ptemp=pValue;
        char buf[256] = {0};
        fr= popen("head -n1 /nvram/dnsmasq.leases | awk '/10.0.0./ {print $3}'","r");
        fgets(buf,sizeof(buf),fr);
	sscanf(buf,"%d.%d.%d.%d",ptemp,ptemp+1,ptemp+2,ptemp+3);
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
  int cli_connected = 0;
  cli_connected = check_client_connected();
    if (NULL == pValue || cli_connected == 0) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
        FILE *fr;
        char *ptemp=pValue;
        char buf[256] = {0};
        fr= popen("cat /var/dnsmasq.conf | awk '/dhcp-range/ {print $1}' | cut -d',' -f3","r");
        fgets(buf,sizeof(buf),fr);
        sscanf(buf,"%d.%d.%d.%d",ptemp,ptemp+1,ptemp+2,ptemp+3);
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
  int cli_connected = 0;
  cli_connected = check_client_connected();
    if(pValue==NULL || cli_connected == 0)
    {    
       return(STATUS_FAILURE);
    }
    else
    {
        char buf[256] = {0};
        FILE *fr;
        char *ptemp=pValue;
        fr= popen("syscfg get lan_ipaddr","r");
        fgets(buf,sizeof(buf),fr);
        sscanf(buf,"%d.%d.%d.%d",ptemp,ptemp+1,ptemp+2,ptemp+3);
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
  int cli_connected = 0;
  cli_connected = check_client_connected();
    if (NULL == pValue || cli_connected == 0) 
    {    
        return STATUS_FAILURE;
    } 
    else 
    {
        FILE *fr;
        char buf[256] = {0};
        char *ptemp=pValue;
        fr= popen("syscfg get lan_ipaddr","r");
        fgets(buf,sizeof(buf),fr);
        sscanf(buf,"%d.%d.%d.%d",ptemp,ptemp+1,ptemp+2,ptemp+3);
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



