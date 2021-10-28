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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utctx/utctx.h"
#include "utctx/utctx_api.h"

#include "platform_hal.h"

#define MAX_BUFFER_SIZE     1024
#define TMP_BUFFER_SIZE     128
#define ONE_KILOBYTE        1024
#define FACTORY_RESET_COUNT_FILE "/nvram/.factory_reset_count"

static int execute(char *command, char *result)
{
    FILE *fp = NULL;
    char output[MAX_BUFFER_SIZE] = {0};
    char *str = NULL,*pos=NULL;

    fp = popen(command, "r");
    if(NULL == fp)
    {
        printf("Failed to run command\n" );
        return RETURN_ERR;
    }

    /* only the first line of the output is of interest */
    fgets(output, sizeof(output)-1, fp);
    if (NULL != (pos=strchr(output, '\n')) ) {
        *pos = '\0';
    }
    str = strstr(output, ":");

    if(NULL != str)
    {
        strcpy(result, (str + 1));
    }
    else
    {
        strcpy(result,output);
    }

    printf("\nresult = %s\n", result);
    pclose(fp);

    return RETURN_OK;
}

static int execute_cmd(char *command, char *result)
{
	FILE *fp = NULL;
	char buf[MAX_BUFFER_SIZE] = {0}, copy_buf[MAX_BUFFER_SIZE] ={0};
	int count = 0;

	fp = popen(command,"r");
	if(fp == NULL)
	{
		return RETURN_ERR;
	}

	if(fgets(buf,sizeof(buf) -1,fp) != NULL)
	{
		for(count=0;buf[count]!='\n';count++)
			copy_buf[count]=buf[count];
		copy_buf[count]='\0';
	}
	strcpy(result,copy_buf);
	pclose(fp);
	return RETURN_OK;
}

/* Note that 0 == RETURN_OK == STATUS_OK    */
/* Note that -1 == RETURN_ERR == STATUS_NOK */

INT platform_hal_GetDeviceConfigStatus(CHAR *pValue) { strcpy(pValue, "Complete"); return RETURN_OK; }

INT platform_hal_GetTelnetEnable(BOOLEAN *pFlag) { *pFlag = FALSE; return RETURN_OK; }
INT platform_hal_SetTelnetEnable(BOOLEAN Flag) { return RETURN_ERR; }
INT platform_hal_GetSSHEnable(BOOLEAN *pFlag)
{
#if 0       	
#ifndef _64BIT_ARCH_SUPPORT_	
    char ssh_access[2] = { 0 };
    UtopiaContext ctx;

    if(NULL==pFlag)
        return RETURN_ERR;

    if (!Utopia_Init(&ctx))
        return RETURN_ERR;

    if (!Utopia_RawGet(&ctx, NULL, "mgmt_wan_sshaccess",
                       ssh_access, sizeof(ssh_access))) {
        Utopia_Free(&ctx, 0);
        return RETURN_ERR;
    }
    *pFlag = atoi(ssh_access);
    Utopia_Free(&ctx, 0);
#endif
#endif    
    return RETURN_OK;
}
INT platform_hal_SetSSHEnable(BOOLEAN Flag)
{
#if 0
#ifndef _64BIT_ARCH_SUPPORT_	
    char ssh_access[2] = { 0 };
    UtopiaContext ctx;
    ssh_access[0] = '0' + ! !Flag;
    if (!Utopia_Init(&ctx))
        return RETURN_ERR;
    if (!Utopia_RawSet(&ctx, NULL, "mgmt_wan_sshaccess", ssh_access)) {
        Utopia_Free(&ctx, 0);
        return RETURN_ERR;
    }
    Utopia_SetEvent(&ctx, Utopia_Event_Firewall_Restart);
    Utopia_Free(&ctx, 1);
#endif    
#endif    
    return RETURN_OK;
}

INT platform_hal_GetSNMPEnable(CHAR* pValue) { return RETURN_ERR; }
INT platform_hal_SetSNMPEnable(CHAR* pValue) { return RETURN_ERR; }
INT platform_hal_GetWebUITimeout(ULONG *pValue) { return RETURN_ERR; }
INT platform_hal_SetWebUITimeout(ULONG value) { return RETURN_ERR; }
INT platform_hal_GetWebAccessLevel(INT userIndex, INT ifIndex, ULONG *pValue) { return RETURN_ERR; }
INT platform_hal_SetWebAccessLevel(INT userIndex, INT ifIndex, ULONG value) { return RETURN_ERR; }

INT platform_hal_PandMDBInit(void)
{
    return RETURN_OK;

}
INT platform_hal_DocsisParamsDBInit(void) { return RETURN_OK; }

INT platform_hal_GetModelName(CHAR* pValue)
{
    char model[TMP_BUFFER_SIZE]= {'\0'};
    int ret = RETURN_ERR;

    if(NULL == pValue)
    {
        return RETURN_ERR;
    }
    ret = execute_cmd("grep 'MODEL_NAME' /etc/device.properties | cut -d '=' -f2", model);
    if(RETURN_OK != ret)
    {
        printf("\nError %s\n", __func__);
    }
    else
    {
        strncpy(pValue, model, strlen(model));
    }

    return ret;
}

INT platform_hal_GetSerialNumber(CHAR* pValue)
{
    char sn[TMP_BUFFER_SIZE] = {'\0'};
    int ret = RETURN_ERR;

    if(NULL == pValue)
    {
        return RETURN_ERR;
    }
    ret = execute("grep 'Serial' /proc/cpuinfo", sn);
    if(RETURN_OK != ret)
    {
        printf("\nError %s\n", __func__);
    }
    else
    {
        strncpy(pValue, sn, strlen(sn));
    }

    return ret;
}

INT platform_hal_GetHardwareVersion(CHAR* pValue)
{
    int ret = RETURN_ERR;
    char hwVer[TMP_BUFFER_SIZE] = {'\0'};
    if(NULL == pValue )
    {
        return RETURN_ERR;
    }

    if(NULL == pValue)
    {
        return RETURN_ERR;
    }
    
	ret = execute("grep 'Revision' /proc/cpuinfo", hwVer);
    if(RETURN_OK != ret)
    {
        printf("\nError %s\n", __func__);
    }

    strcpy(pValue, hwVer);

    return ret;
}

INT platform_hal_GetSoftwareVersion(CHAR* pValue, ULONG maxSize)
{
    if(NULL == pValue )
    {
        return RETURN_ERR;
    }

    strcpy(pValue, "Not Supported");

    return RETURN_OK;
}

INT platform_hal_GetBootloaderVersion(CHAR* pValue, ULONG maxSize)
{
    if(NULL == pValue )
    {
        return RETURN_ERR;
    }

    strcpy(pValue, "Bootloader Version");

    return RETURN_OK;
}

INT platform_hal_GetFirmwareName(CHAR* pValue, ULONG maxSize)
{
    char fn[TMP_BUFFER_SIZE] = {'\0'};
    int ret = RETURN_ERR;

    if(NULL == pValue )
    {
        return RETURN_ERR;
    }
    ret = execute("grep 'imagename' /version.txt", fn);
    if(RETURN_OK != ret)
    {
        printf("\nError %s\n", __func__);
    }
    else
    {
        strncpy(pValue, fn, strlen(fn));
    }

    return ret;
}


INT platform_hal_GetBaseMacAddress(CHAR *pValue) 
{ 
	execute_cmd("ifconfig erouter0 | grep HWaddr | cut -d ' ' -f7",pValue);	
	return RETURN_OK;
}
INT platform_hal_GetHardware(CHAR *pValue)
{
    int ret = RETURN_ERR;
    char hwVer[TMP_BUFFER_SIZE] = {'\0'};

    if(NULL == pValue)
    {
        return RETURN_ERR;
    }

    ret = execute("grep 'Revision' /proc/cpuinfo", hwVer);
    if(RETURN_OK != ret)
    {
        printf("\nError %s\n", __func__);
    }
    else if(strstr(hwVer, "a02082") != NULL)
    {
        strncpy(pValue, "raspberrypi3", strlen("raspberrypi3"));
    }
    else if(strstr(hwVer, "a01041") != NULL)
    {
        strncpy(pValue, "raspberrypi2", strlen("raspberrypi2"));
    }
    else
    {
        strncpy(pValue, "raspberrypi", strlen("raspberrypi"));
    }

    return ret;

}
INT platform_hal_GetTotalMemorySize(ULONG *pulSize)
{
    char totMem[TMP_BUFFER_SIZE] = {'\0'};
    int ret = RETURN_ERR;

    if(NULL == pulSize)
    {
        return RETURN_ERR;
    }

    ret = execute("grep 'MemTotal' /proc/meminfo", totMem);
    if(RETURN_OK != ret)
    {
        printf("\nError %s\n", __func__);
    }
    else
    {
        sscanf(totMem, "%d", pulSize );
     *pulSize = *pulSize/ONE_KILOBYTE;
    }

    return ret;
}

INT platform_hal_GetHardware_MemUsed(CHAR *pValue)
{
    char usedMem[TMP_BUFFER_SIZE]={'\0'};
    int ret = RETURN_ERR;
    long tmp;

    if(NULL == pValue)
    {
        return RETURN_ERR;
    }

    ret = execute("df | grep '/dev' | awk '{print $3}'", usedMem);
    if(RETURN_OK != ret)
    {
        printf("Error: %s", __func__);
    }
    else
    {
        tmp = atoi(usedMem)/ONE_KILOBYTE;
        sprintf(pValue, "%ld",  tmp);
    }

    return ret;
}


INT platform_hal_GetHardware_MemFree(CHAR *pValue)
{
   char freeMem[TMP_BUFFER_SIZE] = {'\0'};
   int ret = RETURN_ERR;
   long tmp;

   if(NULL == pValue)
   {
       return RETURN_ERR;
   }
   ret = execute("df | grep '/dev' | awk '{print $4}'", freeMem);
   if(RETURN_OK != ret)
   {
       printf("Error:%s", __func__);
   }
   else
   {
      tmp = atoi(freeMem)/ONE_KILOBYTE;
      sprintf(pValue, "%ld", tmp);
   }

   return ret;
}

INT platform_hal_GetFreeMemorySize(ULONG *pulSize)
{
    char freeMem[TMP_BUFFER_SIZE] = {'\0'};
    int ret = RETURN_ERR;

    if(NULL == pulSize)
    {
        return RETURN_ERR;
    }

    ret = execute("free| grep 'Mem'| awk '{print $4}'", freeMem);
    if(RETURN_OK != ret)
    {
        printf("Error:%s", __func__);
    }
    else
    {
        *pulSize = atoi(freeMem)/ONE_KILOBYTE;
    }

    return ret;
}

INT platform_hal_GetUsedMemorySize(ULONG *pulSize)
{
    int ret = RETURN_ERR;
    char usedMem[TMP_BUFFER_SIZE] = {'\0'};
    int tmp;

    if(NULL == pulSize)
    {
        return RETURN_ERR;
    }

    ret = execute("free| grep 'Mem'| awk '{print $3}'", usedMem);
    if(RETURN_OK != ret)
    {
        printf("Error: %s", __func__);
    }
    else
    {
        *pulSize = atoi(usedMem)/ONE_KILOBYTE;
    }

    return ret;
}

INT platform_hal_GetFactoryResetCount(ULONG *pulSize)
{

    if(NULL == pulSize)
    {
        return RETURN_ERR;
    }
    FILE *pdbFile = NULL;
    char buf[128]={0};
    pdbFile = fopen(FACTORY_RESET_COUNT_FILE, "r");
    if(pdbFile != NULL)
    {
	 fread(buf,sizeof(buf),1,pdbFile);
	 fclose(pdbFile); 
	 *pulSize = atoi(buf);
    }
    else
    {
         *pulSize = 0;
    }


    return RETURN_OK;
}

INT platform_hal_ClearResetCount(BOOLEAN bFlag)
{
    return RETURN_OK;
}

INT platform_hal_getTimeOffSet(CHAR *pValue)
{
    return RETURN_OK;
}

INT platform_hal_SetDeviceCodeImageTimeout(INT seconds)
{
    return RETURN_OK;
}

INT platform_hal_SetDeviceCodeImageValid(BOOLEAN flag)
{
    return RETURN_OK;
}

INT platform_hal_getCMTSMac(CHAR *pValue)
{ 
	 if (pValue == NULL)
	 {
	     return RETURN_ERR;
	 }
	strcpy(pValue,"00:00:00:00:00:00");
	return RETURN_OK; 
}

//temperature and fan control
INT platform_hal_GetChipTemperature(UINT chipIndex, ULONG *pTempValue) {  //chipIndex:0 for main CPU, 1 for wifi chip.  TempValue is in degrees Celcius 
	if(chipIndex==0) 
		*pTempValue=40;
	else if (chipIndex==0) 
		*pTempValue=41;
	else
		*pTempValue=0;
	return RETURN_OK;
}

INT platform_hal_GetFanSpeed(ULONG *pSpeedValue) {  //SpeedValue is in RPMs 
	*pSpeedValue=3600;
	return RETURN_OK; 
}

INT platform_hal_SetFanSpeed(ULONG SpeeddInRpms) {
	//set the fan speed
	return RETURN_OK; 
}

/* platform_hal_SetSNMPOnboardRebootEnable() function */
/**
* @description : Set SNMP Onboard Reboot Enable value
*                to allow or ignore SNMP reboot
* @param IN    : pValue - SNMP Onboard Reboot Enable value
                 ("disable" or "enable")
*
* @return      : The status of the operation
* @retval      : RETURN_OK if successful
* @retval      : RETURN_ERR if any error is detected
*/
INT platform_hal_SetSNMPOnboardRebootEnable(CHAR* pValue)
{
	return RETURN_OK;
}

INT platform_hal_GetRouterRegion(CHAR* pValue)
{
    return RETURN_OK;
}

/* Utility apis to return common parameters from firewall_lib.c */
char *get_current_wan_ifname()
{
    return "0";
}

INT platform_hal_GetDhcpv4_Options ( dhcp_opt_list ** req_opt_list, dhcp_opt_list ** send_opt_list)
{
    if ((req_opt_list == NULL) || (send_opt_list == NULL))
    {
        return RETURN_ERR;
    }
    return RETURN_OK;
}

INT platform_hal_GetDhcpv6_Options ( dhcp_opt_list ** req_opt_list, dhcp_opt_list ** send_opt_list)
{
    if (req_opt_list == NULL)
    {
        return RETURN_ERR;
    }
    return RETURN_OK;
}

