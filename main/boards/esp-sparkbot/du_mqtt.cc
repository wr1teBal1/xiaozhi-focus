/*
    ESP-SparkBot 的底座
    https://gitee.com/esp-friends/esp_sparkbot/tree/master/example/tank/c2_tracked_chassis
*/

#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_log.h>

#include <cstring>

#include "board.h"
#include "boards/esp-sparkbot/config.h"
#include "iot/thing.h"
#include "mqtt_client.h"
#include "sdkconfig.h"

#define TAG "du_mqtt"
#define CONFIG_BROKER_URL "mqtt://mqtt.eclipseprojects.io"

namespace iot {

class DuMqtt : public Thing {
 private:
  light_mode_t light_mode_ = LIGHT_MODE_ALWAYS_ON;
  esp_mqtt_client_handle_t client;
  bool mqtt_connected = false;

  // MQTT 发送消息函数
  void SendMqttMessage(const std::string& message) {
    if (client == nullptr) {
      ESP_LOGE(TAG, "MQTT client is not initialized!");
      return;
    }

    const char* topic = "robot/control";  // 替换成你需要发布消息的主题
    int msg_id = esp_mqtt_client_publish(client, topic, message.c_str(), 0, 1,
                                         0);  // 发布消息
    ESP_LOGI(TAG, "Message sent, msg_id: %d", msg_id);
  }

  // MQTT 事件处理函数
  static void mqtt_event_handler(void* handler_args, esp_event_base_t base,
                                 int32_t event_id, void* event_data) {
    ESP_LOGD(TAG,
             "Event dispatched from event loop base=%s, event_id=%" PRIi32 "",
             base, event_id);

    switch (event_id) {
      case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT connected successfully");
        break;
      case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT disconnected");
        break;
      case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "Subscribed to topic");
        break;
      case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "Unsubscribed from topic");
        break;
      case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "Message published");
        break;
      case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT event error");
        break;
      default:

        break;
    }
  }

  void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {.address = {.uri = CONFIG_BROKER_URL}},
        // 你可以在此处扩展其他 MQTT 配置项
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == nullptr) {
      ESP_LOGE(TAG, "Failed to initialize MQTT client");
      return;
    }

    esp_mqtt_client_register_event(
        client, static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID),
        mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
  }

 public:
  DuMqtt()
      : Thing("DuMqtt", "小机器人的底座：有履带可以移动；可以调整灯光效果"),
        light_mode_(LIGHT_MODE_ALWAYS_ON) {
    mqtt_app_start();
    // 定义设备的属性
    properties_.AddNumberProperty(
        "light_mode", "灯光效果编号", [this]() -> int {
          return (light_mode_ - 2 <= 0) ? 1 : light_mode_ - 2;
        });

    // 定义设备可以被远程执行的指令
    methods_.AddMethod("GoForward", "向前走", ParameterList(),
                       [this](const ParameterList& parameters) {
                         SendMqttMessage("x0.0 y1.0");
                       });

    methods_.AddMethod("GoBack", "向后退", ParameterList(),
                       [this](const ParameterList& parameters) {
                         SendMqttMessage("x0.0 y-1.0");
                       });

    methods_.AddMethod("TurnLeft", "向左转", ParameterList(),
                       [this](const ParameterList& parameters) {
                         SendMqttMessage("x-1.0 y0.0");
                       });

    methods_.AddMethod("TurnRight", "向右转", ParameterList(),
                       [this](const ParameterList& parameters) {
                         SendMqttMessage("x1.0 y0.0");
                       });

    methods_.AddMethod("Dance", "跳舞", ParameterList(),
                       [this](const ParameterList& parameters) {
                         SendMqttMessage("d1");
                         light_mode_ = LIGHT_MODE_MAX;
                       });

    methods_.AddMethod(
        "SwitchLightMode", "打开灯",
        ParameterList(
            {Parameter("lightmode", "1到6之间的整数", kValueTypeNumber, true)}),
        [this](const ParameterList& parameters) {
          char command_str[5] = {'w', 0, 0};
          char mode = static_cast<char>(parameters["lightmode"].number()) + 2;

          ESP_LOGI(TAG, "Input Light Mode: %c", (mode + '0'));

          if (mode >= 3 && mode <= 8) {
            command_str[1] = mode + '0';
            SendMqttMessage(command_str);
          }
        });
  }
};

}  // namespace iot

DECLARE_THING(DuMqtt);
