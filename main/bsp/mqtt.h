#ifndef _MQTT_H_
#define _MQTT_H_

#include "mqtt_client.h"
#include "esp_log.h"
#include "wifi.h"

// 配置参数
#define CONNECT_URI     "mqtt://mqtts.heclouds.com:1883"
#define CLIENT_ID       "ESP32"
#define USERNAME        "73K93SlK68"
#define PASSWORD        "version=2018-10-31&res=products%2F73K93SlK68%2Fdevices%2FESP32&et=1743741951&method=sha1&sign=LHes2e7WlnBNUGKbFa%2F5XPAfQfw%3D"

#define TOPIC_SUBSCRIBE_TRY_TIMES 10

// 设备发布话题
#define TOPIC_THING_POST "$sys/"USERNAME"/"CLIENT_ID"/thing/property/post"

// 设备订阅话题
#define TOPIC_THING_POST_REPLY "$sys/"USERNAME"/"CLIENT_ID"/thing/property/post/reply"

// 初始化JSON模板 
//IP地址 WIFI名称
#define JSON_INIT_WIFI_DATA_TEMPLATE "{\
\"id\": \"123\",\
\"version\": \"1.0\",\
\"params\": {\
\"IPaddress\": {\
\"value\": \"%s\",\
\"time\": 1712217202000\
},\
\"WIFIName\": {\
\"value\": \"%s\",\
\"time\": 1712217202000}}}"\

// 一氧化碳传感器状态 DHT11传感器状态 光传感器状态 声传感器状态
#define JSON_INIT_SENSOR_STAGE_TEMPLATE "{\
\"id\": \"123\",\
\"version\": \"1.0\",\
\"params\": {\"COSenseStage\": {\
\"value\": %d,\
\"time\": 1712217202000\
},\
\"DHT11Stage\": {\
\"value\": %d,\
\"time\": 1712217202000\
},\
\"LightSenseStage\": {\
\"value\": %d,\
\"time\": 1712217202000\
},\
\"SoundSenseStage\": {\
\"value\": %d,\
\"time\": 1712217202000}}}"

// 温度值 湿度值 有害气体值 光值 声值
#define JSON_SENSOR_DATA_TEMPLATE "{\
\"id\": \"123\",\
\"version\": \"1.0\",\
\"params\": {\
\"DHT11Temp\": {\
\"value\": %.1f,\
\"time\": 1712217202000\
},\
\"DHT11Himi\": {\
\"value\": %.1f,\
\"time\": 1712217202000\
},\
\"COValue\": {\
\"value\": %lu,\
\"time\": 1712217202000\
},\
\"LightValue\": {\
\"value\": %lu,\
\"time\": 1712217202000\
},\
\"SoundValue\": {\
\"value\": %d,\
\"time\": 1712217202000}}}"

static const char *TAG_MQTT = "<MQTT-MASSAGE>";

struct Mqtt_Event_Flag {
    uint8_t Post_Reply;
};

void user_mqtt_init(void);

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

#endif