/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
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

/**********************************************************************

    module: rpi_wifi_hal_version_3.c

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "wifi_hal.h"
#include "wifi_hal_rpi.h"

#if defined(WIFI_HAL_VERSION_3)
#define number_of_radios 2

typedef struct wifi_enum_to_str_map
{
        int enum_val;
        const char *str_val;
} wifi_enum_to_str_map_t;

int rpi_get_radios(void)
{
	return number_of_radios;
}

int rpi_map_str2enum_fromTable(char *istr, unsigned int *ivar, char *delim,
        struct wifi_enum_to_str_map *aTable)
{
        char *tmp_str, *tok, *rest;
        int i;

        tmp_str = strdup(istr);
        *ivar = 0;
        tok = strtok_r(tmp_str, delim, &rest);
        while (tok) {
                for (i = 0; aTable[i].str_val != NULL; i++) {
                        if (!(strcmp(aTable[i].str_val, tok))) {
                                /* match */
                                *ivar |= aTable[i].enum_val;
                                break;
                        }
                }
                tok = strtok_r(NULL, delim, &rest);
        }
        free(tmp_str);

        return RETURN_OK;
}

int rpi_str2uintArray(char *istr, char *delim, unsigned int *count,
        unsigned int *uarray, unsigned int maxCount)
{
        char *tmp_str, *tok, *rest;
        int i = 0;

        if ((istr == NULL) || (delim == NULL) || (count == NULL) || (uarray == NULL)) {
                printf("%s, Error Input param\n", __FUNCTION__);
                return RETURN_ERR;
        }
        tmp_str = strdup(istr);
        if (tmp_str == NULL) {
                printf("%s, Error strdup\n", __FUNCTION__);
                return RETURN_ERR;
        }
        tok = strtok_r(tmp_str, delim, &rest);
        while (tok && (i < maxCount)) {
                uarray[i] = atoi(tok);
                i++;
                tok = strtok_r(NULL, delim, &rest);
        }
        free(tmp_str);
        *count = i;
        return RETURN_OK;
}

/* rate string to wifi_bitrate_t */
static struct wifi_enum_to_str_map wifi_bitrate_infoTable[] =
{
        /* brate        rateStr */
        {WIFI_BITRATE_1MBPS,    "1"},
        {WIFI_BITRATE_2MBPS,    "2"},
        {WIFI_BITRATE_5_5MBPS,  "5.5"},
        {WIFI_BITRATE_6MBPS,    "6"},
        {WIFI_BITRATE_9MBPS,    "9"},
        {WIFI_BITRATE_11MBPS,   "11"},
        {WIFI_BITRATE_12MBPS,   "12"},
        {WIFI_BITRATE_18MBPS,   "18"},
        {WIFI_BITRATE_24MBPS,   "24"},
        {WIFI_BITRATE_36MBPS,   "36"},
        {WIFI_BITRATE_48MBPS,   "48"},
        {WIFI_BITRATE_54MBPS,   "54"},
	{0,  NULL}
};

/* refer to wifi_ap_security_mode_t for the enum definitions */
wifi_enum_to_str_map_t security_mode_table[] = {
        {wifi_security_mode_none,                               "None"},
        {wifi_security_mode_wep_64,                             "WEP-64"},
        {wifi_security_mode_wep_128,                            "WEP-128"},
        {wifi_security_mode_wpa_personal,                       "WPA-Personal"},
        {wifi_security_mode_wpa2_personal,                      "WPA2-Personal"},
        {wifi_security_mode_wpa_wpa2_personal,                  "WPA-WPA2-Personal"},
        {wifi_security_mode_wpa_enterprise,                     "WPA-Enterprise"},
        {wifi_security_mode_wpa2_enterprise,                    "WPA2-Enterprise"},
        {wifi_security_mode_wpa_wpa2_enterprise,                "WPA-WPA2-Enterprise"},
        {wifi_security_mode_wpa3_personal,                      "WPA3-Personal"},
        {wifi_security_mode_wpa3_transition,            "WPA3-Personal-Transition"},
        {wifi_security_mode_wpa3_enterprise,                    "WPA3-Enterprise"},
        {0xff,                                                  NULL}
};

wifi_enum_to_str_map_t encryption_table[] = {
        {wifi_encryption_tkip,          "TKIPEncryption"},
        {wifi_encryption_aes,           "AESEncryption"},
        {wifi_encryption_aes_tkip,      "TKIPandAESEncryption"},
        {0xff,                          NULL}
};

wifi_enum_to_str_map_t mfp_table[] = {
        {wifi_mfp_cfg_disabled,         "Disabled"},
        {wifi_mfp_cfg_optional,         "Optional"},
        {wifi_mfp_cfg_required,         "Required"},
        {0xff,                          NULL}
};

/* refer to wifi_ap_OnBoardingMethods_t for the enum definitions */
wifi_enum_to_str_map_t wps_config_method_table[] = {
        {WIFI_ONBOARDINGMETHODS_LABEL,              "Label"},
        {WIFI_ONBOARDINGMETHODS_DISPLAY,            "Display"},
        {WIFI_ONBOARDINGMETHODS_PUSHBUTTON,         "PushButton"},
        {WIFI_ONBOARDINGMETHODS_PIN,                "Keypad"},
        {0xff,                                          NULL}
};

INT wifi_updateApSecurity(INT apIndex, wifi_vap_security_t *security)
{
	int i;
	char *mfpStr;
	printf("%s: apIndex [%d]\n", __FUNCTION__, apIndex);
	/* set mfp */
	for (i = 0; mfp_table[i].str_val != NULL; ++i) {
		if (mfp_table[i].enum_val == security->mfp)
			break;
	}
	if (mfp_table[i].str_val == NULL) {
		printf("%s: %d: wrong mfp [%d]\n", __FUNCTION__,
				apIndex, security->mfp);
	}
	mfpStr = (char *)mfp_table[i].str_val;
	printf("%s: %d: mfp [%s]\n", __FUNCTION__,apIndex, mfpStr);
	if(wifi_setApSecurityMFPConfig(apIndex,mfpStr) != RETURN_OK)
	{
		printf("%s wifi_setApSecurityMFPConfig failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
		return RETURN_ERR;
	}
	/* set security mode */
	for (i = 0; security_mode_table[i].str_val != NULL; ++i) {
		if (security_mode_table[i].enum_val == security->mode)
			break;
	}
	printf("%s: %d: security mode [%s]\n", __FUNCTION__,apIndex, security_mode_table[i].str_val);
	if(wifi_setApSecurityModeEnabled(apIndex,(char *)security_mode_table[i].str_val) != RETURN_OK)
	{
		printf("%s wifi_setApSecurityModeEnabled failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
		return RETURN_ERR;
	}
	/* set wpa encryption */
	if (security->mode != wifi_security_mode_none) {
		for (i = 0; encryption_table[i].str_val != NULL; ++i) {
			if (encryption_table[i].enum_val == security->encr)
				break;
		}
		printf("%s: %d: encryption [%s]\n", __FUNCTION__, apIndex, encryption_table[i].str_val);
		if(wifi_setApWpaEncryptionMode(apIndex,(char *)encryption_table[i].str_val) != RETURN_OK)
		{
			printf("%s wifi_setApWpaEncryptionMode failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
			return RETURN_ERR;
		}
		/* set wpa psk or passphrase */
		if (security->mode == wifi_security_mode_wpa_personal
				|| security->mode == wifi_security_mode_wpa2_personal
				|| security->mode == wifi_security_mode_wpa3_personal
				|| security->mode == wifi_security_mode_wpa_wpa2_personal) {
			if (security->u.key.type == wifi_security_key_type_psk) {
				if(wifi_setApSecurityPreSharedKey(apIndex,security->u.key.key) != RETURN_OK) {
					printf("%s wifi_setApSecurityPreSharedKey failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
					return RETURN_ERR;
				}
			}	
			else
			{
				if(wifi_setApSecurityKeyPassphrase(apIndex,security->u.key.key) != RETURN_OK)
				{
					printf("%s wifi_setApSecurityKeyPassphrase failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
					return RETURN_ERR;
				}
			}	
		}
	}

	return RETURN_OK;
}

INT wifi_setApSecurity(INT apIndex, wifi_vap_security_t *security)
{
	if (wifi_updateApSecurity(apIndex, security)){
		printf("%s: %d: wifi_updateApSecurity failed\n",
				__FUNCTION__, apIndex);
		return RETURN_ERR;
	}
	return RETURN_OK;
}

INT wifi_setApWpsConfiguration(INT apIndex, wifi_wps_t* wpsConfig)
{
	printf("%s: apIndex [%d]\n", __FUNCTION__, apIndex);
	ULONG pin;
	/* Set WPS enable */		
	if(wifi_setApWpsEnable(apIndex,wpsConfig->enable) != RETURN_OK)
	{
		printf("%s wifi_setApWpsEnable failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
		return RETURN_ERR;	
	}
	/* Set WPS device PIN */
	if (wpsConfig->pin[0] != '\0') {
		pin = atol(wpsConfig->pin);
		if(wifi_setApWpsDevicePIN(apIndex,pin) != RETURN_OK)
		{
			printf("%s wifi_setApWpsDevicePIN failed ! apIndex[%d]\n",__FUNCTION__,apIndex);
			return RETURN_ERR;
		}
	}
	/* Set enabled WPS config methods */
	if (wpsConfig->methods) {
		char enabled_methods[512] = {'\0'}; //buffer is big enough to hold all methods
		int i, size, len;

		len = sizeof(enabled_methods);
		for (i = 0;  wps_config_method_table[i].str_val != NULL; ++i) {
			if (wpsConfig->methods & wps_config_method_table[i].enum_val) {
				if (enabled_methods[0] == '\0')
					size = snprintf(enabled_methods, len, "%s",
							wps_config_method_table[i].str_val);
				else
					size += snprintf(enabled_methods + size, len - size,
							",%s", wps_config_method_table[i].str_val);
			}
		}

		if (wifi_setApWpsConfigMethodsEnabled(apIndex,enabled_methods) != RETURN_OK) {
			printf("%s: %d: wifi_setApWpsConfigMethodsEnabled failed\n",
					__FUNCTION__, apIndex);
			return RETURN_ERR;
		}
	}

	return RETURN_OK;
}

INT wifi_createVAP(wifi_radio_index_t index, wifi_vap_info_map_t *map)
{
	int i, apIndex, ret,enable,acl_mode;
	printf("%s radioIndex = %d\n", __FUNCTION__, index);
	for (i = 0; i < map->num_vaps; i++) 
	{
		apIndex = map->vap_array[i].vap_index;
		printf("%s: ssid=%s, enabled=%d, showSsid=%d, isolation=%d, security=%d\n"
				"\tmgmtPowerControl=%d, bssMaxSta=%d, bssTransitionActivated=%d, nbrReportActivated=%d\n"
				"\tmac_filter_enable=%d, mac_filter_mode=%d\n",
				__FUNCTION__, map->vap_array[i].u.bss_info.ssid,
				map->vap_array[i].u.bss_info.enabled, map->vap_array[i].u.bss_info.showSsid,
				map->vap_array[i].u.bss_info.isolation,
				map->vap_array[i].u.bss_info.security.mode,
				map->vap_array[i].u.bss_info.mgmtPowerControl,
				map->vap_array[i].u.bss_info.bssMaxSta,
				map->vap_array[i].u.bss_info.bssTransitionActivated,
				map->vap_array[i].u.bss_info.nbrReportActivated,
				map->vap_array[i].u.bss_info.mac_filter_enable,
				map->vap_array[i].u.bss_info.mac_filter_mode);
		ret = wifi_setSSIDEnable(apIndex, map->vap_array[i].u.bss_info.enabled);
		if (ret != RETURN_OK) {
			printf("%s:wifi_setSSIDEnable failed, apIndex=[%d]\n",
					__FUNCTION__, apIndex);
			return RETURN_ERR;
		}
		if (!map->vap_array[i].u.bss_info.enabled)
		{
			continue;
		}
		ret = wifi_setSSIDName(apIndex,map->vap_array[i].u.bss_info.ssid);
		if (ret != RETURN_OK) {
			printf("%s: wifi_setSSIDName Failed,"
					" apIndex=%d, ret = %d\n", __FUNCTION__, apIndex, ret);
			return RETURN_ERR;
		}
		ret = wifi_setApSsidAdvertisementEnable(apIndex,map->vap_array[i].u.bss_info.showSsid);
		if (ret != RETURN_OK) {
			printf("%s: wifi_setApSsidAdvertisementEnable Failed,"
					" apIndex=%d, ret = %d\n", __FUNCTION__, apIndex, ret);
			return RETURN_ERR;
		}
		ret = wifi_setApIsolationEnable(apIndex,map->vap_array[i].u.bss_info.isolation);
		if (ret != RETURN_OK) {
			printf("%s wifi_setApIsolationEnable Failed,"
					" apIndex=%d, ret = %d\n", __FUNCTION__, apIndex, ret);
			return RETURN_ERR;
		}
		wifi_setApManagementFramePowerControl(apIndex,map->vap_array[i].u.bss_info.mgmtPowerControl);
		if (map->vap_array[i].u.bss_info.bssMaxSta == 0) {
			/* To be modified */
			map->vap_array[i].u.bss_info.bssMaxSta = 75;
		}

		ret = wifi_setApMaxAssociatedDevices(apIndex, map->vap_array[i].u.bss_info.bssMaxSta);					
		if (ret != RETURN_OK) {
			printf("%s wifi_setApMaxAssociatedDevices Failed,"
					" apIndex=%d, ret = %d\n", __FUNCTION__, apIndex, ret);
			return RETURN_ERR;
		}
		ret = wifi_setBSSTransitionActivation(apIndex,map->vap_array[i].u.bss_info.bssTransitionActivated);
		if (ret != RETURN_OK) {
			printf("%s wifi_setBSSTransitionActivation Failed,"
					" apIndex=%d, ret = %d\n", __FUNCTION__, apIndex, ret);
			return RETURN_ERR;
		}
		ret = wifi_setNeighborReportActivation(apIndex,map->vap_array[i].u.bss_info.nbrReportActivated); 
		if (ret != RETURN_OK) {
			printf("%s wifi_setNeighborReportActivation Failed,"
					" apIndex=%d, ret = %d\n", __FUNCTION__, apIndex, ret);
			return RETURN_ERR;
		}
		ret = wifi_updateApSecurity(apIndex, &(map->vap_array[i].u.bss_info.security));
		if (ret != RETURN_OK) {
			printf("%s: wifi_updateApSecurity failed, apIndex=[%d]\n",
					__FUNCTION__, apIndex);
			return RETURN_ERR;
		}
		if (map->vap_array[i].u.bss_info.mac_filter_enable == false) 
			acl_mode = 0;
		else
		{
			if (map->vap_array[i].u.bss_info.mac_filter_mode == wifi_mac_filter_mode_black_list) 
				acl_mode = 2;
			else 
				acl_mode = 1;
		}
		ret = wifi_setApMacAddressControlMode(apIndex, acl_mode);	
		if(ret != RETURN_OK) {
			printf("%s: wifi_setApMacAddressControlMode failed, apIndex=[%d]\n",
					__FUNCTION__, apIndex);
			return RETURN_ERR;
		}
		ret =  wifi_setApWpsConfiguration(apIndex, &(map->vap_array[i].u.bss_info.wps));
		if(ret != RETURN_OK) {
			printf("%s: wifi_setApWpsConfiguration failed, apIndex=[%d]\n",
					__FUNCTION__, apIndex);
			return RETURN_ERR;
		}
		ret = wifi_setApWmmEnable(apIndex,map->vap_array[i].u.bss_info.wmm_enabled);
		if(ret != RETURN_OK) {
			printf("%s: wifi_setApWmmEnable failed, apIndex=[%d]\n",
					__FUNCTION__, apIndex);
			return RETURN_ERR;
		}
		wifi_applySSIDSettings(apIndex);
	}
	return RETURN_OK;
}

INT rpi_getRadioCapabilities(int radioIndex, wifi_radio_capabilities_t *rcap)
{
	int ret;
	char band[128],possiblechannels[128],buf[128],SecurityMode[50]={0};
        /* numSupportedFreqBand - if dual band is 2G and 5G capable */
        rcap->numSupportedFreqBand = 2;
	memset(band, 0, sizeof(band));
	ret=wifi_getRadioOperatingFrequencyBand(radioIndex,band);
	if (ret != RETURN_OK)
	{
		printf("%s: cannot get radio band for radio index %d\n", __func__, index);
		return RETURN_ERR;
	}

	if (!strcmp(band, "2.4GHz"))
	{
		GetInterfaceName(rcap->ifaceName,"/nvram/hostapd0.conf");
		rcap->band[radioIndex] = WIFI_FREQUENCY_2_4_BAND;
		rcap->mode[radioIndex] = WIFI_80211_VARIANT_G | WIFI_80211_VARIANT_N;
		rcap->channelWidth[radioIndex] = WIFI_CHANNELBANDWIDTH_20MHZ;
		rcap->maxBitRate[radioIndex] = 144;
		rcap->supportedBitRate[radioIndex] |= (WIFI_BITRATE_1MBPS | WIFI_BITRATE_2MBPS |
                                WIFI_BITRATE_5_5MBPS | WIFI_BITRATE_6MBPS | WIFI_BITRATE_9MBPS |
                                WIFI_BITRATE_11MBPS | WIFI_BITRATE_12MBPS);
	}
	else if (!strcmp(band, "5GHz"))
	{
		GetInterfaceName(rcap->ifaceName,"/nvram/hostapd1.conf");
		rcap->band[radioIndex] = WIFI_FREQUENCY_5_BAND;
		rcap->mode[radioIndex] = WIFI_80211_VARIANT_AC;
		rcap->channelWidth[radioIndex] = WIFI_CHANNELBANDWIDTH_40MHZ;
		rcap->maxBitRate[radioIndex] = 300;
		rcap->supportedBitRate[radioIndex] |= (WIFI_BITRATE_6MBPS | WIFI_BITRATE_9MBPS |
                                WIFI_BITRATE_12MBPS | WIFI_BITRATE_18MBPS | WIFI_BITRATE_24MBPS |
                                WIFI_BITRATE_36MBPS | WIFI_BITRATE_48MBPS | WIFI_BITRATE_54MBPS);

	}
	memset(possiblechannels,0,sizeof(possiblechannels));
	ret=wifi_getRadioPossibleChannels(radioIndex,possiblechannels);
	if (ret != RETURN_OK)
	{
                printf("%s: cannot get Possible Channels list for radio index %d\n", __func__, index);
                return RETURN_ERR;
        }
	rpi_str2uintArray(possiblechannels, ", ", &(rcap->channel_list[radioIndex].num_channels),
                        rcap->channel_list[radioIndex].channels_list,
                        MAX_CHANNELS);
	memset(buf,0,sizeof(buf));
        ret=wifi_getRadioTransmitPowerSupported(radioIndex,buf);
	if (ret != RETURN_OK)
        {
                printf("%s: cannot get TransmitPowerSupported list for radio index %d\n", __func__, index);
                return RETURN_ERR;
        }
        rpi_str2uintArray(buf, ", ", &(rcap->transmitPowerSupported_list[radioIndex].numberOfElements),
                        rcap->transmitPowerSupported_list[radioIndex].transmitPowerSupported,
                        MAXNUMBEROFTRANSMIPOWERSUPPORTED);
	rcap->autoChannelSupported = FALSE;
	rcap->DCSSupported = FALSE;
	rcap->zeroDFSSupported = FALSE;
	/* csi */
        rcap->csi.maxDevices = 0;
        rcap->csi.soudingFrameSupported = FALSE;
        ret=wifi_getApSecurityModeEnabled(radioIndex,&SecurityMode);
        if((strcmp(SecurityMode,"WPA-Personal") == 0))
	       rcap->cipherSupported = 1;
        else if((strcmp(SecurityMode,"WPA2-Personal") == 0))
	       rcap->cipherSupported = 2;
        else if((strcmp(SecurityMode,"WPA-WPA2-Personal") == 0))
	       rcap->cipherSupported = 3;
	rcap->numcountrySupported = 1;
	rcap->countrySupported[radioIndex] = wifi_countrycode_IN;
        rcap->maxNumberVAPs = MAX_NUM_VAP_PER_RADIO;

	return RETURN_OK;
}

INT wifi_getHalCapability(wifi_hal_capability_t *cap)
{
	int i,ret = 0;
	printf("%s\n",__FUNCTION__);
	memset(cap, 0, sizeof(wifi_hal_capability_t));
	/* version */
	cap->version.major = WIFI_HAL_MAJOR_VERSION;
	cap->version.minor = WIFI_HAL_MINOR_VERSION;
	cap->wifi_prop.numRadios = rpi_get_radios();
        
	/* get RadioCapabilities */
        for (i = 0; i < cap->wifi_prop.numRadios; i++) {
                ret = rpi_getRadioCapabilities(i, &(cap->wifi_prop.radiocap[i]));
                if (ret < 0) {
                        printf("%s: rpi_getRadioCapabilities idx = %d\n", __FUNCTION__, i);
                        return RETURN_ERR;
                }
        }
	cap->BandSteeringSupported = FALSE; /*not supported now*/
	return RETURN_OK;
}

INT wifi_getApSecurity(INT apIndex, wifi_vap_security_t *security)
{
	int ret,i;
	char security_mode[128] = {'\0'}, encryption[128] = {'\0'},mfpstr[128] = {'\0'};
	memset(security, 0, sizeof(wifi_vap_security_t));
	ret=wifi_getApSecurityModeEnabled(apIndex,security_mode);
	if(ret != RETURN_OK)
	{
		printf("unable to get security mode %s and %d\n",security_mode,apIndex);
		return RETURN_ERR;
	}	
	printf("%s mode %s \n", __FUNCTION__,security_mode);	
	for (i = 0; security_mode_table[i].str_val != NULL; ++i) {
                if (!strcmp(security_mode_table[i].str_val, security_mode)) {
                        security->mode = security_mode_table[i].enum_val;
                        break;
                }
        }
	ret=wifi_getApSecurityMFPConfig(apIndex,mfpstr);
	if(ret != RETURN_OK)
	{
		printf("unable to get security mfp %s and %d\n",mfpstr,apIndex);
		return RETURN_ERR;
	}		
	for (i = 0; mfp_table[i].str_val != NULL; ++i) {
		if (!strcmp(mfp_table[i].str_val, mfpstr)) {
			security->mfp = mfp_table[i].enum_val;
			break;
		}
	}
	ret=wifi_getApWpaEncryptionMode(apIndex,encryption);
	if(ret != RETURN_OK)
	{
		printf("unable to get encryption %s and %d\n",security_mode,apIndex);
		return RETURN_ERR;
	}		
	for (i = 0; encryption_table[i].str_val != NULL; ++i) {
		if (!strcmp(encryption_table[i].str_val, encryption)) {
			security->encr = encryption_table[i].enum_val;
			break;
		}
	}
	 /* get wpa psk or passphrase */
	if (security->mode == wifi_security_mode_wpa_personal
			|| security->mode == wifi_security_mode_wpa2_personal
			|| security->mode == wifi_security_mode_wpa3_personal
			|| security->mode == wifi_security_mode_wpa_wpa2_personal){
		ret=wifi_getApSecurityKeyPassphrase(apIndex,security->u.key.key);
		if(ret == RETURN_OK)
		{
			security->u.key.type = wifi_security_key_type_pass; 	
		}
		else
		{
			if(wifi_getApSecurityPreSharedKey(apIndex,security->u.key.key) == RETURN_OK)
				security->u.key.type = wifi_security_key_type_psk;
		}	
	}
	security->wpa3_transition_disable = false;
	snprintf(security->u.radius.ip, sizeof(security->u.radius.ip), "68.85.15.238");
	security->u.radius.port = 123;
	snprintf(security->u.radius.key, sizeof(security->u.radius.key), "12345678");
	snprintf(security->u.radius.s_ip, sizeof(security->u.radius.s_ip), "68.85.6.243");
	security->u.radius.s_port = 345;
	snprintf(security->u.radius.s_key, sizeof(security->u.radius.s_key), "12345678");
	security->u.radius.dasport = 567;
	snprintf(security->u.radius.daskey, sizeof(security->u.radius.daskey), "12345678");
        return RETURN_OK;
}

INT wifi_getApWpsConfiguration(INT apIndex, wifi_wps_t* wpsConfig)
{
	int len;
	ULONG pin;
	char enabled_methods[512], *method, *rest;
	int i;

	printf("%s: apIndex [%d]\n", __FUNCTION__, apIndex);
	memset(wpsConfig, 0, sizeof(*wpsConfig));
	/* Get WPS enable */
	if(wifi_getApWpsEnable(apIndex, &wpsConfig->enable) != RETURN_OK)
	{
		printf("%s wifi_getApWpsEnable failed , apIndex[%d] \n",__FUNCTION__,apIndex);
		return RETURN_ERR;
	}	
	/* Get WPS device PIN */
	if(wifi_getApWpsDevicePIN(apIndex,&pin) != RETURN_OK)
	{
		printf("%s wifi_getApWpsDevicePIN failed , apIndex[%d] \n",__FUNCTION__,apIndex);
		return RETURN_ERR;
	}
	sprintf(wpsConfig->pin,"%ld",pin);
	/* Get enabled WPS config methods */
	if(wifi_getApWpsConfigMethodsEnabled(apIndex,enabled_methods) != RETURN_OK)
	{
		printf("%s wifi_getApWpsConfigMethodsEnabled failed , apIndex[%d] \n",__FUNCTION__,apIndex);
		return RETURN_ERR;
	}
	method = strtok_r(enabled_methods, ",", &rest);
	while (method != NULL) {
		i = 0;
		while (TRUE) {
			if (wps_config_method_table[i].str_val == NULL) {
				printf("%s: %d: unsupported method [%s]\n",
						__FUNCTION__, apIndex, method);
				break;
			}

			if (!strcmp(method, wps_config_method_table[i].str_val)) {
				wpsConfig->methods |= wps_config_method_table[i].enum_val;
				break;
			}
			++i;
		}
		method = strtok_r(NULL, ",", &rest);
	}

	return RETURN_OK;
}

INT wifi_getRadioVapInfoMap(wifi_radio_index_t index, wifi_vap_info_map_t *map)
{
	int i,j;
	BOOL enabled = false;
	char buf[256];
	int vap_index,ret;
	INT mode;
	char security_mode[128] = {'\0'}, encryption[128] = {'\0'},mfpstr[128] = {'\0'};
	map->num_vaps = MAX_NUM_VAP_PER_RADIO; // XXX: For both radio let's support 2 vaps for now
	map->vap_array[index].radio_index = index;
	//      private and public wifi support only available now
	WIFI_ENTRY_EXIT_DEBUG("Inside %s:%d\n",__func__, __LINE__);
	printf("Entering %s index = %d\n", __func__, (int)index);

	for (i = 0; i < map->num_vaps; i++)
	{
		// XXX: supported vap indexes for now (0,1 - privatewifi, 4,5 - publicwifi)
		if (index == 0)//2.4GHz
		{
			if (i == 0) vap_index = 0;
			else if (i == 1) vap_index = 2;
			else if (i == 2) vap_index = 4;
			else if (i == 3) vap_index = 6;
			else if (i == 4) vap_index = 8;
			/*else if (i == 5) vap_index = 10;
			else if (i == 6) vap_index = 12;
			else if (i == 7) vap_index = 14; */ //Revisit
		}
		else if (index == 1) //5GHz
		{
			if (i == 0) vap_index = 1;
			else if (i == 1) vap_index = 3;
			else if (i == 2) vap_index = 5;
			else if (i == 3) vap_index = 7;
			else if (i == 4) vap_index = 9;
			else if (i == 5) vap_index = 11;
			else if (i == 6) vap_index = 13;
			else if (i == 7) vap_index = 15;
		}	
		else
		{
			printf("Wrong radio index (%d)\n", index);
			return RETURN_ERR;
		}
		strncpy(map->vap_array[i].bridge_name, "brlan0", sizeof(map->vap_array[i].bridge_name) - 1);
		map->vap_array[i].vap_index = vap_index;

		memset(buf, 0, sizeof(buf));
		ret=wifi_getApName(vap_index, buf); //getting APName
		if(ret != RETURN_OK)
		{
			printf("unable to get APName %d and %d\n",(int)index,vap_index);
			return RETURN_ERR;
		}
		strncpy(map->vap_array[i].vap_name, buf, sizeof(map->vap_array[i].vap_name) - 1);
		//getting SSID Name
		memset(buf, 0, sizeof(buf));
		ret=wifi_getSSIDName(vap_index, buf);
		if(ret != RETURN_OK)
		{
			printf("unable to get SSID Name %d and %d:%s\n",(int)index,vap_index,buf);
			map->vap_array[i].u.bss_info.ssid[0] = '\0';
			return RETURN_ERR;
		}
		strncpy(map->vap_array[i].u.bss_info.ssid, buf, sizeof(map->vap_array[i].u.bss_info.ssid) - 1);
		ret = wifi_getSSIDEnable(vap_index, &enabled);
		if (ret != RETURN_OK)
		{
			printf("failed to get SSIDEnable for index %d\n", i);
			map->vap_array[i].u.bss_info.enabled = FALSE;
			return RETURN_ERR;
		}
		map->vap_array[i].u.bss_info.enabled = enabled;
		printf("%s SSID Enable status %d : %d : %d\n",__FUNCTION__,vap_index,enabled,map->vap_array[i].u.bss_info.enabled);
		ret=wifi_getApSsidAdvertisementEnable(vap_index, &enabled);
		if (ret != RETURN_OK)
		{
			printf("unable to get showSSID status %d and %d:%d\n",(int)index,vap_index,enabled);
			map->vap_array[i].u.bss_info.showSsid = FALSE;
			return RETURN_ERR;
		}
		map->vap_array[i].u.bss_info.showSsid = enabled;
		ret=wifi_getApIsolationEnable(vap_index,&enabled);
		if (ret != RETURN_OK)
		{
			printf("unable to get ApIsolation %d and %d:%d\n",(int)index,vap_index,enabled);
			map->vap_array[i].u.bss_info.isolation = FALSE;
			return RETURN_ERR;
		}
		map->vap_array[i].u.bss_info.isolation = enabled; 
		map->vap_array[i].u.bss_info.mgmtPowerControl = 0;//not supported
		ret=wifi_getApMaxAssociatedDevices(vap_index,&map->vap_array[i].u.bss_info.bssMaxSta);
		if (ret != RETURN_OK)
                {
                        printf("unable to get MaxAssDev %d and %d:%d\n",(int)index,vap_index,ret);
			map->vap_array[i].u.bss_info.bssMaxSta = 75;
                        return RETURN_ERR;
                }
		ret=wifi_getBSSTransitionActivation(vap_index, &enabled);
		if (ret != RETURN_OK)
		{
			printf("unable to get BSSTransistion status %d and %d:%d\n",(int)index,vap_index,enabled);
			map->vap_array[i].u.bss_info.bssTransitionActivated = FALSE;
			return RETURN_ERR;
		}
		map->vap_array[i].u.bss_info.bssTransitionActivated = enabled; 
		ret=wifi_getNeighborReportActivation(vap_index, &enabled);
		if (ret != RETURN_OK)
		{
			printf("unable to get neighborreport Activation status %d and %d:%d\n",(int)index,vap_index,enabled);
			map->vap_array[i].u.bss_info.nbrReportActivated = FALSE;
			return RETURN_ERR;
		}
		map->vap_array[i].u.bss_info.nbrReportActivated = enabled; 
		ret = wifi_getApSecurity(vap_index, &(map->vap_array[i].u.bss_info.security));
		if (ret != RETURN_OK) {
			printf("%s: wifi_getApSecurity failed, apIndex=[%d]\n",
					__FUNCTION__, vap_index);
			return RETURN_ERR;
		}

		ret=wifi_getApSecurityKeyPassphrase(vap_index,map->vap_array[i].u.bss_info.security.u.key.key);
		if(ret == RETURN_OK)
		{
			map->vap_array[i].u.bss_info.security.u.key.type = wifi_security_key_type_pass;
		}
		else
		{
			if(wifi_getApSecurityPreSharedKey(vap_index,map->vap_array[i].u.bss_info.security.u.key.key) == RETURN_OK)
				map->vap_array[i].u.bss_info.security.u.key.type = wifi_security_key_type_psk;
		}

		ret=wifi_getApMacAddressControlMode(vap_index, &mode);
		if (ret != RETURN_OK) {
			printf("%s: wifi_getApMacAddressControlMode failed, apIndex=[%d]\n",
					__FUNCTION__, vap_index);
			map->vap_array[i].u.bss_info.mac_filter_enable = FALSE;
			return RETURN_ERR;
		}
		if (mode == 0) 
			map->vap_array[i].u.bss_info.mac_filter_enable = false;
		else 
			map->vap_array[i].u.bss_info.mac_filter_enable = true;

		if (mode == 1) 
			map->vap_array[i].u.bss_info.mac_filter_mode = wifi_mac_filter_mode_white_list;
		else if (mode == 2) 
			map->vap_array[i].u.bss_info.mac_filter_mode = wifi_mac_filter_mode_black_list;
		ret = wifi_getApWpsConfiguration(vap_index, &(map->vap_array[i].u.bss_info.wps));
		if (ret != RETURN_OK) {
			printf("%s: wifi_getApWpsConfiguration failed, vapIndex=[%d]\n",
					__FUNCTION__, vap_index);
			return RETURN_ERR;
		}
		ret=wifi_getApWmmEnable(vap_index, &(map->vap_array[i].u.bss_info.wmm_enabled));
		if (ret != RETURN_OK) {
			printf("%s: wifi_getApWmmEnable failed, vapIndex=[%d]\n",
					__FUNCTION__, vap_index);
			return RETURN_ERR;
		}
		memset(buf, 0, sizeof(buf));
		ret=wifi_getBaseBSSID(vap_index, buf);
		if (ret != RETURN_OK) {
			printf("%s: wifi_getBaseBSSID failed, vapIndex=[%d]\n",
					__FUNCTION__, vap_index);
			return RETURN_ERR;
		}
		sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
				&map->vap_array[i].u.bss_info.bssid[0],
				&map->vap_array[i].u.bss_info.bssid[1],
				&map->vap_array[i].u.bss_info.bssid[2],
				&map->vap_array[i].u.bss_info.bssid[3],
				&map->vap_array[i].u.bss_info.bssid[4],
				&map->vap_array[i].u.bss_info.bssid[5]);

	}	
	WIFI_ENTRY_EXIT_DEBUG("Exiting %s:%d\n",__func__, __LINE__);
	return RETURN_OK;
}

INT wifi_getRadioOperatingParameters(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
	INT ret;
	char band[128];
	ULONG lval;
	BOOL enabled;
	char buf[256];
	UINT basicDataTransmitRates,operationalDataTransmitRates;

	WIFI_ENTRY_EXIT_DEBUG("Inside %s:%d\n",__func__, __LINE__);
	printf("Entering %s index = %d\n", __func__, (int)index);
	memset(operationParam, 0, sizeof(wifi_radio_operationParam_t));
	
	ret = wifi_getRadioEnable(index, &enabled);
	if (ret != RETURN_OK)
	{
		printf("%s: cannot get enabled state for radio index %d\n", __func__,
				index);
		return RETURN_ERR;
	}
	operationParam->enable = enabled;

	memset(band, 0, sizeof(band));
	ret = wifi_getRadioOperatingFrequencyBand(index, band);
	if (ret != RETURN_OK)
	{
		printf("%s: cannot get radio band for radio index %d\n", __func__, index);
		return false;
	}

	if (!strcmp(band, "2.4GHz"))
	{
		operationParam->band = WIFI_FREQUENCY_2_4_BAND;
		operationParam->variant = WIFI_80211_VARIANT_N | WIFI_80211_VARIANT_G;
	}
	else if (!strcmp(band, "5GHz"))
	{
		operationParam->band = WIFI_FREQUENCY_5_BAND;
		operationParam->variant = WIFI_80211_VARIANT_AC;
	}
	else
	{
		printf("%s: cannot decode band for radio index %d ('%s')\n", __func__, index,
				band);
	}
	memset(buf, 0, sizeof(buf));
	ret = wifi_getRadioOperatingChannelBandwidth(index, buf); // XXX: handle errors
	// XXX: only handle 20/40/80 modes for now
	if (!strcmp(buf, "20MHz")) operationParam->channelWidth = WIFI_CHANNELBANDWIDTH_20MHZ;
	else if (!strcmp(buf, "40MHz")) operationParam->channelWidth = WIFI_CHANNELBANDWIDTH_40MHZ;
	else if (!strcmp(buf, "80MHz")) operationParam->channelWidth = WIFI_CHANNELBANDWIDTH_80MHZ;
	else
	{
		printf("Unknown HT mode: %s\n", buf);
		return false;
	}
	/* Channel */
	ret = wifi_getRadioChannel(index, &lval);
	if (ret != RETURN_OK)
	{
		printf("%s: Failed to get channel number for radio index %d\n", __func__, index);
		return false;
	}
	operationParam->channel = lval;
	wifi_getRadioAutoChannelEnable(index,&enabled);
	operationParam->autoChannelEnabled = FALSE; //hardcoded for now
	operationParam->csa_beacon_count = 15; // XXX: hardcoded for now
	/* countryCode */
	operationParam->countryCode = wifi_countrycode_IN; // XXX: hardcoded for now
	operationParam->numSecondaryChannels =  0;
	operationParam->DCSEnabled = FALSE;
	operationParam->dtimPeriod = 2;
	operationParam->beaconInterval = 100;
	operationParam->operatingClass = 0; 
	/* UINT basicDataTransmitRates */
	memset(buf, 0, sizeof(buf));
	ret = wifi_getRadioBasicDataTransmitRates(index,buf);
	if (ret != RETURN_OK)
        {
                printf("%s: Failed to get basicData TansmitRates for radio index %d\n", __func__, index);
                return RETURN_ERR;
        }
        rpi_map_str2enum_fromTable(buf, &basicDataTransmitRates, ",", wifi_bitrate_infoTable);
        operationParam->basicDataTransmitRates = basicDataTransmitRates;  
	/* UINT operationalDataTransmitRates */
	memset(buf, 0, sizeof(buf));
	ret = wifi_getRadioOperationalDataTransmitRates(index,buf);
	if (ret != RETURN_OK)
        {
                printf("%s: Failed to get OperationalData TansmitRates for radio index %d\n", __func__, index);
                return RETURN_ERR;
        }
	rpi_map_str2enum_fromTable(buf, &operationalDataTransmitRates, ",", wifi_bitrate_infoTable);
        operationParam->operationalDataTransmitRates = operationalDataTransmitRates;
	operationParam->fragmentationThreshold = 2346;
	operationParam->rtsThreshold = 2347;
	wifi_getRadioTransmitPower(index,&lval);
	operationParam->transmitPower=lval;
	WIFI_ENTRY_EXIT_DEBUG("Exiting %s:%d\n",__func__, __LINE__);
	return RETURN_OK;
}

INT wifi_setRadioOperatingParameters(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
	bool enable;
	char ChannelWidth[32]="";
	enable = operationParam->enable;	
	if( wifi_setRadioEnable(index,enable) != RETURN_OK)
	{
		printf("wifi_setRadioEnable failed..index[%d]\n",index);
		return RETURN_ERR;
	}		
	printf("%s: radioIndex = %d, Enable=%d\n",
			__FUNCTION__, index, enable);
	/* enable/disable auto channel mode */
	enable = operationParam->autoChannelEnabled;
	if( wifi_setRadioAutoChannelEnable(index,enable) != RETURN_OK)
	{
		printf("wifi_setRadioAutoChannelEnable failed..radioIndex[%d]\n",index);
		//return RETURN_ERR;
	}
	printf("%s: radioIndex = %d, AutoChannelEnabled=%d\n",
			__FUNCTION__, index, enable);
	if (operationParam->channelWidth == 1) 
		strcpy(ChannelWidth,"20MHz");
	else if (operationParam->channelWidth == 2) 
		strcpy(ChannelWidth,"40MHz");
	else if (operationParam->channelWidth == 3) 
		strcpy(ChannelWidth,"80MHz");
	else if (operationParam->channelWidth == 4) 
		strcpy(ChannelWidth,"160MHz");
	else if (operationParam->channelWidth == 5) 
		strcpy(ChannelWidth,"8080MHz");
	if( wifi_setRadioOperatingChannelBandwidth(index,ChannelWidth) != RETURN_OK)
	{
		printf("wifi_setRadioOperatingChannelBandwidth failed radio[%d]:%s \n",index,ChannelWidth);
		return RETURN_ERR;
	}	
	printf("%s: radioIndex = %d, operationParam channelWidth=%d \n",
			__FUNCTION__, index, operationParam->channelWidth);
	if( wifi_setRadioChannel(index,operationParam->channel) != RETURN_OK)
	{
		printf("wifi_setRadioChannel failed radioIndex[%d] \n",index);
		return RETURN_ERR;
	}
	wifi_applySSIDSettings(index);
	return RETURN_OK;
}
#endif
