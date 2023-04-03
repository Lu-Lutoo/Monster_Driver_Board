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
  uint8_t* temp;
  /* get the rx buff head address according to the data word length*/
  if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
  {
      temp = huart->pRxBuffPtr - Size * 2U;
  }
  else
  {
      temp = huart->pRxBuffPtr - Size;
  }
  /* callback */
  if(app_callback != NULL)
  {
    app_callback(huart, temp, Size);
  }
  else
  {
    /* ERROR */
    //!
  }
  /* restart the uart dma receive */
  HAL_UARTEx_ReceiveToIdle_DMA(huart, temp, huart->RxXferSize);
}