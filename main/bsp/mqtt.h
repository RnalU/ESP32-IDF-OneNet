#ifndef _MQTT_H_
#define _MQTT_H_

#include "mqtt_client.h"
#include "esp_log.h"

static const char *TAG_MQTT = "<MQTT-MASSAGE>";

// MQTT配置结构体
const esp_mqtt_client_config_t mqtt_config = {
    .broker.address.uri = "mqtt://mqtts.heclouds.com:1883",
    // .broker.address.hostname = "183.230.40.16",
    // .broker.address.port = 8883,
    // .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
    .credentials.client_id = "ESP32",
    .credentials.username = "73K93SlK68",
    .credentials.authentication.password = "version=2018-10-31&res=products%2F73K93SlK68%2Fdevices%2FESP32&et=1743741951&method=sha1&sign=LHes2e7WlnBNUGKbFa%2F5XPAfQfw%3D",
    .network.disable_auto_reconnect = false,
    .session.keepalive = 10,
};
esp_mqtt_client_handle_t client;

void user_mqtt_init(void);

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

#endif