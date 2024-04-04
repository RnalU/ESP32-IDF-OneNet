#include "led.h"

#define GPIO_OUTPUT_PIN_SEL  (1ULL<<LED_PIN)

void LedGpioConfing(void)
{
    gpio_config_t gpio_init_struct = {0};

    esp_rom_gpio_pad_select_gpio(LED_PIN);
    
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;             
    gpio_init_struct.mode = GPIO_MODE_OUTPUT;                   
    gpio_init_struct.pull_up_en = GPIO_PULLUP_ENABLE;           
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;      
    gpio_init_struct.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;        
    
    //将以上参数配置到引脚
    gpio_config( &gpio_init_struct );

    gpio_set_level(LED_PIN, 1);
}

void LedOn(void)
{
    gpio_set_level(LED_PIN, 0);
}

void LedOff(void)
{
    gpio_set_level(LED_PIN, 1);
}
