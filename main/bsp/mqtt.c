#include "mqtt.h"


void user_mqtt_init(void)
{
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    // 获取MQTT客户端结构体指针
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    // 通过事件ID来分别处理对应的事件
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:    // MQTT连上事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
            ESP_LOGI(TAG_MQTT, "TESTING...");
            // // MQTT Client发布主题函数，主题是/topic/qos1，服务质量qos1，发布的数据是data-3
            // msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            // ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);

            // // MQTT Client订阅主题函数，主题是/topic/qos0，服务质量qos0
            // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            // ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

            // // MQTT Client订阅主题函数，主题是/topic/qos1，服务质量qos1
            // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            // ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

            // // MQTT Client取消订阅主题函数
            // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            // ESP_LOGI(TAG_MQTT, "sent unsubscribe successful, msg_id=%d", msg_id);

            break;
        case MQTT_EVENT_DISCONNECTED:    // MQTT断开连接事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:    // MQTT发送订阅成功事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            // ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);
            break;
        
        case MQTT_EVENT_UNSUBSCRIBED:    // MQTT取消订阅事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:    // MQTT发布成功事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:    // MQTT接收数据事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
            break;

        default:
            ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}


void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) 
{
    ESP_LOGD(TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%d", base, (int)event_id);
    mqtt_event_handler_cb(event_data);
}