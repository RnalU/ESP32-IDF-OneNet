#include "mqtt.h"

extern char ip_address[20];
extern float dht11_data_buff[2];
extern uint32_t illum_voltage;
extern uint32_t harmful_gas_voltage;
extern int mqtt_start_flag;

int mqtt_disconnect_flag = 0;

char topic_json[1000] = "";

// 标志位结构体
struct Mqtt_Event_Flag mqtt_event_flag = {0};

esp_mqtt_client_handle_t client;
esp_mqtt_client_handle_t client_cb;

// MQTT配置结构体
const esp_mqtt_client_config_t mqtt_config = {
    .broker.address.uri = CONNECT_URI,
    // .broker.address.hostname = "183.230.40.16",
    // .broker.address.port = 8883,
    // .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
    .credentials.client_id = CLIENT_ID,
    .credentials.username = USERNAME,
    .credentials.authentication.password = PASSWORD,
    .network.disable_auto_reconnect = false,
    .session.keepalive = 10,
};


// MQTT-JSON 格式化函数
void mqtt_init_convert_json(char* topic_list, const char* json_template, ...)
{
    int count = 0;
    for (int i = 0; json_template[i] != '\0'; i++)
    {
        if (json_template[i] == '%') count++;
    }   

    va_list arg_list;
    va_start(arg_list, json_template);
    // // WIFI
    // sprintf(topic_json, JSON_INIT_WIFI_DATA_TEMPLE, ip_address, EXAMPLE_ESP_WIFI_SSID);
    // ESP_LOGI(TAG_MQTT, "WIFI-JSON格式化完成:%s\n", topic_json);

    // sprintf(topic_json, JSON_INIT_SENSOR_STAGE_TEMPLE, 0, 0, 0, 1);
    // ESP_LOGI(TAG_MQTT, "SENSOR-JSON格式化完成:%s\n", topic_json);
}

void format_json(char* topic_list, const char* json_template, ...)
{
    va_list args;
    va_start(args, json_template);

    int len = vsnprintf(NULL, 0, json_template, args);
    va_end(args);

    char* formatted_str = (char*)malloc((len + 1) * sizeof(char));

    va_start(args, json_template);

    vsprintf(formatted_str, json_template, args);
    va_end(args);

    strcpy(topic_list, formatted_str);
    free(formatted_str);

    ESP_LOGI(TAG_MQTT, "JSON格式化完成:%s\n", topic_json);
}

// MQTT初始化函数
void user_mqtt_init(void)
{
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    // 获取MQTT客户端结构体指针
    client_cb = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    // 通过事件ID来分别处理对应的事件
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:    // MQTT连上事件
            mqtt_disconnect_flag = 0;
            mqtt_start_flag = 1;
            beep_mqtt_connected(5);
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
            ESP_LOGI(TAG_MQTT, "Subscribe OneNet Topic...");
            
            // 订阅 POST_Reply 话题，接收平台获取属性上报后返回的信息
            msg_id = esp_mqtt_client_subscribe(client_cb, TOPIC_THING_POST_REPLY_AC, 0);
            msg_id = esp_mqtt_client_subscribe(client_cb, TOPIC_THING_POST_REPLY_RJ, 0);
            if (msg_id > 0) 
            { 
                ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id); 
                mqtt_event_flag.Post_Reply = 1; 
            }
            else if (msg_id == -1) // 重新订阅
            { 
                int times = 0;
                ESP_LOGI(TAG_MQTT, "sent subscribe fail! Retry..., msg_id=%d", msg_id); 
                while (times++ < TOPIC_SUBSCRIBE_TRY_TIMES)
                {
                    msg_id = esp_mqtt_client_subscribe(client_cb, TOPIC_THING_POST_REPLY_AC, 0);
                    msg_id = esp_mqtt_client_subscribe(client_cb, TOPIC_THING_POST_REPLY_RJ, 0);

                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    ESP_LOGI(TAG_MQTT, "sent subscribe fail! Retry %d...", times);
                    if (msg_id > 0)
                    {
                        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);
                        mqtt_event_flag.Post_Reply = 1;
                        break;  
                    }
                }
            }


            // 向属性话题发布信息
            sprintf(topic_json, JSON_INIT_WIFI_DATA_TEMPLATE, ip_address, EXAMPLE_ESP_WIFI_SSID);
            ESP_LOGI(TAG_MQTT, "WIFI-JSON格式化完成:%s\n", topic_json);

            // format_json(topic_json, JSON_INIT_WIFI_DATA_TEMPLATE, ip_address, EXAMPLE_ESP_WIFI_SSID);
            esp_mqtt_client_publish(client_cb, TOPIC_THING_POST, topic_json, 0, 0, 0);

            sprintf(topic_json, JSON_INIT_SENSOR_STAGE_TEMPLATE, 0, 0, 0, 0);
            ESP_LOGI(TAG_MQTT, "SENSOR-JSON格式化完成:%s\n", topic_json);

            // format_json(topic_json, JSON_INIT_SENSOR_STAGE_TEMPLATE, 0, 0, 0, 0);
            esp_mqtt_client_publish(client_cb, TOPIC_THING_POST, topic_json, 0, 0, 0);
            
            break;

        case MQTT_EVENT_DISCONNECTED:    // MQTT断开连接事件
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
            mqtt_disconnect_flag = 1;
            beep_mqtt_disconnected(5);
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
            ESP_LOGI(TAG_MQTT, "TOPIC=%.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG_MQTT, "DATA=%.*s\r\n", event->data_len, event->data);
            // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            // printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
            mqtt_disconnect_flag = 1;
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