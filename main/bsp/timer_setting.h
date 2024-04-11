#ifndef _TIMER_SETTING_H_
#define _TIMER_SETTING_H_
#include <math.h>

#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/queue.h"

#include "esp_adc_cal.h"
#include "driver/adc.h"

#include "driver/ledc.h"
#include "dht11.h"

QueueHandle_t timerInitConfig(uint32_t resolution_hz, uint64_t alarm_count);

struct DataList{
    float temp;
    float humi;
    uint32_t illum;
    uint32_t harmfulgas;
}; 

void LedcInitConfig(uint16_t freq);
int pwm_duty(int duty);

void beep(int fre, int duty, int time, int sound_val);

void beep_all_init_ok(int sound_val);

void beep_gpio_init_ok(int sound_val);

void beep_wifi_connected(int sound_val);

void beep_mqtt_connected(int sound_val);

void beep_wifi_disconnected(int sound_val);

void beep_mqtt_disconnected(int sound_val);

void beep_harmful_gas_detected(int sound_val);

#endif