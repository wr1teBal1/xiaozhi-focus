//#define SERIAL_H_

#ifndef SERIAL_H_
#define SERIAL_H_

#include "driver/uart.h"
#include <functional>
#include <mutex>

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
    void SerialReceive(std::function<void()> callback);
    static Serial* GetInstance(){
        static Serial instance;
        return &instance;
    }
    
private:
    int baud_rate_;
    std::function<void()> serial_transmit_;
    std::function<void()> serial_receive_;
    std::mutex mtx_;
};

#endif