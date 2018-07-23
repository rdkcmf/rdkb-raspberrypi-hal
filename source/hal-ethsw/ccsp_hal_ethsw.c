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

    File: ccsp_hal_ethsw.c

        For CCSP Component:  Ccsp Provisioning & Managment

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 2014
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This is the stub implementation of Ethernet Switch control.
       
    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Cisco


    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/28/2013  initial revision.

**********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccsp_hal_ethsw.h" 


/**********************************************************************
                    DEFINITIONS
**********************************************************************/

#define  CcspHalEthSwTrace(msg)                     printf("%s - ", __FUNCTION__); printf msg;

/**********************************************************************
                            MAIN ROUTINES
**********************************************************************/

CCSP_HAL_ETHSW_ADMIN_STATUS admin_status;

int is_interface_exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
        return 0;
}

/* CcspHalEthSwInit :  */
/**
* @description Do what needed to intialize the Eth hal.
* @param None
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
INT
CcspHalEthSwInit
    (
        void
    )
{
    return  RETURN_OK;
}


/* CcspHalEthSwGetPortStatus :  */
/**
* @description Retrieve the current port status -- link speed, duplex mode, etc.

* @param PortId      -- Port ID as defined in CCSP_HAL_ETHSW_PORT
* @param pLinkRate   -- Receives the current link rate, as in CCSP_HAL_ETHSW_LINK_RATE
* @param pDuplexMode -- Receives the current duplex mode, as in CCSP_HAL_ETHSW_DUPLEX_MODE
* @param pStatus     -- Receives the current link status, as in CCSP_HAL_ETHSW_LINK_STATUS

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
INT
CcspHalEthSwGetPortStatus
    (
        CCSP_HAL_ETHSW_PORT         PortId,
        PCCSP_HAL_ETHSW_LINK_RATE   pLinkRate,
        PCCSP_HAL_ETHSW_DUPLEX_MODE pDuplexMode,
        PCCSP_HAL_ETHSW_LINK_STATUS pStatus
    )
{
  char *path;
  path = (char *)malloc(20);
  path="/sys/class/net/eth1";

  int eth_if = is_interface_exists(path);

  if(!admin_status && eth_if)
       *pStatus  = CCSP_HAL_ETHSW_LINK_Up;
  else
       *pStatus   = CCSP_HAL_ETHSW_LINK_Down;


    switch (PortId)
    {
        case CCSP_HAL_ETHSW_EthPort1:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_100Mbps;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Full;
            break;
        }

        case CCSP_HAL_ETHSW_EthPort2:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_1Gbps;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Full;
            break;
        }

        case CCSP_HAL_ETHSW_EthPort3:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_NULL;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Auto;
            break;
        }

        case CCSP_HAL_ETHSW_EthPort4:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_NULL;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Auto;
            break;
        }

        default:
        {
            CcspHalEthSwTrace(("Unsupported port id %d\n", PortId));
            return  RETURN_ERR;
        }
    }
    return  RETURN_OK;
}


/* CcspHalEthSwGetPortCfg :  */
/**
* @description Retrieve the current port config -- link speed, duplex mode, etc.

* @param PortId      -- Port ID as defined in CCSP_HAL_ETHSW_PORT
* @param pLinkRate   -- Receives the current link rate, as in CCSP_HAL_ETHSW_LINK_RATE
* @param pDuplexMode -- Receives the current duplex mode, as in CCSP_HAL_ETHSW_DUPLEX_MODE

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
INT
CcspHalEthSwGetPortCfg
    (
        CCSP_HAL_ETHSW_PORT         PortId,
        PCCSP_HAL_ETHSW_LINK_RATE   pLinkRate,
        PCCSP_HAL_ETHSW_DUPLEX_MODE pDuplexMode
    )
{
    switch (PortId)
    {
        case CCSP_HAL_ETHSW_EthPort1:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_Auto;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Auto;

            break;
        }

        case CCSP_HAL_ETHSW_EthPort2:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_1Gbps;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Full;

            break;
        }

        case CCSP_HAL_ETHSW_EthPort3:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_100Mbps;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Auto;

            break;
        }

        case CCSP_HAL_ETHSW_EthPort4:
        {
            *pLinkRate      = CCSP_HAL_ETHSW_LINK_10Mbps;
            *pDuplexMode    = CCSP_HAL_ETHSW_DUPLEX_Half;

            break;
        }

        default:
        {
            CcspHalEthSwTrace(("Unsupported port id %d", PortId));
            return  RETURN_ERR;
        }
    }

    return  RETURN_OK;
}


/* CcspHalEthSwSetPortCfg :  */
/**
* @description Set the port configuration -- link speed, duplex mode

* @param PortId      -- Port ID as defined in CCSP_HAL_ETHSW_PORT
* @param LinkRate    -- Set the link rate, as in CCSP_HAL_ETHSW_LINK_RATE
* @param DuplexMode  -- Set the duplex mode, as in CCSP_HAL_ETHSW_DUPLEX_MODE

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
INT
CcspHalEthSwSetPortCfg
    (
        CCSP_HAL_ETHSW_PORT         PortId,
        CCSP_HAL_ETHSW_LINK_RATE    LinkRate,
        CCSP_HAL_ETHSW_DUPLEX_MODE  DuplexMode
    )
{
    CcspHalEthSwTrace(("set port %d LinkRate to %d, DuplexMode to %d", PortId, LinkRate, DuplexMode));

    switch (PortId)
    {
        case CCSP_HAL_ETHSW_EthPort1:
        {
            break;
        }

        case CCSP_HAL_ETHSW_EthPort2:
        {
            break;
        }

        case CCSP_HAL_ETHSW_EthPort3:
        {
            break;
        }

        case CCSP_HAL_ETHSW_EthPort4:
        {
            break;
        }

        default:
            CcspHalEthSwTrace(("Unsupported port id %d", PortId));
            return  RETURN_ERR;
    }

    return  RETURN_OK;
}


/* CcspHalEthSwGetPortAdminStatus :  */
/**
* @description Retrieve the current port admin status.

* @param PortId      -- Port ID as defined in CCSP_HAL_ETHSW_PORT
* @param pAdminStatus -- Receives the current admin status

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
INT
CcspHalEthSwGetPortAdminStatus
    (
        CCSP_HAL_ETHSW_PORT           PortId,
        PCCSP_HAL_ETHSW_ADMIN_STATUS  pAdminStatus
    )
{
 CcspHalEthSwTrace(("port id %d", PortId));
  int port_num = 0;
  FILE *fp;
  char port[2];
  char *path;
  path = (char *)malloc(20);
  path="/sys/class/net/eth1";

  int eth_if = is_interface_exists(path);

  if(eth_if==0)
  return  RETURN_ERR;

  fp= popen("ls -la /sys/class/net/ | awk '/eth1/{portId=$11}   END {print substr(portId,54,1)}'","r");
  fgets(port,sizeof(port),fp);
  port_num = atoi (port);
  port_num = port_num-1;

   switch (PortId)
    {
        case CCSP_HAL_ETHSW_EthPort1:
        case CCSP_HAL_ETHSW_EthPort2:
        case CCSP_HAL_ETHSW_EthPort3:
        case CCSP_HAL_ETHSW_EthPort4:
        {
        if(port_num==PortId)
             *pAdminStatus   = CCSP_HAL_ETHSW_AdminUp;
        else
             *pAdminStatus   = CCSP_HAL_ETHSW_AdminDown;
            break;
        }
        default:
            CcspHalEthSwTrace(("Unsupported port id %d", PortId));
            return  RETURN_ERR;
    }
if(admin_status)
       *pAdminStatus   = CCSP_HAL_ETHSW_AdminDown;

  return  RETURN_OK;
}

/* CcspHalEthSwSetPortAdminStatus :  */
/**
* @description Set the ethernet port admin status

* @param AdminStatus -- set the admin status, as defined in CCSP_HAL_ETHSW_ADMIN_STATUS

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
INT
CcspHalEthSwSetPortAdminStatus
    (
        CCSP_HAL_ETHSW_PORT         PortId,
        CCSP_HAL_ETHSW_ADMIN_STATUS AdminStatus
    )
{
    CcspHalEthSwTrace(("set port %d AdminStatus to %d", PortId, AdminStatus));

    char cmd1[50];
    char cmd2[50];
    int port_num=0;
    FILE *fp = NULL;
    char port[2];
    char *interface = NULL;;
    char *path = NULL;
    interface = (char *)malloc(5);
    path = (char *)malloc(20);
    strcpy(path,"/sys/class/net/eth1");

    int eth_if=is_interface_exists(path);

    if(eth_if == 0 )
        return  RETURN_ERR;

    fp= popen("ls -la /sys/class/net/ | awk '/eth1/{portId=$11}   END {print substr(portId,54,1)}'","r");
    fgets(port,sizeof(port),fp);
    port_num = atoi (port) - 1;
    strcpy(interface,"eth1");


    sprintf(cmd1,"ip link set %s up",interface);
    sprintf(cmd2,"ip link set %s down",interface);

    switch (PortId)
    {
        case CCSP_HAL_ETHSW_EthPort1:
        case CCSP_HAL_ETHSW_EthPort2:
        case CCSP_HAL_ETHSW_EthPort3:
        case CCSP_HAL_ETHSW_EthPort4:
        {
            if(port_num==PortId)
            {
                 if(AdminStatus==0)
                 {
                    system(cmd1);
                    admin_status=0;
                 }
                 else
                 {
                     system(cmd2);
                     admin_status=1;
                 }
             }
             break;
        }
        default:
            CcspHalEthSwTrace(("Unsupported port id %d", PortId));
            return  RETURN_ERR;
    }
    return  RETURN_OK;
}


/* CcspHalEthSwSetAgingSpeed :  */
/**
* @description Set the ethernet port configuration -- admin up/down, link speed, duplex mode

* @param PortId      -- Port ID as defined in CCSP_HAL_ETHSW_PORT
* @param AgingSpeed  -- integer value of aging speed
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
INT
CcspHalEthSwSetAgingSpeed
    (
        CCSP_HAL_ETHSW_PORT         PortId,
        INT                         AgingSpeed
    )
{
    CcspHalEthSwTrace(("set port %d aging speed to %d", PortId, AgingSpeed));

    return  RETURN_OK;
}


/* CcspHalEthSwLocatePortByMacAddress :  */
/**
* @description Retrieve the port number that the specificed MAC address is associated with (seen)

* @param pMacAddr    -- Specifies the MAC address -- 6 bytes
* @param pPortId     -- Receives the found port number that the MAC address is seen on

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
INT
CcspHalEthSwLocatePortByMacAddress
    (
		unsigned char * pMacAddr, 
		INT * pPortId
    )
{
    CcspHalEthSwTrace
        ((
            "%s -- search for MAC address %02u.%02u.%02u.%02u.%02u.%02u",
            __FUNCTION__,
            pMacAddr[0], pMacAddr[1], pMacAddr[2], 
            pMacAddr[3], pMacAddr[4], pMacAddr[5]
        ));

    *pPortId = CCSP_HAL_ETHSW_EthPort4;

    return  RETURN_OK;
}
