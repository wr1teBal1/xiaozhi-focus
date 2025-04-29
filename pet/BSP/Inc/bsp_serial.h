#ifndef BSP_SERIAL_H_
#define BSP_SERIAL_H_

#include "stdbool.h"
#include "usart.h"

#define SERIAL_TIMEOUT  1000
#define RX_BUFFER_MAX   100

typedef enum
{
    Serial_Forward  = 0x01U,
    Serial_Backward = 0x02U,
    Serial_Left     = 0x03U,
    Serial_Right    = 0x04U,
    Serial_Squart   = 0x05U,
    Serial_Wander   = 0x06U,
    Serial_Standup  = 0x07U,
    Serial_Stop     = 0x08U
}SERIAL_CmdTypedef;

typedef struct{
    uint16_t size;
    uint8_t buffer[RX_BUFFER_MAX];
} Serial_RxBuffer;

void Serial_Init(uint8_t *rx_data);

HAL_StatusTypeDef Serial_Transmit(uint8_t *pData, uint16_t Size);

#endif
