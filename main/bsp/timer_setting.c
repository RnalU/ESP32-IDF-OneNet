#include "timer_setting.h"

#define CONFIG_LOG_MAXIMUM_LEVEL 100

static bool IRAM_ATTR TimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;

    QueueHandle_t queue = (QueueHandle_t)user_data;
    
    static int time = 0;
    time++;
    
    xQueueSendFromISR(queue, &time, &high_task_awoken);
   
    return (high_task_awoken == pdTRUE);
}

QueueHandle_t timerInitConfig(uint32_t resolution_hz, uint64_t alarm_count)
{
    gptimer_handle_t gptimer = NULL;


    QueueHandle_t queue = xQueueCreate(10, sizeof(10));

    if (!queue) {
        ESP_LOGE("queue", "Creating queue failed");
        return NULL;
    }

    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = resolution_hz, // 1MHz, 1 tick = 1us
    };
    gptimer_new_timer(&timer_config, &gptimer);


    gptimer_event_callbacks_t cbs = {
        .on_alarm = TimerCallback,
    };

    gptimer_register_event_callbacks(gptimer, &cbs, queue);

    gptimer_enable(gptimer);

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,  
        .alarm_count = alarm_count, 
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_set_alarm_action(gptimer, &alarm_config);

    gptimer_start(gptimer);
    return queue;
}

int pwm_duty(int duty)
{
    return (int) (pow(2, 13) * ((double)duty / 100.0) - 1);
}

void LedcInitConfig(uint16_t freq)
{
    // 准备并应用led PWM定时器配置
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_13_BIT, 
        .freq_hz          = freq, 
        .clk_cfg          = LEDC_AUTO_CLK    
       
    };
    ledc_timer_config(&ledc_timer);


    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,  
        .channel        = LEDC_CHANNEL_0,   
        .timer_sel      = LEDC_TIMER_0,      
        .intr_type      = LEDC_INTR_DISABLE, 
        .gpio_num       = 7,   
        .duty           = 0,                 
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}

void beep(int fre, int duty, int time, int sleep)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, fre);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    vTaskDelay(time / portTICK_PERIOD_MS);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    vTaskDelay(sleep / portTICK_PERIOD_MS);
}

void beep_all_init_ok(int sound_val)
{
    beep(1046.50, sound_val, 50, 50);
    beep(1318.51, sound_val, 50, 50);
    beep(1567.98, sound_val, 50, 50);
}

void beep_gpio_init_ok(int sound_val)
{
    beep(523, sound_val, 120, 100);
}

void beep_wifi_connected(int sound_val)
{
    beep(659, sound_val, 120, 100);
}

void beep_mqtt_connected(int sound_val)
{
    beep(784, sound_val, 120, 500);
}

void beep_wifi_disconnected(int sound_val)
{
    for (int i = 0; i < 3; i++)
        beep(1046, sound_val, 200, 50);
}

void beep_mqtt_disconnected(int sound_val)
{
        beep(1046, sound_val, 300, 50);
}

void beep_harmful_gas_detected(int sound_val)
{
    for (int i = 0; i < 3; i++)
        beep(1568, sound_val, 50, 50);
}