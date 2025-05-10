#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
//#include "uart_types.h"
#include <driver/gpio.h>
#include <esp_log.h>
#include "serial.h"
//#include <_stdint.h>
//#define uint8_t unsigned char

#define TAG  "Serial"

Serial::Serial(int baud_rate)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_param_config(UART1_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART1_PORT_NUM, UART1_TX_GPIO, UART1_RX_GPIO, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART1_PORT_NUM, UART_BUF_SIZE * 2, UART_BUF_SIZE * 2, 0, NULL, 0));
    ESP_LOGI(TAG, "create"); 
}

Serial::~Serial()
{
    ESP_LOGI(TAG, "delete\r\n");
}

void Serial::SerialTransmit(std::function<void()> callback)
{
    serial_transmit_ = callback;
    serial_transmit_();
}

uint8_t Serial::SerialReceive(std::function<uint8_t()> callback)
{
    serial_receive_ = callback;
    uint8_t data = serial_receive_();
    return data;
}




