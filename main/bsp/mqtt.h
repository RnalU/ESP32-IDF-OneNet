#ifndef _MQTT_H_
#define _MQTT_H_

#include "mqtt_client.h"
#include "esp_log.h"
#include "wifi.h"

// 配置参数
// #define CONNECT_URI     "mqtt://mqtts.heclouds.com:1883"
// #define CLIENT_ID       "ESP32"
// #define USERNAME        "73K93SlK68"
// #define PASSWORD        "version=2018-10-31&res=products%2F73K93SlK68%2Fdevices%2FESP32&et=1743741951&method=sha1&sign=LHes2e7WlnBNUGKbFa%2F5XPAfQfw%3D"



// 配置参数
#define CONNECT_URI     "mqtt://mqtts.heclouds.com:1883"
#define CLIENT_ID       "zsb-esp32"
#define USERNAME        "FbeeehL9Ns"
#define PASSWORD        "version=2018-10-31&res=products%2FFbeeehL9Ns%2Fdevices%2Fzsb-esp32&et=1765430315&method=md5&sign=J3hODRYNm9TgkCkT%2FAl8Kg%3D%3D"


#define TOPIC_SUBSCRIBE_TRY_TIMES 10


// 设备发布话题
#define TOPIC_THING_POST "$sys/FbeeehL9Ns/zsb-esp32/dp/post/json"

// 设备订阅话题
#define TOPIC_THING_POST_REPLY_AC "$sys/FbeeehL9Ns/zsb-esp32/dp/post/json/accepted"
#define TOPIC_THING_POST_REPLY_RJ "$sys/FbeeehL9Ns/zsb-esp32/dp/post/json/rejected"

// 初始化JSON模板 
//IP地址 WIFI名称
#define JSON_INIT_WIFI_DATA_TEMPLATE "{\
\"id\": 001,\
\"dp\": {\         
\"IPaddress\": [{\
\"v\": \"%s\",\          
}],\
\"WIFIName\": [{\     
\"v\": \"%s\",\        
}]}}"

// 一氧化碳传感器状态 DHT11传感器状态 光传感器状态 声传感器状态
#define JSON_INIT_SENSOR_STAGE_TEMPLATE "{\
\"id\": 001,\
\"dp\": {\         
\"COSenseStage\": [{\
\"v\": \"%d\",\   
}],\
\"DHT11Stage\": [{\
\"v\": \"%d\",\   
}],\
\"LightSenseStage\": [{\
\"v\": \"%d\",\
}],\
\"SoundSenseStage\": [{\
\"v\": \"%d\"}]}}"

// 温度值 湿度值 有害气体值 光值 声值
#define JSON_SENSOR_DATA_TEMPLATE "{\
\"id\": 001,\
\"dp\": {\         
\"DHT11Temp\": [{\
\"v\": \"%.1f\",\   
}],\
\"DHT11Himi\": [{\
\"v\": \"%.1f\",\   
}],\
\"COValue\": [{\
\"v\": \"%lu\",\
}],\
\"LightValue\": [{\
\"v\": \"%lu\"\
}],\
\"SoundValue\": [{\
\"v\": \"%d\",\
}]}}"

static const char *TAG_MQTT = "<MQTT-MASSAGE>";

struct Mqtt_Event_Flag {
    uint8_t Post_Reply;
};

void user_mqtt_init(void);

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

#endif