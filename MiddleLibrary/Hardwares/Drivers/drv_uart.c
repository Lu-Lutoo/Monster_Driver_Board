/**
 * @file drv_uart.c
 * @author Lutoo (e19135693@163.com)
 * @brief code for driving uart dma
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "drv_uart.h"

drv_uart_callback app_callback = NULL;

/**
 * @brief enbale uart_it_idle and start dma receive
 * 
 * @param huart UART handle
 * @param pData Pointer to data buffer
 * @param Size Amount of data elements
 * 
 */
void DRV_UART_Init(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UARTEx_ReceiveToIdle_DMA(huart, pData, Size);
}

/**
 * @brief 
 * 
 * @param p Pointer to application uart callback function
 */
void DRV_UART_CallbackRegister(drv_uart_callback p)
{
  app_callback = p;
}


/**
  * @brief  Reception Event Callback (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(app_callback == NULL)
  {
    /* ERROR */
    //! 暂时不写错误处理
    return;
  }
  if(huart->ReceptionType == HAL_UART_RECEPTION_STANDARD)
  {
    /* 空闲中断 */
    if(Size > huart->RxXferSize/2U)
    {
      /* 已触发dma半接收中断 */
      app_callback(huart, huart->pRxBuffPtr + huart->RxXferSize / 2U, Size - huart->RxXferSize / 2U, DRV_UART_IDLE_TRUE);
    }
    else
    {
      /* 未触发dma半触发中断 */
      app_callback(huart, huart->pRxBuffPtr, Size, DRV_UART_IDLE_TRUE);
    }
    /* restart the uart dma receive */
    HAL_UARTEx_ReceiveToIdle_DMA(huart, huart->pRxBuffPtr, huart->RxXferSize);
  }
  else if(huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
    /* 非空闲中断 */
    if(Size == huart->RxXferSize)
    {
      /* 是dma全接收中断 */
      app_callback(huart, huart->pRxBuffPtr + huart->RxXferSize / 2U, huart->RxXferSize - huart->RxXferSize / 2U, DRV_UART_IDLE_FALSE);
    }
    else
    {
      /* dma半接收中断 */
      app_callback(huart, huart->pRxBuffPtr, Size, DRV_UART_IDLE_FALSE);
    }
  }  
}