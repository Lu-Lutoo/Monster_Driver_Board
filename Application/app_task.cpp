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
#include <Hardwares/Device/AT_Bluetooth.h>
#include <Middlewares/beitong_remotecontrl.h>
/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* Private macros ------------------------------------------------------------*/
#define STACK_SIZE_64 64U
#define STACK_SIZE_128 128U
#define STACK_SIZE_256 256U


#define UART1_DMA_BUFF_SIZE 2048U
#define UART2_DMA_BUFF_SIZE 64U //!发送数据超过dma接收长度就会出问题
#define UART1_RECEIVE_SIZE 2048U
#define UART2_RECEIVE_SIZE 64U

#define AT_DEVICE_NAME "BEITONG"
/* Private type --------------------------------------------------------------*/
template<uint16_t size>
struct UART_FIFO_struct
{
    uint16_t length;
    uint8_t buff[size];
};

typedef struct UART_FIFO_struct<UART1_RECEIVE_SIZE> UART1_FIFO_struct;
typedef struct UART_FIFO_struct<UART2_RECEIVE_SIZE> UART2_FIFO_struct;

typedef struct
{
    uint16_t length;
    uint8_t *pData;
    DRV_UART_IDLE_STATE state;
} UART_Queue_Uint;

/* Private variables --------------------------------------------------------------*/
BT_Controller_Classdef Remote_Controller;

uint8_t UART1_DMA_BUFF[UART1_DMA_BUFF_SIZE] = {0};
uint8_t UART2_DMA_BUFF[UART2_DMA_BUFF_SIZE] = {0};

UART1_FIFO_struct UART1_BUFF = {0};
UART2_FIFO_struct UART2_BUFF = {0};

uint8_t test_buff[19] = "we have received\n\r";

QueueHandle_t UART1_RX_Queue;
QueueHandle_t UART2_RX_Queue;

TaskHandle_t TaskUart1Rxhandle;
TaskHandle_t TaskUart2Rxhandle;
TaskHandle_t TaskATBLEhandle;
TaskHandle_t TestHandle;
/* Private function prototypes -----------------------------------------------*/
void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, DRV_UART_IDLE_STATE state);
void uart1_receive_task(void *arg);
void uart2_receive_task(void *arg);
void at_blue_task(void *arg);
void test_task(void *arg);

void AT_transmit(uint8_t *pData, uint16_t Size);
/* function prototypes -------------------------------------------------------*/


void User_Hardware_Init()
{
    DRV_UART_CallbackRegister(uart_dma_rx_callback);
    DRV_UART_Init(&huart1, UART1_DMA_BUFF, UART1_DMA_BUFF_SIZE);
    DRV_UART_Init(&huart2, UART2_DMA_BUFF, UART2_DMA_BUFF_SIZE);

    AT_TransmitRegister(AT_transmit);
}

void Application_Task_Init()
{
    UART1_RX_Queue = xQueueCreate(2, sizeof(UART_Queue_Uint));
    UART2_RX_Queue = xQueueCreate(2, sizeof(UART_Queue_Uint)); // 引用队列

    xTaskCreate(uart1_receive_task, "uart1_rx", STACK_SIZE_64, NULL, 7, &TaskUart1Rxhandle);
    xTaskCreate(uart2_receive_task, "uart2_rx", STACK_SIZE_64, NULL, 7, &TaskUart2Rxhandle);
    xTaskCreate(at_blue_task, "AT", STACK_SIZE_64, NULL, 6, &TaskATBLEhandle);
    //xTaskCreate(test_task, "test", STACK_SIZE_64, NULL, 2, &TestHandle);
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

void at_blue_task(void *arg)
{
    for (;;)
    {
        /* 当未连接且未获取设备码时 */
				if(AT_getconnnectstate() == AT_CONNECT_FAIL && AT_getdevicestate() == AT_GET_DEVICE_FAIL)
				{
            AT_blescan();
            vTaskDelay(5000);
        }
        else if(AT_getconnnectstate() == AT_CONNECT_FAIL && AT_getdevicestate() == AT_GET_DEVICE_TRUE)
        {
            AT_bleconnect();
            vTaskDelay(1000);
        }
        else{
            vTaskDelay(100);
        }
        
    }
}

void uart1_receive_task(void* arg)
{
    static UART_Queue_Uint uint1;
    for (;;)
    {
        if(xQueueReceive(UART1_RX_Queue,&uint1,portMAX_DELAY) == pdPASS)
        {
            if(uint1.length != 0)
            {
                memcpy(UART1_BUFF.buff + UART1_BUFF.length, uint1.pData, uint1.length);
                UART1_BUFF.length += uint1.length;
            }
            if(uint1.state == DRV_UART_IDLE_TRUE)
            {
                /* 一段数据接收完了 */
                //HAL_UART_Transmit_DMA(&huart2, UART1_BUFF.buff, UART1_BUFF.length); //* 发送给串口2
								
                switch (AT_getstate())
                {
                case AT_BLESCAN:
                    AT_Find_Device((char*)UART1_BUFF.buff, (char*)AT_DEVICE_NAME);
                    break;
                case AT_BLECONNECT:
                    if(AT_isData((char*)UART1_BUFF.buff) ==0)
                    {
                        Remote_Controller.Update(UART1_BUFF.buff, UART1_BUFF.length);
                    }
                    break;
                default:
                    break;
                }
								
                /* 缓存区清零 */
                UART1_BUFF.length = 0;
            }
        }
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
                HAL_UART_Transmit_DMA(&huart1, UART2_BUFF.buff, UART2_BUFF.length); //* 发给串口1
                /* 缓存区清零 */
                UART2_BUFF.length = 0;
            }
        }
    }
}


void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, DRV_UART_IDLE_STATE state)
{
    static UART_Queue_Uint uint;
    BaseType_t xHigherPriorityTaskWoken;

    uint.length = Size;
    uint.pData  = pData;
    uint.state  = state;
    if(huart->Instance == USART2)
    {
        xQueueSendFromISR(UART2_RX_Queue, &uint, &xHigherPriorityTaskWoken);
    }
    else if(huart->Instance == USART1)
    {
        xQueueSendFromISR(UART1_RX_Queue, &uint, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void AT_transmit(uint8_t *pData, uint16_t Size)
{
    HAL_UART_Transmit_DMA(&huart1, pData, Size);
    HAL_UART_Transmit_DMA(&huart2, pData, Size); //同步发给串口2在上位机看
}