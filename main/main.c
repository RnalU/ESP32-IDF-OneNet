#include <stdio.h>
#include "unistd.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_adc_cal.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "mqtt_client.h"

#include "led.h"
#include "usart.h"
#include "dht11.h"
#include "timer_setting.h"
#include "mqtt.h"
#include "wifi.h"

static const char *TAG = "<MASSAGE>";

extern char topic_json[1000];
extern struct Mqtt_Event_Flag mqtt_event_flag;
extern esp_mqtt_client_handle_t client;
extern esp_mqtt_client_handle_t client_cb;

// 数据变量
static esp_adc_cal_characteristics_t *adc_chars;
QueueHandle_t queue = 0;
float dht11_data_buff[2] = {0};
uint32_t illum_voltage = 0;
uint32_t harmful_gas_voltage = 0;

void system_init(void)
{
    // led 初始化
    LedGpioConfing();
    
    // ADC初始化
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_12);
    
    // 定时器初始化
    queue = timerInitConfig(1000000, 1000000);
    
    // PWM初始化
    LedcInitConfig(600);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // NVS初始化
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
 
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    
    // MQTT初始化
    user_mqtt_init();

    printf("Hello World\n");
}


void app_main(void)
{
    // 初始化
    system_init();

    // ADC校正
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 3300, adc_chars);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC1_CHANNEL_2, ADC_WIDTH_BIT_12, 3300, adc_chars);

    // 声明变量
    int a1_ch0_raw_val = 0;
    int a1_ch3_raw_val = 0;
    uint8_t led_flag = 1;
    int main_msg_id = 0;
    
    while (1)
    {
        // DHT11
        DHT11(dht11_data_buff);

        // 光敏
        a1_ch0_raw_val = adc1_get_raw(ADC1_CHANNEL_0);
        illum_voltage = esp_adc_cal_raw_to_voltage(a1_ch0_raw_val, adc_chars);

        // 有害气体
        a1_ch3_raw_val = adc1_get_raw(ADC1_CHANNEL_2);
        harmful_gas_voltage = esp_adc_cal_raw_to_voltage(a1_ch3_raw_val, adc_chars);

        // 输出传感器数值
        printf("Temp:%.2f | Humi:%.2f | Illum:%lu | HarmfulGas:%lu \n", dht11_data_buff[0], dht11_data_buff[1], illum_voltage, harmful_gas_voltage);

        if (led_flag == 0)
        {
            led_flag = !led_flag;
            LedOn();
        }
        else
        {
            led_flag = !led_flag;
            LedOff();
        }

        if (mqtt_event_flag.Post_Reply == 1)
        {
            // 向话题发布信息
            sprintf(topic_json, JSON_SENSOR_DATA_TEMPLATE, dht11_data_buff[0], dht11_data_buff[1], harmful_gas_voltage, illum_voltage, 0);
            main_msg_id = esp_mqtt_client_publish(client, TOPIC_THING_POST, topic_json, 0, 0, 0);
            if (main_msg_id >= 0) { ESP_LOGI(TAG_MQTT, "Send OK! Stage:%d", main_msg_id); }
            else { ESP_LOGI(TAG_MQTT, "Send Fail! Stage:%d", main_msg_id); }
        }

        // 延迟
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
}