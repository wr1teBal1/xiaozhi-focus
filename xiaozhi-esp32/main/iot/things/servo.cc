#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"

#include <driver/gpio.h>
#include "driver/ledc.h"

#include <esp_log.h>

#define TAG "Servo"

namespace iot {

// 这里仅定义 Lamp 的属性和方法，不包含具体的实现
class Servo : public Thing {
private:
#ifdef CONFIG_IDF_TARGET_ESP32
    gpio_num_t gpio_num_ = GPIO_NUM_9;
#else
    gpio_num_t servo_num_ = GPIO_NUM_9;
#endif
    bool power_ = false;

    void InitializeServo() {
        // gpio_config_t config = {
        //     .pin_bit_mask = (1ULL << gpio_num_),
        //     .mode = GPIO_MODE_OUTPUT,
        //     .pull_up_en = GPIO_PULLUP_DISABLE,
        //     .pull_down_en = GPIO_PULLDOWN_DISABLE,
        //     .intr_type = GPIO_INTR_DISABLE,
        // };
        // ESP_ERROR_CHECK(gpio_config(&config));
        // gpio_set_level(gpio_num_, 0);        
        ledc_timer_config_t servo_timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_10_BIT,
            .timer_num = LEDC_TIMER_1,
            .freq_hz = 50,
            .clk_cfg = LEDC_AUTO_CLK,            
        };
        ledc_channel_config_t servo = {
            .gpio_num = servo_num_,           
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = LEDC_CHANNEL_1,
            .intr_type = LEDC_INTR_DISABLE,            
            .timer_sel = LEDC_TIMER_1,
            .duty = 0
        };
        ledc_channel_config(&servo);
        ledc_timer_config(&servo_timer);
    }

public:
    Servo() : Thing("Servo", "一个驱动机械臂的舵机"){
        InitializeServo();
        properties_.AddNumberProperty("angle", "舵机的转动角度", [this]() -> int {
            // 这里可以添加获取当前亮度的逻辑
            auto backlight = Board::GetInstance().GetBacklight();
            return backlight ? backlight->brightness() : 100;//
        });
        // 定义设备可以被远程执行的指令
        methods_.AddMethod("SetAngle", "设置舵机转动角度", ParameterList({
            Parameter("angle", "0到180之间的整数", kValueTypeNumber, true)
        }), [this](const ParameterList& parameters) {
            ESP_LOGI(TAG,"get");
            uint8_t angle = static_cast<uint8_t>(parameters["angle"].number());
            uint8_t duty = (uint8_t)(angle / 180.0 * 102 + 25.6);            
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        });
};
}; // namespace iot
}

DECLARE_THING(Servo);