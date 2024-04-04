#ifndef _DHT11_H_
#define _DHT11_H_

#include "driver/gpio.h"
#include "esp_rom_sys.h"

void Delay_ms(uint16_t ms);
void DHT11(float* data_list);

#endif 