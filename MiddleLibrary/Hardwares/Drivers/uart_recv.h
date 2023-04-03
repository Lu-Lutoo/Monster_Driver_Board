#ifndef __UART_RECV_H
#define __UART_RECV_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32g4xx_hal.h"


extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

#define UART_RECV       huart2
#define UART_RECV_DMA   hdma_usart2_rx
#define UART_SEND_DMA   hdma_usart2_tx

#define RECV_DMA_SIZE    1024

typedef struct
{
    uint8_t data[RECV_DMA_SIZE];
    uint16_t length;
    uint16_t read_start;
    uint16_t read_stop;
} uart_recv_s;

void start_uart_dma(void);
uint8_t wait_usart_idle(void);
uint8_t is_recv_data(void);
uint16_t get_recv_data(uint8_t *recv_buff);
//void print_log(uint8_t* data, uint16_t length);
#endif

#ifdef __cplusplus
}
#endif