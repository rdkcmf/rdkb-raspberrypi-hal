/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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
#ifndef __WIFI_HAL_RPI_H__
#define __WIFI_HAL_RPI_H__
/* Enum to define WiFi Bands */

typedef enum
{
    band_invalid = -1,
    band_2_4 = 0,
    band_5 = 1,
} wifi_band;

/*hostapd will read file from nvram /etc/usr/ccsp/wifi/ will contains default
configuration required for Factory Reset*/
#define WIFI_ENTRY_EXIT_DEBUG printf
#define HOSTAPD_FNAME "/nvram/hostapd"
#define SEC_FNAME "/etc/sec_file.txt"
#define BW_FNAME "/etc/bw_file.txt"
#define MAX_BUF_SIZE 128
#define MAX_CMD_SIZE 1024
#define CONFIG_PREFIX "/nvram/hostapd"
#define WIFI_HAL_TOTAL_NO_OF_APS 16

typedef struct _bs_AssDevices
{
	UCHAR bs_mac0[6];
	UCHAR bs_mac1[6];
} bs_AssDevices_t;

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
#endif
