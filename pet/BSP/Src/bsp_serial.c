#include <stdint.h>
#include "usart.h"
#include "stdio.h"
#include "bsp_serial.h"
#include "stdbool.h"

bool rxflag;
Serial_RxBuffer serial_rxbuffer;

int fputc(int ch, FILE *file)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 1000);
	return ch;
}

void Serial_Init(uint8_t* rx_data)
{
    // HAL_UARTEx_ReceiveToIdle_IT(&huart1, serial_rxbuffer.buffer, RX_BUFFER_MAX);
    HAL_UART_Receive_IT(&huart1, rx_data, 1);
}

HAL_StatusTypeDef Serial_Transmit(uint8_t* pData, uint16_t Size)
{
    return HAL_UART_Transmit(&huart1, pData, Size, SERIAL_TIMEOUT);
}
