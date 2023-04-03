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
#define UART2_DMA_BUFF_SIZE 16U
#define UART2_RECEIVE_SIZE 256U
/* Private type --------------------------------------------------------------*/
template<uint16_t size>
struct UART_FIFO_struct
{
    uint16_t length;
    uint8_t buff[size];
};

typedef struct UART_FIFO_struct<UART2_RECEIVE_SIZE> UART2_FIFO_struct;

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
    UART2_RX_Queue = xQueueCreate(3, sizeof(UART2_BUFF));

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
    static UART2_FIFO_struct uart2_buff;
    for (;;)
    {
        if(xQueueReceive(UART2_RX_Queue,&uart2_buff,portMAX_DELAY) == pdPASS)
        {
            HAL_UART_Transmit_DMA(&huart2, uart2_buff.buff, uart2_buff.length);
        }
    }
}


void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, DRV_UART_IDLE_STATE state)
{
    BaseType_t xHigherPriorityTaskWoken;
    if (huart->Instance == USART2) {
        if(state == DRV_UART_IDLE_FALSE || Size != 0)
        {
            /* 接收没有停止 */
            memcpy(UART2_BUFF.buff + UART2_BUFF.length, pData, Size);
            UART2_BUFF.length += Size;
        }
        if(state == DRV_UART_IDLE_TRUE)
        {
            xQueueSendFromISR(UART2_RX_Queue, &UART2_BUFF, &xHigherPriorityTaskWoken);
            UART2_BUFF.length = 0;
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}