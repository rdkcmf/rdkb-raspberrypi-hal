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

/**********************************************************************

    module: rpi_wifi_hal_assoc_devices_details.c

    ---------------------------------------------------------------

    description:

        This api's implementation functions provides the detailed 
        informations of currently associated devices list with 
        the access point.

*************************************************************************/

/*******************************************************************/
/*************************   Header Files  ***********************/
/*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include "wifi_hal.h"

/*******************************************************************/
/**************************   define   ***************************/
/*******************************************************************/

#define MAX_BUF_SIZE 128
#define MAX_CMD_SIZE 1024
#define RPI_LEN_512  512
#define RPI_LEN_32   32 

#define wifi_dbg_printf printf

#define MACADDRESS_SIZE    6
#define MACSTR_BUF_LEN    18

#define CHECK_BUF_STATUS(a,b) \
        if((strlen(a) != 0) && (strlen(b) != 0)) {\
        memset(a,0,sizeof(a)); \
        memset(b,0,sizeof(b)); }
#define CHECK_BUFFER_STATUS(a) \
        if(strlen(a) != 0) { \
        memset(a,0,sizeof(a)); }

/*******************************************************************/
/*********************   Type Definition   **********************/
/*******************************************************************/

/* this struct contains all the data we need to fill the wifi hal structures*/
struct client_data {
        unsigned char mac_addr_str[MACADDRESS_SIZE];
        char sta_mode[64];
        char ip_addr[64];
        char OperatingChannelBandwidth[64];
        unsigned int auth;
        unsigned int auth_fails;
        unsigned int assoc;
        unsigned int assoc_fails;
        unsigned int deauth;
        unsigned int diassoc;
        int tx_allretries;
        int tx_retried;
        int tx_exceed_retry;
        int processed_rssi;
        int processed_snr;
        uint64_t tx_bytes;
        uint64_t tx_pkts;
        uint64_t tx_err;
        uint64_t tx_ucast;
        uint64_t rx_ucast;
        ULONG    tx_rate;
        ULONG    rx_rate;
        uint64_t rx_bytes;
        uint64_t rx_pkts;
        ULONG    DataFramesSentAck;
};

/*******************************************************************/
/********************* Function Declarations  **********************/
/*******************************************************************/

/*******************************************************************/
/********************* Function Declarations  **********************/
/*******************************************************************/

void rpi_adjusting_the_stringsize(char *src_str,char *dest_str);
int *rpi_wifi_get_clients_count(int *pcount,char *ifname,int apIndex);
int rpi_getApAssociatedDeviceDiagnosticResult(int apIndex, wifi_associated_dev_t **associated_dev_array, unsigned int *dev_cnt);
int rpi_getApAssociatedDeviceDiagnosticResult3(int apIndex, wifi_associated_dev3_t **associated_dev_array, unsigned int *dev_cnt);

/*******************************************************************/
/************** Static Function Declarations  **********************/
/*******************************************************************/

static int get_assoc_client_details2(char *data_buf,char *cmd,char *dev_data);
static int get_assoc_clients_data(const int version, char *ifname, int ai, struct client_data *dd,char *client_mac,int apIndex);
static int associated_clients_diagnostic_helper(const int version, int apIndex, void **out_wad_array, unsigned int *out_count);

/*******************************************************************/
/********************* Function Definitions  **********************/
/*******************************************************************/

/* rpi_adjusting_the_stringsize() function*/
/**
* Descrption:
*   Adjusting the string size from src string to destn string
*
* Parameters : src_str,dest_str
*
* @src_str source string(Input string)
* @destn_str Destination string,to be returned
*
**/

void rpi_adjusting_the_stringsize(char *src_str,char *dest_str)
{
    int count = 0;
    if(strlen(src_str) > 0)
    {
    for(count = 0;src_str[count]!='\n';count++)
        dest_str[count]=src_str[count];
    dest_str[count]='\0';
    }
}

/* rpi_wifi_get_clients_count() function */
/**
* Description:
*  Gets the associated clients count
* Parameters : *pcount,*ifname,apIndex
*
* @pcount Associated Clients count,to be returned
* @ifname interface name based on index
* @apIndex Access Point Index Number
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
**/

int *rpi_wifi_get_clients_count(int *pcount,char *ifname,int apIndex)
{
        const char cmd[MAX_CMD_SIZE]={0};
        char buf[MAX_BUF_SIZE] = {0};

        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s all_sta | grep dot11RSNAStatsSTAAddress | wc -l",apIndex,ifname);
        if(_syscmd(cmd,buf,sizeof(buf)) == RETURN_ERR)
        {
                wifi_dbg_printf("%s No associated list found :%s \n",__FUNCTION__,buf);
                return RETURN_ERR;
        }
        *pcount = atoi(buf);
        wifi_dbg_printf("[func]%s,[ifname]:%s,[cli-count]:%d \n",__FUNCTION__,ifname,*pcount);
        return RETURN_OK;
}

/* DRAFT
 * hostapd_cli -p <path of the socket> -i<wireless_name> sta <connected_mac_address>
Example : hostapd_cli -p /var/run/hostapd1 -iwlan1 sta 48:3b:38:35:cc:fb
48:3b:38:35:cc:fb
flags=[AUTH][ASSOC][AUTHORIZED]
aid=0
capability=0x0
listen_interval=0
supported_rates=0c 12
timeout_next=NULLFUNC POLL
dot11RSNAStatsSTAAddress=48:3b:38:35:cc:fb
dot11RSNAStatsVersion=1
dot11RSNAStatsSelectedPairwiseCipher=00-0f-ac-4
dot11RSNAStatsTKIPLocalMICFailures=0
dot11RSNAStatsTKIPRemoteMICFailures=0
wpa=2
AKMSuiteSelector=00-0f-ac-2
hostapdWPAPTKState=11
hostapdWPAPTKGroupState=0
rx_packets=432
tx_packets=164
rx_bytes=85643
tx_bytes=89672
inactive_msec=220
signal=-75
rx_rate_info=20
tx_rate_info=300
connected_time=60
*/

/* get_assoc_client_details2() function */
/**
* Description:
*  Gets the associated clients informations based on inputs strings
* Parameters : *ifname,*data_buf,*dev_data,*dev_mac
*
* @data_buf destn string,to be returned
* @dev_data Input query
* @dev_mac Associated device mac address
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
**/

static int get_assoc_client_details2(char *data_buf,char *cmd,char *dev_data)
{
        char buf[MAX_BUF_SIZE] = {0};
        if(_syscmd(cmd,buf,sizeof(buf)) == RETURN_ERR)
        {
                wifi_dbg_printf("%s No associated device %s found :%s \n",__FUNCTION__,dev_data,buf);
                return RETURN_ERR;
        }
        rpi_adjusting_the_stringsize(buf,data_buf);
        return RETURN_OK;
}


/* get_assoc_clients_data() function */
/**
* Description:
*  Gets the associated clients informations based on inputs strings
* Parameters : version,*ifname,ai,*dd,*client_mac,apIndex
*
* @version         version of wifi associated device struct(wifi_associated_dev3_t,wifi_associated_dev_t)
* @ifname          interface name based on apindex
* @ai              Associated index value
* @*dd             Associated device data struct,to be returned
* @client_mac      Associated device Mac Address
* @apIndex         Access Point Index Number
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
**/

static int get_assoc_clients_data(const int version, char *ifname, int ai, struct client_data *dd,char *client_mac,int apIndex)
{
        char mac_str[MACSTR_BUF_LEN] = {0};
        int  wificlientindex = 0;
        char buf[MAX_BUF_SIZE] = {0}, cmd[MAX_CMD_SIZE] = {0};
        int  arr1[MACADDRESS_SIZE];
        unsigned char mac[MACADDRESS_SIZE] = {0};

        /* get associated devices MAC Address*/
        if (ai == -1)
        {
                wifi_dbg_printf("Associated client mac address %s \n",client_mac);
                strncpy(mac_str,client_mac,sizeof(mac_str));
        }
        else
        {
                snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s all_sta | grep dot11RSNAStatsSTAAddress | cut -d '=' -f2 | sed '%dq;d'",apIndex,ifname,ai+1);
                if(get_assoc_client_details2(buf,cmd,"MAC list") == RETURN_ERR)
                        return RETURN_ERR;
                strncpy(mac_str, buf,sizeof(mac_str));
        }
	 if( MACADDRESS_SIZE == sscanf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",&arr1[0],&arr1[1],&arr1[2],&arr1[3],&arr1[4],&arr1[5]) )
        {
                for( wificlientindex = 0; wificlientindex < MACADDRESS_SIZE; ++wificlientindex )
                {
                        mac[wificlientindex] = (unsigned char) arr1[wificlientindex];

                }
                memcpy(dd->mac_addr_str,mac,(sizeof(unsigned char))*6);

        }

        wifi_dbg_printf("ASSOC MAC ADDR %02X:%02X:%02X:%02X:%02X:%02X.. \n",dd->mac_addr_str[0],dd->mac_addr_str[1],dd->mac_addr_str[2],dd->mac_addr_str[3],dd->mac_addr_str[4],dd->mac_addr_str[5]);

        /*get associated devices rssi value*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"iw dev %s station dump | sed -n -e '/Station %s/,/Station/ p' | grep signal |cut -d ' ' -f3 | awk '{print $1}'",ifname,mac_str);
        if(get_assoc_client_details2(buf,cmd,"RSSI") == RETURN_ERR)
                return RETURN_ERR;
        dd->processed_rssi = atoi(buf);
	dd->processed_snr = (dd->processed_rssi) + 95;

	 /*get associated device ip addr value*/
        if (version < 3)
        {
                /* ip address deprecated in v3 */
                CHECK_BUF_STATUS(buf,cmd);
                snprintf(cmd,sizeof(cmd),"grep -i %s /nvram/dnsmasq.leases | cut -d ' ' -f3",mac_str);
                if(get_assoc_client_details2(buf,cmd,"ip addr") == RETURN_ERR)
                        return RETURN_ERR;
                strncpy(dd->ip_addr,buf,sizeof(dd->ip_addr));
        }
	
        /* get associated devices received packets*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"rx_packets=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
        if(get_assoc_client_details2(buf,cmd,"rx_packets") == RETURN_ERR)
                return RETURN_ERR;
        dd->rx_pkts = atoi(buf);

        /* get associated devices sent packets*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"tx_packets=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
        if(get_assoc_client_details2(buf,cmd,"tx_pkts") == RETURN_ERR)
                return RETURN_ERR;
        dd->tx_pkts = atoi(buf);

	/* get associated devices sent bytes*/
	if(apIndex == 1)//temp fix
	{
		dd->tx_bytes = (dd->tx_pkts) * 8;
	}
	else
	{	
		CHECK_BUF_STATUS(buf,cmd);
		snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"tx_bytes=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
		if(get_assoc_client_details2(buf,cmd,"tx_bytes") == RETURN_ERR)
			return RETURN_ERR;
		dd->tx_bytes = atoi(buf);
	}

	/* get associated devices received bytes*/
	if(apIndex == 1)//temp fix
	{
		dd->rx_bytes = (dd->rx_pkts) * 8;
	}
	else
	{	
		CHECK_BUF_STATUS(buf,cmd);
		snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"rx_bytes=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
		if(get_assoc_client_details2(buf,cmd,"rx_bytes") == RETURN_ERR)
			return RETURN_ERR;
		dd->rx_bytes = atoi(buf);
	}

	 /* get associated devices received rate*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"rx_rate_info=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
        if(get_assoc_client_details2(buf,cmd,"rx_rate") == RETURN_ERR)
                return RETURN_ERR;
        dd->rx_rate = atoi(buf);

	 /* get associated devices sent rate*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"tx_rate_info=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
        if(get_assoc_client_details2(buf,cmd,"tx_rate") == RETURN_ERR)
                return RETURN_ERR;
        dd->tx_rate = atoi(buf);
	dd->DataFramesSentAck = atoi(buf);

	 /*get per clients stats*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep -w \"\\[AUTH\\]\\[ASSOC\\]\" | wc -l",apIndex,ifname,mac_str);
	if(get_assoc_client_details2(buf,cmd,"stats") == RETURN_ERR)
                return RETURN_ERR;
        if(atoi(buf) == 1)
        {
                dd->assoc = 1;
                dd->auth = 1;
                dd->deauth = 0;
                dd->diassoc = 0;
                dd->auth_fails = 0;
        }

	 /* get associated devices error*/
        CHECK_BUF_STATUS(buf,cmd);
        snprintf(cmd,sizeof(cmd),"hostapd_cli -p /var/run/hostapd%d -i%s sta %s | grep \"dot11RSNAStatsTKIPLocalMICFailures=\" | cut -d '=' -f2",apIndex,ifname,mac_str);
        if(get_assoc_client_details2(buf,cmd,"Errors") == RETURN_ERR)
                return RETURN_ERR;
        dd->tx_err = atoi(buf);
	dd->tx_allretries = 0;
	dd->tx_retried = 0;
	dd->tx_exceed_retry = 0;

	/* get associated device OperatingChannelBandwidth */
        CHECK_BUF_STATUS(buf,cmd);
	if ((apIndex == 0) || (apIndex == 4))
		strcpy(buf,"20 MHZ");
	else
		strcpy(buf,"40 MHZ");
        strncpy(dd->OperatingChannelBandwidth,buf,sizeof(dd->OperatingChannelBandwidth));

        /*get associated device wifi standard */ /* temp added this code, need to do re-work*/
        if(strcmp(buf,"20 MHZ") == 0)
                strncpy(dd->sta_mode,"b/g",sizeof(dd->sta_mode));
        else if(strcmp(buf,"40 MHZ") == 0)
                strncpy(dd->sta_mode,"n",sizeof(dd->sta_mode));
        else if(strcmp(buf,"80 MHZ") == 0)
                strncpy(dd->sta_mode,"a",sizeof(dd->sta_mode));
        else if(strcmp(buf,"160 MHZ") == 0)
                strncpy(dd->sta_mode,"ac",sizeof(dd->sta_mode));
        else
                strncpy(dd->sta_mode,"n",sizeof(dd->sta_mode));

	 return RETURN_OK;
}

#define FILL_V1(wad,dd) \
        memcpy((wad)->cli_MACAddress, (dd)->mac_addr_str, sizeof((wad)->cli_MACAddress)); \
        strncpy((wad)->cli_IPAddress, (dd)->ip_addr, sizeof((wad)->cli_IPAddress)); \
        (wad)->cli_AuthenticationState = ((dd)->auth != 0)?1:0; \
        (wad)->cli_LastDataDownlinkRate = (dd)->tx_rate/1000; \
        (wad)->cli_LastDataUplinkRate = (dd)->rx_rate/1000; \
        (wad)->cli_SignalStrength = (dd)->processed_rssi; \
        (wad)->cli_Retransmissions = (dd)->tx_allretries; \
        (wad)->cli_Active = 1; \
        (wad)->cli_OperatingStandard[0] = '\0'; \
        strncpy((wad)->cli_OperatingStandard, (dd)->sta_mode, sizeof((wad)->cli_OperatingStandard)); \
        strncpy((wad)->cli_OperatingChannelBandwidth, (dd)->OperatingChannelBandwidth,sizeof((wad)->cli_OperatingChannelBandwidth)); \
        (wad)->cli_SNR = (dd)->processed_snr; \
        (wad)->cli_InterferenceSources[0] = '\0'; \
        (wad)->cli_DataFramesSentAck = (dd)->DataFramesSentAck; \
        (wad)->cli_DataFramesSentNoAck = (dd)->tx_retried; \
        (wad)->cli_BytesSent = (dd)->tx_bytes; \
        (wad)->cli_BytesReceived = (dd)->rx_bytes; \
        (wad)->cli_RSSI = (dd)->processed_rssi; \
        (wad)->cli_MinRSSI = (wad)->cli_RSSI; \
        (wad)->cli_MaxRSSI = (wad)->cli_RSSI; \
        (wad)->cli_Disassociations = (dd)->diassoc; \
        (wad)->cli_AuthenticationFailures = (dd)->auth_fails;

#define FILL_V2(wad,dd) \
        (wad)->cli_Associations = (dd)->assoc;

#define FILL_V3(wad,dd) \
        (wad)->cli_PacketsSent = (dd)->tx_pkts; \
        (wad)->cli_PacketsReceived = (dd)->rx_pkts; \
        (wad)->cli_ErrorsSent = (dd)->tx_err; \
        (wad)->cli_RetransCount = (dd)->tx_allretries; \
        (wad)->cli_FailedRetransCount = (dd)->tx_exceed_retry; \
        (wad)->cli_RetryCount = (dd)->tx_retried; \
        (wad)->cli_MultipleRetryCount = (dd)->tx_retried; \
        (wad)->cli_MaxDownlinkRate = (dd)->tx_rate; \
        if ((wad)->cli_MaxDownlinkRate) (wad)->cli_MaxDownlinkRate /= 1000; /* mbps to kbps */ \
        (wad)->cli_MaxUplinkRate = (dd)->rx_rate; \
        if ((wad)->cli_MaxUplinkRate) (wad)->cli_MaxUplinkRate /= 1000; /* mbps to kbps */

/* associated_clients_diagnostic_helper() function */
/**
* Description:
*  Gets the associated clients informations based on inputs strings
* Parameters : version,apIndex,*out_wad_array,*out_count
*
* @version             version of wifi associated device struct(wifi_associated_dev3_t,wifi_associated_dev2_t,wifi_associated_dev_t)
* @apIndex             AccessPoint index
* @out_wad_array       Associated devices data struct,to be returned
* @out_count Active    Associated Devices count,to be returned
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected
*
**/

static int associated_clients_diagnostic_helper(const int version, int apIndex, void **out_wad_array, unsigned int *out_count)
{
        size_t wad_size = 0;
        void *wad_array = NULL;
        int index = 0;
        int client_count = 0;
        char buf[RPI_LEN_512] = {0};
        char hconf[RPI_LEN_32] = {0};
        struct client_data dd;

        char ifname[RPI_LEN_32]= {0};
        //private wifi - 2g and 5g
        if((apIndex == 0 ) || (apIndex == 1) || (apIndex == 4 ) || (apIndex == 5 ))
        {
                wifi_dbg_printf("supported apIndex number...%d \n",apIndex);
		snprintf(hconf,sizeof(hconf),"/nvram/hostapd%d.conf",apIndex);
                GetInterfaceName(ifname,hconf);
                rpi_wifi_get_clients_count (&client_count,ifname,apIndex);

                /* No clients, no work. It is ok to return NULL wad_array */
                if (client_count == 0)
                        goto out_success;
                if (version == 1)
                        wad_size = sizeof(wifi_associated_dev_t);
                else if (version == 3)
                        wad_size = sizeof(wifi_associated_dev3_t);


                wad_array = calloc(client_count, wad_size);
                if (wad_array == NULL)
		 {
                        wifi_dbg_printf("func[%s] apIndex[%d], version[%d] failed to allocate memory.\n",
                                        __FUNCTION__, apIndex, version);
                        goto out_error;
                }
                for (index = 0; index < client_count; ++index)
                {
                        /* For collection of connected devices with both bands */
                        if(apIndex == 1)
                                snprintf(buf,sizeof(buf),"hostapd_cli -p /var/run/hostapd%d -i%s all_sta | grep dot11RSNAStatsSTAAddress | sed '%dq;d' >> /tmp/AllAssociated_Devices_5G.txt",apIndex,ifname,index+1);
                        else if(apIndex == 0)
                                snprintf(buf,sizeof(buf),"hostapd_cli -p /var/run/hostapd%d -i%s all_sta | grep dot11RSNAStatsSTAAddress | sed '%dq;d' >> /tmp/AllAssociated_Devices_2G.txt",apIndex,ifname,index+1);
                        system(buf);
                        memset(&dd, 0, sizeof(dd));
                        if (get_assoc_clients_data(version,ifname, index, &dd,0,apIndex) == RETURN_ERR)
                                break;

                        if (version == 1)
                        {
                                wifi_associated_dev_t *dev1 = &((wifi_associated_dev_t *)wad_array)[index];
                                FILL_V1(dev1, &dd);
                        }
                        else if (version == 3)
                        {
                                wifi_associated_dev3_t *dev3 = &((wifi_associated_dev3_t *)wad_array)[index];
                                FILL_V1(dev3, &dd);
                                FILL_V2(dev3, &dd);
                                FILL_V3(dev3, &dd);
                        }

                }
                if (access("/tmp/AllAssociated_Devices_2G.txt", F_OK) == 0)
                        system("sort -u -o /tmp/AllAssociated_Devices_2G.txt /tmp/AllAssociated_Devices_2G.txt"); //for removing the duplicate lines
                if (access("/tmp/AllAssociated_Devices_5G.txt", F_OK) == 0)
                        system("sort -u -o /tmp/AllAssociated_Devices_5G.txt /tmp/AllAssociated_Devices_5G.txt");
                /* If we got here it means get_assoc_client_data() failed or we are done with
		 * all the clients. If index is still 0 that means we failed to process any
                 * clients. */
                if (index == 0)
                        goto out_error;
                else
                        goto out_success;

        }
        else if((apIndex == 3 ) || (apIndex == 2)) 
        {
                wifi_dbg_printf("Unsupported apIndex number..So simply returning NULL \n");
                goto out_success;
        }
        else if((apIndex >= 7) && (apIndex <=16)) 
        {
                wifi_dbg_printf("Unsupported apIndex number..So simply returning NULL \n");
                goto out_success;
        }
        else
        {
                wifi_dbg_printf("Invalid apIndex number..Please check the input again \n");
                goto out_error;
        }

out_success:
        *out_count = index;
        *out_wad_array = wad_array;
        return RETURN_OK;

out_error:
        if (wad_array)
                free(wad_array);
        *out_wad_array = NULL;
        *out_count = 0;
        return RETURN_ERR;
}

/* rpi_getApAssociatedDeviceDiagnosticResult() function */
/**
* @brief The function  provides a list of the devices currently associated with the access point.
*
* HAL funciton should allocate an data structure array, and return to caller with "associated_dev_array".
* Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.
*
* @param[in] apIndex                Access Point index
* @param[in] associated_dev_array   Associated device array, to be returned
* @param[in] dev_cnt                Array size, to be returned
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
**/

int rpi_getApAssociatedDeviceDiagnosticResult(int apIndex, wifi_associated_dev_t **associated_dev_array, unsigned int *dev_cnt)
{
        int ret = RETURN_OK;
        ret = associated_clients_diagnostic_helper(1, apIndex, (void **)associated_dev_array, dev_cnt);
        if (ret == RETURN_ERR)
                wifi_dbg_printf("func[%s], apIndex[%d] ret[%d] associated_dev_array[%p] dev_cnt[%d]\n",
                        __FUNCTION__, apIndex, ret, *associated_dev_array, *dev_cnt);
        return ret;
}

/**
* rpi_getApAssociatedDeviceDiagnosticResult3() function
* @brief The function  provides a list of the devices currently associated with the access point.
*
* HAL funciton should allocate an data structure array, and return to caller with "associated_dev_array".
* Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.
*
* @param[in] apIndex                Access Point index
* @param[in] associated_dev_array   Associated device array, to be returned
* @param[in] dev_cnt                Array size, to be returned
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
**/

int rpi_getApAssociatedDeviceDiagnosticResult3(int apIndex, wifi_associated_dev3_t **associated_dev_array, unsigned int *dev_cnt)
{
        int ret = RETURN_OK;
        ret = associated_clients_diagnostic_helper(3, apIndex, (void **)associated_dev_array, dev_cnt);
        if (ret == RETURN_ERR)
                wifi_dbg_printf("out func[%s], apIndex[%d] ret[%d] associated_dev_array[%p] dev_cnt[%d]\n",
                        __FUNCTION__, apIndex, ret, *associated_dev_array, *dev_cnt);
        return ret;
}

