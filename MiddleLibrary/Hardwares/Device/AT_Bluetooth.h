/**
 * @file AT_Bluetooth.h
 * @author Lutoo (e19135693@163.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef void (*AT_Transmit_P)(uint8_t *pData, uint16_t Size);

typedef enum
{
    AT,
    AT_BLESCAN,
    AT_BLECONNECT,
    AT_BLESTATE
} AT_STATE;

typedef enum
{
    AT_CONNECT_FAIL,
    AT_CONNECT_TRUE
} AT_CONNECT_STATE;

typedef enum
{
    AT_GET_DEVICE_FAIL,
    AT_GET_DEVICE_TRUE
} AT_GET_DEVICE_STATE;

void AT_Find_Device(char *pt, char *ps);
uint8_t AT_CheckOK(char *pt);
AT_STATE AT_getstate(void);
AT_CONNECT_STATE AT_getconnnectstate(void);
AT_GET_DEVICE_STATE AT_getdevicestate(void);
void AT_TransmitRegister(AT_Transmit_P p); 
void AT_(void);
void AT_blescan(void);  // state = AT_BLESCAN; Need AT_Find_Device;
void AT_bleconnect(void); // state = AT_BLECONNECT;Nees AT_isData
void AT_blestate(void);             // state = AT_BLESTATE; Need
uint8_t AT_isData(char *pt);

#ifdef __cplusplus
}
#endif








