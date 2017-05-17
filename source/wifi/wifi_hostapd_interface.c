/*
* If not stated otherwise in this file or this component's Licenses.txt file the
* following copyright and licenses apply:
*
* Copyright 2016 RDK Management
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

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include"wifi_hal.h"

#ifdef WIFI_DEBUG
#define wifi_dbg_printf printf
#else
#define wifi_dbg_printf(format,args...) printf("")
#endif


#define wifi_printf printf
#define MAX_APS 2
int wifi_readHostapd(int ap,struct hostap_conf *conf);
int wifi_hostapdWrite(int ap,struct params *params);
int wifi_hostapdRead(int ap,struct params *params,char *output);
struct  hostap_conf conf[MAX_APS];
int wifi_writeHostapd(int ap,struct params *params);
struct  params params;
int wifi_readHostapd_all_aps();
#define STANDALONE_TEST 0
#undef STANDALONE_TEST
#if defined STANDALONE_TEST
int _syscmd(char *cmd, char *retBuf, int retBufSize)
{
    FILE *f;
    char *ptr = retBuf;
    int bufSize=retBufSize, bufbytes=0, readbytes=0;

    if((f = popen(cmd, "r")) == NULL) {
        printf("popen %s error\n", cmd);
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


int main()
{
 
 int  ap=1;
 int i;
 char output[20];
 wifi_readHostapd_all_aps();

 for(i=0;i<MAX_APS;i++)
 {
    wifi_dbg_printf("\ni=%d\n",i);
    wifi_dbg_printf("\nssid[%d]=%s\n",i,conf[i].ssid);
    wifi_dbg_printf("\npassphrase[%d]=%s\n",i,conf[i].passphrase);
 } 
 strcpy(params.name,"ssid");
// strcpy(params.value,"MY_SSID_VALUE");
 wifi_hostapdRead(ap,&params,output);
 strcpy(params.name,"wpa_passphrase");
 ap=1;
// strcpy(params.value,"MY_PASSPHRASE");
 wifi_hostapdRead(ap,&params,output);
 strcpy(params.name,"ssid");
 strcpy(params.value,"ssid");
 wifi_hostapdWrite(1,&params);
}
#endif

int wifi_hostapdRead(int ap,struct params *params,char *output)
{
    char file_name[20];
    char cmd[MAX_CMD_SIZE]={'\0'};
    char buf[MAX_BUF_SIZE]={'\0'};
    char *ch;
    char *pos;
	printf("\n Params Name is %s\n",params->name);
    if(strcmp(params->name, "beaconType") == 0)
    {
        sprintf(cmd,"grep 'AP_SECMODE%d' %s",ap,SEC_FNAME);
        printf("\ncmd=%s\n",cmd);
    }
    else
    {
        sprintf(file_name,"%s%d.conf",HOSTAPD_FNAME,ap);
        sprintf(cmd,"grep '%s=' %s",params->name,file_name);
    }
    if(_syscmd(cmd,buf,sizeof(buf)) == RETURN_ERR)
    {
        wifi_dbg_printf("\nError %d:%s:%s\n",__LINE__,__func__,__FILE__);
        return RETURN_ERR;
    }
    if (buf[0]=='\0')
        return RETURN_ERR;
    pos = buf;
    while(*pos != '\0')
    {
        if (*pos == '\n')
        {
            *pos = '\0';
            break;
        }
        pos++;
    }
    pos = strchr(buf, '=');
    if (pos == NULL)
    {
        wifi_dbg_printf("Line %d: invalid line '%s'",__LINE__, buf);
        return RETURN_ERR;
    }
    *pos = '\0';//capture the value of the string
    pos++;
    strncpy(output,pos,strlen(pos));
}

int wifi_hostapdWrite(int ap,struct params *params)
{
  char cmd[MAX_CMD_SIZE];
  char wpa_val[2];
  char cur_val[127]={'\0'};
  char buf[MAX_BUF_SIZE];
  if(strncmp(params->name,"beaconType",strlen("beaconType")) ==0 )
  {
    wifi_dbg_printf("\nparams name is BeaconType params value is %s \n",params->value);
    if((strcmp(params->value,"WPAand11i")==0))
        strcpy(wpa_val,"3");
    else if((strcmp(params->value,"11i")==0))
        strcpy(wpa_val,"2");
    else if((strcmp(params->value,"WPA")==0))
        strcpy(wpa_val,"1");

    wifi_hostapdRead(ap,params,cur_val);


    if(ap==0)
            strncpy(params->name,"AP_SECMODE0",strlen("AP_SECMODE0"));
    else if(ap==1)
        strncpy(params->name,"AP_SECMODE1",strlen("AP_SECMODE1"));
    else
        {
            wifi_dbg_printf("\n%s %d Invalid AP\n",__func__,__LINE__);
            return RETURN_ERR;
        }
    
    sprintf(cmd,"sed -i 's/%s=%s/%s=%s/g' %s",params->name,cur_val,params->name,params->value,SEC_FNAME);
    wifi_dbg_printf("\n%s cur_val for secfile=%s cmd=%s",__func__,cur_val,cmd);
    _syscmd(cmd,buf,sizeof(buf));

    memset(params->name,'\0',sizeof(params->name));
    memset(params->value,'\0',sizeof(params->value));
    memset(cur_val,'\0',sizeof(cur_val));
    strncpy(params->name,"wpa",strlen("wpa"));
    strncpy(params->value,wpa_val,strlen(wpa_val));

    /* If new security mode value for param "wpa" is '3' then set it to '2'.
       Security mode '3' is supposed to support both WPA-Personal and WPA2-personal
       but it is supporting only WPA-Personal and not to WPA2-Personal for security 
       mode setting '3'.
    */
    if( ('3' == wpa_val[0]) && ( wifi_getApIndexForWiFiBand(band_2_4) == ap) )
    {
         wifi_dbg_printf("\n Current value of param wpa is 3, setting it to 2.\n");
         strcpy(params->value, "2");
    }


	wifi_hostapdRead(ap,params,cur_val);
    sprintf(cmd,"sed -i 's/%s=%s/%s=%s/g' %s%d.conf",params->name,cur_val,params->name,params->value,HOSTAPD_FNAME,ap);
	printf("\ncur_val for wpa=%s wpa_val=%s\ncmd=%s\n",cur_val,wpa_val,cmd);
    _syscmd(cmd,buf,sizeof(buf));
    system("systemctl restart hostapd.service");
    return RETURN_OK;
  }
  
  wifi_hostapdRead(ap,params,cur_val);
  printf("\ncur_value=%s\n",cur_val);
  memset(cmd,'\0',sizeof(cmd));
  sprintf(cmd,"sed -i 's/%s=%s/%s=%s/g' %s%d.conf",params->name,cur_val,params->name,params->value,HOSTAPD_FNAME,ap);
  _syscmd(cmd,buf,sizeof(buf));
  wifi_dbg_printf("\ncmdsss=%s\n",cmd);
  system("systemctl restart hostapd.service");
  return RETURN_OK;
}


#ifdef USE_HOSTAPD_STRUCT

int wifi_readHostapd(int ap,struct hostap_conf *conf)
{
    char *pos;
    FILE *f;
    int count=0;
    int line=0;
    char file_name[20];
    char cmd[CMD_SIZE]={'\0'};
    char buf[BUF_SIZE]={'\0'};
    char *ch;
    sprintf(file_name,"%s%d.conf",HOSTAPD_FNAME,ap);
    wifi_dbg_printf("\ncmd=%s \n",file_name);
    f=fopen(file_name,"r");
    if (f == NULL) 
    {
        wifi_dbg_printf("Could not open configuration file '%s'for reading.",HOSTAPD_FNAME);
        return -1;
    }
    while (fgets(buf, sizeof(buf), f))
    {
        wifi_dbg_printf("\ncount=%d\n",count);
        count++;
        line++;
        /*Ignore comments*/
        if (buf[0] == '#') 
        continue;

        pos = buf;
        while (*pos != '\0') 
         {
              if (*pos == '\n') 
              {
                *pos = '\0';
                 break;
              }
              pos++;
        }
     
        if (buf[0]=='\0')
        continue;
        pos = strchr(buf, '=');
        if (pos == NULL) 
        {
               wifi_dbg_printf("Line %d: invalid line '%s'",__LINE__, buf);
              continue;
        }
        *pos = '\0';//capture the value of the string
        pos++;
        wifi_dbg_printf("\nname=%s value=%s\n",buf,pos);
        hostapd_fill(conf,buf,pos,line);
    }
    printf("ssid=%s",conf->ssid);
}

int read_hostapd_all_aps()
{
 int i;
 for(i=0;i<MAX_APS;i++)
 {
    wifi_dbg_printf("\ni=%d",i);
    wifi_dbg_printf("\nCalling read_hostapd\n");
    read_hostapd(i,&conf[i]);//fill the structure for both the hostapd configuration files
 }
}
hostapd_fill(struct hostap_conf *conf, char *buf, char *pos, int line)
{
    if(strcmp(buf,"ssid")==0)
    {
        memset(conf->ssid,'\0',sizeof(conf->ssid));
        strncpy(conf->ssid,pos,sizeof(conf->ssid));
    }
    else if(strcmp(buf,"wpa_passphrase")==0)
    {
        int len=strlen(pos);
        wifi_dbg_printf("\npass=%s\n",pos);

        if(len < 8 || len > 63)
        {
             wifi_dbg_printf("Line %d invalid Wpa Passphrase length %d \
                    expected (8..63) ",__LINE__,len);
        wifi_dbg_printf("\n4\n");
        }
        else
        {
            wifi_dbg_printf("\npass=%s\n",pos);
            free(conf->passphrase);
            conf->passphrase=strdup(pos);
        }
    }

}



int wifi_writeHostapd(int ap,struct params *params)
{
    char file_str[1024]={'\0'};
    FILE *f;
    char *pos;
    int line=0;
    int count=0;
    char file_name[20];
    sprintf(file_name,"%s%d.conf",HOSTAPD_FNAME,ap);
    wifi_dbg_printf("\nfile_name=%s\n",file_name);
    wifi_dbg_printf("\n%s\n",file_name);
    f=fopen(file_name,"r");
    if (f == NULL)
       {
           wifi_dbg_printf("Could not open configuration file '%s'for reading.",HOSTAPD_FNAME);
        return 0;
    }
    while (fgets(buf, sizeof(buf), f))
    {
        wifi_dbg_printf("\ncount=%d\n",count);
        count++;line++;
        pos = buf;
        while (*pos != '\0')
        {
            if (*pos == '\n')
            {
                *pos = '\0';
                break;
             }
             pos++;
         } 

         if (buf[0]=='\0')
             continue;
    
        pos = strchr(buf, '=');
          if (pos == NULL)
          {
              wifi_dbg_printf("Line %d: invalid line '%s'",__LINE__, buf);
              continue;
          }
          *pos = '\0';//capture the value of the string
          pos++;
//        wifi_dbg_printf("\n%s\n",buf);
//        wifi_writeHostapd(ap,params,buf,pos,line);
    }
    fclose(f);
    //refresh the conf structure 
    wifi_readHostapd(ap,&conf[ap]);
}
#endif
