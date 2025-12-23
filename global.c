#include "global.h"

volatile unsigned int flag_timer_1ms = 0;
volatile unsigned int flag_button_press = 0;
volatile unsigned int flag_uart_ready = 0;

char rx_buffer[RX_BUFFER_SIZE];
unsigned int rx_index = 0;
