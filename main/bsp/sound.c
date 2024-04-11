#include "sound.h"

void sound_sensor_init(void)
{
    gpio_config_t gpio_init_struct = {0};

    esp_rom_gpio_pad_select_gpio(SOUND_SENSOR_GPIO);
    
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;             
    gpio_init_struct.mode = GPIO_MODE_DEF_INPUT;             
    gpio_init_struct.pull_up_en = GPIO_PULLUP_DISABLE;           
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;      
    gpio_init_struct.pin_bit_mask = 1ull<<4;        
    
    //将以上参数配置到引脚
    gpio_config( &gpio_init_struct );

    gpio_set_level(SOUND_SENSOR_GPIO, 0);
}

int sound_get_stage(void)
{
    return gpio_get_level(SOUND_SENSOR_GPIO);
}