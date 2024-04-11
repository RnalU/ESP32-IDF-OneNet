idf_component_register(SRCS "main.c" 
                        "bsp/led.c"
                        "bsp/usart.c"
                        "bsp/dht11.c"
                        "bsp/timer_setting.c"
                        "bsp/mqtt.c"
                        "bsp/wifi.c"
                        "bsp/sound.c"
                        INCLUDE_DIRS 
                        "."
                        "./bsp/")