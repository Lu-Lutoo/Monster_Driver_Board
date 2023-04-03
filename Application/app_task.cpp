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
#include <Hardwares/Drivers/drv_uart.h>
#include <Hardwares/Drivers/uart_recv.h>
/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* Private macros ------------------------------------------------------------*/
#define STACK_SIZE_64 64U
#define STACK_SIZE_128 128U
#define STACK_SIZE_256 256U


#define UART1_BUFF_SIZE 32U
#define UART2_BUFF_SIZE 32U
/* Private type --------------------------------------------------------------*/
typedef struct 
{
    uint8_t *pData;
    uint16_t Size;
}APP_UART_Unit;


/* Private variables --------------------------------------------------------------*/
uint8_t UART1_BUFF[UART1_BUFF_SIZE] = {0};
uint8_t UART2_BUFF[UART2_BUFF_SIZE] = {0};
uint8_t test_buff[7]                = "dddddd";

QueueHandle_t UART1_RX_Queue;
QueueHandle_t UART2_RX_Queue;

TaskHandle_t TaskUart2Rxhandle;
TaskHandle_t TestHandle;
/* Private function prototypes -----------------------------------------------*/
void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void uart2_receive_task(void *arg);
void test_task(void *arg);
/* function prototypes -------------------------------------------------------*/


void User_Hardware_Init()
{
    //DRV_UART_CallbackRegister(uart_dma_rx_callback);
    //DRV_UART_Init(&huart1, UART1_BUFF, UART1_BUFF_SIZE);
    //DRV_UART_Init(&huart2, UART2_BUFF, UART2_BUFF_SIZE);
	start_uart_dma();
}

void Application_Task_Init()
{
   // UART1_RX_Queue = xQueueCreate(5, sizeof(APP_UART_Unit));
    //UART2_RX_Queue = xQueueCreate(5, sizeof(APP_UART_Unit));

    //xTaskCreate(uart2_receive_task, "uart2_rx", STACK_SIZE_128, NULL, 8, &TaskUart2Rxhandle);
    xTaskCreate(test_task, "test", STACK_SIZE_128, NULL, 2, &TestHandle);
}

void test_task(void* arg)
{
    for (;;)
    {
        if(is_recv_data() == 1)
        {
            get_recv_data(UART2_BUFF);
            HAL_UART_Transmit_DMA(&huart2, UART2_BUFF, 10);
        }
        vTaskDelay(100);
    }
}


void uart2_receive_task(void* arg)
{
    static APP_UART_Unit uart2_unit;
    for (;;)
    {
        if(xQueueReceive(UART2_RX_Queue,&uart2_unit,portMAX_DELAY) == pdPASS)
        {
            HAL_UART_Transmit_DMA(&huart2, uart2_unit.pData, uart2_unit.Size);
        }
    }
}


void uart_dma_rx_callback(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    //! 很糟糕没有实现多缓冲
    APP_UART_Unit UART_Uint;
    BaseType_t xHigherPriorityTaskWoken;
    UART_Uint.pData = pData;
    UART_Uint.Size  = Size;
    if (huart->Instance == USART2) {
        //xQueueSendFromISR(UART2_RX_Queue, &UART_Uint, &xHigherPriorityTaskWoken);
			HAL_UART_Transmit_DMA(&huart2, pData, Size);
    }
}