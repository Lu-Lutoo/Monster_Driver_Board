/**
 * @file AT_Bluetooth.c
 * @author Lutoo (e19135693@163.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "AT_Bluetooth.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define AT_BUFF_SIZE 128

uint8_t AT_buff[AT_BUFF_SIZE] = {0};
uint8_t device_id   = 0;
AT_Transmit_P transmit     = NULL;
AT_STATE state             = AT;
AT_CONNECT_STATE connect_state = AT_CONNECT_FAIL;
AT_GET_DEVICE_STATE get_device_state = AT_GET_DEVICE_FAIL;

char *temp_p;

/**
 * @brief 
 * 
 * @param pt 
 * @param ps 
 * @return uint8_t Device id,if id equal zero,then the Device dosen't exit
 */
void AT_Find_Device(char* pt,char* ps)
{
    char *p,*p_end;
    int device;
    p = strstr(pt, ps);
    if( p == NULL)
    {
        get_device_state = AT_GET_DEVICE_FAIL;
        return;
    }
    *(p-1) = '\0';
    p = strrchr(pt, ':');
    temp_p = p + 1;
    p_end   = strchr(temp_p, '/');
    *p_end  = '\0';
    device_id  = (uint8_t)atoi(temp_p);
    get_device_state = AT_GET_DEVICE_TRUE;
}

uint8_t AT_CheckOK(char *pt)
{
    char *p;
    p = strstr(pt, "OK");
    if(p == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


AT_STATE AT_getstate()
{
    return state;
}

AT_CONNECT_STATE AT_getconnnectstate()
{
    return connect_state;
}

AT_GET_DEVICE_STATE AT_getdevicestate()
{
    return get_device_state;
}

void AT_TransmitRegister(AT_Transmit_P p)
{
    transmit = p;
}



void AT_()
{
    int length;
    if(transmit == NULL)
    {
        /* ERROR */
        return;
    } 
    length = sprintf((char*)AT_buff, "AT\n\r");
    transmit(AT_buff, length);
}

void AT_blescan()
{
    int length;
    if(transmit == NULL)
    {
        /* ERROR */
        return;
    } 
    length = sprintf((char*)AT_buff, "AT+BLESCAN\n\r");
    state  = AT_BLESCAN;
    transmit(AT_buff, length);
}

void AT_bleconnect()
{
    int length;
    if(transmit == NULL)
    {
        /* ERROR */
        return;
    } 
    length = sprintf((char*)AT_buff, "AT+BLECONNECT=%d\n\r",device_id);
    state  = AT_BLECONNECT;
    transmit(AT_buff, length);
}

void AT_blestate()
{
    int length;
    if(transmit == NULL)
    {
        /* ERROR */
        return;
    } 
    length = sprintf((char*)AT_buff, "AT+BLESTATE\n\r");
    state  = AT_BLESTATE;
    transmit(AT_buff, length);
}

uint8_t AT_isData(char *pt)
{
    char *p;
    p = strstr(pt, "+DATA");
    if(p != NULL)
    {
        connect_state = AT_CONNECT_TRUE;
        return 0;
    }
    p = strstr(pt, "+DISCONNECT");
    if(p != NULL)
    {
        connect_state = AT_CONNECT_FAIL;
        return 1;
    }
    return 2;//不用管这种情况
}