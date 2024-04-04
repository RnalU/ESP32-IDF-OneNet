#include "usart.h"

void uart_init_config(uart_port_t uart_port, uint32_t baud_rate, int tx_pin, int rx_pin)
{
    uart_config_t uart_config = {0};

    uart_config.baud_rate = baud_rate;
    uart_config.data_bits = UART_DATA_8_BITS;  // 八位数据位
    uart_config.parity = UART_PARITY_DISABLE;  // 不需要校验位
    uart_config.stop_bits = UART_STOP_BITS_1;  // 设置停止位
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;

    uart_param_config(uart_port, &uart_config);
    uart_set_pin(uart_port, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_port, 255, 255, 0, NULL, 0);
}

void uart_rx_task(void)
{
    uint8_t rx_data[200]={0};
    uint8_t temp[50]={0};
    while(1)
    {
        int rx_bytes = uart_read_bytes(UART_NUM_2, rx_data, 200, 10 / portTICK_PERIOD_MS);
        if( rx_bytes > 0 )//数据长度大于0，说明接收到数据
        {
            rx_data[rx_bytes] = 0;//将串口数据的最后一个设置为0，形成字符串

            //输出接收数据的长度
            sprintf((const char*)temp,"uart2 string length : %d\r\n", rx_bytes);
            uart_write_bytes(UART_NUM_2, (const char*)temp, strlen((const char*)temp));
            uart_write_bytes(UART_NUM_2, (const char*)"uart2 received : ", strlen("uart2 received : "));
            uart_write_bytes(UART_NUM_2, (const char*)rx_data, strlen((const char*)rx_data));
            //UART环缓冲区刷新。
            uart_flush(UART_NUM_2);
        }
    }


}