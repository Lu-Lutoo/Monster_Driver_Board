/**
 * @file drv_uart.h
 * @author Lutoo (e19135693@163.com)
 * @brief code for driving uart dma
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef STM32G431xx
#include "stm32g4xx_hal.h"
#endif

typedef enum
{
    DRV_UART_IDLE_FALSE = 0x00U,
    DRV_UART_IDLE_TRUE  = 0x01U
} DRV_UART_IDLE_STATE;

/*!< Function pointer on application uart callback */
typedef void (*drv_uart_callback)(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, DRV_UART_IDLE_STATE state);

void DRV_UART_Init(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void DRV_UART_CallbackRegister(drv_uart_callback p);

#ifdef __cplusplus
}
#endif