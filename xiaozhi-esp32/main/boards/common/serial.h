//#define SERIAL_H_

#ifndef SERIAL_H_
#define SERIAL_H_

#include "driver/uart.h"
#include <functional>

#define UART1_TX_GPIO GPIO_NUM_17
#define UART1_RX_GPIO GPIO_NUM_18
//#define UART1_PORT_NUM UART_NUM_1
#define UART1_PORT_NUM (uart_port_t)1
#define UART_BUF_SIZE       1024

class Serial{
public:
    Serial(int baud_rate=115200);
    ~Serial();
    void SerialTransmit(std::function<void()> callback);
    uint8_t SerialReceive(std::function<uint8_t()> callback);
    // void SerialTransmit();
    // uint8_t SerialReceive();
    // void OnLongPress(std::function<void()> callback);
    // void OnClick(std::function<void()> callback);
    // void OnDoubleClick(std::function<void()> callback);
private:
    // gpio_num_t gpio_num_;
    // button_handle_t button_handle_ = nullptr;
    int baud_rate_;
    std::function<void()> serial_transmit_;
    std::function<uint8_t()> serial_receive_;
    // std::function<void()> on_long_press_;
    // std
    //std::function<void()> on_click_;
    // std::function<void()> on_double_click_;
};

#endif