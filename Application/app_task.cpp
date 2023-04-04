/**
 * @file app_task.cpp
 * @author Lutoo (e19135693@163.com)
 * @brief code for the application
 * @version 0.1
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include <app_task.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <string.h>
#include <portable.h>

#include <Hardwares/Drivers/drv_uart.h>
#include <Hardwares/Drivers/uart_recv.h>
/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* Private macros ------------------------------------------------------------*/
#define STACK_SIZE_64 64U
#define STACK_SIZE_128 128U
#define STACK_SIZE_256 256U


#define UART1_DMA_BUFF_SIZE 32U
#define UART2_DMA_BUFF_SIZE 1028U //!发送数据超过dma接收长度就会出问题
#define UART2_RECEIVE_SIZE 1028U
/* Private type --------------------------------------------------------------*/
template<uint16_t size>
struct UART_FIFO_struct
{
    uint16_t length;
    uint8_t buff[size];
};

typedef struct UART_FIFO_struct<UART2_RECEIVE_SIZE> UART2_FIFO_struct;

typedef struct
{
    uint16_t length;
    uint8_t *pData;
    DRV_UART_IDLE_STATE state;
} UART_Queue_Uint;

/* Private variables --------------------------------------------------------------*/
uint8_t UART1_DMA_BUFF[UART1_DMA_BUFF_SIZE] = {0};
uint8_t UART2_DMA_BUFF[UART2_DMA_BUFF_SIZE] = {0};

UART2_FIFO_struct UART2_BUFF = {0};

uint8_t test_buff[7]                = "dddddd";

QueueHandle_t UART1_RX_Queue;
QueueHandle_t UART2_RX_Queue;

TaskHandle_t TaskUart2Rxhandle;
TaskHandle_t TestHandle;
/* Private function prototypes -----------------------------------------------*/
void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, DRV_UART_IDLE_STATE state);
void uart2_receive_task(void *arg);
void test_task(void *arg);
/* function prototypes -------------------------------------------------------*/


void User_Hardware_Init()
{
    DRV_UART_CallbackRegister(uart_dma_rx_callback);
    //DRV_UART_Init(&huart1, UART1_BUFF, UART1_BUFF_SIZE);
    DRV_UART_Init(&huart2, UART2_DMA_BUFF, UART2_DMA_BUFF_SIZE);
	//start_uart_dma();
}

void Application_Task_Init()
{
    UART2_RX_Queue = xQueueCreate(8, sizeof(UART_Queue_Uint)); // 引用队列

    xTaskCreate(uart2_receive_task, "uart2_rx", STACK_SIZE_128, NULL, 8, &TaskUart2Rxhandle);
    xTaskCreate(test_task, "test", STACK_SIZE_128, NULL, 2, &TestHandle);
}

void test_task(void* arg)
{
    for (;;)
    {
        /*
        if(is_recv_data() == 1)
        {
            get_recv_data(UART2_BUFF);
            HAL_UART_Transmit_DMA(&huart2, UART2_BUFF, 10);
        }
        */
        vTaskDelay(100);
    }
}


void uart2_receive_task(void* arg)
{
    static UART_Queue_Uint uint2;
    for (;;)
    {
        if(xQueueReceive(UART2_RX_Queue,&uint2,portMAX_DELAY) == pdPASS)
        {
            if(uint2.length != 0)
            {
                memcpy(UART2_BUFF.buff + UART2_BUFF.length, uint2.pData, uint2.length);
                UART2_BUFF.length += uint2.length;
            }
            if(uint2.state == DRV_UART_IDLE_TRUE)
            {
                /* 一段数据接收完了 */
                HAL_UART_Transmit_DMA(&huart2, UART2_BUFF.buff, UART2_BUFF.length);
                /* 缓存区清零 */
                UART2_BUFF.length = 0;
            }
        }
    }
}


void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, DRV_UART_IDLE_STATE state)
{
    static UART_Queue_Uint uint2;
    BaseType_t xHigherPriorityTaskWoken;
    if (huart->Instance == USART2)
    {
        uint2.length = Size;
        uint2.pData  = pData;
        uint2.state  = state;
        xQueueSendFromISR(UART2_RX_Queue, &uint2, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}