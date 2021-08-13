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
   Copyright [2017] [Technicolor, Inc.]

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

#include "fwupgrade_hal.h"
#define HTTP_DWNLD_CONFIG_FILE      "/tmp/httpDwnld.conf"
#define HTTP_DWNLD_IF_FILE          "/tmp/httpDwnldIf.conf"

#define REBOOT_REASON_SW_UPGRADE    "Software_upgrade"

static int gDwdInProgressFlag = 0; /* flag to set dload inprogress */

static INT fwupgrade_hal_util_get_syscmd_output( char *pCmd, char *pOutput, int iOutputSize );

/* * fwupgrade_hal_util_get_syscmd_output() */
static INT fwupgrade_hal_util_get_syscmd_output( char *pCmd, char *pOutput, int iOutputSize )
{
	FILE   *FilePtr            = NULL;
	char   bufContent[ 256 ]  = { 0 };

	if ( ( NULL == pCmd ) || ( NULL == pOutput ) || ( 0 == iOutputSize ) )
	{
		return RETURN_ERR;
	}

	FilePtr = popen( pCmd, "r" );

	if ( FilePtr )
	{
		char *pos;

		fgets( bufContent, 256, FilePtr );
		fclose( FilePtr );
		FilePtr = NULL;

		// Remove line \n charecter from string
		if ( ( pos = strchr( bufContent, '\n' ) ) != NULL )
			*pos = '\0';

		snprintf( pOutput, iOutputSize, "%s", bufContent );
	}

	return RETURN_ERR;
}


/*
    download the image from httpserver and store in /tmp
 */
static INT download_image_from_server(char *httpUrl, char* fileName)
{
	// TBD should have been dynamically allocated
	char cmd[1400] = {0};
	char res[16] = {0};
	FILE* fp = NULL;
	INT ret = 0;

	gDwdInProgressFlag = 1;
	fprintf(stderr,"%s: Curl Command: curl -fgLo /firmware/imagedwld/%s %s;\n", __func__, fileName, httpUrl);

	// TBD need to append entire args from input except the local download location
	snprintf(cmd, sizeof(cmd),
			"rm -rf /tmp/xconf /firmware;  mkdir -p /tmp/xconf /firmware/imagedwld; \
			curl -fgLo /firmware/imagedwld/%s %s; \
			echo $? > /tmp/xconf/dload_status",
			fileName,httpUrl);
	system(cmd);

	gDwdInProgressFlag = 0;
	// is download successful
	fp = fopen("/tmp/xconf/dload_status", "r");
	if(NULL == fp)
	{
		printf("dload_status : file open error!");
		return RETURN_ERR;
	}

	fgets(res, sizeof(res)-1, fp);
	fclose(fp);
	ret = atoi(res);
	if(0 != ret)
	{
		printf("download from remote server failed!\n");
		return RETURN_ERR;
	}

	// OK, image download at /firmware/imagedwld/*.wic
	fprintf(stderr,"### Debug ### Image download successful at /firmware/imagedwld/%s \n", fileName);

	return RETURN_OK;
}

/* FW Download HAL API Prototype */
/* fwupgrade_hal_set_download_url  - 1 */
/* Description: Set Download Settings
Parameters : char* pUrl;
Parameters : char* pfilename;

@return the status of the operation
@retval RETURN_OK if successful.
@retval RETURN_ERR if any Downloading is in process or Url string is invalided.
*/
INT fwupgrade_hal_set_download_url (char* pUrl, char* pfilename)
{
	fprintf(stderr,"Entering %s \n",__FUNCTION__);
	if ((pUrl == NULL) || (pfilename==NULL))
	{
		return RETURN_ERR;
	}
	else
	{
		FILE* fp;
		char httpUrl[1024] = {0};
		char fileName[256] = {0};
		int ret_status = 0;

		/* To Get the previous URL if any and compare with new one */
		ret_status = fwupgrade_hal_get_download_url(httpUrl, fileName);

		if(ret_status == RETURN_OK)
		{
			if ((strcmp(httpUrl, pUrl) == 0) && (strcmp(fileName, pfilename) == 0))
			{
				fprintf(stderr,"HTTP URL and file name is same as previous! \n");
			}
			else
			{
				fprintf(stderr,"HTTP URL or filename Changed! \n");
				system("rm /tmp/xconf/dload_status");
			}
		}
		else
		{
			system("rm /tmp/xconf/dload_status");
		}
		fp = fopen(HTTP_DWNLD_CONFIG_FILE, "w");
		if(fp == NULL)
		{
			return RETURN_ERR;
		}
		fprintf(fp, "%s\n%s\n", pUrl, pfilename);
		fclose(fp);
		fprintf(stderr,"%s Stored HTTP download URL and filename to %s file\n", __func__, HTTP_DWNLD_CONFIG_FILE);

		return RETURN_OK;
	}
}


/* fwupgrade_hal_get_download_Url: */
/* Description: Get FW Download Url
Parameters : char* pUrl
Parameters : char* pfilename;
@return the status of the operation.
@retval RETURN_OK if successful.
@retval RETURN_ERR if http url string is empty.
*/
INT fwupgrade_hal_get_download_url (char *pUrl, char* pfilename)
{
	if ((pUrl == NULL) || (pfilename==NULL))
	{
		return RETURN_ERR;
	}
	else
	{
		FILE* fp;
		char* fc;

		fprintf(stderr,"Entering %s\n", __func__);

		fp = fopen(HTTP_DWNLD_CONFIG_FILE, "r");
		if(fp == NULL)
		{
			return RETURN_ERR;
		}
		fc = pUrl;
		while((*fc = (char)fgetc(fp)) != '\n')
		{
			++fc;
		}
		*fc = '\0';
		fc = pfilename;
		while((*fc = (char)fgetc(fp)) != '\n')
		{
			++fc;
		}
		*fc = '\0';
		fprintf(stderr,"%s pfilename: %s\n", __func__, pfilename);
		fclose(fp);

		return RETURN_OK;
	}
}

/* interface=0 for wan0, interface=1 for erouter0 */
INT fwupgrade_hal_set_download_interface(unsigned int interface)
{
	fprintf(stderr,"Entering %s\n", __func__);
	FILE *fp = NULL;	
	if( interface > 1 )
	{
		return RETURN_ERR;
	}
	// Save the interface numerical value to the config file
	fp = fopen(HTTP_DWNLD_IF_FILE, "w");
	if(fp == NULL)
	{
		return RETURN_ERR;
	}
	fprintf(fp, "%d\n", interface);
	fclose(fp);
	return RETURN_OK;
}


/* interface=0 for wan0, interface=1 for erouter0 */
INT fwupgrade_hal_get_download_interface(unsigned int* pinterface)
{
	if (pinterface == NULL)
	{
		return RETURN_ERR;
	}
	else
	{
		FILE *fp = NULL;
		char ifNum;

		fp = fopen(HTTP_DWNLD_IF_FILE, "r");
		if(fp == NULL)
		{
			return RETURN_ERR;
		}
		ifNum = fgetc(fp);
		*pinterface = atoi(&ifNum);
		fprintf(stderr,"%s Download interface numerical value: %d\n", __func__, *pinterface);
		fclose(fp);
		return RETURN_OK;
	}
}


/* fwupgrade_hal_download */
/**
Description: Start FW Download
Parameters: <None>
@return the status of the operation.
@retval RETURN_OK if successful.
@retval RETURN_ERR if any Downloading is in process.

*/
INT fwupgrade_hal_download ()
{
    fprintf(stderr,"Entering %s\n", __func__);
    struct hostent *host;
    struct in_addr **addr_list;
    char hostname[1024] = {0};
    char fullhostname[256] = {0};
    char *pstr;
    int i = 0;
    char dlHttpUrl[1024] = {0};
    char dlFilename[256] = {0};
    unsigned char ipAddrInt[4] = {0};
    char cmd[512] = {0};

    if( fwupgrade_hal_get_download_url(dlHttpUrl, dlFilename) != RETURN_OK)
    {
        return RETURN_ERR;
    }

    if( strstr(dlHttpUrl, "http://") == NULL && strstr(dlHttpUrl, "https://") == NULL && strstr(dlHttpUrl, "www.") == NULL )
    {
        return 400;
    }

    if((pstr = strstr(dlHttpUrl, "http://")))
    {
        pstr += strlen("http://");
        strcpy(fullhostname, "http://");
    }
    else if((pstr = strstr(dlHttpUrl, "https://")))
    {
        pstr += strlen("https://");
        strcpy(fullhostname, "https://");
    }
    else if((pstr = strstr(dlHttpUrl, "www.")))
    {
        pstr += strlen("www.");
        strcpy(fullhostname, "www.");
    }
    
    while( *pstr != '/' && *pstr != '\0' && *pstr != ':' )
    {
        hostname[i++] = *pstr;
        ++pstr;
    }
    hostname[i] = '\0';
    strcat(fullhostname, hostname);

    if ((host = gethostbyname(dlHttpUrl)) == NULL)
    {
        if ((host = gethostbyname(fullhostname)) == NULL)
        {
            if ((host = gethostbyname(hostname)) == NULL)
            {
                fprintf(stderr,"Failed on gethostbyname() call. hostname: %s\n", hostname);
                return 400;
            }
        }
    }

    fprintf(stderr,"host->h_addrtype = %d, %s\n", host->h_addrtype,
            host->h_addrtype == AF_INET ? "AF_INET" : host->h_addrtype == AF_INET6 ? "AF_INET6" : "Unknown");
    addr_list = (struct in_addr **) host->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++)
    {
        printf("addr_list[%d] = %s\n", i, inet_ntoa(*addr_list[i]));
    }

    // Convert the dot-text format IP address to an array of numbers, a strange format used by the s/w download module
    memset(hostname, 0, sizeof(hostname));
    snprintf(hostname, sizeof(hostname), "%s", inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    pstr = hostname;
    for(i=0; i<4; i++)
    {
        ipAddrInt[i] = *pstr - '0';
        ++pstr;
        while(*pstr != '.' && *pstr != '\0')
        {
            ipAddrInt[i] *= 10;
            ipAddrInt[i] += *pstr - '0';
            ++pstr;
        }
        ++pstr;
    }
     fprintf(stderr,"Host IP address: %d.%d.%d.%d\n", ipAddrInt[0], ipAddrInt[1], ipAddrInt[2], ipAddrInt[3]);


    // Download the image to tmp
    if(RETURN_OK != download_image_from_server(dlHttpUrl, dlFilename))
    {
        fprintf(stderr,"failed download the image to CPE\n");
        return RETURN_ERR;
    }
    //flash the image to the device
    system("/lib/rdk/rpi_image_Flasher.sh");
    return RETURN_OK;
}


/* fwupgrade_hal_get_download_status */
/**
Description: Get the FW Download Status
Parameters : <None>
@return the status of the HTTP Download.
?   0 ? Download is not started.
?   Number between 0 to 100: Values of percent of download.
?   200 ? Download is completed and waiting for reboot.
?   400 -  Invalided Http server Url
?   401 -  Cannot connect to Http server
?   402 -  File is not found on Http server
?   403 -  HW_Type_DL_Protection Failure
?   404 -  HW Mask DL Protection Failure
?   405 -  DL Rev Protection Failure
?   406 -  DL Header Protection Failure
?   407 -  DL CVC Failure
?   500 -  General Download Failure
?   */
INT fwupgrade_hal_get_download_status()
{
	fprintf(stderr,"Entering %s\n", __func__);
	FILE* DL_StatusFile = NULL;
	char str[16] = {0};
	int dl_stat = 0;
	DL_StatusFile = fopen("/tmp/xconf/dload_status", "r");
	if(NULL != DL_StatusFile)
	{
		fgets(str, sizeof(str)-1, DL_StatusFile);
		fclose(DL_StatusFile);
		dl_stat = atoi(str);
		if(0 != dl_stat)
		{
			fprintf(stderr,"download from remote server failed!\n");
			return 500;
		}
		return 200;
	}
	if ( gDwdInProgressFlag == 1 )
	{
		return 100;
	}
	return 0;
}

/* fwupgrade_hal_reboot_ready */
/*
Description: Get the Reboot Ready Status
Parameters:
ULONG *pValue- Values of 1 for Ready, 2 for Not Ready
@return the status of the operation.
@retval RETURN_OK if successful.
@retval RETURN_ERR if any error is detected

*/
INT fwupgrade_hal_reboot_ready(ULONG *pValue)
{
    fprintf(stderr,"Entering %s\n", __func__);

    if (pValue == NULL)
    {
        return RETURN_ERR;
    }
    *pValue = 1;
    return RETURN_OK;
}

/* fwupgrade_hal_reboot_now */
/*
Description:  Http Download Reboot Now
Parameters : <None>
@return the status of the reboot operation.
@retval RETURN_OK if successful.
@retval RETURN_ERR if any reboot is in process.
*/
INT fwupgrade_hal_download_reboot_now()
{
	fprintf(stderr,"Entering %s\n", __func__);
	int rebootCount=1,
	    IsNeeds2Configure = 1;
	char cmd[128]={0},
	     acOutput[64] = { 0 };

	system("touch /nvram/reboot_due_to_sw_upgrade");

	//Check whether already reboot-reason configured or not. since this case will avoid overwrite "Forced_Software_upgrade" reason
	if ( ( RETURN_OK == fwupgrade_hal_util_get_syscmd_output("syscfg get X_RDKCENTRAL-COM_LastRebootCounter", acOutput, sizeof(acOutput)) ) &&
			( 0 == strncmp( acOutput, "1", 1 ) ) )
	{
		//No need to configure this
		IsNeeds2Configure = 0;
	}

	//Configure reboot reason if already not configured
	if( 1 == IsNeeds2Configure )
	{
		sprintf(cmd, "syscfg set X_RDKCENTRAL-COM_LastRebootReason %s ",REBOOT_REASON_SW_UPGRADE);
		system(cmd);
		sprintf(cmd, "syscfg set X_RDKCENTRAL-COM_LastRebootCounter %d ",rebootCount);
		system(cmd);
		system("syscfg commit");
	}

        fprintf(stderr,"### reboot now ###\n");
	system("/rdklogger/backupLogs.sh true");

	// reboot the device
	system("reboot");
	return RETURN_OK;
}

/* fwupgrade_hal_update_and_factoryreset */
/*
Description:  Do FW update and Factory reset
Parameters : <None>
@return the status of the operation.
@retval RETURN_OK if successful.
@retval RETURN_ERR if any reboot/Download is in process.
*/
INT fwupgrade_hal_update_and_factoryreset()
{
    fprintf(stderr,"Entering %s\n", __func__);

    // Image Download to temp
    if(RETURN_OK != fwupgrade_hal_download())
    {
        fprintf(stderr,"failed download the image to CPE\n");
        return RETURN_ERR;
    }

    // Will do signature checks , switch banks and reboot
    if(RETURN_OK != fwupgrade_hal_download_reboot_now())
    {
        fprintf(stderr,"failed download_Reboot the CPE\n");
        return RETURN_ERR;
    }

    return RETURN_OK;
}

/*  fwupgrade_hal_download_install: */
/**
* @description: Downloads and upgrades the firmware
* @param None
* @return the status of the Firmware download and upgrade status
* @retval RETURN_OK if successful.
* @retval RETURN_ERR in case of remote server not reachable
*/
INT fwupgrade_hal_download_install(const char *url)
{
    return RETURN_OK;
}
