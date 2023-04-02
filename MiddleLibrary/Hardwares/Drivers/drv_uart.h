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

/*!< Function pointer on application uart callback */
typedef void (*drv_uart_callback)(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

void DRV_UART_Init(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, drv_uart_callback p);


#ifdef __cplusplus
}
#endif