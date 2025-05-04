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
  esp_mqtt_client_handle_t client;
  bool mqtt_connected = false;

  // MQTT 发送消息函数
  void SendMqttMessage(const std::string& message) {
    if (client == nullptr) {
      ESP_LOGE(TAG, "MQTT client is not initialized!");
      return;
    }

    const char* topic = "iot/control/jmk/lamp";  // MQTT主题，用于区分不同设备
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
  Lamp()
      : Thing("Lamp", "可以控制房间灯光开关") {
    mqtt_app_start();
    // 定义设备的方法
    methods_.AddMethod(
        "SwitchLight", "开关灯",
        ParameterList(
            {Parameter("state", "true为开灯，false为关灯", kValueTypeBoolean, true)}),
        [this](const ParameterList& parameters) {
          bool is_on = parameters["state"].boolean();
          char command_str[5] = {'w', is_on ? '1' : '0', 0};
          ESP_LOGI(TAG, "Switch Light: %s", is_on ? "ON" : "OFF");
          SendMqttMessage(command_str);
        });

  }
};

}  // namespace iot

DECLARE_THING(DuMqtt);
