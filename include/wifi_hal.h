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

    module: wifi_hal.h

        For CCSP Component:  Wifi_Provisioning_and_management

    ---------------------------------------------------------------

    description:

        This header file gives the function call prototypes and 
        structure definitions used for the RDK-Broadband 
        Wifi radio hardware abstraction layer

        NOTE:
        THIS VERSION IS AN EARLY DRAFT INTENDED TO GET COMMENTS FROM COMCAST.
        TESTING HAS NOT YET BEEN COMPLETED.  
       
    ---------------------------------------------------------------

    environment:

        This HAL layer is intended to support Wifi drivers 
        through an open API.  

    ---------------------------------------------------------------

    HAL version:

        The version of the Wifi HAL is specified in #defines below.

    --------------------------------------------------------------- 

    author:

        zhicheng_qiu@cable.comcast.com 
        Charles Moreman, moremac@cisco.com
		
    ---------------------------------------------------------------

	Notes:

	What is new for 2.2.0
	  1. Add Country Code support
	  2. Add more DCS function
	  3. Move RadiusSecret from struct wifi_radius_setting_t to wifi_getApSecurityRadiusServer function
	  4. Add wifi_getApSecuritySecondaryRadiusServer
	What is new for 2.2.1
	  1. Add wifi_setRadioTrafficStatsMeasure, wifi_setRadioTrafficStatsRadioStatisticsEnable
	What is new for 2.2.2
	  1. Add Band Steering HAL
**********************************************************************/
/**
* @file wifi_hal.h
* @author zhicheng_qiu@cable.comcast.com
* @brief For CCSP Component:  Wifi_Provisioning_and_management
*
* @description Wifi subsystem level APIs that are common to Client and Access Point devices. This HAL layer is intended to support Wifi drivers through an open API. This sample implementation file gives the function call prototypes and structure definitions used for the RDK-Broadband Wifi hardware abstraction layer.
* This header file gives the function call prototypes and structure definitions used for the RDK-Broadband Wifi radio hardware abstraction layer.
*/

#ifndef __WIFI_HAL_H__
#define __WIFI_HAL_H__

#ifndef ULLONG
#define ULLONG unsigned long long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef BOOL
#define BOOL  unsigned char
#endif

#ifndef CHAR
#define CHAR  char
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef INT
#define INT   int
#endif

#ifndef UINT
#define UINT  unsigned int
#endif

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef ENABLE
#define ENABLE   1
#endif

#ifndef RETURN_OK
#define RETURN_OK   0
#endif

#ifndef RETURN_ERR
#define RETURN_ERR   -1
#endif


#ifndef RADIO_INDEX_1
#define RADIO_INDEX_1 1
#define RADIO_INDEX_2 2
#define AP_INDEX_1 1
#define AP_INDEX_2 2
#define AP_INDEX_3 3
#define AP_INDEX_4 4
#define AP_INDEX_5 5
#define AP_INDEX_6 6
#define AP_INDEX_7 7
#define AP_INDEX_8 8
#define AP_INDEX_9 9
#define AP_INDEX_10 10
#define AP_INDEX_11 11
#define AP_INDEX_12 12
#define AP_INDEX_13 13
#define AP_INDEX_14 14
#define AP_INDEX_15 15
#define AP_INDEX_16 16
#endif

#define MAX_BUF_SIZE 128
#define MAX_CMD_SIZE 1024

//defines for HAL version 2.2.2
#define WIFI_HAL_MAJOR_VERSION 2   // This is the major verion of this HAL.
#define WIFI_HAL_MINOR_VERSION 2   // This is the minor verson of the HAL.
#define WIFI_HAL_MAINTENANCE_VERSION 2   // This is the maintenance version of the HAL.

// 802.11v BSS Transition Management Definitions

#define MAX_URL_LEN         512
#define MAX_CANDIDATES      64
#define MAX_VENDOR_SPECIFIC 32

/**********************************************************************
                STRUCTURE DEFINITIONS
**********************************************************************/

typedef unsigned char mac_address_t[6];
typedef mac_address_t   bssid_t;
typedef char            ssid_t[32];
typedef char            mac_addr_str_t[18];

//>> Deprecated: used for old RDKB code. 
typedef struct _wifi_basicTrafficStats
{
     ULONG wifi_BytesSent;
     ULONG wifi_BytesReceived;
     ULONG wifi_PacketsSent;
     ULONG wifi_PacketsReceived;
     ULONG wifi_Associations; 	
} wifi_basicTrafficStats_t;

typedef struct _wifi_trafficStats
{
     ULONG wifi_ErrorsSent;
     ULONG wifi_ErrorsReceived;
     ULONG wifi_UnicastPacketsSent;
     ULONG wifi_UnicastPacketsReceived;
     ULONG wifi_DiscardedPacketsSent;
     ULONG wifi_DiscardedPacketsReceived;
     ULONG wifi_MulticastPacketsSent;
     ULONG wifi_MulticastPacketsReceived;
     ULONG wifi_BroadcastPacketsSent;
     ULONG wifi_BroadcastPacketsRecevied;
     ULONG wifi_UnknownPacketsReceived;
} wifi_trafficStats_t;

typedef struct _wifi_radioTrafficStats
{
	 ULONG wifi_ErrorsSent;	
     ULONG wifi_ErrorsReceived;   
     ULONG wifi_DiscardPacketsSent; 
     ULONG wifi_DiscardPacketsReceived; 
	 ULONG wifi_PLCPErrorCount;	
	 ULONG wifi_FCSErrorCount;	
	 ULONG wifi_InvalidMACCount;	
	 ULONG wifi_PacketsOtherReceived;	
	 INT   wifi_Noise; 	
	 
} wifi_radioTrafficStats_t;	

typedef struct _wifi_ssidTrafficStats
{
     ULONG wifi_RetransCount;	
     ULONG wifi_FailedRetransCount; 	
     ULONG wifi_RetryCount;  
     ULONG wifi_MultipleRetryCount; 	
     ULONG wifi_ACKFailureCount;  	
     ULONG wifi_AggregatedPacketCount; 	
	 
} wifi_ssidTrafficStats_t;  

typedef struct _wifi_neighbor_ap
{
	 CHAR  ap_Radio[64];	
	 CHAR  ap_SSID[64];	
	 CHAR  ap_BSSID[64];	
	 CHAR  ap_Mode[64];	
	 UINT  ap_Channel;
	 INT   ap_SignalStrength;
	 CHAR  ap_SecurityModeEnabled[64];	
	 CHAR  ap_EncryptionMode[64];	
	 CHAR  ap_OperatingFrequencyBand[16];	
	 CHAR  ap_SupportedStandards[64];	
	 CHAR  ap_OperatingStandards[16];	
	 CHAR  ap_OperatingChannelBandwidth[16];	
	 UINT  ap_BeaconPeriod;	
	 INT   ap_Noise;	
	 CHAR  ap_BasicDataTransferRates[256];	
	 CHAR  ap_SupportedDataTransferRates[256];
	 UINT  ap_DTIMPeriod;
	 UINT  ap_ChannelUtilization;	
	 
} wifi_neighbor_ap_t;
//<<

typedef struct _wifi_radioTrafficStats2
{
     ULONG radio_BytesSent;	//The total number of bytes transmitted out of the interface, including framing characters.
     ULONG radio_BytesReceived;	//The total number of bytes received on the interface, including framing characters.
     ULONG radio_PacketsSent;	//The total number of packets transmitted out of the interface.
     ULONG radio_PacketsReceived; //The total number of packets received on the interface.

	 ULONG radio_ErrorsSent;	//The total number of outbound packets that could not be transmitted because of errors.
     ULONG radio_ErrorsReceived;    //The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol.
     ULONG radio_DiscardPacketsSent; //The total number of outbound packets which were chosen to be discarded even though no errors had been detected to prevent their being transmitted. One possible reason for discarding such a packet could be to free up buffer space.
     ULONG radio_DiscardPacketsReceived; //The total number of inbound packets which were chosen to be discarded even though no errors had been detected to prevent their being delivered. One possible reason for discarding such a packet could be to free up buffer space.
	 ULONG radio_PLCPErrorCount;	//The number of packets that were received with a detected Physical Layer Convergence Protocol (PLCP) header error.	
	 ULONG radio_FCSErrorCount;	//The number of packets that were received with a detected FCS error. This parameter is based on dot11FCSErrorCount from [Annex C/802.11-2012].
	 ULONG radio_InvalidMACCount;	//The number of packets that were received with a detected invalid MAC header error.
	 ULONG radio_PacketsOtherReceived;	//The number of packets that were received, but which were destined for a MAC address that is not associated with this interface.
	 INT   radio_NoiseFloor; 	//The noise floor for this radio channel where a recoverable signal can be obtained. Expressed as a signed integer in the range (-110:0).  Measurement should capture all energy (in dBm) from sources other than Wi-Fi devices as well as interference from Wi-Fi devices too weak to be decoded. Measured in dBm
	 ULONG radio_ChannelUtilization; //Percentage of time the channel was occupied by the radio's own activity (Activity Factor) or the activity of other radios.  Channel utilization MUST cover all user traffic, management traffic, and time the radio was unavailable for CSMA activities, including DIFS intervals, etc.  The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected from the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1.  Units in Percentage
	 INT   radio_ActivityFactor; //Percentage of time that the radio was transmitting or receiving Wi-Fi packets to/from associated clients. Activity factor MUST include all traffic that deals with communication between the radio and clients associated to the radio as well as management overhead for the radio, including NAV timers, beacons, probe responses,time for receiving devices to send an ACK, SIFC intervals, etc.  The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected from the just completed interval.   If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage
	 INT   radio_CarrierSenseThreshold_Exceeded; //Percentage of time that the radio was unable to transmit or receive Wi-Fi packets to/from associated clients due to energy detection (ED) on the channel or clear channel assessment (CCA). The metric is calculated and updated in this Parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected from the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in Percentage
	 INT   radio_RetransmissionMetirc; //Percentage of packets that had to be re-transmitted. Multiple re-transmissions of the same packet count as one.  The metric is calculated and updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".   The calculation of this metric MUST only use the data collected from the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units  in percentage
	 INT   radio_MaximumNoiseFloorOnChannel; //Maximum Noise on the channel during the measuring interval.  The metric is updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected in the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1.  Units in dBm
	 INT   radio_MinimumNoiseFloorOnChannel; //Minimum Noise on the channel. The metric is updated in this Parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected in the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm
	 INT   radio_MedianNoiseFloorOnChannel; //Median Noise on the channel during the measuring interval.   The metric is updated in this parameter at the end of the interval defined by "Radio Statistics Measuring Interval".  The calculation of this metric MUST only use the data collected in the just completed interval.  If this metric is queried before it has been updated with an initial calculation, it MUST return -1. Units in dBm
	 ULONG radio_StatisticsStartTime; 	 //The date and time at which the collection of the current set of statistics started.  This time must be updated whenever the radio statistics are reset.
	
} wifi_radioTrafficStats2_t;	//for radio only

typedef struct _wifi_radioTrafficStatsMeasure
{
	 INT   radio_RadioStatisticsMeasuringRate; //Input //"The rate at which radio related statistics are periodically collected.  Only statistics that explicitly indicate the use of this parameter MUST use the rate set in this parameter  Other parameter's are assumed to collect data in real-time or nearly real-time. Default value is 30 seconds.  This parameter MUST be persistent across reboots. If this parameter is changed,  then use of the new rate MUST be deferred until the start of the next interval and all metrics using this rate MUST return -1 until the completion of the next full interval Units in Seconds"
	 INT   radio_RadioStatisticsMeasuringInterval; //Input //The interval for which radio data MUST be retained in order and at the end of which appropriate calculations are executed and reflected in the associated radio object's.  Only statistics that explicitly indicate the use of this parameter MUST use the interval set in this parameter  Default value is 30 minutes.  This parameter MUST be persistent across reboots.   If this item is modified, then all metrics leveraging this interval as well as the metrics �Total number 802.11 packet of TX� and �Total number 802.11 packet of RX� MUST be re-initialized immediately.  Additionally, the �Statistics Start Time� must be reset to the current time. Units in Seconds
} wifi_radioTrafficStatsMeasure_t;	//for radio only


typedef struct _wifi_ssidTrafficStats2
{
     ULONG ssid_BytesSent;	//The total number of bytes transmitted out of the interface, including framing characters.
     ULONG ssid_BytesReceived;	//The total number of bytes received on the interface, including framing characters.
     ULONG ssid_PacketsSent;	//The total number of packets transmitted out of the interface.
     ULONG ssid_PacketsReceived; //The total number of packets received on the interface.

     ULONG ssid_RetransCount;	//The total number of transmitted packets which were retransmissions. Two retransmissions of the same packet results in this counter incrementing by two.
     ULONG ssid_FailedRetransCount; //The number of packets that were not transmitted successfully due to the number of retransmission attempts exceeding an 802.11 retry limit. This parameter is based on dot11FailedCount from [802.11-2012].	
     ULONG ssid_RetryCount;  //The number of packets that were successfully transmitted after one or more retransmissions. This parameter is based on dot11RetryCount from [802.11-2012].	
     ULONG ssid_MultipleRetryCount; //The number of packets that were successfully transmitted after more than one retransmission. This parameter is based on dot11MultipleRetryCount from [802.11-2012].	
     ULONG ssid_ACKFailureCount;  //The number of expected ACKs that were never received. This parameter is based on dot11ACKFailureCount from [802.11-2012].	
     ULONG ssid_AggregatedPacketCount; //The number of aggregated packets that were transmitted. This applies only to 802.11n and 802.11ac.	
	 
	 ULONG ssid_ErrorsSent;	//The total number of outbound packets that could not be transmitted because of errors.
     ULONG ssid_ErrorsReceived;    //The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol.
     ULONG ssid_UnicastPacketsSent;	//The total number of inbound packets that contained errors preventing them from being delivered to a higher-layer protocol.
     ULONG ssid_UnicastPacketsReceived;  //The total number of received packets, delivered by this layer to a higher layer, which were not addressed to a multicast or broadcast address at this layer.
     ULONG ssid_DiscardedPacketsSent; //The total number of outbound packets which were chosen to be discarded even though no errors had been detected to prevent their being transmitted. One possible reason for discarding such a packet could be to free up buffer space.
     ULONG ssid_DiscardedPacketsReceived; //The total number of inbound packets which were chosen to be discarded even though no errors had been detected to prevent their being delivered. One possible reason for discarding such a packet could be to free up buffer space.
     ULONG ssid_MulticastPacketsSent; //The total number of packets that higher-level protocols requested for transmission and which were addressed to a multicast address at this layer, including those that were discarded or not sent.
     ULONG ssid_MulticastPacketsReceived; //The total number of received packets, delivered by this layer to a higher layer, which were addressed to a multicast address at this layer.  
     ULONG ssid_BroadcastPacketsSent;  //The total number of packets that higher-level protocols requested for transmission and which were addressed to a broadcast address at this layer, including those that were discarded or not sent.
     ULONG ssid_BroadcastPacketsRecevied; //The total number of packets that higher-level protocols requested for transmission and which were addressed to a broadcast address at this layer, including those that were discarded or not sent.
     ULONG ssid_UnknownPacketsReceived;  //The total number of packets received via the interface which were discarded because of an unknown or unsupported protocol.

} wifi_ssidTrafficStats2_t;  //for ssid only


//Please do not edit the elements for this data structure 
typedef struct _wifi_neighbor_ap2
{
	 //CHAR  ap_Radio[64];	//The value MUST be the path name of a row in the�Device.WiFi.Radio�table. The Radio that detected the neighboring WiFi SSID.  
	 CHAR  ap_SSID[64];	//The current service set identifier in use by the neighboring WiFi SSID. The value MAY be empty for hidden SSIDs.
	 CHAR  ap_BSSID[64];	//[MACAddress] The BSSID used for the neighboring WiFi SSID.
	 CHAR  ap_Mode[64];	//The mode the neighboring WiFi radio is operating in. Enumeration of: AdHoc, Infrastructure
	 UINT  ap_Channel;	//The current radio channel used by the neighboring WiFi radio.
	 INT   ap_SignalStrength;	//An indicator of radio signal strength (RSSI) of the neighboring WiFi radio measured in�dBm, as an average of the last 100 packets received.
	 CHAR  ap_SecurityModeEnabled[64];	//The type of encryption the neighboring WiFi SSID advertises. Enumeration of:None, WPA-WPA2 etc.
	 CHAR  ap_EncryptionMode[64];	//Comma-separated list of strings. The type of encryption the neighboring WiFi SSID advertises. Each list item is an enumeration of: TKIP, AES
	 CHAR  ap_OperatingFrequencyBand[16];	//Indicates the frequency band at which the radio this SSID instance is operating. Enumeration of:2.4GHz, 5GHz
	 CHAR  ap_SupportedStandards[64];	//Comma-separated list of strings. List items indicate which IEEE 802.11 standards this�Result�instance can support simultaneously, in the frequency band specified by�OperatingFrequencyBand. Each list item is an enumeration of:
	 CHAR  ap_OperatingStandards[16];	//Comma-separated list of strings. Each list item MUST be a member of the list reported by theSupportedStandards�parameter. List items indicate which IEEE 802.11 standard that is detected for thisResult.
	 CHAR  ap_OperatingChannelBandwidth[16];	//Indicates the bandwidth at which the channel is operating. Enumeration of:
	 UINT  ap_BeaconPeriod;	//Time interval (in�ms) between transmitting beacons.
	 INT   ap_Noise;	//Indicator of average noise strength (in�dBm) received from the neighboring WiFi radio.
	 CHAR  ap_BasicDataTransferRates[256];	//Comma-separated list (maximum list length 256) of strings. Basic data transmit rates (in Mbps) for the SSID. For example, if�BasicDataTransferRates�is "1,2", this indicates that the SSID is operating with basic rates of 1 Mbps and 2 Mbps.
	 CHAR  ap_SupportedDataTransferRates[256];	//Comma-separated list (maximum list length 256) of strings. Data transmit rates (in Mbps) for unicast frames at which the SSID will permit a station to connect. For example, if�SupportedDataTransferRates�is "1,2,5.5", this indicates that the SSID will only permit connections at 1 Mbps, 2 Mbps and 5.5 Mbps.
	 UINT  ap_DTIMPeriod;	//The number of beacon intervals that elapse between transmission of Beacon frames containing a TIM element whose DTIM count field is 0. This value is transmitted in the DTIM Period field of beacon frames. [802.11-2012]
	 UINT  ap_ChannelUtilization;	//Indicates the fraction of the time AP senses that the channel is in use by the neighboring AP for transmissions.
	 
} wifi_neighbor_ap2_t;	//COSA_DML_NEIGHTBOURING_WIFI_RESULT

typedef struct _wifi_diag_ipping_setting
{
	 CHAR  ipping_Interface[256];	//The value MUST be the path name of a row in the IP.Interface table. The IP-layer interface over which the test is to be performed. This identifies the source IP address to use when performing the test. Example: Device.IP.Interface.1. If an empty string is specified, the CPE MUST use the interface as directed by its routing policy (Forwarding table entries) to determine the appropriate interface.
	 CHAR  ipping_Host[256];	//Host name or address of the host to ping. In the case where Host is specified by name, and the name resolves to more than one address, it is up to the device implementation to choose which address to use.
	 UINT  ipping_NumberOfRepetitions;	//Number of repetitions of the ping test to perform before reporting the results.	
	 UINT  ipping_Timeout;	//Timeout in milliseconds for the ping test.	
	 UINT  ipping_DataBlockSize;	//Size of the data block in bytes to be sent for each ping.
	 UINT  ipping_DSCP;	//DiffServ codepoint to be used for the test packets. By default the CPE SHOULD set this value to zero.

} wifi_diag_ipping_setting_t;	

typedef struct _wifi_diag_ipping_result
{
	 CHAR  ipping_DiagnosticsState[64];	//Indicates availability of diagnostic data. Enumeration of:	Complete, Error_CannotResolveHostName, 	Error_Internal, Error_Other
	 UINT  ipping_SuccessCount;	//Result parameter indicating the number of successful pings (those in which a successful response was received prior to the timeout) in the most recent ping test.	
	 UINT  ipping_FailureCount;	//Result parameter indicating the number of failed pings in the most recent ping test.
	 UINT  ipping_AverageResponseTime;	//Result parameter indicating the average response time in milliseconds over all repetitions with successful responses of the most recent ping test. If there were no successful responses, this value MUST be zero.
	 UINT  ipping_MinimumResponseTime;	//Result parameter indicating the minimum response time in milliseconds over all repetitions with successful responses of the most recent ping test. If there were no successful responses, this value MUST be zero.
	 UINT  ipping_MaximumResponseTime;	//Result parameter indicating the maximum response time in milliseconds over all repetitions with successful responses of the most recent ping test. If there were no successful responses, this value MUST be zero.
	 
} wifi_diag_ipping_result_t;

//>> -------------------------------- wifi_ap_hal --------------------------------------------
//>> Deprecated: used for old RDKB code. 
typedef struct _wifi_device
{
     UCHAR wifi_devMacAddress[6];
     CHAR wifi_devIPAddress[64];
     BOOL wifi_devAssociatedDeviceAuthentiationState;
     INT  wifi_devSignalStrength;
     INT  wifi_devTxRate;
     INT  wifi_devRxRate;
} wifi_device_t;
//<<

//Please do not edit the elements for this data structure 
typedef struct _wifi_associated_dev
{
     //UCHAR cli_devMacAddress[6];
     //CHAR  cli_devIPAddress[64];
     //BOOL  cli_devAssociatedDeviceAuthentiationState;
     //INT   cli_devSignalStrength;
     //INT   cli_devTxRate;
     //INT   cli_devRxRate;
	 
	 UCHAR cli_MACAddress[6];		// The MAC address of an associated device.
	 CHAR  cli_IPAddress[64];		// IP of the associated device
	 BOOL  cli_AuthenticationState; // Whether an associated device has authenticated (true) or not (false).
	 UINT  cli_LastDataDownlinkRate; //The data transmit rate in kbps that was most recently used for transmission from the access point to the associated device.
	 UINT  cli_LastDataUplinkRate; 	// The data transmit rate in kbps that was most recently used for transmission from the associated device to the access point.
	 INT   cli_SignalStrength; 		//An indicator of radio signal strength of the uplink from the associated device to the access point, measured in dBm, as an average of the last 100 packets received from the device.
	 UINT  cli_Retransmissions; 	//The number of packets that had to be re-transmitted, from the last 100 packets sent to the associated device. Multiple re-transmissions of the same packet count as one.
	 BOOL  cli_Active; 				//	boolean	-	Whether or not this node is currently present in the WiFi AccessPoint network.
	
	 CHAR  cli_OperatingStandard[64];	//Radio standard the associated Wi-Fi client device is operating under. Enumeration of:
	 CHAR  cli_OperatingChannelBandwidth[64];	//The operating channel bandwidth of the associated device. The channel bandwidth (applicable to 802.11n and 802.11ac specifications only). Enumeration of:
	 INT   cli_SNR;		//A signal-to-noise ratio (SNR) compares the level of the Wi-Fi signal to the level of background noise. Sources of noise can include microwave ovens, cordless phone, bluetooth devices, wireless video cameras, wireless game controllers, fluorescent lights and more. It is measured in decibels (dB).
	 CHAR  cli_InterferenceSources[64]; //Wi-Fi operates in two frequency ranges (2.4 Ghz and 5 Ghz) which may become crowded other radio products which operate in the same ranges. This parameter reports the probable interference sources that this Wi-Fi access point may be observing. The value of this parameter is a comma seperated list of the following possible sources: eg: MicrowaveOven,CordlessPhone,BluetoothDevices,FluorescentLights,ContinuousWaves,Others
	 ULONG cli_DataFramesSentAck;	//The DataFramesSentAck parameter indicates the total number of MSDU frames marked as duplicates and non duplicates acknowledged. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification.
	 ULONG cli_DataFramesSentNoAck;	//The DataFramesSentNoAck parameter indicates the total number of MSDU frames retransmitted out of the interface (i.e., marked as duplicate and non-duplicate) and not acknowledged, but does not exclude those defined in the DataFramesLost parameter. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification.
	 ULONG cli_BytesSent;	//The total number of bytes transmitted to the client device, including framing characters.
	 ULONG cli_BytesReceived;	//The total number of bytes received from the client device, including framing characters.
	 INT   cli_RSSI;	//The Received Signal Strength Indicator, RSSI, parameter is the energy observed at the antenna receiver for transmissions from the device averaged over past 100 packets recevied from the device.
	 INT   cli_MinRSSI;	//The Minimum Received Signal Strength Indicator, RSSI, parameter is the minimum energy observed at the antenna receiver for past transmissions (100 packets).
	 INT   cli_MaxRSSI;	//The Maximum Received Signal Strength Indicator, RSSI, parameter is the energy observed at the antenna receiver for past transmissions (100 packets).
	 UINT  cli_Disassociations;	//This parameter  represents the total number of client disassociations. Reset the parameter evey 24hrs or reboot
	 UINT  cli_AuthenticationFailures;	//This parameter indicates the total number of authentication failures.  Reset the parameter evey 24hrs or reboot
	 
} wifi_associated_dev_t;	//~COSA_DML_WIFI_AP_ASSOC_DEVICE

typedef struct _wifi_associated_dev3
{
        mac_address_t cli_MACAddress;           // The MAC address of an associated device.
        CHAR  cli_IPAddress[64];                // IP of the associated device  (deprecated, keep it empty)
        BOOL  cli_AuthenticationState; // Whether an associated device has authenticated (true) or not (false).
        UINT  cli_LastDataDownlinkRate; //The data transmit rate in kbps that was most recently used for transmission from the access point to the associated device.
        UINT  cli_LastDataUplinkRate;   // The data transmit rate in kbps that was most recently used for transmission from the associated device to the access point.
        INT   cli_SignalStrength;               //An indicator of radio signal strength of the uplink from the associated device to the access point, measured in dBm, as an average of the last 100 packets received from the device.
        UINT  cli_Retransmissions;      //The number of packets that had to be re-transmitted, from the last 100 packets sent to the associated device. Multiple re-transmissions of the same packet count as one.
        BOOL  cli_Active;                               //      boolean -       Whether or not this node is currently present in the WiFi AccessPoint network.

        CHAR  cli_OperatingStandard[64];        //Radio standard the associated Wi-Fi client device is operating under. Enumeration of:
        CHAR  cli_OperatingChannelBandwidth[64];        //The operating channel bandwidth of the associated device. The channel bandwidth (applicable to 802.11n and 802.11ac specifications only). Enumeration of:
        INT   cli_SNR;          //A signal-to-noise ratio (SNR) compares the level of the Wi-Fi signal to the level of background noise. Sources of noise can include microwave ovens, cordless phone, bluetooth devices, wireless video cameras, wireless game controllers, fluorescent lights and more. It is measured in decibels (dB).
        CHAR  cli_InterferenceSources[64]; //Wi-Fi operates in two frequency ranges (2.4 Ghz and 5 Ghz) which may become crowded other radio products which operate in the same ranges. This parameter reports the probable interference sources that this Wi-Fi access point may be observing. The value of this parameter is a comma seperated list of the following possible sources: eg: MicrowaveOven,CordlessPhone,BluetoothDevices,FluorescentLights,ContinuousWaves,Others
        ULONG cli_DataFramesSentAck;    //The DataFramesSentAck parameter indicates the total number of MSDU frames marked as duplicates and non duplicates acknowledged. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification.
        ULONG cli_DataFramesSentNoAck;  //The DataFramesSentNoAck parameter indicates the total number of MSDU frames retransmitted out of the interface (i.e., marked as duplicate and non-duplicate) and not acknowledged, but does not exclude those defined in the DataFramesLost parameter. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification.
        ULONG cli_BytesSent;    //The total number of bytes transmitted to the client device, including framing characters.
        ULONG cli_BytesReceived;        //The total number of bytes received from the client device, including framing characters.
        INT   cli_RSSI; //The Received Signal Strength Indicator, RSSI, parameter is the energy observed at the antenna receiver for transmissions from the device averaged over past 100 packets recevied from the device.
        INT   cli_MinRSSI;      //The Minimum Received Signal Strength Indicator, RSSI, parameter is the minimum energy observed at the antenna receiver for past transmissions (100 packets).
	    INT   cli_MaxRSSI;      //The Maximum Received Signal Strength Indicator, RSSI, parameter is the energy observed at the antenna receiver for past transmissions (100 packets).
        UINT  cli_Disassociations;      //This parameter  represents the total number of client disassociations. Reset the parameter evey 24hrs or reboot
        UINT  cli_AuthenticationFailures;       //This parameter indicates the total number of authentication failures.  Reset the parameter evey 24hrs or reboot

        ULLONG   cli_Associations;      // Stats handle used to determine reconnects; increases for every association (stat delta calcualtion)

        ULONG cli_PacketsSent; //The total number of packets transmitted to the Associated Device.
        ULONG cli_PacketsReceived; //The total number of packets received from the Associated Device.
        ULONG cli_ErrorsSent; //The total number of outbound packets that could not be transmitted because of errors. These might be due to the number of retransmissions exceeding the retry limit, or from other causes.
        ULONG cli_RetransCount; //The total number of transmitted packets which were retransmissions for each client on the vAP. Two retransmissions of the same packet results in this counter incrementing by two. Three retransmissions of the same packet results in this counter incrementing by three....
        ULONG cli_FailedRetransCount;  //The number of packets that were not transmitted successfully due to the number of retransmission attempts exceeding an 802.11 retry limit.
        ULONG cli_RetryCount;  //The number of packets that were successfully transmitted after one or more retransmissions
        ULONG cli_MultipleRetryCount; //The number of packets that were successfully transmitted after more than one retransmission.
        UINT  cli_MaxDownlinkRate; //The Max data transmit rate in kbps for the access point to the associated device.
        UINT  cli_MaxUplinkRate;   // The Max data transmit rate in kbps for the associated device to the access point.
} wifi_associated_dev3_t;

typedef struct _bs_AssDevices
{
	UCHAR bs_mac0[6];
	UCHAR bs_mac1[6];
} bs_AssDevices_t;

typedef struct _wifi_radius_setting_t
{
	 INT  RadiusServerRetries; 			//Number of retries for Radius requests.
	 INT  RadiusServerRequestTimeout; 	//Radius request timeout in seconds after which the request must be retransmitted for the # of retries available.	
	 INT  PMKLifetime; 					//Default time in seconds after which a Wi-Fi client is forced to ReAuthenticate (def 8 hrs).	
	 BOOL PMKCaching; 					//Enable or disable caching of PMK.	
	 INT  PMKCacheInterval; 			//Time interval in seconds after which the PMKSA (Pairwise Master Key Security Association) cache is purged (def 5 minutes).	
	 INT  MaxAuthenticationAttempts; 	//Indicates the # of time, a client can attempt to login with incorrect credentials. When this limit is reached, the client is blacklisted and not allowed to attempt loging into the network. Settings this parameter to 0 (zero) disables the blacklisting feature.
	 INT  BlacklistTableTimeout; 		//Time interval in seconds for which a client will continue to be blacklisted once it is marked so.	
	 INT  IdentityRequestRetryInterval; //Time Interval in seconds between identity requests retries. A value of 0 (zero) disables it.	
	 INT  QuietPeriodAfterFailedAuthentication;  //The enforced quiet period (time interval) in seconds following failed authentication. A value of 0 (zero) disables it.	
	 //UCHAR RadiusSecret[64];			//The secret used for handshaking with the RADIUS server [RFC2865]. When read, this parameter returns an empty string, regardless of the actual value.
		 
} wifi_radius_setting_t;	

//typedef struct wifi_AC_parameters_record  // Access Catagoriy parameters.  see 802.11-2012 spec for descriptions
//{
//     INT CWmin;       // CWmin variable
//     INT CWmax;       // CWmax vairable
//     INT AIFS;        // AIFS
//     ULONG TxOpLimit;  // TXOP Limit
//} wifi_AC_parameters_record_t;


//typedef struct _wifi_qos
//{
//     wifi_AC_parameters_record_t BE_AcParametersRecord;      // Best Effort QOS parameters, ACI == 0
//     wifi_AC_parameters_record_t BK_AcParametersRecord;      // Background QOS parameters, ACI == 1
//     wifi_AC_parameters_record_t VI_AcParametersRecord;      // Video QOS parameters, ACI == 2
//     wifi_AC_parameters_record_t VO_AcParametersRecord;      // Voice QOS parameters, ACI == 3
//}  wifi_qos_t;

typedef struct _wifi_apRssi {
    CHAR  ap_BSSID[6];          //BSSID
    UINT  ap_channelWidth;       //The channel width; 1 for 20Mhz, 2 for 40 MHz, 4 for 80 MHz, 8 for 160 MHz, 10 for 80+80Mhz
    INT   ap_rssi;              //RSSI of the neighboring AP in dBm.
} wifi_apRssi_t;

typedef struct _wifi_channelMetrics {
    INT  channel_number;        //each channel is only 20MHz bandwidth
    BOOL channel_in_pool;       //If channel_in_pool is false, driver do not need to scan this channel
    INT  channel_noise;         //this is used to return the average noise floor in dbm
    BOOL channel_radar_noise;   //if channel_number is in DFS channel, this is used to return if radar signal is present on DFS channel (5G only)
    INT  channel_non_80211_noise;           //average non 802.11 noise
    INT  channel_utilization;   //this is used to return the 802.11 utilization in percent
    INT  channel_txpower;       //this is used to return the current txpower in dbm on this channel
    wifi_apRssi_t channel_rssi_list[64];    //RSSI list from the neighbor AP on this channel. The list should be sorted descendly based on ap_rssi. If there are more than 64 AP on this channel, return first 64. 
    UINT channel_rssi_count;    //RSSI counter in channel_rssi_list
} wifi_channelMetrics_t;

//<< -------------------------------- wifi_ap_hal --------------------------------------------

//---------------------------------------------------------------------------------------------------
/* wifi_getHalVersion() function */
/**
* @description Get the wifi hal version in string, eg "2.0.0". WIFI_HAL_MAJOR_VERSION.WIFI_HAL_MINOR_VERSION.WIFI_HAL_MAINTENANCE_VERSION
*
* @param output_string - WiFi Hal version, to be returned
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
* @sideeffect None
*/
//Wifi system api
//Get the wifi hal version in string, eg "2.0.0".  WIFI_HAL_MAJOR_VERSION.WIFI_HAL_MINOR_VERSION.WIFI_HAL_MAINTENANCE_VERSION
INT wifi_getHalVersion(CHAR *output_string);   //RDKB   

//---------------------------------------------------------------------------------------------------
//
// Wifi subsystem level APIs that are common to Client and Access Point devices.
//
//---------------------------------------------------------------------------------------------------

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
//clears internal variables to implement a factory reset of the Wi-Fi subsystem
INT wifi_factoryReset();	//RDKB

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
//Restore all radio parameters without touch access point parameters
INT wifi_factoryResetRadios(); //RDKB

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
//Restore selected radio parameters without touch access point parameters
INT wifi_factoryResetRadio(int radioIndex); 	//RDKB

/* wifi_setLED() function */
/**
* @description Set the system LED status
*
* @param radioIndex - Index of Wi-Fi Radio channel
* @param enable - LED status
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
//Set the system LED status
INT wifi_setLED(INT radioIndex, BOOL enable);	//RDKB

/* wifi_init() function */
/**
* @description This function call initializes all Wi-Fi radios. Implementation 
* specifics may dictate the functionality since different hardware implementations 
* may have different initilization requirements.
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
// Initializes the wifi subsystem (all radios)
INT wifi_init();                              //RDKB

/* wifi_reset() function */
/**
* @description Resets the Wifi subsystem. This includes reset of all AP varibles.
* Implementation specifics may dictate what is actualy reset since different hardware 
* implementations may have different requirements.
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
// resets the wifi subsystem, deletes all APs
INT wifi_reset();                            //RDKB

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
// turns off transmit power for the entire Wifi subsystem, for all radios
INT wifi_down();                       		//RDKB

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
// creates initial implementation dependent configuration files that are later used for variable storage.  Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0)
INT wifi_createInitialConfigFiles();                                                                                    

/* wifi_getRadioCountryCode() function */
/**
* @description Outputs the country code to a max 64 character string
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Country code, to be returned
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
// outputs the country code to a max 64 character string
INT wifi_getRadioCountryCode(INT radioIndex, CHAR *output_string);   

/* wifi_setRadioCountryCode() function */
/**
* @description Set the country code for selected Wi-Fi radio channel.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param CountryCode - Country code 
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
INT wifi_setRadioCountryCode(INT radioIndex, CHAR *CountryCode);       


//---------------------------------------------------------------------------------------------------
//Wifi Tr181 API

//Device.WiFi.

/* wifi_getRadioNumberOfEntries() function */
/**
* @description Get the total number of radios in this wifi subsystem
*
* @param output - Total no. of radios, to be returned
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
//Device.WiFi.RadioNumberOfEntries
//Get the total number of radios in this wifi subsystem
INT wifi_getRadioNumberOfEntries(ULONG *output); //Tr181

/* wifi_getSSIDNumberOfEntries() function */
/**
* @description Get the total number of SSID entries in this wifi subsystem
*
* @param output - Total no. of SSID entries, to be returned
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
//Device.WiFi.SSIDNumberOfEntries
//Get the total number of SSID entries in this wifi subsystem 
INT wifi_getSSIDNumberOfEntries(ULONG *output); //Tr181

//Device.WiFi.AccessPointNumberOfEntries

//Device.WiFi.EndPointNumberOfEntries
//End points are managed by RDKB
//INT wifi_getEndPointNumberOfEntries(INT radioIndex, ULONG *output); //Tr181

//---------------------------------------------------------------------------------------------------
//
// Wifi radio level APIs that are common to Client and Access Point devices
//
//---------------------------------------------------------------------------------------------------

//Device.WiFi.Radio.

/* wifi_getRadioEnable() function */
/**
* @description Get the Radio enable config parameter
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - Radio Enable status, to be returned
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
//Device.WiFi.Radio.{i}.Enable
//Get the Radio enable config parameter
INT wifi_getRadioEnable(INT radioIndex, BOOL *output_bool);	//RDKB

/* wifi_setRadioEnable() function */
/**
* @description Set the Radio enable config parameter
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Set the selected radio's status as Enable/Disable
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
//Set the Radio enable config parameter
INT wifi_setRadioEnable(INT radioIndex, BOOL enable);		//RDKB

/* wifi_getRadioStatus() function */
/**
* @description Get the Radio enable status
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - Selected radio's enable status, to be returned
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
//Device.WiFi.Radio.{i}.Status
//Get the Radio enable status
INT wifi_getRadioStatus(INT radioIndex, BOOL *output_bool);	//RDKB

/* wifi_getRadioIfName() function */
/**
* @description Get the Radio Interface name from platform, eg "wifi0"
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Interface name, to be returned
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
//Device.WiFi.Radio.{i}.Alias

//Device.WiFi.Radio.{i}.Name
//Get the Radio Interface name from platform, eg "wifi0"
INT wifi_getRadioIfName(INT radioIndex, CHAR *output_string); //Tr181

//Device.WiFi.Radio.{i}.LastChange

//Device.WiFi.Radio.{i}.LowerLayers

//Device.WiFi.Radio.{i}.Upstream

/* wifi_getRadioMaxBitRate() function */
/**
* @description Get the maximum PHY bit rate supported by this interface. eg: "216.7 Mb/s", "1.3 Gb/s"
* The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* 
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Maximum bit rate supported, to be returned
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
//Device.WiFi.Radio.{i}.MaxBitRate
//Get the maximum PHY bit rate supported by this interface. eg: "216.7 Mb/s", "1.3 Gb/s"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioMaxBitRate(INT radioIndex, CHAR *output_string);	//RDKB

/* wifi_getRadioSupportedFrequencyBands() function */
/**
* @description Get Supported frequency bands at which the radio can operate. eg: "2.4GHz,5GHz"
* The output_string is a max length 64 octet string that is allocated by the RDKB code.  
* Implementations must ensure that strings are not longer than this.
* 
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Supported frequency bands, to be returned
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
//Device.WiFi.Radio.{i}.SupportedFrequencyBands
//Get Supported frequency bands at which the radio can operate. eg: "2.4GHz,5GHz"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioSupportedFrequencyBands(INT radioIndex, CHAR *output_string);	//RDKB

/* wifi_getRadioOperatingFrequencyBand() function */
/**
* @description Get the frequency band at which the radio is operating, eg: "2.4GHz".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* 
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Operating frequency band, to be returned
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
//Device.WiFi.Radio.{i}.OperatingFrequencyBand
//Get the frequency band at which the radio is operating, eg: "2.4GHz"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioOperatingFrequencyBand(INT radioIndex, CHAR *output_string); //Tr181

/* wifi_getRadioSupportedStandards() function */
/**
* @description Get the Supported Radio Mode. eg: "b,g,n"; "n,ac".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.
* Implementations must ensure that strings are not longer than this.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Supported radio mode, to be returned
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
//Device.WiFi.Radio.{i}.SupportedStandards
//Get the Supported Radio Mode. eg: "b,g,n"; "n,ac"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioSupportedStandards(INT radioIndex, CHAR *output_string); //Tr181

/* wifi_getRadioStandard() function */
/**
* @description Get the radio operating mode, and pure mode flag. eg: "ac".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.
* Implementations must ensure that strings are not longer than this.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Radio operating mode, to be returned
* @param gOnly - Boolean pointer variable need to be updated based on the "output_string"
* @param nOnly - Boolean pointer variable need to be updated based on the "output_string"
* @param acOnly - Boolean pointer variable need to be updated based on the "output_string"
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
//Device.WiFi.Radio.{i}.OperatingStandards
//Get the radio operating mode, and pure mode flag. eg: "ac"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioStandard(INT radioIndex, CHAR *output_string, BOOL *gOnly, BOOL *nOnly, BOOL *acOnly);	//RDKB

/* wifi_setRadioChannelMode() function */
/**
* @description Set the radio operating mode, and pure mode flag.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param channelMode - Pass the channelMode for specified radio index
* @param gOnlyFlag - Pass operating mode flag for setting pure mode flag
* @param nOnlyFlag - Pass operating mode flag for setting pure mode flag
* @param acOnlyFlag - Pass operating mode flag for setting pure mode flag
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
//Set the radio operating mode, and pure mode flag. 
INT wifi_setRadioChannelMode(INT radioIndex, CHAR *channelMode, BOOL gOnlyFlag, BOOL nOnlyFlag, BOOL acOnlyFlag);	//RDKB

/* wifi_getRadioPossibleChannels() function */
/**
* @description Get the list of supported channel. eg: "1-11".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.
* Implementations must ensure that strings are not longer than this.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - List of supported radio channels, to be returned
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
//Device.WiFi.Radio.{i}.PossibleChannels
//Get the list of supported channel. eg: "1-11"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioPossibleChannels(INT radioIndex, CHAR *output_string);	//RDKB

/* wifi_getRadioChannelsInUse() function */
/**
* @description Get the list of supported channel. eg: "1-11".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.
* Implementations must ensure that strings are not longer than this.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - List of supported radio channels, to be returned
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
//Device.WiFi.Radio.{i}.ChannelsInUse
//Get the list for used channel. eg: "1,6,9,11"
//The output_string is a max length 256 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioChannelsInUse(INT radioIndex, CHAR *output_string);	//RDKB

/* wifi_getRadioChannel() function */
/**
* @description Get the running channel number.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_ulong - Running channel number, to be returned
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
//Device.WiFi.Radio.{i}.Channel
//Get the running channel number 
INT wifi_getRadioChannel(INT radioIndex,ULONG *output_ulong);	//RDKB

/* wifi_setRadioChannel() function */
/**
* @description Set the running channel number.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param channel - Channel number to be set as running wifi radio channel
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
//Set the running channel number 
INT wifi_setRadioChannel(INT radioIndex, ULONG channel);	//RDKB	//AP only

/* wifi_setRadioAutoChannelEnable() function */
/**
* @description Enables or disables a driver level variable to indicate if auto channel selection is enabled on this radio.
* This "auto channel" means the auto channel selection when radio is up. (which is different from the dynamic channel/frequency selection (DFC/DCS))
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Enable/Disable selected radio channel as auto channel radio
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
//Enables or disables a driver level variable to indicate if auto channel selection is enabled on this radio
//This "auto channel" means the auto channel selection when radio is up. (which is different from the dynamic channel/frequency selection (DFC/DCS))
INT wifi_setRadioAutoChannelEnable(INT radioIndex, BOOL enable); //RDKB

INT wifi_getRouterEnable(INT wlanIndex, BOOL *enabled);

/* wifi_getRadioAutoChannelSupported() function */
/**
* @description Check if the driver support the AutoChannel.
* \n Device.WiFi.Radio.{i}.AutoChannelSupported
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param BOOL *output_bool - Value of Auto Channel Supported, to be returned
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
//Device.WiFi.Radio.{i}.AutoChannelSupported
//Check if the driver support the AutoChannel
INT wifi_getRadioAutoChannelSupported(INT radioIndex, BOOL *output_bool); //Tr181

/* wifi_getRadioAutoChannelEnable() function */
/**
* @description Get the AutoChannel enable status.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param BOOL *output_bool - Auto Channel Enabled status, to be returned
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
//Get the AutoChannel enable status
INT wifi_getRadioAutoChannelEnable(INT radioIndex, BOOL *output_bool);	//Tr181

/* wifi_setRadioAutoChannelEnable() function */
/**
* @description Enables or disables a driver level variable to indicate if auto channel selection is enabled on this radio.
* This "auto channel" means the auto channel selection when radio is up. (which is different from the dynamic channel/frequency selection (DFC/DCS))
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Enable/Disable selected radio channel as auto channel radio
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
//Set the AutoChannel enable status
INT wifi_setRadioAutoChannelEnable(INT radioIndex, BOOL enable);	//Tr181

/* wifi_getRadioDCSSupported() function */
/**
* @description Device.WiFi.Radio.{i}.X_COMCAST-COM_DCSSupported.
* Check if the driver support the DCS
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - DCS Supported flag for the radio index, to be returned
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
//Device.WiFi.Radio.{i}.X_COMCAST-COM_DCSSupported
//Check if the driver support the DCS
INT wifi_getRadioDCSSupported(INT radioIndex, BOOL *output_bool); 	//RDKB

/* wifi_getRadioDCSEnable() function */
/**
* @description Get DCS of the selected wifi radio channel's enable/disable status.
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_DCSEnable
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - DCS Enable flag for the selected radio index, to be returned
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
//Device.WiFi.Radio.{i}.X_COMCAST-COM_DCSEnable
INT wifi_getRadioDCSEnable(INT radioIndex, BOOL *output_bool);		//RDKB

/* wifi_setRadioDCSEnable() function */
/**
* @description Enable/Disable selected wifi radio channel's DCS.
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_DCSEnable
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Set the value of DCS Enable flag for the selected radio index
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
INT wifi_setRadioDCSEnable(INT radioIndex, BOOL enable);			//RDKB

/* wifi_getRadioDCSChannelPool() function */
/**
* @description Get radio DCS channel pool.
* The output_string is a max length 256 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* The value of this parameter is a comma seperated list of channel number.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_pool - DCS channel pool for the selected radio index,to be returned
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
//The output_string is a max length 256 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
//The value of this parameter is a comma seperated list of channel number
INT wifi_getRadioDCSChannelPool(INT radioIndex, CHAR *output_pool);			//RDKB

/* wifi_setRadioDCSChannelPool() function */
/**
* @description Set radio DCS channel pool.
* The output_string is a max length 256 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* The value of this parameter is a comma seperated list of channel number.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param pool - Set DCS channel pool for the selected radio index
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
INT wifi_setRadioDCSChannelPool(INT radioIndex, CHAR *pool);			//RDKB

/* wifi_getRadioDCSScanTime() function */
/**
* @description Get radio DCS scan time.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_interval_seconds - Get the interval time in seconds
* @param output_dwell_milliseconds - Get the dwell time in milliseconds
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
INT wifi_getRadioDCSScanTime(INT radioIndex, INT *output_interval_seconds, INT *output_dwell_milliseconds);

/* wifi_setRadioDCSScanTime() function */
/**
* @description Set radio DCS scan time.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param interval_seconds - Set the interval time in seconds
* @param dwell_milliseconds - Set the dwell time in milliseconds
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
INT wifi_setRadioDCSScanTime(INT radioIndex, INT interval_seconds, INT dwell_milliseconds);

/* wifi_getRadioDfsSupport() function */
/**
* @description Get radio DFS support.
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_DfsSupported
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - Get DFS support for the selected radio index in the pre-allocated buffer
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
//Device.WiFi.Radio.{i}.X_COMCAST-COM_DfsSupported
//Get radio DFS support
INT wifi_getRadioDfsSupport(INT radioIndex, BOOL *output_bool);		//RDKB

/* wifi_getRadioDfsEnable() function */
/**
* @description Get the Dfs enable status.
* Data model parameter used to check the DFS enable status is,
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_DfsEnable
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - Get DFS Enable status of the selected radio channel
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
//Device.WiFi.Radio.{i}.X_COMCAST-COM_DfsEnable					
INT wifi_getRadioDfsEnable(INT radioIndex, BOOL *output_bool);		//RDKB				

/* wifi_setRadioDfsEnable() function */
/**
* @description Set the Dfs enable status.
* Data model parameter used to check the DFS enable status is "Device.WiFi.Radio.{i}.X_COMCAST-COM_DfsEnable".
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Set DFS Enable status of the selected radio channel
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
INT wifi_setRadioDfsEnable(INT radioIndex, BOOL enabled);			//RDKB				

/* wifi_getRadioAutoChannelRefreshPeriodSupported() function */
/**
* @description Check if the driver support the AutoChannelRefreshPeriod.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - Get auto channel refresh period support for the selected radio channel in the pre-allocated bool buffer.
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
//Device.WiFi.Radio.{i}.AutoChannelRefreshPeriod
//Check if the driver support the AutoChannelRefreshPeriod
INT wifi_getRadioAutoChannelRefreshPeriodSupported(INT radioIndex, BOOL *output_bool); //Tr181

/* wifi_getRadioAutoChannelRefreshPeriod() function */
/**
* @description Check if the driver support the AutoChannelRefreshPeriod.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_bool - Get auto channel refresh period support for the selected radio channel in the pre-allocated bool buffer.
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
//Get the DCS refresh period in seconds
INT wifi_getRadioAutoChannelRefreshPeriod(INT radioIndex, ULONG *output_ulong); //Tr181

/* wifi_setRadioAutoChannelRefreshPeriod() function */
/**
* @description Set the DCS refresh period in seconds.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param seconds - Set auto channel refresh period in seconds support for the selected radio channel
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
//Set the DCS refresh period in seconds
INT wifi_setRadioAutoChannelRefreshPeriod(INT radioIndex, ULONG seconds); //Tr181

/* wifi_getRadioOperatingChannelBandwidth() function */
/**
* @description Get the Operating Channel Bandwidth. eg "20MHz", "40MHz", "80MHz", "80+80", "160".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Get operating channel bandwidth for the selected radio channel in the pre-allocated char buffer
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
//Device.WiFi.Radio.{i}.OperatingChannelBandwidth
//Get the Operating Channel Bandwidth. eg "20MHz", "40MHz", "80MHz", "80+80", "160"
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioOperatingChannelBandwidth(INT radioIndex, CHAR *output_string); //Tr181

/* wifi_setRadioOperatingChannelBandwidth() function */
/**
* @description Set the Operating Channel Bandwidth. eg "20MHz", "40MHz", "80MHz", "80+80", "160".
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param bandwidth - Set operating channel bandwidth for the selected radio channel
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
//Set the Operating Channel Bandwidth.
INT wifi_setRadioOperatingChannelBandwidth(INT radioIndex, CHAR *bandwidth); //Tr181	//AP only

/* wifi_getRadioExtChannel() function */
/**
* @description Get the secondary extension channel position, "AboveControlChannel" or "BelowControlChannel". (this is for 40MHz and 80MHz bandwith only).
* The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* \n Device.WiFi.Radio.{i}.ExtensionChannel
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Secondary extension channel position, to be returned
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
//Device.WiFi.Radio.{i}.ExtensionChannel
//Get the secondary extension channel position, "AboveControlChannel" or "BelowControlChannel". (this is for 40MHz and 80MHz bandwith only)
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioExtChannel(INT radioIndex, CHAR *output_string); //Tr181

/* wifi_setRadioExtChannel() function */
/**
* @description Set the secondary extension channel position, "AboveControlChannel" or "BelowControlChannel". (this is for 40MHz and 80MHz bandwith only).
* \n Device.WiFi.Radio.{i}.ExtensionChannel
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param string - Secondary extension channel position
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
//Set the extension channel.
INT wifi_setRadioExtChannel(INT radioIndex, CHAR *string); //Tr181	//AP only

/* wifi_getRadioGuardInterval() function */
/**
* @description Get the guard interval value. eg "400nsec" or "800nsec".
* The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* \n Device.WiFi.Radio.{i}.GuardInterval
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_string - Guard interval value, to be returned
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
//Device.WiFi.Radio.{i}.GuardInterval
//Get the guard interval value. eg "400nsec" or "800nsec" 
//The output_string is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioGuardInterval(INT radioIndex, CHAR *output_string);	//Tr181

/* wifi_setRadioGuardInterval() function */
/**
* @description Set the guard interval value. eg "400nsec" or "800nsec".
* \n Device.WiFi.Radio.{i}.GuardInterval
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param string - Guard interval value
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
//Set the guard interval value.
INT wifi_setRadioGuardInterval(INT radioIndex, CHAR *string);	//Tr181

/* wifi_getRadioMCS() function */
/**
* @description Get the Modulation Coding Scheme index, eg: "-1", "1", "15".
* \n Device.WiFi.Radio.{i}.MCS
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_INT - Modulation Coding Scheme index, to be returned
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
//Device.WiFi.Radio.{i}.MCS
//Get the Modulation Coding Scheme index, eg: "-1", "1", "15"
INT wifi_getRadioMCS(INT radioIndex, INT *output_INT); //Tr181

/* wifi_setRadioMCS() function */
/**
* @description Set the Modulation Coding Scheme index, eg: "-1", "1", "15".
* \n Device.WiFi.Radio.{i}.MCS
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param MCS - Modulation Coding Scheme index value
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
//Set the Modulation Coding Scheme index
INT wifi_setRadioMCS(INT radioIndex, INT MCS); //Tr181

/* wifi_getRadioTransmitPowerSupported() function */
/**
* @description Get supported Transmit Power list, eg : "0,25,50,75,100".
* The output_list is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
* \n Device.WiFi.Radio.{i}.TransmitPowerSupported
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_list - Transmit power list, to be returned
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
//Device.WiFi.Radio.{i}.TransmitPowerSupported
//Get supported Transmit Power list, eg : "0,25,50,75,100"
//The output_list is a max length 64 octet string that is allocated by the RDKB code.  Implementations must ensure that strings are not longer than this.
INT wifi_getRadioTransmitPowerSupported(INT radioIndex, CHAR *output_list); //Tr181

/* wifi_getRadioTransmitPower() function */
/**
* @description Get current Transmit Power, eg "75", "100".
* The transmite power level is in units of full power for this radio.
* \n Device.WiFi.Radio.{i}.TransmitPower
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param ULONG *output_ulong - Current Transmit power value, to be returned
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
//Device.WiFi.Radio.{i}.TransmitPower
//Get current Transmit Power, eg "75", "100"
//The transmite power level is in units of full power for this radio.
INT wifi_getRadioTransmitPower(INT radioIndex, ULONG *output_ulong);	//RDKB

/* wifi_setRadioTransmitPower() function */
/**
* @description Set current Transmit Power, eg "75", "100".
* The transmite power level is in units of full power for this radio.
* \n Device.WiFi.Radio.{i}.TransmitPower
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param TransmitPower - Transmit power value
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
//Set Transmit Power
//The transmite power level is in units of full power for this radio.
INT wifi_setRadioTransmitPower(INT radioIndex, ULONG TransmitPower);	//RDKB

/* wifi_getRadioIEEE80211hSupported() function */
/**
* @description Get 80211h Supported.  
* 80211h solves interference with satellites and radar using the same 5 GHz frequency band.
* \n Device.WiFi.Radio.{i}.IEEE80211hSupported
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param Supported - 80211h Supported, to be returned
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
//Device.WiFi.Radio.{i}.IEEE80211hSupported
//get 80211h Supported.  80211h solves interference with satellites and radar using the same 5 GHz frequency band
INT wifi_getRadioIEEE80211hSupported(INT radioIndex, BOOL *Supported);  //Tr181

/* wifi_getRadioIEEE80211hEnabled() function */
/**
* @description Get 80211h feature enable.
* \n Device.WiFi.Radio.{i}.IEEE80211hEnabled
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - 80211h feature enable, to be returned
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
//Device.WiFi.Radio.{i}.IEEE80211hEnabled
//Get 80211h feature enable
INT wifi_getRadioIEEE80211hEnabled(INT radioIndex, BOOL *enable);  //Tr181

/* wifi_setRadioIEEE80211hEnabled() function */
/**
* @description Set 80211h feature enable.
* \n Device.WiFi.Radio.{i}.IEEE80211hEnabled
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - 80211h feature enable
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
//Set 80211h feature enable
INT wifi_setRadioIEEE80211hEnabled(INT radioIndex, BOOL enable);  //Tr181

/* wifi_getRadioCarrierSenseThresholdRange() function */
/**
* @description Indicates the Carrier Sense ranges supported by the radio. It is measured in dBm. Refer section A.2.3.2 of CableLabs Wi-Fi MGMT Specification.
* \n Device.WiFi.Radio.{i}.RegulatoryDomain
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_CarrierSenseThresholdRange		
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output - Carrier sense threshold range, to be returned
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
//Device.WiFi.Radio.{i}.RegulatoryDomain

//Device.WiFi.Radio.{i}.X_COMCAST-COM_CarrierSenseThresholdRange		
//Indicates the Carrier Sense ranges supported by the radio. It is measured in dBm. Refer section A.2.3.2 of CableLabs Wi-Fi MGMT Specification.
INT wifi_getRadioCarrierSenseThresholdRange(INT radioIndex, INT *output);  //P3

/* wifi_getRadioCarrierSenseThresholdInUse() function */
/**
* @description The RSSI signal level at which CS/CCA detects a busy condition. This attribute enables APs to increase minimum sensitivity to avoid detecting busy condition from multiple/weak Wi-Fi sources in dense Wi-Fi environments. It is measured in dBm. Refer section A.2.3.2 of CableLabs Wi-Fi MGMT Specification.
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_CarrierSenseThresholdInUse
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output - Carrier sense threshold in use, to be returned
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
//Device.WiFi.Radio.{i}.X_COMCAST-COM_CarrierSenseThresholdInUse
//The RSSI signal level at which CS/CCA detects a busy condition. This attribute enables APs to increase minimum sensitivity to avoid detecting busy condition from multiple/weak Wi-Fi sources in dense Wi-Fi environments. It is measured in dBm. Refer section A.2.3.2 of CableLabs Wi-Fi MGMT Specification.
INT wifi_getRadioCarrierSenseThresholdInUse(INT radioIndex, INT *output);	//P3

/* wifi_setRadioCarrierSenseThresholdInUse() function */
/**
* @description Set Carrier sense threshold in use for the selected radio index. 
* The RSSI signal level at which CS/CCA detects a busy condition. This attribute enables APs to increase minimum sensitivity to avoid detecting busy condition from multiple/weak Wi-Fi sources in dense Wi-Fi environments. It is measured in dBm.
* \n Device.WiFi.Radio.{i}.X_COMCAST-COM_CarrierSenseThresholdInUse
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param threshold - Carrier sense threshold, to be returned
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
INT wifi_setRadioCarrierSenseThresholdInUse(INT radioIndex, INT threshold);	//P3

//Device.WiFi.Radio.{i}.X_COMCAST-COM_ChannelSwitchingCount
//This parameter indicates the total number of Channel Changes.  Reset the parameter every 24 hrs or reboot
//INT wifi_getRadioChannelSwitchingCount(INT radioIndex, INT *output); 	//P3


/* wifi_getRadioBeaconPeriod() function */
/**
* @description Time interval between transmitting beacons (expressed in milliseconds). This parameter is based ondot11BeaconPeriod from [802.11-2012].
* \n Device.WiFi.Radio.{i}.BeaconPeriod
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output - Radio Beacon period, to be returned
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
//Device.WiFi.Radio.{i}.BeaconPeriod
//Time interval between transmitting beacons (expressed in milliseconds). This parameter is based ondot11BeaconPeriod from [802.11-2012].
INT wifi_getRadioBeaconPeriod(INT radioIndex, UINT *output); 

/* wifi_setRadioBeaconPeriod() function */
/**
* @description Time interval between transmitting beacons (expressed in milliseconds). This parameter is based ondot11BeaconPeriod from [802.11-2012].
* \n Device.WiFi.Radio.{i}.BeaconPeriod
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param BeaconPeriod - Radio Beacon period
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
INT wifi_setRadioBeaconPeriod(INT radioIndex, UINT BeaconPeriod); 

/* wifi_getRadioBasicDataTransmitRates() function */
/**
* @description Get the set of data rates, in Mbps, that have to be supported by all stations that desire to join this BSS. The stations have to be able to receive and transmit at each of the data rates listed inBasicDataTransmitRates. For example, a value of "1,2", indicates that stations support 1 Mbps and 2 Mbps. Most control packets use a data rate in BasicDataTransmitRates.
* \n Device.WiFi.Radio.{i}.BasicDataTransmitRates
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output - Comma-separated list of strings, to be returned
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
//Device.WiFi.Radio.{i}.BasicDataTransmitRates
//Comma-separated list of strings. The set of data rates, in Mbps, that have to be supported by all stations that desire to join this BSS. The stations have to be able to receive and transmit at each of the data rates listed inBasicDataTransmitRates. For example, a value of "1,2", indicates that stations support 1 Mbps and 2 Mbps. Most control packets use a data rate in BasicDataTransmitRates.	
INT wifi_getRadioBasicDataTransmitRates(INT radioIndex, CHAR *output);

/* wifi_setRadioBasicDataTransmitRates() function */
/**
* @description Set the data rates, in Mbps, that have to be supported by all stations that desire to join this BSS. The stations have to be able to receive and transmit at each of the data rates listed inBasicDataTransmitRates. For example, a value of "1,2", indicates that stations support 1 Mbps and 2 Mbps. Most control packets use a data rate in BasicDataTransmitRates.
* \n Device.WiFi.Radio.{i}.BasicDataTransmitRates
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param TransmitRates - Comma-separated list of strings
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
INT wifi_setRadioBasicDataTransmitRates(INT radioIndex, CHAR *TransmitRates);

//---------------------------------------------------------------------------------------------------
//Device.WiFi.Radio.{i}.Stats.

//Device.WiFi.Radio.{i}.Stats.BytesSent
//Device.WiFi.Radio.{i}.Stats.BytesReceived
//Device.WiFi.Radio.{i}.Stats.PacketsSent
//Device.WiFi.Radio.{i}.Stats.PacketsReceived
//Device.WiFi.Radio.{i}.Stats.ErrorsSent
//Device.WiFi.Radio.{i}.Stats.ErrorsReceived
//Device.WiFi.Radio.{i}.Stats.DiscardPacketsSent
//Device.WiFi.Radio.{i}.Stats.DiscardPacketsReceived
//Device.WiFi.Radio.{i}.Stats.PLCPErrorCount
//Device.WiFi.Radio.{i}.Stats.FCSErrorCount
//Device.WiFi.Radio.{i}.Stats.InvalidMACCount
//Device.WiFi.Radio.{i}.Stats.PacketsOtherReceived
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_NoiseFloor
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_ChannelUtilization
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_ActivityFactor
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_CarrierSenseThreshold_Exceeded
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RetransmissionMetirc
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_MaximumNoiseFloorOnChannel
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_MinimumNoiseFloorOnChannel
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_MedianNoiseFloorOnChannel
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_StatisticsStartTime

/* wifi_getRadioTrafficStats2() function */
/**
* @description Get detail radio traffic static info.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_struct - wifi_radioTrafficStats2_t *output_struct, all traffic stats info to be returned
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
//Get detail radio traffic static info
INT wifi_getRadioTrafficStats2(INT radioIndex, wifi_radioTrafficStats2_t *output_struct); //Tr181

/* wifi_setRadioTrafficStatsMeasure() function */
/**
* @description Set radio traffic static Measuring rules.
* \n Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RadioStatisticsMeasuringRate
* \n Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RadioStatisticsMeasuringInterval
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param input_struct - wifi_radioTrafficStatsMeasure_t *input_struct, traffic stats measure info
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
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RadioStatisticsMeasuringRate
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RadioStatisticsMeasuringInterval
//Set radio traffic static Measureing rules
INT wifi_setRadioTrafficStatsMeasure(INT radioIndex, wifi_radioTrafficStatsMeasure_t *input_struct); //Tr181

/* wifi_setRadioTrafficStatsRadioStatisticsEnable() function */
/**
* @description Set radio traffic statistics enable.
* \n Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RadioStatisticsEnable bool writable
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Enable/disable, traffic stats statistics
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
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_RadioStatisticsEnable bool writable
INT wifi_setRadioTrafficStatsRadioStatisticsEnable(INT radioIndex, BOOL enable);

//-----------------------------------------------------------------------------------------------
/* wifi_getRadioStatsReceivedSignalLevel() function */
/**
* @description Clients associated with the AP over a specific interval.  The histogram MUST have a range from -110to 0 dBm and MUST be divided in bins of 3 dBM, with bins aligning on the -110 dBm end of the range.  Received signal levels equal to or greater than the smaller boundary of a bin and less than the larger boundary are included in the respective bin.  The bin associated with the client?s current received signal level MUST be incremented when a client associates with the AP.   Additionally, the respective bins associated with each connected client?s current received signal level MUST be incremented at the interval defined by "Radio Statistics Measuring Rate".  The histogram?s bins MUST NOT be incremented at any other time.  The histogram data collected during the interval MUST be published to the parameter only at the end of the interval defined by "Radio Statistics Measuring Interval".  The underlying histogram data MUST be cleared at the start of each interval defined by "Radio Statistics Measuring Interval?. If any of the parameter's representing this histogram is queried before the histogram has been updated with an initial set of data, it MUST return -1. Units dBm.
* \n Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_ReceivedSignalLevel.{i}.ReceivedSignalLevel
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param signalIndex - Signal index
* @param SignalLevel - Signal level, to be returned
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
//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_ReceivedSignalLevel.{i}.

//Device.WiFi.Radio.{i}.Stats.X_COMCAST-COM_ReceivedSignalLevel.{i}.ReceivedSignalLevel
//Clients associated with the AP over a specific interval.  The histogram MUST have a range from -110to 0 dBm and MUST be divided in bins of 3 dBM, with bins aligning on the -110 dBm end of the range.  Received signal levels equal to or greater than the smaller boundary of a bin and less than the larger boundary are included in the respective bin.  The bin associated with the client?s current received signal level MUST be incremented when a client associates with the AP.   Additionally, the respective bins associated with each connected client?s current received signal level MUST be incremented at the interval defined by "Radio Statistics Measuring Rate".  The histogram?s bins MUST NOT be incremented at any other time.  The histogram data collected during the interval MUST be published to the parameter only at the end of the interval defined by "Radio Statistics Measuring Interval".  The underlying histogram data MUST be cleared at the start of each interval defined by "Radio Statistics Measuring Interval?. If any of the parameter's representing this histogram is queried before the histogram has been updated with an initial set of data, it MUST return -1. Units dBm
INT wifi_getRadioStatsReceivedSignalLevel(INT radioIndex, INT signalIndex, INT *SignalLevel); //Tr181

//-----------------------------------------------------------------------------------------------------
/* wifi_applyRadioSettings() function */
/**
* @description This API is used to apply (push) all previously set radio level variables and make these settings active in the 
* hardware. Not all implementations may need this function.  If not needed for a particular implementation simply return no-error 
* (0).
*
* @param radioIndex - Index of Wi-Fi radio channel
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
//This API is used to apply (push) all previously set radio level variables and make these settings active in the hardware
//Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0)
INT wifi_applyRadioSettings(INT radioIndex);  


/* wifi_getRadioResetCount() function */
/**
* @description Get the radio reset count.
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param output_int - Reset count, to be returned
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
//Radio reset count
INT wifi_getRadioResetCount(INT radioIndex, ULONG *output_int);

//---------------------------------------------------------------------------------------------------
//
// Wifi SSID level APIs common to Client and Access Point devices.
//
//---------------------------------------------------------------------------------------------------

INT wifi_setApEnableOnLine(ULONG wlanIndex,BOOL enable);

//Device.WiFi.SSID.{i}.

/* wifi_getSSIDRadioIndex() function */
/**
* @description Get the radio index associated with the SSID entry.
*
* @param ssidIndex - SSID index
* @param radioIndex - Radio index, to be returned
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
//Get the radio index assocated with this SSID entry
INT wifi_getSSIDRadioIndex(INT ssidIndex, INT *radioIndex);

/* wifi_getSSIDEnable() function */
/**
* @description Get SSID enable configuration parameters (not the SSID enable status).
* \n Device.WiFi.SSID.{i}.Enable
*
* @param ssidIndex - SSID index
* @param output_bool - SSID enable, to be returned
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
//Device.WiFi.SSID.{i}.Enable
//Get SSID enable configuration parameters (not the SSID enable status)
INT wifi_getSSIDEnable(INT ssidIndex, BOOL *output_bool); //Tr181

/* wifi_setSSIDEnable() function */
/**
* @description Set SSID enable configuration parameters.
* \n Device.WiFi.SSID.{i}.Enable
*
* @param ssidIndex - SSID index
* @param enable - SSID enable value
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
//Set SSID enable configuration parameters
INT wifi_setSSIDEnable(INT ssidIndex, BOOL enable); //Tr181

/* wifi_getSSIDStatus() function */
/**
* @description Get SSID enable status.
* \n Device.WiFi.SSID.{i}.Status
*
* @param ssidIndex - SSID index
* @param output_string - SSID enable status, to be returned
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
//Device.WiFi.SSID.{i}.Status
//Get the SSID enable status
INT wifi_getSSIDStatus(INT ssidIndex, CHAR *output_string); //Tr181

/* wifi_getSSIDName() function */
/**
* @description Get SSID Name associated with the Access Point index.
* Outputs a 32 byte or less string indicating the SSID name.  Sring buffer must be preallocated by the caller.
* \n Device.WiFi.SSID.{i}.Name
* \n Device.WiFi.SSID.{i}.Alias
*
* @param apIndex - Access Point index
* @param output_string - SSID enable status, to be returned
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
//Device.WiFi.SSID.{i}.Alias

//Device.WiFi.SSID.{i}.Name
// Outputs a 32 byte or less string indicating the SSID name.  Sring buffer must be preallocated by the caller.
INT wifi_getSSIDName(INT apIndex, CHAR *output_string);        

/* wifi_setSSIDName() function */
/**
* @description Set SSID Name associated with the Access Point index.
* \n Device.WiFi.SSID.{i}.Name
* \n Device.WiFi.SSID.{i}.Alias
*
* @param apIndex - Access Point index
* @param ssid_string - SSID Name
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
// accepts a max 32 byte string and sets an internal variable to the SSID name          
INT wifi_setSSIDName(INT apIndex, CHAR *ssid_string);
// push the ssid name to the hardware //repleaced by wifi_applySSIDSettings
//INT wifi_pushSSIDName(INT apIndex, CHAR *ssid);                         


/* wifi_getBaseBSSID() function */
/**
* @description Get the BSSID.
* \n Device.WiFi.SSID.{i}.BSSID
*
* @param ssidIndex - SSID index
* @param output_string - Base BSSID, to be returned
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
//Device.WiFi.SSID.{i}.LastChange

//Device.WiFi.SSID.{i}.LowerLayers

//Device.WiFi.SSID.{i}.BSSID
//Get the BSSID 
INT wifi_getBaseBSSID(INT ssidIndex, CHAR *output_string);	//RDKB

/* wifi_getSSIDMACAddress() function */
/**
* @description Get the MAC address associated with this Wifi SSID.
* \n Device.WiFi.SSID.{i}.MACAddress
*
* @param ssidIndex - SSID index
* @param output_string - MAC Address, to be returned
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
//Device.WiFi.SSID.{i}.MACAddress
//Get the MAC address associated with this Wifi SSID
INT wifi_getSSIDMACAddress(INT ssidIndex, CHAR *output_string); //Tr181

//Device.WiFi.SSID.{i}.SSID

//-----------------------------------------------------------------------------------------------
//Device.WiFi.SSID.{i}.Stats.
//Device.WiFi.SSID.{i}.Stats.BytesSent
//Device.WiFi.SSID.{i}.Stats.BytesReceived
//Device.WiFi.SSID.{i}.Stats.PacketsSent
//Device.WiFi.SSID.{i}.Stats.PacketsReceived

//Device.WiFi.SSID.{i}.Stats.RetransCount		
//Device.WiFi.SSID.{i}.Stats.FailedRetransCount	
//Device.WiFi.SSID.{i}.Stats.RetryCount	
//Device.WiFi.SSID.{i}.Stats.MultipleRetryCount	
//Device.WiFi.SSID.{i}.Stats.ACKFailureCount	
//Device.WiFi.SSID.{i}.Stats.AggregatedPacketCount	
	 
//Device.WiFi.SSID.{i}.Stats.ErrorsSent
//Device.WiFi.SSID.{i}.Stats.ErrorsReceived
//Device.WiFi.SSID.{i}.Stats.UnicastPacketsSent
//Device.WiFi.SSID.{i}.Stats.UnicastPacketsReceived
//Device.WiFi.SSID.{i}.Stats.DiscardPacketsSent
//Device.WiFi.SSID.{i}.Stats.DiscardPacketsReceived
//Device.WiFi.SSID.{i}.Stats.MulticastPacketsSent
//Device.WiFi.SSID.{i}.Stats.MulticastPacketsReceived
//Device.WiFi.SSID.{i}.Stats.BroadcastPacketsSent
//Device.WiFi.SSID.{i}.Stats.BroadcastPacketsReceived
//Device.WiFi.SSID.{i}.Stats.UnknownProtoPacketsReceived	

/* wifi_getSSIDTrafficStats2() function */
/**
* @description Get the basic SSID traffic static info.
*
* @param ssidIndex - SSID index
* @param output_struct - wifi_ssidTrafficStats2_t *output_struct SSID traffic 
* stats, to be returned
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
//Get the basic SSID traffic static info
INT wifi_getSSIDTrafficStats2(INT ssidIndex, wifi_ssidTrafficStats2_t *output_struct); //Tr181

/* wifi_applySSIDSettings() function */
/**
* @description Apply SSID and AP (in the case of Acess Point devices) to the hardware.
* Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0).
*
* @param ssidIndex - SSID index
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
//Apply SSID and AP (in the case of Acess Point devices) to the hardware
//Not all implementations may need this function.  If not needed for a particular implementation simply return no-error (0)
INT wifi_applySSIDSettings(INT ssidIndex);


//-----------------------------------------------------------------------------------------------
//Device.WiFi.NeighboringWiFiDiagnostic.	
//Device.WiFi.NeighboringWiFiDiagnostic.DiagnosticsState
//Device.WiFi.NeighboringWiFiDiagnostic.ResultNumberOfEntries

//-----------------------------------------------------------------------------------------------
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.	
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.Radio
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.SSID
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.BSSID
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.Mode						
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.Channel
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.SignalStrength
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.SecurityModeEnabled
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.EncryptionMode	
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.OperatingFrequencyBand
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.SupportedStandards		
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.OperatingStandards
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.OperatingChannelBandwidth
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.BeaconPeriod	
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.Noise
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.BasicDataTransferRates
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.SupportedDataTransferRates
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.DTIMPeriod
//Device.WiFi.NeighboringWiFiDiagnostic.Result.{i}.X_COMCAST-COM_ChannelUtilization

/* wifi_getNeighboringWiFiDiagnosticResult2() function */
/**
* @description Start the wifi scan and get the result into output buffer for RDKB to parser. The result will be used to manage endpoint list.
* HAL funciton should allocate an data structure array, and return to caller with 
"neighbor_ap_array". 
*
* @param radioIndex - Radio index
* @param neighbor_ap_array - wifi_neighbor_ap2_t **neighbor_ap_array, neighbour 
access point info to be returned
* @param output_array_size - UINT *output_array_size, to be returned
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
//Start the wifi scan and get the result into output buffer for RDKB to parser. The result will be used to manage endpoint list
//HAL funciton should allocate an data structure array, and return to caller with "neighbor_ap_array"
INT wifi_getNeighboringWiFiDiagnosticResult2(INT radioIndex, wifi_neighbor_ap2_t **neighbor_ap_array, UINT *output_array_size); //Tr181	

//>> Deprecated: used for old RDKB code. 
/** Deprecated: used for old RDKB code. */
INT wifi_getSSIDTrafficStats(INT ssidIndex, wifi_ssidTrafficStats_t *output_struct); //Tr181
/** Deprecated: used for old RDKB code. */
INT wifi_getBasicTrafficStats(INT apIndex, wifi_basicTrafficStats_t *output_struct);  // outputs basic traffic stats per AP
/** Deprecated: used for old RDKB code. */
INT wifi_getWifiTrafficStats(INT apIndex, wifi_trafficStats_t *output_struct); // outputs more detailed traffic stats per AP
/** Deprecated: used for old RDKB code. */
INT wifi_getNeighboringWiFiDiagnosticResult(wifi_neighbor_ap_t **neighbor_ap_array, UINT *output_array_size); //Tr181
/** Deprecated: used for old RDKB code. */
INT wifi_getAllAssociatedDeviceDetail(INT apIndex, ULONG *output_ulong, wifi_device_t **output_struct); //RDKB
//<<

//>> -------------------- wifi_ap_hal -----------------------------------
//---------------------------------------------------------------------------------------------------
//
// Additional Wifi radio level APIs used for RDKB Access Point devices
//
//---------------------------------------------------------------------------------------------------

/* wifi_getBandSteeringCapability() function */
/**
* @description To get Band Steering Capability. 
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering object  
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.Capability bool r/o
*
* @param support - Band Steering Capability support, to be returned
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
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering object
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.Capability bool r/o
//To get Band Steering Capability
INT wifi_getBandSteeringCapability(BOOL *support); 

/* wifi_getBandSteeringEnable() function */
/**
* @description To get Band Steering enable status. 
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.Enable bool r/w
*
* @param enable - Band Steering enable status, to be returned
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
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.Enable bool r/w
//To get Band Steering enable status
INT wifi_getBandSteeringEnable(BOOL *enable);

/* wifi_setBandSteeringEnable() function */
/**
* @description To turn on/off Band steering. 
*
* @param enable - Band Steering enable status
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
//To turn on/off Band steering
INT wifi_setBandSteeringEnable(BOOL enable);

/* wifi_getBandSteeringBandUtilizationThreshold() function */
/**
* @description To set and read the band steering BandUtilizationThreshold parameters. 
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.UtilizationThreshold int r/w
*
* @param radioIndex - Radio Index
* @param pBuThreshold - Steering bane utilization threshold, to be returned
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
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.UtilizationThreshold int r/w
//to set and read the band steering BandUtilizationThreshold parameters 
INT wifi_getBandSteeringBandUtilizationThreshold (INT radioIndex, INT *pBuThreshold);

/* wifi_setBandSteeringBandUtilizationThreshold() function */
/**
* @description To set the band steering BandUtilizationThreshold parameters. 
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.UtilizationThreshold int r/w
*
* @param radioIndex - Radio Index
* @param buThreshold - Steering bane utilization threshold
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
INT wifi_setBandSteeringBandUtilizationThreshold (INT radioIndex, INT buThreshold);

/* wifi_getBandSteeringRSSIThreshold() function */
/**
* @description To read the band steering RSSIThreshold parameters. 
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.RSSIThreshold int r/w
*
* @param radioIndex - Radio Index
* @param pRssiThreshold - Band steering RSSIThreshold value, to be returned
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
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.RSSIThreshold int r/w
//to set and read the band steering RSSIThreshold parameters 
INT wifi_getBandSteeringRSSIThreshold (INT radioIndex, INT *pRssiThreshold);

/* wifi_setBandSteeringRSSIThreshold() function */
/**
* @description To set the band steering RSSIThreshold parameters. 
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.RSSIThreshold int r/w
*
* @param radioIndex - Radio Index
* @param rssiThreshold - Band steering RSSIThreshold value
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
INT wifi_setBandSteeringRSSIThreshold (INT radioIndex, INT rssiThreshold);

/* wifi_getBandSteeringPhyRateThreshold() function */
/**
* @description To read the band steering physical modulation rate threshold parameters.
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.PhyRateThreshold int r/w
*
* @param radioIndex - Radio Index
* @param pPrThreshold - Physical modulation rate threshold value, to be returned
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
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.PhyRateThreshold int r/w
//to set and read the band steering physical modulation rate threshold parameters 
INT wifi_getBandSteeringPhyRateThreshold (INT radioIndex, INT *pPrThreshold); //If chip is not support, return -1

/* wifi_setBandSteeringPhyRateThreshold() function */
/**
* @description To set the band steering physical modulation rate threshold parameters.
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.BandSetting.{i}.PhyRateThreshold int r/w
*
* @param radioIndex - Radio Index
* @param prThreshold - Physical modulation rate threshold value
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
INT wifi_setBandSteeringPhyRateThreshold (INT radioIndex, INT prThreshold); //If chip is not support, return -1

/* wifi_getBandSteeringLog() function */
/**
* @description To get the band steering log. If no steering or record_index is out of boundary, return 
-1.
* \n Device.WiFi.X_RDKCENTRAL-COM_BandSteering.History string r/o
*
* @param record_index - record index
* @param pSteeringTime - returns the UTC time in seconds
* @param pClientMAC - pClientMAC is pre allocated as 64bytes
* @param pSourceSSIDIndex - Source SSID index
* @param pDestSSIDIndex - Destination SSID index
* @param pSteeringReason - returns the predefined steering trigger reason
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
//Device.WiFi.X_RDKCENTRAL-COM_BandSteering.History string r/o
INT wifi_getBandSteeringLog(INT record_index, ULONG *pSteeringTime, CHAR *pClientMAC, INT *pSourceSSIDIndex, INT *pDestSSIDIndex, INT *pSteeringReason); //if no steering or redord_index is out of boundary, return -1. pSteeringTime returns the UTC time in seconds. pClientMAC is pre allocated as 64bytes. pSteeringReason returns the predefined steering trigger reason 
	
/* wifi_factoryResetAP() function */
/**
* @description Restore AP paramters to default without change other AP nor Radio parameters (No need to reboot wifi)
*
* @param apIndex - Access Point index
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

INT wifi_getBandSteeringApGroup(char *output_ApGroup);
INT wifi_getBandSteeringOverloadInactiveTime (INT radioIndex, INT *overloadInactiveTime);
INT wifi_getBandSteeringIdleInactiveTime (INT radioIndex, INT *idleInactiveTime);
INT wifi_setBandSteeringApGroup(char *ApGroup);
INT wifi_setBandSteeringOverloadInactiveTime (INT radioIndex, INT overloadInactiveTime);
INT wifi_setBandSteeringIdleInactiveTime (INT radioIndex, INT idleInactiveTime);
INT wifi_getBandSteeringOverloadInactiveTime (INT radioIndex, INT *overloadInactiveTime);
INT wifi_setBandSteeringSSIDSecurityParams ();
INT wifi_checkSSIDSecurityParams ();
INT wifi_resetBandSteeringSSIDSecurityParams ();


INT wifi_factoryResetAP(int apIndex); 	//Restore AP paramters to default without change other AP nor Radio parameters (No need to reboot wifi)

/* wifi_setRadioCtsProtectionEnable() function */
/**
* @description Enables CTS protection for the radio used by this AP
*
* @param apIndex - Access Point index
* @param enable - CTS protection enable value
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
INT wifi_setRadioCtsProtectionEnable(INT apIndex, BOOL enable);          //P3 // enables CTS protection for the radio used by this AP

/* wifi_setRadioObssCoexistenceEnable() function */
/**
* @description enables OBSS Coexistence - fall back to 20MHz if necessary for the radio used by this 
AP.
*
* @param apIndex - Access Point index
* @param enable - OBSS Coexistence enable value
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
INT wifi_setRadioObssCoexistenceEnable(INT apIndex, BOOL enable);        // enables OBSS Coexistence - fall back to 20MHz if necessary for the radio used by this ap

/* wifi_setRadioFragmentationThreshold() function */
/**
* @description Sets the fragmentation threshold in bytes for the radio used by this 
AP.
*
* @param apIndex - Access Point index
* @param threshold - Fragmentation Threshold value
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
INT wifi_setRadioFragmentationThreshold(INT apIndex, UINT threshold);    //P3 // sets the fragmentation threshold in bytes for the radio used by this ap

/* wifi_setRadioSTBCEnable() function */
/**
* @description Enable STBC mode in the hardware, 0 == not enabled, 1 == enabled.
*
* @param radioIndex - Radio index
* @param STBC_Enable - STBC mode enable value
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
INT wifi_setRadioSTBCEnable(INT radioIndex, BOOL STBC_Enable);           // enable STBC mode in the hardwarwe, 0 == not enabled, 1 == enabled 

/* wifi_getRadioAMSDUEnable() function */
/**
* @description Outputs A-MSDU enable status, 0 == not enabled, 1 == enabled.
*
* @param radioIndex - Radio index
* @param output_bool - A-MSDU enable status, to be returned
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
INT wifi_getRadioAMSDUEnable(INT radioIndex, BOOL *output_bool);         // outputs A-MSDU enable status, 0 == not enabled, 1 == enabled 

/* wifi_setRadioAMSDUEnable() function */
/**
* @description Enables A-MSDU in the hardware, 0 == not enabled, 1 == enabled.
*
* @param radioIndex - Radio index
* @param amsduEnable - A-MSDU enable status value
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
INT wifi_setRadioAMSDUEnable(INT radioIndex, BOOL amsduEnable);          // enables A-MSDU in the hardware, 0 == not enabled, 1 == enabled  

/* wifi_getRadioTxChainMask() function */
/**
* @description Outputs the number of Tx streams.
*
* @param radioIndex - Radio index
* @param output_int - Number of Tx streams, to be returned
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
INT wifi_getRadioTxChainMask(INT radioIndex, INT *output_int);           //P2  // outputs the number of Tx streams

/* wifi_setRadioTxChainMask() function */
/**
* @description Sets the number of Tx streams to an enviornment variable.
*
* @param radioIndex - Radio index
* @param numStreams - Number of Tx streams
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
INT wifi_setRadioTxChainMask(INT radioIndex, INT numStreams);            //P2  // sets the number of Tx streams to an enviornment variable  

/* wifi_getRadioRxChainMask() function */
/**
* @description Outputs the number of Rx streams.
*
* @param radioIndex - Radio index
* @param output_int - Number of Rx streams, to be returned
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
INT wifi_getRadioRxChainMask(INT radioIndex, INT *output_int);           //P2  // outputs the number of Rx streams

/* wifi_setRadioRxChainMask() function */
/**
* @description Sets the number of Rx streams to an enviornment variable.
*
* @param radioIndex - Radio index
* @param numStreams - Number of Rx streams
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
INT wifi_setRadioRxChainMask(INT radioIndex, INT numStreams);            //P2  // sets the number of Rx streams to an enviornment variable

//>> Deprecated: 
/** Deprecated */
INT wifi_pushBridgeInfo(INT apIndex); 									 //P2  // Push the BridgeInfo environment variables to the hardware
/** Deprecated */
INT wifi_pushRadioChannel(INT radioIndex, UINT channel);                 //P2  // push the channel number setting to the hardware  //Applying changes with wifi_applyRadioSettings().
/** Deprecated */
INT wifi_pushRadioChannelMode(INT radioIndex);                           //P2  // push the channel mode enviornment variable that is set by "wifi_setChannelMode()" to the hardware  //Applying changes with wifi_applyRadioSettings().
/** Deprecated */
INT wifi_pushRadioTxChainMask(INT radioIndex);                           //P2  // push the enviornment varible that is set by "wifi_setTxChainMask()" to the hardware //Applying changes with wifi_applyRadioSettings().
/** Deprecated */
INT wifi_pushRadioRxChainMask(INT radioIndex);                           //P2  // push the enviornment varible that is set by "wifi_setRxChainMask()" to the hardware //Applying changes with wifi_applyRadioSettings().
//<<

/* wifi_pushSSID() function */
/**
* @description Push the enviornment varible that is set by "wifi_setSsidName" to the hardware.
*
* @param apIndex - Access Point index
* @param ssid - WiFi SSID value
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
INT wifi_pushSSID(INT apIndex, CHAR *ssid); 							 // push the enviornment varible that is set by "wifi_setSsidName" to the hardware    

/* wifi_pushSsidAdvertisementEnable() function */
/**
* @description Push the enviornment varible that is set by "wifi_setApSsidAdvertisementEnable" to the hardware.
*
* @param apIndex - Access Point index
* @param enable - SSID Advertisement value
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
INT wifi_pushSsidAdvertisementEnable(INT apIndex, BOOL enable);			 // push the enviornment varible that is set by "wifi_setApSsidAdvertisementEnable" to the hardware	

/* wifi_getRadioUpTime() function */
/**
* @description Get the number of seconds elapsed since radio is started.
*
* @param radioIndex - Radio index
* @param uptime - Wifi uptime, to be returned
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
INT wifi_getRadioUpTime(INT radioIndex, ULONG *uptime);					 // get the number of seconds elapsed since radio is started	 


/* wifi_getRadioReverseDirectionGrantSupported() function */
/**
* @description Get radio RDG enable Support.
*
* @param radioIndex - Radio index
* @param output_bool - RDG enable support value, to be returned
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
INT wifi_getRadioReverseDirectionGrantSupported(INT radioIndex, BOOL *output_bool);    //Get radio RDG enable Support

/* wifi_getRadioReverseDirectionGrantEnable() function */
/**
* @description Get radio RDG enable setting.
*
* @param radioIndex - Radio index
* @param output_bool - RDG enable setting value, to be returned
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
INT wifi_getRadioReverseDirectionGrantEnable(INT radioIndex, BOOL *output_bool);    //Get radio RDG enable setting

/* wifi_setRadioReverseDirectionGrantEnable() function */
/**
* @description Set radio RDG enable setting.
*
* @param radioIndex - Radio index
* @param enable - RDG enable setting value
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
INT wifi_setRadioReverseDirectionGrantEnable(INT radioIndex, BOOL enable);			//Set radio RDG enable setting

/* wifi_getRadioDeclineBARequestEnable() function */
/**
* @description Get radio ADDBA (ADD Block Acknowledgement) enable setting.
*
* @param radioIndex - Radio index
* @param output_bool - Radio ADDBA enable setting value, to be returned
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
INT wifi_getRadioDeclineBARequestEnable(INT radioIndex, BOOL *output_bool);			//Get radio ADDBA enable setting

/* wifi_setRadioDeclineBARequestEnable() function */
/**
* @description Set radio ADDBA (ADD Block Acknowledgement) enable setting.
*
* @param radioIndex - Radio index
* @param enable - Radio ADDBA enable setting value
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
INT wifi_setRadioDeclineBARequestEnable(INT radioIndex, BOOL enable);				//Set radio ADDBA enable setting

/* wifi_getRadioAutoBlockAckEnable() function */
/**
* @description Get radio auto block ack enable setting.
*
* @param radioIndex - Radio index
* @param output_bool - Auto block ack enable setting value, to be returned
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
INT wifi_getRadioAutoBlockAckEnable(INT radioIndex, BOOL *output_bool);				//Get radio auto block ack enable setting

/* wifi_setRadioAutoBlockAckEnable() function */
/**
* @description Set radio auto block ack enable setting.
*
* @param radioIndex - Radio index
* @param enable - Auto block ack enable setting value
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
INT wifi_setRadioAutoBlockAckEnable(INT radioIndex, BOOL enable);					//Set radio auto block ack enable setting

/* wifi_getRadio11nGreenfieldSupported() function */
/**
* @description Get radio 11n pure mode enable support.
*
* @param radioIndex - Radio index
* @param output_bool - Radio 11n pure mode enable support value, to be returned
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
INT wifi_getRadio11nGreenfieldSupported(INT radioIndex, BOOL *output_bool);			//Get radio 11n pure mode enable Support

/* wifi_getRadio11nGreenfieldEnable() function */
/**
* @description Get radio 11n pure mode enable setting.
*
* @param radioIndex - Radio index
* @param output_bool - Radio 11n pure mode enable setting, to be returned
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
INT wifi_getRadio11nGreenfieldEnable(INT radioIndex, BOOL *output_bool);			//Get radio 11n pure mode enable setting

/* wifi_setRadio11nGreenfieldEnable() function */
/**
* @description Set radio 11n pure mode enable setting.
*
* @param radioIndex - Radio index
* @param enable - Radio 11n pure mode enable setting
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
INT wifi_setRadio11nGreenfieldEnable(INT radioIndex, BOOL enable);					//Set radio 11n pure mode enable setting

/* wifi_getRadioIGMPSnoopingEnable() function */
/**
* @description Get radio IGMP snooping enable setting.
*
* @param radioIndex - Radio index
* @param output_bool - Radio IGMP snooping enable setting, to be returned
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
INT wifi_getRadioIGMPSnoopingEnable(INT radioIndex, BOOL *output_bool);				//Get radio IGMP snooping enable setting

/* wifi_setRadioIGMPSnoopingEnable() function */
/**
* @description Set radio IGMP snooping enable setting.
*
* @param radioIndex - Radio index
* @param enable - Radio IGMP snooping enable setting
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
INT wifi_setRadioIGMPSnoopingEnable(INT radioIndex, BOOL enable);					//Set radio IGMP snooping enable setting
//---------------------------------------------------------------------------------------------------
//
// Additional Wifi AP level APIs used for Access Point devices
//
//---------------------------------------------------------------------------------------------------


//AP HAL

/* wifi_createAp() function */
/**
* @description creates a new ap and pushes these parameters to the hardware.
*
* @param apIndex - Access Point index
* @param radioIndex - Radio index
* @param essid - SSID Name
* @param hideSsid - True/False, to SSID advertisement enable value
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
INT wifi_createAp(INT apIndex, INT radioIndex, CHAR *essid, BOOL hideSsid);  // creates a new ap and pushes these parameters to the hardware

/* wifi_deleteAp() function */
/**
* @description Deletes this ap entry on the hardware, clears all internal variables associaated with this ap.
*
* @param apIndex - Access Point index
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
INT wifi_deleteAp(INT apIndex);                                     // deletes this ap entry on the hardware, clears all internal variables associaated with this ap

/* wifi_getApName() function */
/**
* @description Outputs a 16 byte or less name assocated with the AP.  
* String buffer must be pre-allocated by the caller.
*
* @param apIndex - Access Point index
* @param output_string - Access Point name, to be returned 
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
INT wifi_getApName(INT apIndex, CHAR *output_string);                 // Outputs a 16 byte or less name assocated with the AP.  String buffer must be pre-allocated by the caller

/* wifi_getApIndexFromName() function */
/**
* @description Outputs the index number in that corresponds to the SSID string.
*
* @param inputSsidString - WiFi SSID Name
* @param ouput_int - Access Point index, to be returned 
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
INT wifi_getApIndexFromName(CHAR *inputSsidString, INT *ouput_int);	 // Outputs the index number in that corresponds to the SSID string

/* wifi_getApBeaconType() function */
/**
* @description Outputs a 32 byte or less string indicating the beacon type as "None", "Basic", "WPA", "11i", "WPAand11i".
*
* @param apIndex - Access Point index
* @param output_string - Beacon type, to be returned 
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
INT wifi_getApBeaconType(INT apIndex, CHAR *output_string);           // Outputs a 32 byte or less string indicating the beacon type as "None", "Basic", "WPA", "11i", "WPAand11i"

/* wifi_setApBeaconType() function */
/**
* @description Sets the beacon type enviornment variable. Allowed input strings are "None", "Basic", "WPA, "11i", "WPAand11i".
*
* @param apIndex - Access Point index
* @param beaconTypeString - Beacon type 
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
INT wifi_setApBeaconType(INT apIndex, CHAR *beaconTypeString);        // Sets the beacon type enviornment variable. Allowed input strings are "None", "Basic", "WPA, "11i", "WPAand11i"

/* wifi_setApBeaconInterval() function */
/**
* @description Sets the beacon interval on the hardware for this AP.
*
* @param apIndex - Access Point index
* @param beaconInterval - Beacon interval 
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
INT wifi_setApBeaconInterval(INT apIndex, INT beaconInterval);        // sets the beacon interval on the hardware for this AP

/* wifi_setApDTIMInterval() function */
/**
* @description Sets the DTIM interval for this AP.
*
* @param apIndex - Access Point index
* @param dtimInterval - DTIM interval 
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
INT wifi_setApDTIMInterval(INT apIndex, INT dtimInterval);			  // Sets the DTIM interval for this AP	

/* wifi_getApRtsThresholdSupported() function */
/**
* @description Get the packet size threshold supported.
*
* @param apIndex - Access Point index
* @param output_bool - Packet size threshold supported, to be returned 
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
INT wifi_getApRtsThresholdSupported(INT apIndex, BOOL *output_bool);  // Get the packet size threshold supported. 

/* wifi_setApRtsThreshold() function */
/**
* @description Sets the packet size threshold in bytes to apply RTS/CTS backoff rules.
*
* @param apIndex - Access Point index
* @param threshold - Packet size threshold 
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
INT wifi_setApRtsThreshold(INT apIndex, UINT threshold);              // sets the packet size threshold in bytes to apply RTS/CTS backoff rules. 

/* wifi_getApWpaEncryptionMode() function */
/**
* @description ouputs up to a 32 byte string as either "TKIPEncryption", "AESEncryption", or "TKIPandAESEncryption".
*
* @param apIndex - Access Point index
* @param output_string - WPA Encryption mode, to be returned 
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
INT wifi_getApWpaEncryptionMode(INT apIndex, CHAR *output_string);    // ouputs up to a 32 byte string as either "TKIPEncryption", "AESEncryption", or "TKIPandAESEncryption"

/* wifi_setApWpaEncryptionMode() function */
/**
* @description Sets the encyption mode enviornment variable.  Valid string format is "TKIPEncryption", "AESEncryption", or "TKIPandAESEncryption".
*
* @param apIndex - Access Point index
* @param encMode - WPA Encryption mode 
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
INT wifi_setApWpaEncryptionMode(INT apIndex, CHAR *encMode);          // sets the encyption mode enviornment variable.  Valid string format is "TKIPEncryption", "AESEncryption", or "TKIPandAESEncryption"

/* wifi_removeApSecVaribles() function */
/**
* @description Deletes internal security varable settings for this ap.
*
* @param apIndex - Access Point index
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
INT wifi_removeApSecVaribles(INT apIndex);                            // deletes internal security varable settings for this ap

/* wifi_disableApEncryption() function */
/**
* @description changes the hardware settings to disable encryption on this ap.
*
* @param apIndex - Access Point index
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
INT wifi_disableApEncryption(INT apIndex);                            // changes the hardware settings to disable encryption on this ap

/* wifi_setApAuthMode() function */
/**
* @description Set the authorization mode on this ap. mode mapping as: 1: open, 2: shared, 4:auto.
*
* @param apIndex - Access Point index
* @param mode - Authorization mode
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
INT wifi_setApAuthMode(INT apIndex, INT mode);                        // set the authorization mode on this ap. mode mapping as: 1: open, 2: shared, 4:auto

/* wifi_setApBasicAuthenticationMode() function */
/**
* @description Sets an enviornment variable for the authMode. Valid strings are "None", "EAPAuthentication" or "SharedAuthentication".
*
* @param apIndex - Access Point index
* @param authMode - Authentication mode
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
INT wifi_setApBasicAuthenticationMode(INT apIndex, CHAR *authMode);   // sets an enviornment variable for the authMode. Valid strings are "None", "EAPAuthentication" or "SharedAuthentication"

/* wifi_getApBasicAuthenticationMode() function */
/**
* @description Gets an enviornment variable for the authMode.
*
* @param apIndex - Access Point index
* @param authMode - Authentication mode
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
INT wifi_getApBasicAuthenticationMode(INT apIndex, CHAR *authMode);   // gets an enviornment variable for the authMode. Valid strings are "None", "EAPAuthentication" or "SharedAuthentication"


/* wifi_getApNumDevicesAssociated() function */
/**
* @description Outputs the number of stations associated per AP.
*
* @param apIndex - Access Point index
* @param output_ulong - Number of stations, to be returned
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
INT wifi_getApNumDevicesAssociated(INT apIndex, ULONG *output_ulong); // Outputs the number of stations associated per AP

/* wifi_kickApAssociatedDevice() function */
/**
* @description Manually removes any active wi-fi association with the device specified on this ap.
*
* @param apIndex - Access Point index
* @param client_mac - Client device MAC address
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
INT wifi_kickApAssociatedDevice(INT apIndex, CHAR *client_mac);  	// manually removes any active wi-fi association with the device specified on this ap

/* wifi_getApRadioIndex() function */
/**
* @description Outputs the radio index for the specified ap.
*
* @param apIndex - Access Point index
* @param output_int - Radio index, to be returned
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
INT wifi_getApRadioIndex(INT apIndex, INT *output_int);                // outputs the radio index for the specified ap

/* wifi_setApRadioIndex() function */
/**
* @description Sets the radio index for the specific ap.
*
* @param apIndex - Access Point index
* @param radioIndex - Radio index
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
INT wifi_setApRadioIndex(INT apIndex, INT radioIndex);                // sets the radio index for the specific ap

/* wifi_getApAclDevices() function */
/**
* @description Get the ACL MAC list per AP.
*
* @param apIndex - Access Point index
* @param macArray - Mac Array list, to be returned
* @param buf_size - Buffer size for the mac array list
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
INT wifi_getApAclDevices(INT apIndex, CHAR *macArray, UINT buf_size);	// Get the ACL MAC list per AP

/* wifi_addApAclDevice() function */
/**
* @description Adds the mac address to the filter list.
*
* @param apIndex - Access Point index
* @param DeviceMacAddress - Mac Address of a device
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
INT wifi_addApAclDevice(INT apIndex, CHAR *DeviceMacAddress);         // adds the mac address to the filter list

/* wifi_delApAclDevice() function */
/**
* @description Deletes the mac address from the filter list.
*
* @param apIndex - Access Point index
* @param DeviceMacAddress - Mac Address of a device
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
INT wifi_delApAclDevice(INT apIndex, CHAR *DeviceMacAddress);         // deletes the mac address from the filter list

/* wifi_getApAclDeviceNum() function */
/**
* @description Outputs the number of devices in the filter list.
*
* @param apIndex - Access Point index
* @param output_uint - Number of devices, to be returned
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
INT wifi_getApAclDeviceNum(INT apIndex, UINT *output_uint);           // outputs the number of devices in the filter list

/* wifi_kickApAclAssociatedDevices() function */
/**
* @description Enable kick for devices on acl black list.
*
* @param apIndex - Access Point index
* @param enable - Enable/disable kick for devices on acl black list
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
INT wifi_kickApAclAssociatedDevices(INT apIndex,BOOL enable);         // enable kick for devices on acl black list

/* wifi_setApMacAddressControlMode() function */
/**
* @description Sets the mac address filter control mode.  0 == filter disabled, 1 == filter as whitelist, 2 == filter as blacklist.
*
* @param apIndex - Access Point index
* @param filterMode - Mac Address filter control mode
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
INT wifi_setApMacAddressControlMode(INT apIndex, INT filterMode);     // sets the mac address filter control mode.  0 == filter disabled, 1 == filter as whitelist, 2 == filter as blacklist

/* wifi_setApVlanEnable() function */
/**
* @description Enables internal gateway VLAN mode.  In this mode a Vlan tag is added to upstream (received) data packets before exiting the Wifi driver.  VLAN tags in downstream data are stripped from data packets before transmission.  Default is FALSE.
*
* @param apIndex - Access Point index
* @param VlanEnabled - Internal gateway VLAN mode
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
INT wifi_setApVlanEnable(INT apIndex, BOOL VlanEnabled);              // enables internal gateway VLAN mode.  In this mode a Vlan tag is added to upstream (received) data packets before exiting the Wifi driver.  VLAN tags in downstream data are stripped from data packets before transmission.  Default is FALSE. 

/* wifi_setApVlanID() function */
/**
* @description Sets the vlan ID for this ap to an internal enviornment variable.
*
* @param apIndex - Access Point index
* @param vlanId - VLAN ID
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
INT wifi_setApVlanID(INT apIndex, INT vlanId);                        // sets the vlan ID for this ap to an internal enviornment variable

/* wifi_getApBridgeInfo() function */
/**
* @description Gets bridgeName, IP address and Subnet.BridgeName is a maximum of 32 characters.
*
* @param index - Access Point index
* @param bridgeName - Bridge name, to be returned
* @param IP - IP Address, to be returned
* @param subnet - Subnet, to be returned
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
INT wifi_getApBridgeInfo(INT index, CHAR *bridgeName, CHAR *IP, CHAR *subnet);	// gets bridgeName, IP address and Subnet.

/* wifi_setApBridgeInfo() function */
/**
* @description Sets bridgeName, IP address and Subnet to internal enviornment variables. BridgeName is a maximum of 32 characters.
*
* @param apIndex - Access Point index
* @param bridgeName - Bridge name
* @param IP - IP Address
* @param subnet - Subnet
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
INT wifi_setApBridgeInfo(INT apIndex, CHAR *bridgeName, CHAR *IP, CHAR *subnet);   //sets bridgeName, IP address and Subnet to internal enviornment variables. bridgeName is a maximum of 32 characters, 
//INT wifi_pushApBridgeInfo(INT apIndex);                               // push the BridgeInfo enviornment variables to the hardware //Applying changes with wifi_applyRadioSettings()

/* wifi_resetApVlanCfg() function */
/**
* @description Reset the vlan configuration for this ap.
*
* @param apIndex - Access Point index
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
INT wifi_resetApVlanCfg(INT apIndex);                                 // reset the vlan configuration for this ap
//INT wifi_setApBridging(INT apIndex, BOOL bridgeEnable);             // set the enviornment variables to control briding.  If isolation is requried then disable bridging.  //use wifi_setApIsolationEnable instead
//INT wifi_getApRouterEnable(INT apIndex, BOOL *output_bool);           //P4 // Outputs a bool that indicates if router is enabled for this ap
//INT wifi_setApRouterEnable(INT apIndex, BOOL routerEnabled);          //P4 // sets the routerEnabled variable for this ap

/* wifi_createHostApdConfig() function */
/**
* @description Creates configuration variables needed for WPA/WPS.  These variables are implementation dependent and in some implementations these variables are used by hostapd when it is started.  Specific variables that are needed are dependent on the hostapd implementation. These variables are set by WPA/WPS security functions in this wifi HAL.  If not needed for a particular implementation this function may simply return no error.
*
* @param apIndex - Access Point index
* @param createWpsCfg - Enable/Disable WPS Configuration creation
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
INT wifi_createHostApdConfig(INT apIndex, BOOL createWpsCfg);       // creates configuration variables needed for WPA/WPS.  These variables are implementation dependent and in some implementations these variables are used by hostapd when it is started.  Specific variables that are needed are dependent on the hostapd implementation. These variables are set by WPA/WPS security functions in this wifi HAL.  If not needed for a particular implementation this function may simply return no error.

/* wifi_startHostApd() function */
/**
* @description Starts hostapd, uses the variables in the hostapd config with format compatible with the specific hostapd implementation.
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
INT wifi_startHostApd();                                            // starts hostapd, uses the variables in the hostapd config with format compatible with the specific hostapd implementation

/* wifi_stopHostApd() function */
/**
* @description Stops hostapd
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
INT wifi_stopHostApd();                                             // stops hostapd

/* wifi_restartHostApd() function */
/**
* @description restarts hostapd
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
INT wifi_restartHostApd();
//-----------------------------------------------------------------------------------------------

/* wifi_setApEnable() function */
/**
* @description Sets the AP enable status variable for the specified ap.
*
* @param apIndex - Access Point index
* @param enable - Enable/Disable AP enable status variable
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
//Device.WiFi.AccessPoint.{i}.	
//Device.WiFi.AccessPoint.{i}.Enable
INT wifi_setApEnable(INT apIndex, BOOL enable);                       // sets the AP enable status variable for the specified ap.

/* wifi_getApEnable() function */
/**
* @description Outputs the setting of the internal variable that is set by wifi_setEnable().
*
* @param apIndex - Access Point index
* @param output_bool - AP enable status, to be returned
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
INT wifi_getApEnable(INT apIndex, BOOL *output_bool);                 // Outputs the setting of the internal variable that is set by wifi_setEnable().  


/* wifi_getApStatus() function */
/**
* @description Outputs the AP "Enabled" "Disabled" status from driver. 
* \n Device.WiFi.AccessPoint.{i}.Status
*
* @param apIndex - Access Point index
* @param output_string - AP status, to be returned
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
//Device.WiFi.AccessPoint.{i}.Status
INT wifi_getApStatus(INT apIndex, CHAR *output_string);  				// Outputs the AP "Enabled" "Disabled" status from driver 

/* wifi_getApSsidAdvertisementEnable() function */
/**
* @description Indicates whether or not beacons include the SSID name. Outputs 1 if SSID on the AP is enabled, else ouputs 0.
* \n Device.WiFi.AccessPoint.{i}.SSIDAdvertisementEnabled
*
* @param apIndex - Access Point index
* @param output_bool - SSID Advertisement enabled, to be returned
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
//Device.WiFi.AccessPoint.{i}.SSIDAdvertisementEnabled		
//Indicates whether or not beacons include the SSID name.
INT wifi_getApSsidAdvertisementEnable(INT apIndex, BOOL *output_bool);// outputs a 1 if SSID on the AP is enabled, else ouputs 0

/* wifi_setApSsidAdvertisementEnable() function */
/**
* @description Sets an internal variable for ssid advertisement.  Set to 1 to enable, set to 0 to disable.
* \n Device.WiFi.AccessPoint.{i}.SSIDAdvertisementEnabled
*
* @param apIndex - Access Point index
* @param enable - SSID Advertisement enable value
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
INT wifi_setApSsidAdvertisementEnable(INT apIndex, BOOL enable);      // sets an internal variable for ssid advertisement.  Set to 1 to enable, set to 0 to disable

/* wifi_pushApSsidAdvertisementEnable() function */
/**
* @description Push the ssid advertisement enable variable to the hardware //Applying changs with wifi_applyRadioSettings().
*
* @param apIndex - Access Point index
* @param enable - SSID Advertisement enable value
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
INT wifi_pushApSsidAdvertisementEnable(INT apIndex, BOOL enable);     // push the ssid advertisement enable variable to the hardware //Applying changs with wifi_applyRadioSettings()

/* wifi_getApRetryLimit() function */
/**
* @description Get the maximum number of retransmission for a packet. This corresponds to IEEE 802.11 parameter dot11ShortRetryLimit.
* \n Device.WiFi.AccessPoint.{i}.RetryLimit
*
* @param apIndex - Access Point index
* @param output - Maximum number of retransmission for a packet, to be returned
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
//Device.WiFi.AccessPoint.{i}.RetryLimit		
//The maximum number of retransmission for a packet. This corresponds to IEEE 802.11 parameter dot11ShortRetryLimit.
INT wifi_getApRetryLimit(INT apIndex, UINT *output); 

/* wifi_setApRetryLimit() function */
/**
* @description Set the maximum number of retransmission for a packet. This corresponds to IEEE 802.11 parameter dot11ShortRetryLimit.
* \n Device.WiFi.AccessPoint.{i}.RetryLimit
*
* @param apIndex - Access Point index
* @param number - Maximum number of retransmission for a packet
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
INT wifi_setApRetryLimit(INT apIndex, UINT number); 

INT wifi_getApEnableOnLine(INT wlanIndex, BOOL *enabled);

/* wifi_getApWMMCapability() function */
/**
* @description Indicates whether this access point supports WiFi Multimedia (WMM) Access Categories (AC).
* \n Device.WiFi.AccessPoint.{i}.WMMCapability
*
* @param apIndex - Access Point index
* @param output - WMM capability, to be returned
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
//Device.WiFi.AccessPoint.{i}.WMMCapability	
//Indicates whether this access point supports WiFi Multimedia (WMM) Access Categories (AC).
INT wifi_getApWMMCapability(INT apIndex, BOOL *output); 

/* wifi_getApUAPSDCapability() function */
/**
* @description Indicates whether this access point supports WMM Unscheduled Automatic Power Save Delivery (U-APSD). Note: U-APSD support implies WMM support.
* \n Device.WiFi.AccessPoint.{i}.UAPSDCapability
*
* @param apIndex - Access Point index
* @param output - U-APSD capability, to be returned
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
//Device.WiFi.AccessPoint.{i}.UAPSDCapability		
//Indicates whether this access point supports WMM Unscheduled Automatic Power Save Delivery (U-APSD). Note: U-APSD support implies WMM support.
INT wifi_getApUAPSDCapability(INT apIndex, BOOL *output); 			
			
/* wifi_getApWmmEnable() function */
/**
* @description Indicates whether WMM support is currently enabled. When enabled, this is indicated in beacon frames.
* \n Device.WiFi.AccessPoint.{i}.WMMEnable
*
* @param apIndex - Access Point index
* @param output - WMM support enabled status, to be returned
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
//Device.WiFi.AccessPoint.{i}.WMMEnable		
//Whether WMM support is currently enabled. When enabled, this is indicated in beacon frames.
INT wifi_getApWmmEnable(INT apIndex, BOOL *output);                   

/* wifi_setApWmmEnable() function */
/**
* @description Enables/disables WMM on the hardwawre for this AP.  enable==1, disable == 0.
*
* @param apIndex - Access Point index
* @param enable - WMM support enabled status
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
INT wifi_setApWmmEnable(INT apIndex, BOOL enable);                    // enables/disables WMM on the hardwawre for this AP.  enable==1, disable == 0

/* wifi_getApWmmUapsdEnable() function */
/**
* @description Indicates whether U-APSD support is currently enabled. When enabled, this is indicated in beacon frames. Note: U-APSD can only be enabled if WMM is also enabled.
* \n Device.WiFi.AccessPoint.{i}.UAPSDEnable
*
* @param apIndex - Access Point index
* @param output - U-APSD support enabled status, to be returned
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
//Device.WiFi.AccessPoint.{i}.UAPSDEnable		
//Whether U-APSD support is currently enabled. When enabled, this is indicated in beacon frames. Note: U-APSD can only be enabled if WMM is also enabled.
INT wifi_getApWmmUapsdEnable(INT apIndex, BOOL *output);               

/* wifi_setApWmmUapsdEnable() function */
/**
* @description Enables/disables Automatic Power Save Delivery on the hardwarwe for this AP.
*
* @param apIndex - Access Point index
* @param enable - U-APSD enable/disable value
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
INT wifi_setApWmmUapsdEnable(INT apIndex, BOOL enable);               // enables/disables Automatic Power Save Delivery on the hardwarwe for this AP

/* wifi_setApWmmOgAckPolicy() function */
/**
* @description Sets the WMM ACK policy on the hardware. AckPolicy false means do not acknowledge, true means acknowledge.
*
* @param apIndex - Access Point index
* @param class
* @param ackPolicy - Acknowledge policy
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
// Sets the WMM ACK polity on the hardware. AckPolicy false means do not acknowledge, true means acknowledge
INT wifi_setApWmmOgAckPolicy(INT apIndex, INT class, BOOL ackPolicy);  //RDKB
			
/* wifi_getApIsolationEnable() function */
/**
* @description Get AP isolation value.A value of true means that the devices connected to the Access Point are isolated from all other devices within the home network (as is typically the case for a Wireless Hotspot).
* \n Device.WiFi.AccessPoint.{i}.IsolationEnable
*
* @param apIndex - Access Point index
* @param output - AP Isolation enable, to be returned 
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
//Device.WiFi.AccessPoint.{i}.IsolationEnable	
//Enables or disables device isolation.	A value of true means that the devices connected to the Access Point are isolated from all other devices within the home network (as is typically the case for a Wireless Hotspot).	
INT wifi_getApIsolationEnable(INT apIndex, BOOL *output); //Tr181		

/* wifi_setApIsolationEnable() function */
/**
* @description Enables or disables device isolation. A value of true means that the devices connected to the Access Point are isolated from all other devices within the home network (as is typically the case for a Wireless Hotspot).
* \n Device.WiFi.AccessPoint.{i}.IsolationEnable
*
* @param apIndex - Access Point index
* @param enable - AP Isolation enable value 
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
INT wifi_setApIsolationEnable(INT apIndex, BOOL enable); //Tr181					

/* wifi_getApMaxAssociatedDevices() function */
/**
* @description Get maximum associated devices with the Access Point index. 
* The maximum number of devices that can simultaneously be connected to the access point. A value of 0 means that there is no specific limit.
* \n Device.WiFi.AccessPoint.{i}.MaxAssociatedDevices
*
* @param apIndex - Access Point index
* @param output - Maximum associated devices, to be returned 
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
//Device.WiFi.AccessPoint.{i}.MaxAssociatedDevices	
//The maximum number of devices that can simultaneously be connected to the access point. A value of 0 means that there is no specific limit.			
INT wifi_getApMaxAssociatedDevices(INT apIndex, UINT *output); //Tr181		

/* wifi_setApMaxAssociatedDevices() function */
/**
* @description Set maximum associated devices with the Access Point index. 
* The maximum number of devices that can simultaneously be connected to the access point. A value of 0 means that there is no specific limit.
* \n Device.WiFi.AccessPoint.{i}.MaxAssociatedDevices
*
* @param apIndex - Access Point index
* @param number - Maximum associated devices 
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
INT wifi_setApMaxAssociatedDevices(INT apIndex, UINT number); //Tr181					
					
/* wifi_getApAssociatedDevicesHighWatermarkThreshold() function */
/**
* @description Get the HighWatermarkThreshold value, that is lesser than or equal to MaxAssociatedDevices. Setting this parameter does not actually limit the number of clients that can associate with this access point as that is controlled by MaxAssociatedDevices.	MaxAssociatedDevices or 50. The default value of this parameter should be equal to MaxAssociatedDevices. In case MaxAssociatedDevices is 0 (zero), the default value of this parameter should be 50. A value of 0 means that there is no specific limit and Watermark calculation algorithm should be turned off.
* \n Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkThreshold
*
* @param apIndex - Access Point index
* @param output - HighWatermarkThreshold value, to be returned 
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
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkThreshold	
//The HighWatermarkThreshold value that is lesser than or equal to MaxAssociatedDevices. Setting this parameter does not actually limit the number of clients that can associate with this access point as that is controlled by MaxAssociatedDevices.	MaxAssociatedDevices or 50. The default value of this parameter should be equal to MaxAssociatedDevices. In case MaxAssociatedDevices is 0 (zero), the default value of this parameter should be 50. A value of 0 means that there is no specific limit and Watermark calculation algorithm should be turned off.			
INT wifi_getApAssociatedDevicesHighWatermarkThreshold(INT apIndex, UINT *output); //Tr181	//P3

/* wifi_setApAssociatedDevicesHighWatermarkThreshold() function */
/**
* @description Set the HighWatermarkThreshold value, that is lesser than or equal to MaxAssociatedDevices. Setting this parameter does not actually limit the number of clients that can associate with this access point as that is controlled by MaxAssociatedDevices.	MaxAssociatedDevices or 50. The default value of this parameter should be equal to MaxAssociatedDevices. In case MaxAssociatedDevices is 0 (zero), the default value of this parameter should be 50. A value of 0 means that there is no specific limit and Watermark calculation algorithm should be turned off.
* \n Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkThreshold
*
* @param apIndex - Access Point index
* @param Threshold - HighWatermarkThreshold value 
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
INT wifi_setApAssociatedDevicesHighWatermarkThreshold(INT apIndex, UINT Threshold); //Tr181		//P3			

/* wifi_getApAssociatedDevicesHighWatermarkThresholdReached() function */
/**
* @description Get the number of times the current total number of associated device has reached the HighWatermarkThreshold value. This calculation can be based on the parameter AssociatedDeviceNumberOfEntries as well. Implementation specifics about this parameter are left to the product group and the device vendors. It can be updated whenever there is a new client association request to the access point.
* \n Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkThresholdReached
* @param apIndex - Access Point index
* @param output - Number of times the current total number of associated device has reached the HighWatermarkThreshold value, to be returned 
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
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkThresholdReached		
//Number of times the current total number of associated device has reached the HighWatermarkThreshold value. This calculation can be based on the parameter AssociatedDeviceNumberOfEntries as well. Implementation specifics about this parameter are left to the product group and the device vendors. It can be updated whenever there is a new client association request to the access point.	
INT wifi_getApAssociatedDevicesHighWatermarkThresholdReached(INT apIndex, UINT *output); //Tr181 //P3

/* wifi_getApAssociatedDevicesHighWatermark() function */
/**
* @description Maximum number of associated devices that have ever associated with the access point concurrently since the last reset of the device or WiFi module.
* \n Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermark
*
* @param apIndex - Access Point index
* @param output - Maximum number of associated devices that have ever associated with the access point concurrently, to be returned 
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
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermark	
//Maximum number of associated devices that have ever associated with the access point concurrently since the last reset of the device or WiFi module.	
INT wifi_getApAssociatedDevicesHighWatermark(INT apIndex, UINT *output); //Tr181	//P3

/* wifi_getApAssociatedDevicesHighWatermarkDate() function */
/**
* @description Get Date and Time at which the maximum number of associated devices ever associated with the access point concurrenlty since the last reset of the device or WiFi module (or in short when was X_COMCAST-COM_AssociatedDevicesHighWatermark updated). This dateTime value is in UTC.
* \n Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkDate
*
* @param apIndex - Access Point index
* @param output_in_seconds - Date and Time at which the maximum number of associated devices ever associated with the access point concurrenlty, to be returned 
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
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_AssociatedDevicesHighWatermarkDate		
//Date and Time at which the maximum number of associated devices ever associated with the access point concurrenlty since the last reset of the device or WiFi module (or in short when was X_COMCAST-COM_AssociatedDevicesHighWatermark updated). This dateTime value is in UTC.	
INT wifi_getApAssociatedDevicesHighWatermarkDate(INT apIndex, ULONG *output_in_seconds); //Tr181	//P3

					
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingServiceCapability	boolean	R	
//When true, indicates whether the access point supports interworking with external networks.	

//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingServiceEnable	boolean	W	
//Enables or disables capability of the access point to intework with external network. When enabled, the access point includes Interworking IE in the beacon frames.	

//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_PasspointCapability	boolean	R	
//Indicates whether this access point supports Passpoint (aka Hotspot 2.0). The Passpoint enabled AccessPoint must use WPA2-Enterprise security and WPS must not be enabled.	

//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_PasspointEnable	boolean	W	
//Whether Passpoint (aka Hotspot 2.0) support is currently enabled. When enabled, Passpoint specific information elemenets are indicated in beacon frames.	

//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_MAC_FilteringMode	string	R	
//"The current operational state of the MAC Filtering Mode, Enumeration of:    Allow-ALL, Allow, Deny			

//-----------------------------------------------------------------------------------------------				  
//Device.WiFi.AccessPoint.{i}.Security.	

/* wifi_getApSecurityModesSupported() function */
/**
* @description Indicates which security modes this AccessPoint instance is capable of supporting. Each list item is an enumeration of: None,WEP-64,WEP-128,WPA-Personal,WPA2-Personal,WPA-WPA2-Personal,WPA-Enterprise,WPA2-Enterprise,WPA-WPA2-Enterprise.
* \n Device.WiFi.AccessPoint.{i}.Security.ModesSupported
*
* @param apIndex - Access Point index
* @param output - Comma-separated list of security modes, to be returned 
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
//Device.WiFi.AccessPoint.{i}.Security.ModesSupported	
//Comma-separated list of strings. Indicates which security modes this AccessPoint instance is capable of supporting. Each list item is an enumeration of: None,WEP-64,WEP-128,WPA-Personal,WPA2-Personal,WPA-WPA2-Personal,WPA-Enterprise,WPA2-Enterprise,WPA-WPA2-Enterprise
INT wifi_getApSecurityModesSupported(INT apIndex, CHAR *output); 			
			
/* wifi_getApSecurityModeEnabled() function */
/**
* @description Get the Security modes supported. The value MUST be a member of the list reported by the ModesSupported parameter. Indicates which security mode is enabled.
* \n Device.WiFi.AccessPoint.{i}.Security.ModeEnabled	string	W
*
* @param apIndex - Access Point index
* @param output - Enabled security mode, to be returned 
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
//Device.WiFi.AccessPoint.{i}.Security.ModeEnabled	string	W	
//The value MUST be a member of the list reported by the ModesSupported parameter. Indicates which security mode is enabled.
INT wifi_getApSecurityModeEnabled(INT apIndex, CHAR *output);    

/* wifi_setApSecurityModeEnabled() function */
/**
* @description Enable supported security mode. The value MUST be a member of the list reported by the ModesSupported parameter. Indicates which security mode is enabled.
* \n Device.WiFi.AccessPoint.{i}.Security.ModeEnabled	string	W
*
* @param apIndex - Access Point index
* @param encMode - Supported security mode 
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
INT wifi_setApSecurityModeEnabled(INT apIndex, CHAR *encMode);        

//Device.WiFi.AccessPoint.{i}.Security.WEPKey	
//A WEP key expressed as a hexadecimal string.

/* wifi_getApSecurityPreSharedKey() function */
/**
* @description Get PreSharedKey associated with a AP. A literal PreSharedKey (PSK) expressed as a hexadecimal string.
* \n Device.WiFi.AccessPoint.{i}.Security.PreSharedKey
*
* @param apIndex - Access Point index
* @param output_string - PreSharedKey, to be returned 
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
//Device.WiFi.AccessPoint.{i}.Security.PreSharedKey		
//A literal PreSharedKey (PSK) expressed as a hexadecimal string.
INT wifi_getApSecurityPreSharedKey(INT apIndex, CHAR *output_string);         // output_string must be pre-allocated as 64 character string by caller

/* wifi_setApSecurityPreSharedKey() function */
/**
* @description Set PreSharedKey associated with a AP. A literal PreSharedKey (PSK) expressed as a hexadecimal string.
* \n Device.WiFi.AccessPoint.{i}.Security.PreSharedKey
*
* @param apIndex - Access Point index
* @param preSharedKey - PreSharedKey 
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
INT wifi_setApSecurityPreSharedKey(INT apIndex, CHAR *preSharedKey);          // sets an enviornment variable for the psk. Input string preSharedKey must be a maximum of 64 characters

/* wifi_getApSecurityKeyPassphrase() function */
/**
* @description Get a passphrase from which the PreSharedKey is to be generated, for WPA-Personal or WPA2-Personal or WPA-WPA2-Personal security modes.
* \n Device.WiFi.AccessPoint.{i}.Security.KeyPassphrase string-(63)	W
*
* @param apIndex - Access Point index
* @param output_string - Security key passphrase, to be returned 
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
//Device.WiFi.AccessPoint.{i}.Security.KeyPassphrase	string�(63)	W	
//A passphrase from which the PreSharedKey is to be generated, for WPA-Personal or WPA2-Personal or WPA-WPA2-Personal security modes.
INT wifi_getApSecurityKeyPassphrase(INT apIndex, CHAR *output_string);        // outputs the passphrase, maximum 63 characters

/* wifi_setApSecurityKeyPassphrase() function */
/**
* @description Set a passphrase from which the PreSharedKey is to be generated, for WPA-Personal or WPA2-Personal or WPA-WPA2-Personal security modes.
* \n Device.WiFi.AccessPoint.{i}.Security.KeyPassphrase string-(63)	W
*
* @param apIndex - Access Point index
* @param passPhrase - Security key passphrase
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
INT wifi_setApSecurityKeyPassphrase(INT apIndex, CHAR *passPhrase);           // sets the passphrase enviornment variable, max 63 characters

//Device.WiFi.AccessPoint.{i}.Security.RekeyingInterval	unsignedInt	W	
//The interval (expressed in seconds) in which the keys are re-generated.
//INT wifi_getApSecurityWpaRekeyInterval(INT apIndex, INT *output_int);         // outputs the rekey interval
//INT wifi_setApSecurityWpaRekeyInterval(INT apIndex, INT rekeyInterval);       // sets the internal variable for the rekey interval
INT wifi_setRouterEnable(INT wlanIndex, INT *RouterEnabled);

/* wifi_setApSecurityReset() function */
/**
* @description When set to true, this AccessPoint instance's WiFi security settings are reset to their factory default values. The affected settings include ModeEnabled, WEPKey, PreSharedKey and KeyPassphrase.
* \n Device.WiFi.AccessPoint.{i}.Security.Reset
*
* @param apIndex - Access Point index
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
//Device.WiFi.AccessPoint.{i}.Security.Reset	
//When set to true, this AccessPoint instance's WiFi security settings are reset to their factory default values. The affected settings include ModeEnabled, WEPKey, PreSharedKey and KeyPassphrase.
INT wifi_setApSecurityReset(INT apIndex);

//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_KeyPassphrase	string�(63)	RW	
//A passphrase from which the PreSharedKey is to be generated, for WPA-Personal or WPA2-Personal or WPA-WPA2-Personal security modes.	If KeyPassphrase is written, then PreSharedKey is immediately generated. The ACS SHOULD NOT set both the KeyPassphrase and the PreSharedKey directly (the result of doing this is undefined). The key is generated as specified by WPA, which uses PBKDF2 from PKCS #5: Password-based Cryptography Specification Version 2.0 ([RFC2898]).	This custom parameter is defined to enable reading the Passphrase via TR-069 /ACS. When read it should return the actual passphrase			
//INT wifi_getApKeyPassphrase(INT apIndex, CHAR *output); //Tr181	
//INT wifi_setApKeyPassphrase(INT apIndex, CHAR *passphase); //Tr181	

//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_WEPKey	string	RW	
//A WEP key expressed as a hexadecimal string.	WEPKey is used only if ModeEnabled is set to WEP-64 or WEP-128.	A 5 byte WEPKey corresponds to security mode WEP-64 and a 13 byte WEPKey corresponds to security mode WEP-128.	This custom parameter is defined to enable reading the WEPKey via TR-069/ACS. When read it should return the actual WEPKey.	If User enters 10 or 26 Hexadecimal characters, it should return keys as Hexadecimal characters.	If user enters 5 or 13 ASCII character key it should return key as ASCII characters.			

/* wifi_getApSecurityMFPConfig() function */
/**
* @brief To retrive the MFPConfig for each VAP
*
* The affected settings Device.WiFi.AccessPoint.{i}.Security.Reset
*
* @param[in] apIndex  Access Point index
* @param[out] output_string. Preallocated buffer for 64bytes. Allowed output string are "Disabled", "Optional", "Required"
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
INT wifi_getApSecurityMFPConfig(INT apIndex, CHAR *output_string);

/* wifi_setApSecurityMFPConfig() function */
/**
* @brief the hal is used to set the MFP config for each VAP.
*        1. mfpconfig need to be saved into wifi config in persistent way (so that it could be automatically applied after the wifi or vap restart)
*        2. mfpconfig need to be applied right away.
*
* The affected settings include Device.WiFi.AccessPoint.{i}.Security.Reset
*
* @param[in] apIndex  Access Point index
* @param[in] MfpConfig,  The allowed string for MFPConfig are "Disabled", "Optional", "Required"
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
INT wifi_setApSecurityMFPConfig(INT apIndex, CHAR *MfpConfig);


//-----------------------------------------------------------------------------------------------

/* wifi_getApSecurityRadiusServer() function */
/**
* @description Get the IP Address and port number of the RADIUS server, which 
are used for WLAN security. RadiusServerIPAddr is only applicable when ModeEnabled is an Enterprise type (i.e. WPA-Enterprise, WPA2-Enterprise or WPA-WPA2-Enterprise).  String is 64 bytes max.
* \n Device.WiFi.AccessPoint.{i}.Security.RadiusServerIPAddr
* \n Device.WiFi.AccessPoint.{i}.Security.RadiusServerPort
* \n Device.WiFi.AccessPoint.{i}.Security.RadiusSecret
*
* @param apIndex - Access Point index
* @param IP_output - IP Address, to be returned
* @param Port_output - Port output, to be returned
* @param RadiusSecret_output - Radius Secret output, to be returned
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
//Device.WiFi.AccessPoint.{i}.Security.RadiusServerIPAddr	
//Device.WiFi.AccessPoint.{i}.Security.RadiusServerPort	
//Device.WiFi.AccessPoint.{i}.Security.RadiusSecret
//The IP Address and port number of the RADIUS server used for WLAN security. RadiusServerIPAddr is only applicable when ModeEnabled is an Enterprise type (i.e. WPA-Enterprise, WPA2-Enterprise or WPA-WPA2-Enterprise).  String is 64 bytes max
INT wifi_getApSecurityRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusSecret_output); //Tr181	

/* wifi_setApSecurityRadiusServer() function */
/**
* @description Set the IP Address and port number of the RADIUS server, which 
are used for WLAN security. RadiusServerIPAddr is only applicable when ModeEnabled is an Enterprise type (i.e. WPA-Enterprise, WPA2-Enterprise or WPA-WPA2-Enterprise).  String is 64 bytes max.
* \n Device.WiFi.AccessPoint.{i}.Security.RadiusServerIPAddr
* \n Device.WiFi.AccessPoint.{i}.Security.RadiusServerPort
* \n Device.WiFi.AccessPoint.{i}.Security.RadiusSecret
*
* @param apIndex - Access Point index
* @param IPAddress - IP Address
* @param port - Port 
* @param RadiusSecret - Radius Secret
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
INT wifi_setApSecurityRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusSecret); //Tr181	

/* wifi_getApSecuritySecondaryRadiusServer() function */
/**
* @description Get secondary IP Address, port number and RADIUS server, which 
are used for WLAN security. RadiusServerIPAddr is only applicable when ModeEnabled is an Enterprise type (i.e. WPA-Enterprise, WPA2-Enterprise or WPA-WPA2-Enterprise).  String is 64 bytes max.
*
* @param apIndex - Access Point index
* @param IP_output - IP Address, to be returned
* @param Port_output - Port,to be returned
* @param RadiusSecret_output - Radius Secret, to be returned
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
INT wifi_getApSecuritySecondaryRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output, CHAR *RadiusSecret_output); //Tr181	

/* wifi_setApSecuritySecondaryRadiusServer() function */
/**
* @description Set secondary IP Address, port number and RADIUS server, which 
are used for WLAN security. RadiusServerIPAddr is only applicable when ModeEnabled is an Enterprise type (i.e. WPA-Enterprise, WPA2-Enterprise or WPA-WPA2-Enterprise).  String is 64 bytes max.
*
* @param apIndex - Access Point index
* @param IPAddress - IP Address
* @param port - Port
* @param RadiusSecret - Radius Secret
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
INT wifi_setApSecuritySecondaryRadiusServer(INT apIndex, CHAR *IPAddress, UINT port, CHAR *RadiusSecret); //Tr181	

/* wifi_getApSecurityRadiusSettings() function */
/**
* @description Get Access Point security radius settings. 
* \n Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.
*
* @param apIndex - Access Point index
* @param output - wifi_radius_setting_t info (*output), to be returned
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
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.		
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.RadiusServerRetries	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.RadiusServerRequestTimeout	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.PMKLifetime	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.PMKCaching	boolean	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.PMKCacheInterval	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.MaxAuthenticationAttempts	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.BlacklistTableTimeout	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.IdentityRequestRetryInterval	int	W	
//Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.QuietPeriodAfterFailedAuthentication	int	W		
INT wifi_getApSecurityRadiusSettings(INT apIndex, wifi_radius_setting_t *output); //Tr181	

/* wifi_setApSecurityRadiusSettings() function */
/**
* @description Set Access Point security radius settings. 
* \n Device.WiFi.AccessPoint.{i}.Security.X_COMCAST-COM_RadiusSettings.
*
* @param apIndex - Access Point index
* @param input - wifi_radius_setting_t info
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
INT wifi_setApSecurityRadiusSettings(INT apIndex, wifi_radius_setting_t *input); //Tr181	


//-----------------------------------------------------------------------------------------------

/* wifi_getApWpsEnable() function */
/**
* @description Outputs the WPS enable state of this ap in output_bool.
* \n Device.WiFi.AccessPoint.{i}.WPS.
* \n Device.WiFi.AccessPoint.{i}.WPS.Enable
*
* @param apIndex - Access Point index
* @param output_bool - WPS enable state, to be returned
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
//Device.WiFi.AccessPoint.{i}.WPS.
//Device.WiFi.AccessPoint.{i}.WPS.Enable	
//Enables or disables WPS functionality for this access point.
INT wifi_getApWpsEnable(INT apIndex, BOOL *output_bool);              // outputs the WPS enable state of this ap in output_bool 

/* wifi_setApWpsEnable() function */
/**
* @description Enables or disables WPS functionality for this access point.
* Sets the WPS enable enviornment variable for this ap to the value of enableValue, 1==enabled, 0==disabled.
* \n Device.WiFi.AccessPoint.{i}.WPS.
* \n Device.WiFi.AccessPoint.{i}.WPS.Enable
*
* @param apIndex - Access Point index
* @param enableValue - WPS enable state
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
INT wifi_setApWpsEnable(INT apIndex, BOOL enableValue);               // sets the WPS enable enviornment variable for this ap to the value of enableValue, 1==enabled, 0==disabled

/* wifi_getApWpsConfigMethodsSupported() function */
/**
* @description Indicates WPS configuration methods supported by the device. Each list item is an enumeration of: USBFlashDrive,Ethernet,ExternalNFCToken,IntegratedNFCToken,NFCInterface,PushButton,PIN.
* Sets the WPS enable enviornment variable for this ap to the value of enableValue, 1==enabled, 0==disabled.
* \n Device.WiFi.AccessPoint.{i}.WPS.ConfigMethodsSupported
*
* @param apIndex - Access Point index
* @param output - WPS configuration methods supported (Comma-separated list of strings), to be returned.
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
//Device.WiFi.AccessPoint.{i}.WPS.ConfigMethodsSupported	
//Comma-separated list of strings. Indicates WPS configuration methods supported by the device. Each list item is an enumeration of: USBFlashDrive,Ethernet,ExternalNFCToken,IntegratedNFCToken,NFCInterface,PushButton,PIN
INT wifi_getApWpsConfigMethodsSupported(INT apIndex, CHAR *output); //Tr181				

/* wifi_getApWpsConfigMethodsEnabled() function */
/**
* @description Indicates WPS configuration methods enabled on the device. Each list item MUST be a member of the list reported by the ConfigMethodsSupported parameter.
* \n Device.WiFi.AccessPoint.{i}.WPS.ConfigMethodsEnabled	string	W
*
* @param apIndex - Access Point index
* @param output_string - WPS configuration methods enabled, to be returned.
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
//Device.WiFi.AccessPoint.{i}.WPS.ConfigMethodsEnabled	string	W	
//Comma-separated list of strings. Each list item MUST be a member of the list reported by the ConfigMethodsSupported parameter. Indicates WPS configuration methods enabled on the device.
INT wifi_getApWpsConfigMethodsEnabled(INT apIndex, CHAR *output_string); // Outputs a common separated list of the enabled WPS config methods, 64 bytes max

/* wifi_setApWpsConfigMethodsEnabled() function */
/**
* @description Enable WPS configuration methods on the device. Each list item MUST be a member of the list reported by the ConfigMethodsSupported parameter.
* \n Device.WiFi.AccessPoint.{i}.WPS.ConfigMethodsEnabled	string	W
*
* @param apIndex - Access Point index
* @param methodString - WPS configuration methods enabled.
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
INT wifi_setApWpsConfigMethodsEnabled(INT apIndex, CHAR *methodString); // sets an enviornment variable that specifies the WPS configuration method(s).  methodString is a comma separated list of methods USBFlashDrive,Ethernet,ExternalNFCToken,IntegratedNFCToken,NFCInterface,PushButton,PIN

/* wifi_getApWpsDevicePIN() function */
/**
* @description Outputs the WPS device pin value, ulong_pin must be allocated by the caller.
*
* @param apIndex - Access Point index
* @param output_ulong - WPS Device PIN value, to be returned.
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
INT wifi_getApWpsDevicePIN(INT apIndex, ULONG *output_ulong);         // outputs the pin value, ulong_pin must be allocated by the caller

/* wifi_setApWpsDevicePIN() function */
/**
* @description Set an enviornment variable for the WPS pin for the selected AP.
*
* @param apIndex - Access Point index
* @param pin - WPS Device PIN value
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
INT wifi_setApWpsDevicePIN(INT apIndex, ULONG pin);                   // set an enviornment variable for the WPS pin for the selected AP

/* wifi_getApWpsConfigurationState() function */
/**
* @description Get WPS configuration state. Output string is either Not configured or Configured, max 32 
characters.
*
* @param apIndex - Access Point index
* @param output_string - WPS configuration state, to be returned
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
INT wifi_getApWpsConfigurationState(INT apIndex, CHAR *output_string); // Output string is either Not configured or Configured, max 32 characters

/* wifi_setApWpsEnrolleePin() function */
/**
* @description Sets the WPS pin for this AP.
*
* @param apIndex - Access Point index
* @param pin - WPS enroll Pin 
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
INT wifi_setApWpsEnrolleePin(INT apIndex, CHAR *pin);                 // sets the WPS pin for this AP

/* wifi_setApWpsButtonPush() function */
/**
* @description This function is called when the WPS push button has been pressed for this AP.
*
* @param apIndex - Access Point index
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
INT wifi_setApWpsButtonPush(INT apIndex);                             // This function is called when the WPS push button has been pressed for this AP

/* wifi_cancelApWPS() function */
/**
* @description Cancels WPS mode for this AP.
*
* @param apIndex - Access Point index
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
INT wifi_cancelApWPS(INT apIndex);                                    // cancels WPS mode for this AP

//-----------------------------------------------------------------------------------------------

/* wifi_getApAssociatedDeviceDiagnosticResult() function */
/**
* @description HAL funciton should allocate an data structure array, and return to caller with "associated_dev_array".
* \n Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.	
*
* @param apIndex - Access Point index
* @param associated_dev_array - Associated device array, to be returned
* @param output_array_size - Array size, to be returned
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
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.	  
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_OperatingStandard	
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_OperatingChannelBandwidth	
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_SNR
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_InterferenceSources	//P3
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_DataFramesSentAck		//P3
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_DataFramesSentNoAck	//P3
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_BytesSent
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_BytesReceived
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_RSSI
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_MinRSSI				//P3
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_MaxRSSI				//P3
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_Disassociations		//P3
//Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.X_COMCAST-COM_AuthenticationFailures	//P3
//HAL funciton should allocate an data structure array, and return to caller with "associated_dev_array"
INT wifi_getApAssociatedDeviceDiagnosticResult(INT apIndex, wifi_associated_dev_t **associated_dev_array, UINT *output_array_size); //Tr181	

//------------------------------------------------------------------------------------------------------
////SSID stearing APIs using blacklisting
//INT wifi_setSsidSteeringPreferredList(INT radioIndex,INT apIndex, INT *preferredAPs[32]);  // prevent any client device from assocating with this ipIndex that has previously had a valid assocation on any of the listed "preferred" SSIDs unless SsidSteeringTimeout has expired for this device. The array lists all APs that are preferred over this AP.  Valid AP values are 1 to 32. Unused positions in this array must be set to 0. This setting becomes active when committed.  The wifi subsystem must default to no preferred SSID when initalized.  
////Using the concept of an �preferred list� provides a solution to most use cases that requrie SSID Steering.  To implement this approach, the AP places the STA into the Access Control DENY list for a given SSID only if the STA has previously associated to one of the SSIDs in the �preferred list� that for SSID.
//INT wifi_setSsidSteeringTimout(INT radioIndex,INT apIndex, ULONG SsidSteeringTimout);  // only prevent the client device from assocatign with this apIndex if the device has connected to a preferred SSID within this timeout period - in units of hours.  This setting becomes active when committed.  


/* wifi_newApAssociatedDevice_callback() function */
/**
* @description This call back will be invoked when new wifi client come to associate to AP.	
*
* @param apIndex - Access Point Index
* @param associated_dev - wifi_associated_dev_t *associated_dev, associated 
device info
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
//This call back will be invoked when new wifi client come to associate to AP. 
typedef INT ( * wifi_newApAssociatedDevice_callback)(INT apIndex, wifi_associated_dev_t *associated_dev);

/* wifi_newApAssociatedDevice_callback_register() function */
/**
* @description Callback registration function.	
*
* @param callback_proc - wifi_newApAssociatedDevice_callback callback function
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
//Callback registration function.
void wifi_newApAssociatedDevice_callback_register(wifi_newApAssociatedDevice_callback callback_proc);

INT wifi_setApBeaconRate(INT radioIndex,CHAR *beaconRate);
INT wifi_getApBeaconRate(INT apIndex, CHAR *beaconRate);

/* Enum to define WiFi Bands */
typedef enum
{
    band_invalid = -1,
    band_2_4 = 0,
    band_5 = 1,
} wifi_band;

/* INT wifi_getApIndexForWiFiBand(wifi_band band)  */
/**
* @description Get the AP index for requested WiFi Band.
*
* @param wifi_band - WiFi band for which AP Index is required
*
* @return AP Index for requested WiFi Band
*
*/
INT wifi_getApIndexForWiFiBand(wifi_band band);

/*hostapd will read file from nvram /etc/usr/ccsp/wifi/ will contains default
configuration required for Factory Reset*/
#define HOSTAPD_FNAME "/nvram/hostapd"
#define SEC_FNAME "/etc/sec_file.txt"
#define BW_FNAME "/etc/bw_file.txt"
enum hostap_names
{
    ssid=0,
    passphrase=1,
};
struct params
{
     char name[64];
     char value[64];
};
typedef struct __param_list {
	unsigned int count;
	struct params *parameter_list;
}param_list_t;
struct hostap_conf
{
    char ssid[32];
    char *passphrase;
    char *wpa_pairwise;
    char *wpa;
    char *wpa_keymgmt;
};
/**
 * @brief VAP Telemetry information.
 *
 * Structure which holds the VAP Telemetry information.
 */
typedef struct {
    UINT                txOverflow;            // WiFi TX overflow counter.
} wifi_VAPTelemetry_t;


//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService. 
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.AccessNetworkType	
//Access Network Type value to be included in the Interworking IE in the beaconds. (refer 8.4.2.94 of IEEE Std 802.11-2012). Possible values are: 0 - Private network;1 - Private network with guest access;2 - Chargeable public network;3 - Free public network;4 - Personal device network;5 - Emergency services only network;6-13 - Reserved;14 - Test or experimental;15 - Wildcard
//INT wifi_setAccessNetworkType(INT apIndex, INT accessNetworkType);   // P3

//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.Internet	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.VenueGroupCode	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.VenueTypeCode	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.HESSID	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.DGAFEnable	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.ANQPDomainID
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.VenueNamesNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.OperatorNamesNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.ConsortiumOIsNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.DomainNamesNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.3GPPNetworksNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_InterworkingService.NAIRealmsNumberOfEntries	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.VenueNames.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.VenueNames.{i}.LanguageCode
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.VenueNames.{i}.VanueName

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OperatorNames.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OperatorNames.{i}.LanguageCode
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OperatorNames.{i}.OperatorName

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.ConsortiumOIs.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.ConsortiumOIs.{i}.OI	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.DomainNames.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.DomainNames.{i}.DomainName	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.3GPPNetworks.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.3GPPNetworks.{i}.MCC	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.3GPPNetworks.{i}.MNC	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.NAIRealmEncodingType	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.NAIRealm	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethodsNumberOfEntries	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethods.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethods.{i}.EAPMethod	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethods.{i}.AuthenticationParametersNumberOfEntries	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethods.{i}.AuthenticationParameters.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethods.{i}.AuthenticationParameters.{i}.ID	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.NAIRealms.{i}.EAPMethods.{i}.AuthenticationParameters.{i}.Value	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.LinkStatus	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.AtCapacity	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.DownlinkSpeed	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.UplinkSpeed	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.DownlinkLoad	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.WANMetrics.UplinkLoad	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProvidersNumberOfEntries	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.OSUServerURI	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.OSUMethodsList	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.OSUNAI	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.NamesNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.IconsNumberOfEntries	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}ServiceDescriptionsNumberOfEntries	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Names.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Names.{i}.LanguageCode	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Names.{i}.OSUProviderFriendlyName	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Icons.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Icons.{i}.IconWidth	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Icons.{i}.IconHeight	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.Icons.{i}.LanguageCode	

//-----------------------------------------------------------------------------------------------
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.ServiceDescriptions.{i}.	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.ServiceDescriptions.{i}.LanguageCode	
//Device.WiFi.AccessPoint.{i}.X_COMCAST-COM_Passpoint.OSU.OSUProviders.{i}.ServiceDescriptions.{i}.ServiceDescription	

//-----------------------------------------------------------------------------------------------
//Device.IP.Diagnostics.	
//Device.IP.Diagnostics.IPPing.	
//Device.IP.Diagnostics.IPPing.DiagnosticsState	
//Device.IP.Diagnostics.IPPing.Interface
//Device.IP.Diagnostics.IPPing.Host	
//Device.IP.Diagnostics.IPPing.NumberOfRepetitions		
//Device.IP.Diagnostics.IPPing.Timeout	
//Device.IP.Diagnostics.IPPing.DataBlockSize	
//Device.IP.Diagnostics.IPPing.DSCP			

//Device.IP.Diagnostics.IPPing.SuccessCount	
//Device.IP.Diagnostics.IPPing.FailureCount		
//Device.IP.Diagnostics.IPPing.AverageResponseTime		
//Device.IP.Diagnostics.IPPing.MinimumResponseTime		
//Device.IP.Diagnostics.IPPing.MaximumResponseTime			

//Start the ping test and get the result
//INT wifi_getIPDiagnosticsIPPingResult(wifi_diag_ipping_setting_t *input, wifi_diag_ipping_result_t *result); //Tr181		
//--------------------------------------------------------------------------------------------------
// Wifi Airtime Management and QOS APIs to control contention based access to airtime
//INT wifi_clearDownLinkQos(INT apIndex);                             // clears the QOS parameters to the WMM default values for the downlink direction (from the access point to the stations.  This set must take affect when the api wifi_applySSIDSettings() is called.
//INT wifi_setDownLinkQos(INT apIndex, wifi_qos_t qosStruct);        // sets the QOS variables used in the downlink direction (from the access point to the stations).  Values must be allowable values per IEEE 802.11-2012 section 8.4.2.31.  Note:  Some implementations may requrie that all downlink APs on the same radio are set to the same QOS values. Default values are per the WMM spec.  This set must take affect when the api wifi_applySSIDSettings() is called.
//INT wifi_clearUpLinkQos(INT apIndex);                               // clears the QOS parameters to the WMM default values for the uplink direction (from the Wifi stations to the ap.  This must take affect when the api wifi_applySSIDSettings() is called.
//INT wifi_setUpLinkQos (INT apIndex, wifi_qos_t qosStruct);         // sets the QOS variables used in the uplink direction (from the Wifi stations to the AP). Values must be allowable values per IEEE 802.11-2012 section 8.4.2.31. The default values must be per the WMM spec.  This set must take affect when the api wifi_applySSIDSettings() is called.

//--------------------------------------------------------------------------------------------------
// Wifi Airtime Management and QOS APIs to control downlink queue prioritization
//INT wifi_getDownLinkQueuePrioritySupport (INT apIndex, INT *supportedPriorityLevels);  //This api is used to get the the number of supported downlink queuing priority levels for each AP/SSID.  If priority queuing levels for AP/SSIDs are not supported, the output should be set to 1. A value of 1 indicates that only the same priority level is supported for all AP/SSIDs.
//INT wifi_setDownLinkQueuePriority(INT apIndex, INT priorityLevel); // this sets the queue priority level for each AP/SSID in the downlink direction.  It is used with the downlink QOS api to manage priority access to airtime in the downlink direction.  This set must take affect when the api wifi_applySSIDSettings() is called.

int wifi_hostapdWrite(int ap,param_list_t *params);
int wifi_hostapdRead(int ap,struct params *params,char *output);

/* wifi_setRadioDcsScanning() function */
/**
* @description Enable/Disable selected wifi radio channel's DCS.
* \n Device.WiFi.Radio.{i}.X_RDKCENTRAL_COM_DCSEnable
*
* @param radioIndex - Index of Wi-Fi radio channel
* @param enable - Set the value of DCS Enable flag for the selected radio index
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
INT wifi_setRadioDcsScanning(INT radioIndex, BOOL enable);                        //RDKB

//This call back will be invoked when driver detect the client authentication fail.
//event_type: 0=unknow reason; 1=wrong password; 2=timeout;
typedef INT ( * wifi_apAuthEvent_callback)(INT apIndex, char *MAC, INT event_type);
//Callback registration function.
void wifi_apAuthEvent_callback_register(wifi_apAuthEvent_callback callback_proc);

typedef enum {
        WIFI_EVENT_CHANNELS_CHANGED,
        WIFI_EVENT_DFS_RADAR_DETECTED
} wifi_chan_eventType_t;

typedef void (*wifi_chan_eventCB_t)(UINT radioIndex, wifi_chan_eventType_t event, UCHAR channel);

typedef enum {
        CHAN_STATE_AVAILABLE = 1,
        CHAN_STATE_DFS_NOP_FINISHED,
        CHAN_STATE_DFS_NOP_START,
        CHAN_STATE_DFS_CAC_START,
        CHAN_STATE_DFS_CAC_COMPLETED
} wifi_channelState_t;

typedef struct _wifi_channelMap_t {
        INT ch_number;
        wifi_channelState_t ch_state;
} wifi_channelMap_t;

/* wifi_getRadioChannels() function */
/*
 * Description: This function returns a map of current states of each possible channel for given radio.
 * The implementation must fill the map for each channel reported by wifi_getRadioPossibleChannels().
 *
 * Parameters :
 * radioIndex - The index of the radio. First radio is index 0. 2nd radio is index 1 - type INT
 * output_map - a pointer to an array of wifi_channelMap_t structures, preallocated by the caller.
 *              This is where the output is written. If the item in the array is not used,
 *              the "ch_number" should be set to 0.
 * output_map_size - The size of the output_map array in array items.
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
 */
INT wifi_getRadioChannels(INT radioIndex, wifi_channelMap_t *output_map, INT output_map_size);

INT wifi_chan_eventRegister(wifi_chan_eventCB_t event_cb);

/**
* @brief This function is to read the ACL mode.
*
* @param[in]  apIndex             Access Point index
* @param[out] output_filterMode   Mac Address control mode
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
INT wifi_getApMacAddressControlMode(INT apIndex, INT *output_filterMode);

/**
* @brief Deletes all Device MAC address from the Access control filter list.
*
* @param[in]  apIndex           Access Point index
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
* @execution Synchronous
* @sideeffect None
*
*/
INT wifi_delApAclDevices(INT apINdex);

/**
* @brief To read the run time ssid name.
*
* @param[in]  apIndex         Access Point index
* @param[out] output_string   SSID name, to be returned
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
INT wifi_getSSIDNameStatus(INT apIndex, CHAR *output_string);

INT wifi_getApChannel(INT radioIndex,ULONG *output_ulong);

/* @description Set the neighbor report capability to activated or deactivated,
 * same as enabled or disabled.  The word "activated" is used here because
 * that's what's used in the 802.11 specification.  When deactivate the
 * gateway ignores a neighbor report request as defined in 802.11-2016 section
 * 11.11.10.3.
 *
 * Reciept of the TR-181 Object
 * Device.WiFi.AccessPoint.{i}.X_RDKCENTRAL-COM_NeighborReportActivated via
 * TR-069 or WebPA causes this function to be called with the value of the
 * Object.
 *
 * @param apIndex - AP Index the setting applies to.
 * @param activate - True for activate false for deactivate.
 * @return The status of the operation.
 * @retval RETURN_OK if successful.
 * @retval RETURN_ERR if any error is detected.
 */
INT wifi_setNeighborReportActivation(UINT apIndex, BOOL activate);

/* @description Get the neighbor report capability of activated or deactivated,
 * same as enabled or disabled.
 *
 * Reciept of the TR-181 Object
 * Device.WiFi.AccessPoint.{i}.X_RDKCENTRAL-COM_NeighborReportActivated via
 * TR-069 or WebPA causes this function to be called with the value of the
 * Object.
 *
 * @param apIndex - AP Index the setting applies to.
 * @param activate - True for activate false for deactivate.
 * @return The status of the operation.
 * @retval RETURN_OK if successful.
 * @retval RETURN_ERR if any error is detected.
 */
INT wifi_getNeighborReportActivation(UINT apIndex, BOOL *activate);

typedef struct _wifi_associated_dev_rate_info_rx_stats {
        // rate table index see table above
        UCHAR nss;                                      /**< 0 equals legacy protocolss (OFDM, CCK) 1 - n spatial stream (HT, VHT) */
        UCHAR mcs;                                              /**< 0 - 7 (HT) - 9 (VHT) */
        USHORT bw;                                      /**< 20, 40, 80, 160 ... (to be considered 5 , 10, 80+80) ... */
        ULLONG flags;                           /**< Flag indicating data validation that HAS_BYTES, HAS_MSDUS, HAS_MPDUS, HAS_PPDUS, HAS_BW_80P80, HAS_RSSI_COMB, HAS_RSSI_ARRAY */
        ULLONG bytes;                                   /**< number of bytes received for given rate */
        ULLONG msdus;                                   /**< number of MSDUs received for given rate */
        ULLONG mpdus;                                   /**< number of MPDUs received for given rate */
        ULLONG ppdus;                                   /**< number of PPDUs received for given rate */
        ULLONG retries;                         /**< number of retries received for given rate */
        UCHAR rssi_combined;                    /**< Last RSSI received on give rate */
        /* Per antenna RSSI (above noise floor) for all widths (primary,secondary)
                -----------------------------------------------
                | chain_num |  20MHz [pri20                   ]
                |           |  40MHZ [pri20,sec20             ]
                |           |  80MHz [pri20,sec20,sec40,      ]
                |           | 160MHz [pri20,sec20,sec40,sec80 ]
                -----------------------------------------------
                |  1        |  rssi  [pri20,sec20,sec40,sec80 ]
                |  ...      |  ...
                |  8        |  rssi  [pri20,sec20,sec40,sec80 ]
                ----------------------------------------------- */
        UCHAR rssi_array[8][4];                 //<! 8=antennas, 4=20+20+40+80 extension rssi
} wifi_associated_dev_rate_info_rx_stats_t;

/* wifi_getApAssociatedDeviceRxStatsResult() function */
/**
* @brief Get the associated client per rate receive status.
*
* @param [in]  radioIndex          The index of radio array.
* @param [in]  clientMacAddress    Client mac address UCHAR[6]
* @param [out] stats_array         Client receive status
* @param [out] output_array_size   The length of output array
* @param [out] handle              Status validation handle used to determine reconnects;
*                                  increases for every association.
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
INT wifi_getApAssociatedDeviceRxStatsResult(INT radioIndex, mac_address_t *clientMacAddress, wifi_associated_dev_rate_info_rx_stats_t **stats_array, UINT *output_array_size, ULLONG *handle);

typedef struct _wifi_associated_dev_rate_info_tx_stats {
        // rate table index see table above
        UCHAR nss;                                      /**< 0 equals legacy protocolss (OFDM, CCK) 1 - n spatial stream (HT, VHT) */
        UCHAR mcs;                                              /**< 0 - 7 (HT) - 9 (VHT) */
        USHORT bw;                                      /**< 20, 40, 80, 160 ... (to be considered 5 , 10, 80+80) ... */
        ULLONG flags;                           /**< Flag indicating data validation that HAS_BYTES, HAS_MSDUS, HAS_MPDUS, HAS_PPDUS, HAS_BW_80P80, HAS_RSSI_COMB, HAS_RSSI_ARRAY */
        ULLONG bytes;                                   /**< number of bytes transmitted for given rate */
        ULLONG msdus;                                   /**< number of MSDUs transmitted for given rate */
        ULLONG mpdus;                                   /**< number of MPDUs transmitted for given rate */
        ULLONG ppdus;                                   /**< number of PPDUs transmitted for given rate */
        ULLONG retries;                         /**< number of transmittion retries for given rate */
        ULLONG attempts;                                /**< number of attempts trying transmitt on given rate */
} wifi_associated_dev_rate_info_tx_stats_t;

/**
* @brief Get the associated client per rate transmission status.
*
* @param [in]  radioIndex          The index of radio array.
* @param [in]  clientMacAddress    Client mac address UCHAR[6]
* @param [out] stats_array         Client transmission status
* @param [out] output_array_size   The length of output array
* @param [out] handle              Status validation handle used to determine reconnects;
*                                  increases for every association.
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

INT wifi_getApAssociatedDeviceTxStatsResult(INT radioIndex, mac_address_t *clientMacAddress, wifi_associated_dev_rate_info_tx_stats_t **stats_array, UINT *output_array_size, ULLONG *handle);

typedef struct _wifi_rssi_snapshot {
        UCHAR  rssi[4];                                 /**< Last 4 RSSI frames received */
        UCHAR  time_s[4];                                  /**< Time of when last 4 RSSI were received */
        USHORT count;                                      /**< Sequence numer of received managemant (bcn, ack) frames  */
} wifi_rssi_snapshot_t;

typedef struct _wifi_associated_dev_stats {
        ULLONG  cli_rx_bytes;                           /**< The total number of bytes received from the client device, including framing characters. */
        ULLONG  cli_tx_bytes;                           /**< The total number of bytes transmitted to the client device, including framing characters. */
        ULLONG  cli_rx_frames;                          /**< The total number of frames received from the client */
        ULLONG  cli_tx_frames;                          /**< The total number of frames transmitted to the client */
        ULLONG  cli_rx_retries;                         /**< Number of rx retries */
        ULLONG  cli_tx_retries;                         /**< Number of tx retries. cli_Retransmissions */
        ULLONG  cli_rx_errors;                          /**< Number of numer of rx error */
        ULLONG  cli_tx_errors;                          /**< Number of tx errors */
        double  cli_rx_rate;                                    /**< average rx data rate used */
        double  cli_tx_rate;                                    /**< average tx data rate used} wifi_associated_dev_t; */
        wifi_rssi_snapshot_t cli_rssi_bcn;      /**< RSSI from last 4 beacons received (STA) */
        wifi_rssi_snapshot_t cli_rssi_ack;      /**< RSSI from last 4 ack received     (AP) */
} wifi_associated_dev_stats_t;

/**
* @brief Get the associated device  status.
*
* @param [in]  apIndex               The index of access point array
* @param [in]  clientMacAddress      client mac address UCHAR[6]
* @param [out] associated_dev_stats  Associated device status
* @param [in]  handle                Status validation handle used to determine reconnects;
*                                    increases for every association
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
INT wifi_getApAssociatedDeviceStats(INT apIndex, mac_address_t *clientMacAddress, wifi_associated_dev_stats_t *associated_dev_stats, ULLONG *handle);

typedef struct _wifi_associated_dev2
{
        mac_address_t cli_MACAddress;           /**< The MAC address of an associated device. */
        CHAR  cli_IPAddress[64];                /**< IP of the associated device */
        BOOL  cli_AuthenticationState; /**< Whether an associated device has authenticated (true) or not (false). */
        UINT  cli_LastDataDownlinkRate; /**< The data transmit rate in kbps that was most recently used for transmission from the access point to the associated device. */
        UINT  cli_LastDataUplinkRate;   /**< The data transmit rate in kbps that was most recently used for transmission from the associated device to the access point. */
        INT   cli_SignalStrength;               /**< An indicator of radio signal strength of the uplink from the associated device to the access point, measured in dBm, as an average of the last 100 packets received from the device. */
        UINT  cli_Retransmissions;      /**< The number of packets that had to be re-transmitted, from the last 100 packets sent to the associated device. Multiple re-transmissions of the same packet count as one. */
        BOOL  cli_Active;                               /**<    boolean -       Whether or not this node is currently present in the WiFi AccessPoint network. */

        CHAR  cli_OperatingStandard[64];        /**< Radio standard the associated Wi-Fi client device is operating under. Enumeration of: */
        CHAR  cli_OperatingChannelBandwidth[64];        /**< The operating channel bandwidth of the associated device. The channel bandwidth (applicable to 802.11n and 802.11ac specifications only). Enumeration of: */
        INT   cli_SNR;          /**< A signal-to-noise ratio (SNR) compares the level of the Wi-Fi signal to the level of background noise. Sources of noise can include microwave ovens, cordless phone, bluetooth devices, wireless video cameras, wireless game controllers, fluorescent lights and more. It is measured in decibels (dB). */
        CHAR  cli_InterferenceSources[64]; /**< Wi-Fi operates in two frequency ranges (2.4 Ghz and 5 Ghz) which may become crowded other radio products which operate in the same ranges. This parameter reports the probable interference sources that this Wi-Fi access point may be observing. The value of this parameter is a comma seperated list of the following possible sources: eg: MicrowaveOven,CordlessPhone,BluetoothDevices,FluorescentLights,ContinuousWaves,Others */
        ULONG cli_DataFramesSentAck;    /**< The DataFramesSentAck parameter indicates the total number of MSDU frames marked as duplicates and non duplicates acknowledged. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification. */
        ULONG cli_DataFramesSentNoAck;  /**< The DataFramesSentNoAck parameter indicates the total number of MSDU frames retransmitted out of the interface (i.e., marked as duplicate and non-duplicate) and not acknowledged, but does not exclude those defined in the DataFramesLost parameter. The value of this counter may be reset to zero when the CPE is rebooted. Refer section A.2.3.14 of CableLabs Wi-Fi MGMT Specification. */
        ULONG cli_BytesSent;    /**< The total number of bytes transmitted to the client device, including framing characters. */
        ULONG cli_BytesReceived;        /**< The total number of bytes received from the client device, including framing characters. */
        INT   cli_RSSI; /**< The Received Signal Strength Indicator, RSSI, parameter is the energy observed at the antenna receiver for transmissions from the device averaged over past 100 packets recevied from the device. */
        INT   cli_MinRSSI;      /**< The Minimum Received Signal Strength Indicator, RSSI, parameter is the minimum energy observed at the antenna receiver for past transmissions (100 packets). */
        INT   cli_MaxRSSI;      /**< The Maximum Received Signal Strength Indicator, RSSI, parameter is the energy observed at the antenna receiver for past transmissions (100 packets). */
        UINT  cli_Disassociations;      /**< This parameter  represents the total number of client disassociations. Reset the parameter evey 24hrs or reboot */
        UINT  cli_AuthenticationFailures;       /**< This parameter indicates the total number of authentication failures.  Reset the parameter evey 24hrs or reboot */

        ULLONG   cli_Associations;      /**<  Stats handle used to determine reconnects; increases for every association (stat delta calcualtion) */
} wifi_associated_dev2_t;

/**
* @brief The function  provides a list of the devices currently associated with the access point.
*
* HAL funciton should allocate an data structure array, and return to caller with "associated_dev_array".
* Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.
*
* @param[in] apIndex                Access Point index
* @param[in] associated_dev_array   Associated device array, to be returned
* @param[in] output_array_size      Array size, to be returned
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

INT wifi_getApAssociatedDeviceDiagnosticResult2(INT apIndex, wifi_associated_dev2_t **associated_dev_array, UINT *output_array_size); //Tr181

typedef struct _wifi_channelStats {
        INT  ch_number;                                         /**< each channel is only 20MHz bandwidth */
        BOOL ch_in_pool;                                /**< If ch_in_pool is false, driver do not need to scan this channel */
        INT  ch_noise;                                  /**< this is used to return the average noise floor in dbm */
        BOOL ch_radar_noise;                            /**< if ch_number is in DFS channel, this is used to return if radar signal is present on DFS channel (5G only) */
        INT  ch_max_80211_rssi;                         /**< max RSSI from the neighbor AP in dbm on this channel. */
        INT  ch_non_80211_noise;                        /**< average non 802.11 noise */
        INT  ch_utilization;                            /**< this is used to return the 802.11 utilization in percent */
        ULLONG ch_utilization_total; /**<  Total time radio spent receiveing or transmitting on that channel (ch_utilization_active) */
        ULLONG ch_utilization_busy; /**<  Time radio detected that channel was busy (Busy = Rx + Tx + Interference) */
        ULLONG ch_utilization_busy_tx; /**<  Time time radio spent transmitting on channel */
        ULLONG ch_utilization_busy_rx; /**<  Time radio spent receiving on channel (Rx = Rx_obss + Rx_self + Rx_errr (self and obss errors) */
        ULLONG ch_utilization_busy_self; /**< Time radio spend receiving on channel from its own connected clients */
        ULLONG ch_utilization_busy_ext; /**< Time radio detected that extended channel was busy (40MHz extention channel busy */
} wifi_channelStats_t;                                  //<!This data structure is for each channel

//SURVEY CHANNEL
/* wifi_getWifiChannelStats() function */
/**
* @brief Get the channels utilization status.
*
* @param[in]      radioIndex                      The index of the radio
* @param[in, out] input_output_channelStats_array The array initially filled with requested channel numbers.
*                                                 The same array is used as an output with channel statistics
*                                                 details. Data for each channel must be written to the corresponding
*                                                 element of the array. When array_size = 0, the API returns ONCHAN
*                                                 stats in a single wifi_channelStats_t element.
* @param[out]     array_size                      The length of the output array
*
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
//Get the basic Radio channel traffic static info
INT wifi_getRadioChannelStats(INT radioIndex, wifi_channelStats_t *input_output_channelStats_array, INT array_size);

typedef enum
{
    WIFI_RADIO_SCAN_MODE_NONE = 0,
    WIFI_RADIO_SCAN_MODE_FULL,
    WIFI_RADIO_SCAN_MODE_ONCHAN,
    WIFI_RADIO_SCAN_MODE_OFFCHAN,
    WIFI_RADIO_SCAN_MODE_SURVEY
} wifi_neighborScanMode_t;

/**
* @brief This API initates the scanning.
*
* @param[in]  apIndex       The index of access point array.
* @param[out] scan_mode     Scan modes.
* @param[out] dwell_time    Amount of time spent on each channel in the hopping sequence.
* @param[out] chan_num      The channel number.
* @param[out] chan_list     List of channels.
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
INT wifi_startNeighborScan(INT apIndex, wifi_neighborScanMode_t scan_mode, INT dwell_time, UINT chan_num, UINT *chan_list);

/**
* @brief Returns the Wifi scan status.
*
* @param[in]  radioIndex           Radio index
* @param[out] neighbor_ap_array    Neighbour access point info to be returned
* @param[out] output_array_size    Length of the output array.
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
INT wifi_getNeighboringWiFiStatus(INT radioIndex, wifi_neighbor_ap2_t **neighbor_ap_array, UINT *output_array_size); //Mesh

/**
 * @brief Wifi Steering Event Types
 * These are the different steering event types that are sent by the wifi_hal
 * steering library.
 */
typedef enum {
    WIFI_STEERING_EVENT_PROBE_REQ           = 1,    /**< Probe Request Event                             */
    WIFI_STEERING_EVENT_CLIENT_CONNECT,             /**< Client association completed successfully Event */
    WIFI_STEERING_EVENT_CLIENT_DISCONNECT,          /**< Client Disconnect Event                         */
    WIFI_STEERING_EVENT_CLIENT_ACTIVITY,            /**< Client Active Change Event                      */
    WIFI_STEERING_EVENT_CHAN_UTILIZATION,           /**< Channel Utilization Event                       */
    WIFI_STEERING_EVENT_RSSI_XING,                  /**< Client RSSI Crossing Event                      */
    WIFI_STEERING_EVENT_RSSI,                       /**< Instant Measurement Event                       */
    WIFI_STEERING_EVENT_AUTH_FAIL                   /**< Client Auth Failure Event                       */
} wifi_steering_eventType_t;

/**
 * @brief Probe Request Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_PROBE_REQ
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address         */
    UINT                            rssi;           /**< RSSI of probe frame        */
    BOOL                            broadcast;      /**< True if broadcast probe    */
    BOOL                            blocked;        /**< True if response blocked   */
} wifi_steering_evProbeReq_t;

/**
 * @brief STA datarate information
 * These are STA capabilities values
 */
typedef struct {
    UINT                            maxChwidth;         /**< Max bandwidth supported                */
    UINT                            maxStreams;         /**< Max spatial streams supported          */
    UINT                            phyMode;            /**< PHY Mode supported                     */
    UINT                            maxMCS;             /**< Max MCS  supported                     */
    UINT                            maxTxpower;         /**< Max TX power supported                 */
    UINT                            isStaticSmps;       /**< Operating in Static SM Power Save Mode */
    UINT                            isMUMimoSupported;  /**< Supports MU-MIMO                       */
} wifi_steering_datarateInfo_t;

typedef struct {
    BOOL                            linkMeas;           /**< Supports link measurement      */
    BOOL                            neighRpt;           /**< Supports neighbor reports      */
    BOOL                            bcnRptPassive;      /**< Supports Passive 11k scans     */
    BOOL                            bcnRptActive;       /**< Supports Active 11k scans      */
    BOOL                            bcnRptTable;        /**< Supports beacon report table   */
    BOOL                            lciMeas;            /**< Supports LCI measurement       */
    BOOL                            ftmRangeRpt;        /**< Supports FTM Range report      */
} wifi_steering_rrmCaps_t;

/**
 * @brief Client Connect Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_CLIENT_CONNECT
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address                     */
    UINT                            isBTMSupported; /**< Client supports BSS TM                 */
    UINT                            isRRMSupported; /**< Client supports RRM                    */
    BOOL                            bandCap2G;      /**< Client is 2.4GHz capable               */
    BOOL                            bandCap5G;      /**< Client is 5GHz capable                 */
    wifi_steering_datarateInfo_t    datarateInfo;   /**< Client supported datarate information  */
    wifi_steering_rrmCaps_t         rrmCaps;        /**< Client supported RRM capabilites       */
} wifi_steering_evConnect_t;

/**
 * @brief Wifi Disconnect Sources
 *
 * These are the possible sources of a wifi disconnect.
 * If the disconnect was initiated by the client, then @b DISCONNECT_SOURCE_REMOTE
 * should be used.
 * If initiated by the local AP, then @b DISCONNECT_SOURCE_LOCAL should be used.
 * If this information is not available, then @b DISCONNECT_SOURCE_UNKNOWN should be used.
 */
typedef enum {
    DISCONNECT_SOURCE_UNKNOWN               = 0,    /**< Unknown source             */
    DISCONNECT_SOURCE_LOCAL,                        /**< Initiated locally          */
    DISCONNECT_SOURCE_REMOTE                        /**< Initiated remotely         */
} wifi_disconnectSource_t;

/**
 * @brief Wifi Disconnect Types
 * These are the types of wifi disconnects.
 */
typedef enum {
    DISCONNECT_TYPE_UNKNOWN                 = 0,    /**< Unknown type               */
    DISCONNECT_TYPE_DISASSOC,                       /**< Disassociation             */
    DISCONNECT_TYPE_DEAUTH                          /**< Deauthentication           */
} wifi_disconnectType_t;

/**
 * @brief Client Disconnect Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_CLIENT_DISCONNECT
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address         */
    UINT                            reason;         /**< Reason code of disconnect  */
    wifi_disconnectSource_t         source;         /**< Source of disconnect       */
    wifi_disconnectType_t           type;           /**< Disconnect Type            */
} wifi_steering_evDisconnect_t;

/**
 * @brief Client Activity Change Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_CLIENT_ACTIVITY
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address         */
    BOOL                            active;         /**< True if client is active   */
} wifi_steering_evActivity_t;

/**
 * @brief Channel Utilization Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_CHAN_UTILIZATION
 */
typedef struct {
    UINT                            utilization;    /**< Channel Utilization 0-100  */
} wifi_steering_evChanUtil_t;

/**
 * @brief RSSI Crossing Values
 * These are the RSSI crossing values provided in RSSI crossing events
 */
typedef enum {
    WIFI_STEERING_RSSI_UNCHANGED            = 0,    /**< RSSI hasn't crossed        */
    WIFI_STEERING_RSSI_HIGHER,                      /**< RSSI went higher           */
    WIFI_STEERING_RSSI_LOWER                        /**< RSSI went lower            */
} wifi_steering_rssiChange_t;

/**
 * @brief Client RSSI Crossing Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_RSSI_XING
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address         */
    UINT                            rssi;           /**< Clients current RSSI       */
    wifi_steering_rssiChange_t      inactveXing;    /**< Inactive threshold Value   */
    wifi_steering_rssiChange_t      highXing;       /**< High threshold Value       */
    wifi_steering_rssiChange_t      lowXing;        /**< Low threshold value        */
} wifi_steering_evRssiXing_t;

/**
 * @brief Client RSSI Measurement Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_RSSI, which is sent in
 * response to a requset for the client's current RSSI measurement
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address         */
    UINT                            rssi;           /**< Clients current RSSI       */
} wifi_steering_evRssi_t;

/**
 * @brief Auth Failure Event Data
 * This data is provided with @b WIFI_STEERING_EVENT_AUTH_FAIL
 */
typedef struct {
    mac_address_t                   client_mac;     /**< Client MAC Address         */
    UINT                            rssi;           /**< RSSI of auth frame         */
    UINT                            reason;         /**< Reject Reason              */
    BOOL                            bsBlocked;      /**< True if purposely blocked  */
    BOOL                            bsRejected;     /**< True if rejection sent     */
} wifi_steering_evAuthFail_t;

/**
 * @brief Wifi Steering Event
 * This is the data containing a single steering event.
 */
typedef struct {
    wifi_steering_eventType_t       type;           /**< Event Type                 */
    INT                             apIndex;        /**< apIndex event is from      */
    ULLONG                          timestamp_ms;   /**< Optional: Event Timestamp  */
    union {
        wifi_steering_evProbeReq_t      probeReq;   /**< Probe Request Data         */
        wifi_steering_evConnect_t       connect;    /**< Client Connect Data        */
        wifi_steering_evDisconnect_t    disconnect; /**< Client Disconnect Data     */
        wifi_steering_evActivity_t      activity;   /**< Client Active Change Data  */
        wifi_steering_evChanUtil_t      chanUtil;   /**< Channel Utilization Data   */
        wifi_steering_evRssiXing_t      rssiXing;   /**< Client RSSI Crossing Data  */
        wifi_steering_evRssi_t          rssi;       /**< Client Measured RSSI Data  */
        wifi_steering_evAuthFail_t      authFail;   /**< Auth Failure Data          */
    } data;
} wifi_steering_event_t;

/**
 * @brief Wifi Steering Event Callback Definition
 *
 * This is the definition of the event callback provided when upper layer
 * registers for steering events.
 *
 * @warning the @b event passed to the callback is not dynamically
 * allocated the call back function handler must allocate wifi_steering_event_t
 * and copy the "event" into that
 */
typedef void (*wifi_steering_eventCB_t)(UINT steeringgroupIndex, wifi_steering_event_t *event);

/**
 * @brief Register for Steering Event Callbacks.
 *
 * This is called by the upper layer to register for steering event
 * callbacks.
 *
 * @param[in]  event_cb Event callback function pointer.
 *
 * @return RETURN_OK on success, RETURN_ERR on failure.
 *
 * @warning the @b event passed to the callback should be a dynamically
 * allocated event which upper layer will free by calling wifi_steering_eventFree()
 */
INT wifi_steering_eventRegister(wifi_steering_eventCB_t event_cb);

/**
 * @brief Unregister for Steering Event Callbacks.
 *
 * This is called by the upper layer to stop receiving event callbacks.
 *
 * @return RETURN_OK on success, RETURN_ERR on failure
 */
INT wifi_steering_eventUnregister(void);

/**
 * @brief Configuration per apIndex
 *
 * This defines the configuration for each @b apIndex added to a steering
 * group
 *
 * Channel utilization is to be sampled every @b utilCheckIntervalSec seconds,
 * and after collecting $b utilAvgCount samples, the steering event
 * @b WIFI_STEERING_EVENT_CHAN_UTILIZATION should be sent with the averaged value.
 *
 * Client active/inactive checking is done every @b inactCheckIntervalSec seconds
 * and if a given client is idle/inactive for @b inactCheckThresholdSec seconds then
 * it should be marked as inactive.  Whenever a client changes states between active
 * and inactive, the steering event @b WIFI_STEERING_EVENT_CLIENT_ACTIVITY should be
 * sent.
 */
typedef struct {
    INT         apIndex;

    UINT        utilCheckIntervalSec;   /**< Channel utilization check interval        */
    UINT        utilAvgCount;           /**< Number of samples to average           */

    UINT        inactCheckIntervalSec;  /**< Client inactive check internval        */
    UINT        inactCheckThresholdSec; /**< Client inactive threshold              */
} wifi_steering_apConfig_t;

/**
 * @brief Add a Steering Group.
 *
 * A steering group defines a group of apIndex's which can have steering done
 * between them.
 *
 * @param[in] steeringgroupIndex  Wifi Steering Group index
 * @param[in] cfg_2               2.4G apConfig
 * @param[in] cfg_5               5G apConfig
 *
 * @return RETURN_OK on success, RETURN_ERR on failure
 *
 * @warning All apIndex's provided within a group must have the same SSID,
 * encryption, and passphrase configured for steering to function properly.
 *
 * @note The hal need to allocate (no matter static or dynamic) to store those two config
 * if cfg_2 and cfg_5 are NULL, this steering group will be removed
 */
INT wifi_steering_setGroup(UINT steeringgroupIndex, wifi_steering_apConfig_t *cfg_2, wifi_steering_apConfig_t *cfg_5);

/**
 * @brief Configuration per Client
 *
 * This defines the per-client, per-apIndex configuration settings.  The
 * high water mark + low water mark pairs define RSSI ranges, in which
 * given packet types (probe or auth) are responded to as long as the RSSI
 * of the request packet is within the defined range.
 *
 * The RSSI crossings define thresholds which result in steering events
 * being generated when a connected clients RSSI crosses above or below
 * the given threshold.
 *
 * authRejectReason, when non-zero, results in auth requests being
 * rejected with the given reason code.  When set to zero, auth requests
 * that do not fall in the RSSI hwm+lwm range will be silently ignored.
 *
 * @see https://supportforums.cisco.com/document/141136/80211-association-status-80211-deauth-reason-codes
 */
typedef struct {
    UINT        rssiProbeHWM;           /**< Probe response RSSI high water mark    */
    UINT        rssiProbeLWM;           /**< Probe response RSSI low water mark     */
    UINT        rssiAuthHWM;            /**< Auth response RSSI high water mark     */
    UINT        rssiAuthLWM;            /**< Auth response RSSI low water mark      */
    UINT        rssiInactXing;          /**< Inactive RSSI crossing threshold       */
    UINT        rssiHighXing;           /**< High RSSI crossing threshold           */
    UINT        rssiLowXing;            /**< Low RSSI crossing threshold            */
    UINT        authRejectReason;       /**< Inactive RSSI crossing threshold       */
} wifi_steering_clientConfig_t;

/**
 * @brief Add Client Config to apIndex.
 *
 * The upper layer calls this funciton to @b add/modify per-client configuration @p config
 * of @p client_mac for @p apIndex
 *
 * @param[in] steeringgroupIndex   Wifi Steering Group index
 * @param[in] apIndex              Accesspoint index the client config should be added to
 * @param[in] client_mac           The Client's MAC address.
 *                                 If client_mac is not there, the hal need to add record,
 *                                 else, the hal need to update the config
 * @param[in] config               The client configuration
 *
 * @return RETURN_OK on success, RETURN_ERR on failure
 */
INT wifi_steering_clientSet(
                                UINT steeringgroupIndex,
                                INT apIndex,
                                mac_address_t client_mac,
                                wifi_steering_clientConfig_t *config);

/**
 * @brief Remove Client Config from apIndex
 *
 * The upper layer calls this function to @b remove per-client configuration
 * of @p client_mac from @p apIndex
 *
 * @param[in] steeringgroupIndex  Wifi Steering Group index
 * @param[in] apIndex             Access point index, the client config to be removed.
 * @param[in] client_mac          The Client's MAC address
 *
 * @return RETURN_OK on success, RETURN_ERR on failure
 */
INT wifi_steering_clientRemove(
                                UINT steeringgroupIndex,
                                INT apIndex,
                                mac_address_t client_mac);

/**
 * @brief Initiate Instant Client RSSI Measurement.
 *
 * This initiates an instant client RSSI measurement.  The recommended method of
 * performing this measurement is to send five NUL wifi frames to the client, and
 * average the RSSI of the ACK frames returned.  This averaged RSSI value should
 * be sent back using @b WIFI_STEERING_EVENT_RSSI steering event type.
 * Instant measurement improves user experience by not reacting to false-positive
 * RSSI crossings.
 * If for some reason instant measurement is not supported, the function should
 * return RETURN_ERR and set errno to @b ENOTSUP.
 *
 * @param[in]  steeringgroupIndex  Wifi Steering Group index
 * @param[in]  apIndex             Access point index, the client config should be added to
 * @param[in]  client_mac          The Client's MAC address
 *
 * @return RETURN_OK on success, RETURN_ERR on failure.  Set errno to ENOTSUP if
 * instant measurement is not supported
 */
INT wifi_steering_clientMeasure(
                                UINT steeringgroupIndex,
                                INT apIndex,
                                mac_address_t client_mac);

/**
 * @brief Initiate a Client Disconnect.
 *
 * This is used by the upper layer to kick off a client, for steering purposes.
 *
 * @param[in]  steeringgroupIndex  Wifi Steering Group index
 * @param[in]  apIndex             The access point index, the client config should be added to
 * @param[in]  client_mac          The Client's MAC address
 * @param[in]  type                Disconnect Type
 * @param[in]  reason              Reason code to provide in deauth/disassoc frame.
 *
 * @return RETURN_OK on success, RETURN_ERR on failure
 * @see https://supportforums.cisco.com/document/141136/80211-association-status-80211-deauth-reason-codes
 */
INT wifi_steering_clientDisconnect(
                                UINT steeringgroupIndex,
                                INT apIndex,
                                mac_address_t client_mac,
                                wifi_disconnectType_t type,
                                UINT reason);

typedef struct {
    ULONG               tsf;    // 8 octet TSF timer value.
    USHORT              duration;
} wifi_BTMTerminationDuration_t;

typedef struct {
    CHAR                condensedStr[3];  // 2 char country code from do11CountryString.
} wifi_CondensedCountryString_t;

typedef struct {
    USHORT              offset;
    USHORT              interval;
} wifi_TSFInfo_t;

typedef struct {
    UCHAR               preference;
} wifi_BSSTransitionCandidatePreference_t;

typedef struct {
    USHORT              bearing;
    UINT                dist;
    USHORT              height;
} wifi_Bearing_t;

// Wide Bandwidth Channel Element, ID = 194.  802.11-2016 section 9.4.2.161.
typedef struct {
    UCHAR               bandwidth;
    UCHAR               centerSeg0;
    UCHAR               centerSeg1;
} wifi_WideBWChannel_t;


// HT Capabilities Element, ID = 45.  802.11-2016 section 9.4.2.56.
typedef struct {

    USHORT                  info;           // Bitfield where bit 0 is info[0] bit 0.
    UCHAR                   ampduParams;
    UCHAR                   mcs[16];        // Bitfield where bit 0 is mcs[0] bit 0.
    USHORT                  extended;       // Bitfield where bit 0 is ele_HTExtendedCapabilities[0] bit 0.
    UINT                    txBeamCaps;     // Bitfield where bit 0 is ele_TransmitBeamFormingCapabilities[0] bit 0.
    UCHAR                   aselCaps;
} wifi_HTCapabilities_t;

// VHT Capabilities Element, ID = 191.  802.11-2016 section 9.4.2.158.
typedef struct {
    UINT                    info;
    // The Supported VHT-MCS and NSS Set field is 64 bits long, but is broken
    // into 4 16 bit fields for convenience.
    USHORT                  mcs;
    USHORT                  rxHighestSupportedRate;
    USHORT                  txVHTmcs;
    USHORT                  txHighestSupportedRate;
} wifi_VHTCapabilities_t;

// HT OperationElement, ID = 61, 802.11-2016 section 9.4.2.57.
typedef struct {
    UCHAR                   primary;
    UCHAR                   opInfo[5];   // Bitfield where bit 0 is ele_HTOperationInfo[0] bit 0;
    UCHAR                   mcs[16];
} wifi_HTOperation_t;

// VHT Operation Element, ID = 192.  802.11-2016 section 9.4.2.159.
typedef struct {
    wifi_WideBWChannel_t        opInfo;         // channel width, center of seg0, center of seg1
    USHORT                      mcs_nss;        // Bit field.
} wifi_VHTOperation_t;

// Secondary Channel Offset Element, ID = 62, 802.11-2016 section
// 9.4.2.20.
typedef struct {
    UCHAR                       secondaryChOffset;
} wifi_SecondaryChannelOffset_t;

// RM Enabled Capabilities Element, ID = 70, 802.11-2016 section
// 9.4.2.45.
typedef struct {
    // This is a bit field defined by table 9-157.  Bit 0 for all of the
    // capabilities is ele_RMEnabledCapabilities[5] bit 0.
    UCHAR                       capabilities[5];
} wifi_RMEnabledCapabilities_t;

// Vendor Specific Element, ID = 221.  802.11-2016 section 9.4.2.26.
typedef struct {
    // 3 or 5 octet OUI depending on format; see 802.11-2016 section 9.4.1.32.
    UCHAR           oui[5];
    // Vendor specific content.
    UCHAR           buff[MAX_VENDOR_SPECIFIC];
} wifi_VendorSpecific_t;

// Measurement Pilot Transmission Element, ID = 66, 802.11-2016 section
// 9.4.2.42.
typedef struct {
    UCHAR                       pilot;
    // Series of (sub)elements.  Table 9-155 only lists vendor specific.
    wifi_VendorSpecific_t                       vendorSpecific;
} wifi_MeasurementPilotTransmission_t;

typedef struct {
    bssid_t             bssid;
    //  32 bit optional value, bit fileds are
    //  b0, b1 for reachability
    //  b2 security
    //  b3 key scope
    //  b4 to b9 capabilities
    //  b10 mobility domain
    //  b11 high troughput
    //  b12 very high throughput
    //  b13 ftm
    //  b14 to b31 reserved
    UINT                info;
    UCHAR               opClass;
    UCHAR               channel;
    UCHAR               phyTable;
    BOOL                tsfPresent;
    wifi_TSFInfo_t      tsfInfo;
    BOOL                condensedCountrySringPresent;
    wifi_CondensedCountryString_t   condensedCountryStr;
    BOOL                bssTransitionCandidatePreferencePresent;
    wifi_BSSTransitionCandidatePreference_t         bssTransitionCandidatePreference;
    BOOL                btmTerminationDurationPresent;
    wifi_BTMTerminationDuration_t   btmTerminationDuration;
    BOOL                bearingPresent;
    wifi_Bearing_t      bearing;
    BOOL                wideBandWidthChannelPresent;
    wifi_WideBWChannel_t    wideBandwidthChannel;
    BOOL                htCapsPresent;
    wifi_HTCapabilities_t   htCaps;
    BOOL                vhtCapsPresent;
    wifi_VHTCapabilities_t  vbhtCaps;
    BOOL                    htOpPresent;
    wifi_HTOperation_t      htOp;
    BOOL                    vhtOpPresent;
    wifi_VHTOperation_t     vhtOp;
    BOOL                    secondaryChannelOffsetPresent;
    wifi_SecondaryChannelOffset_t   secondaryChannelOffset;
    BOOL                    rmEnabledCapsPresent;
    wifi_RMEnabledCapabilities_t    rmEnabledCaps;
    BOOL                            msmtPilotTransmissionPresent;
    wifi_MeasurementPilotTransmission_t     msmtPilotTransmission;
    BOOL                    vendorSpecificPresent;
    wifi_VendorSpecific_t   vendorSpecific;
    ssid_t                  target_ssid;
} wifi_NeighborReport_t;

// BSS Transition Management Request Frame, 802.11-2016 section 9.6.14.9.
typedef struct {
    UCHAR               token;              /**< set by STA to relate reports */
    UCHAR               requestMode;        /**< Requested instructions for the STA. */
    USHORT              timer;
    UCHAR               validityInterval;
    // The optional fields may include:
    // 1. BSS Termination Duration Subelement, ID = 4. 802.11-2016 Figure 9-300.
    // 2. Session Information URL.
    // 3. BSS Transition Candidate List Entries
    wifi_BTMTerminationDuration_t    termDuration;
    UCHAR               disassociationImminent;
    USHORT              urlLen;
    CHAR                url[MAX_URL_LEN];
    UCHAR               numCandidates;
    wifi_NeighborReport_t    candidates[MAX_CANDIDATES];
} wifi_BTMRequest_t;

/**
 * @brief Set a BTM Request to a non-AP STA.  The callback register
 * function should be called first so that the response can be handled by the
 * application.
 *
 * @param apIndex; index of the vAP to send the request from.
 * @param peerMACAddress; MAC address of the peer device to send the request to.
 * @param in_struct; BTM Request Frame to send to the non-AP STA.
 * @return The status of the operation.
 * @retval RETURN_OK if successful.
 * @retval RETURN_ERR if any error is detected.
 *
 * @execution Synchronous
 * @sideeffect None
 *
 * @note This function must not suspend and must not invoke any blocking system
 * calls.
 */
INT wifi_setBTMRequest(UINT apIndex,
                        CHAR       *peerMac,
                        wifi_BTMRequest_t *request);


// 802.11k Beacon request & report structures and function prototypes
#define MAX_REQUESTED_ELEMS     8
#define MAX_CHANNELS            16

typedef struct {
    UCHAR               condition;
    UCHAR               threshold;
} wifi_BeaconReporting_t;

typedef struct {
    UCHAR               ids[MAX_REQUESTED_ELEMS];
} wifi_RequestedElementIDS_t;

typedef wifi_RequestedElementIDS_t  wifi_ExtdRequestedElementIDS_t;

// AP Channel Report Element, ID = 51, 802.11-2016 section 9.4.2.36.
typedef struct {
    UCHAR               opClass;
    UCHAR               channels[MAX_CHANNELS];
} wifi_ChannelReport_t;

// 802.11-2016 section 9.4.2.21.7
typedef struct {
    UCHAR               opClass;
    UCHAR               channel;
    USHORT              randomizationInterval;
    USHORT              duration;
    UCHAR               mode;
    bssid_t             bssid;
    BOOL                ssidPresent;
    ssid_t              ssid;
    BOOL                beaconReportingPresent;
    wifi_BeaconReporting_t  beaconReporting;
    BOOL                reportingRetailPresent;
    UCHAR               reportingDetail;
    BOOL                wideBandWidthChannelPresent;
    wifi_WideBWChannel_t    wideBandwidthChannel;
    BOOL                requestedElementIDSPresent;
    wifi_RequestedElementIDS_t      requestedElementIDS;
    BOOL                extdRequestedElementIDSPresent;
    wifi_ExtdRequestedElementIDS_t  extdRequestedElementIDS;
    BOOL                channelReportPresent;
    wifi_ChannelReport_t    channelReport;
    BOOL                vendorSpecificPresent;
    wifi_VendorSpecific_t   vendorSpecific;
    USHORT               numRepetitions;
} wifi_BeaconRequest_t;

/* @description Set a radio measurement (RM) beacon request.  Causes the
 * request to be sent based on the information in the request parameter.
 * Sent from the AP at apIndex.  Returns an error if a callback has not been
 * registered for the AP.
 *
 * @param apIndex; index of the vAP to send the request from.
 * @param peerMACAddress, MAC address of the peer device to send the request
 *      to.  Must be an external device MAC address.
 * @param in_request; pointer to a Beacon Report request structure.
 * @param out_DialogToken; the token chosen by the STA for the requested
 *      measurement(s);
 * @return The status of the operation.
 * @retval RETURN_OK if successful.
 * @retval RETURN_ERR if any error is detected.  If the AP can determine that
 *      the target device does not support Radio Measurement, then an error
 *      is returned.
 */
INT wifi_setRMBeaconRequest(UINT apIndex,
                             CHAR *peer,
                             wifi_BeaconRequest_t *in_request,
                             UCHAR *out_DialogToken);

/* wifi_apDisassociatedDevice_callback() function */
/**
* @brief This call back will be invoked when new wifi client disassociates from Access Point.
*
* @param[in] apIndex          Access Point Index
* @param[in] MAC                          MAC address of disassociated device
* @param[in] event_type           type of disassociation, explicit or due to client inactivity
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
typedef INT ( * wifi_apDisassociatedDevice_callback)(INT apIndex, char *MAC, INT event_type);

/* wifi_apDisassociatedDevice_callback_register() function */
/**
* @brief Callback registration function.
*
* @param[in] callback_proc  wifi_apDisassociatedDevice_callback callback function
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
void wifi_apDisassociatedDevice_callback_register(wifi_apDisassociatedDevice_callback callback_proc);

typedef struct _wifi_client_associated_dev
{
        UCHAR MACAddress[6];                /**< The MAC address of an associated device. */
        INT NetworkID;                      /**< Network ID */
        INT connected;                      /**< If network is connected */
        INT reason;
        INT locally_generated;
} wifi_client_associated_dev_t;

typedef INT ( * wifi_client_event_callback)(INT apIndex, wifi_client_associated_dev_t *state);

void wifi_client_event_callback_register(wifi_client_event_callback callback_proc);

typedef struct wifi_sta_network {
        int id;
        char bridge[16]; /* valid for 4addr_multi_ap */
        char ssid[32];
        char psk[128];
        char bssid[6]; /* 00:00:00:00:00:00 means any */
        size_t ssid_len;
        size_t psk_len;
        int multi_ap;
        char pairwise[64];
        char proto[64];
        char key_mgmt[64];
        unsigned long flags; /* enum wifi_sta_network_flags */
} wifi_sta_network_t;

typedef struct {
	BOOL                                    interworkingEnabled;
	UINT                                    accessNetworkType;
	BOOL                                        internetAvailable;
	BOOL                                        asra;
	BOOL                                        esr;
	BOOL                                        uesa;
	BOOL                                        venueOptionPresent;    /**< True when venue information has not been provided, e.g. the hostspot is in a residence. */
	UCHAR                                       venueType;
	BOOL                                    hessOptionPresent;
	mac_addr_str_t                      hessid;    /**< Optional; use empty string to indicate no value provided. */
} wifi_InterworkingElement_t;

INT wifi_getSTANetworks(INT apIndex, wifi_sta_network_t **out_staNetworks_array, INT out_array_size, BOOL *out_scan_cur_freq);

INT wifi_setSTANetworks(INT apIndex, wifi_sta_network_t **staNetworks_array, INT array_size, BOOL scan_cur_freq);

//<< ------------------------------ wifi_ap_hal -----------------------

#else
#error "! __WIFI_HAL_H__"
#endif
