#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"
#include "driver/uart.h"
#include <driver/gpio.h>
#include "driver/ledc.h"
#include <string.h>
#include <esp_log.h>
#include "serial.h"

#define TAG "Movement"

namespace iot {

// 这里仅定义 Lamp 的属性和方法，不包含具体的实现
class Movement : public Thing {
private:
    void InitializeMovement()
    {
        movement_ = "静止";
        // uart_config_t uart_config = {
        //     .baud_rate = 9600,
        //     .data_bits = UART_DATA_8_BITS,
        //     .parity = UART_PARITY_DISABLE,
        //     .stop_bits = UART_STOP_BITS_1,
        //     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        //     .source_clk = UART_SCLK_DEFAULT,
        // };
        // ESP_ERROR_CHECK(uart_param_config(UART1_PORT_NUM, &uart_config));
        // ESP_ERROR_CHECK(uart_set_pin(UART1_PORT_NUM, UART1_TX_GPIO, UART1_RX_GPIO, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        // ESP_ERROR_CHECK(uart_driver_install(UART1_PORT_NUM, UART_BUF_SIZE * 2, UART_BUF_SIZE * 2, 0, NULL, 0));
    }

    Serial *serial_ = Serial::GetInstance();
    //Serial serial_;
    std::string movement_;
    //uint8_t move_byte;

public:
    Movement() : Thing("Movement", "你可以控制你的动作，包括前进，后退，左转，右转，自由行走，蹲下，停止"){
        InitializeMovement();
        properties_.AddStringProperty("movement", "获取当前的动作状态", [this]() -> std::string {
            // 这里可以添加获取当前动作的逻辑
 //           auto backlight = Board::GetInstance().GetBacklight();
            return movement_;
        });
        // 定义设备可以被远程执行的指令，包括前进，后退，左转，右转，自由行走，蹲下，停止;"为前进，后退，左转，右转，自由行走，蹲下，停止的一个值"
        methods_.AddMethod("SetMovement", "设置你的动作", ParameterList({
            Parameter("movement", "动作有:forward(前进),backward(后退),left(左转),right(右转),stop(停止),squat(蹲下)", kValueTypeString, true)
        }), [this](const ParameterList& parameters) {
            std::string movement = static_cast<std::string>(parameters["movement"].string());            
            //ControlMovement(movement);
            
            //uint8_t data=0x07;
            //uart_write_bytes(UART1_PORT_NUM, &data, 1);
            ESP_LOGI(TAG, "get");
            serial_->SerialTransmit([this, &movement]()
            {
                uint8_t move_byte;
                if(this->movement_ == movement)
                {
                    ESP_LOGI(TAG, "same");
                    return;  
                }                        
                if(movement == "停止" || movement == "stop")
                {
                    move_byte = 0x00;
                    ESP_LOGI(TAG, "stop");
                }
                else if(movement == "前进" || movement == "forward")
                {
                    move_byte = 0x01;
                    ESP_LOGI(TAG, "forward");
                }
                else if(movement == "后退" || movement == "backward")
                {
                    move_byte = 0x02;
                    ESP_LOGI(TAG, "backward");
                }
                else if(movement == "左转" || movement == "left")
                {
                    move_byte = 0x03;
                    ESP_LOGI(TAG, "left");

                }
                else if(movement == "右转" || movement == "right")
                {
                    move_byte = 0x04;
                    ESP_LOGI(TAG, "right");
                }
                else if(movement == "自由行走" || movement == "wander")
                {
                    move_byte = 0x05;
                    ESP_LOGI(TAG, "wander");
                }
                else if(movement == "蹲下" || movement == "squat")
                {
                    move_byte = 0x06;
                    ESP_LOGI(TAG, "squat");
                }                
                uart_write_bytes(UART1_PORT_NUM, &move_byte, 1);
                this->movement_ = movement; });
                ESP_LOGI(TAG, "get");
        });
};
}; // namespace iot
}

DECLARE_THING(Movement);