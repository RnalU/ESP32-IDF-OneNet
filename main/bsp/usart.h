#ifndef _USART_H_
#define _USART_H_

// 导入相关库
#include "driver/uart.h"
#include "driver/gpio.h"

#include <string.h>

void uart_init_config(uart_port_t uart_port, uint32_t baud_rate, int tx_pin, int rx_pin);

void uart_rx_task(void);

#endif  