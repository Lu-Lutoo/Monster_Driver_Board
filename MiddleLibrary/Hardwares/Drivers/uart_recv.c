#include "uart_recv.h"
#include <string.h>

uart_recv_s uart_recv;
uint8_t uart_recv_buffer[RECV_DMA_SIZE];

uint8_t hf_it_flag = 0;

void start_uart_dma(void)
{
    HAL_UART_Receive_DMA(&UART_RECV,uart_recv_buffer,sizeof(uart_recv_buffer));
}

uint8_t wait_usart_idle(void)
{
    uint8_t ret_val = 0;
    if(__HAL_UART_GET_FLAG(&UART_RECV, UART_FLAG_IDLE) != 0)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&UART_RECV);
        ret_val = 1;
    }
    else
    {
        ret_val = 0;
    }
    return ret_val;
}


uint8_t is_recv_data(void)
{
    uint8_t ret_val = 0;
    
    if((wait_usart_idle() == 1 || hf_it_flag== 1) && (sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA)) != uart_recv.read_start)
    {
        hf_it_flag = 0;
        if(sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA) > uart_recv.read_start)
        {
            // print_log("Once recv!\r\n");
            memcpy(uart_recv.data, &uart_recv_buffer[uart_recv.read_start], sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA) - uart_recv.read_start);
            uart_recv.length = sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA) - uart_recv.read_start;
            // print_log("Once recv suc!\r\n");
        }
        else
        {
            // print_log("Twice recv!\r\n");
            memcpy(uart_recv.data, &uart_recv_buffer[uart_recv.read_start], sizeof(uart_recv_buffer) - uart_recv.read_start);
            memcpy(&uart_recv.data[sizeof(uart_recv_buffer) - uart_recv.read_start], uart_recv_buffer, sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA));

            uart_recv.length = sizeof(uart_recv_buffer) - uart_recv.read_start + sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA);
            // print_log("Twice recv suc!\r\n");
        }
        uart_recv.read_start = sizeof(uart_recv_buffer) - __HAL_DMA_GET_COUNTER(&UART_RECV_DMA);
        // print_log("read_start:%d\r\n",uart_recv.read_start);
        ret_val = 1;
    }
    return ret_val;
}

uint16_t get_recv_data(uint8_t *recv_buff)
{
    memcpy(recv_buff, uart_recv.data, uart_recv.length);
    return uart_recv.length;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    hf_it_flag = 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    hf_it_flag = 1;
}
