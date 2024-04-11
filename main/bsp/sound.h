#ifndef _SOUND_H_
#define _SOUND_H_

#include "driver/gpio.h"

#define SOUND_SENSOR_GPIO 4

void sound_sensor_init(void);

int sound_get_stage(void);

#endif