#ifndef GLOBAL_H
#define GLOBAL_H

/* ========================================================================== */
/* GLOBAL FLAGS & VARIABLES (Bi?n toàn c?c dùng chung)                        */
/* ========================================================================== */

// C? báo ng?t Timer 1ms (Set trong Timer ISR, Clear trong Main)
extern volatile unsigned int flag_timer_1ms;

// C? báo nút nh?n (Set trong EXTI ISR, Clear trong Main)
extern volatile unsigned int flag_button_press;

// C? báo dã nh?n xong 1 l?nh UART (g?p ký t? \n ho?c \r)
extern volatile unsigned int flag_uart_ready;

// Buffer nh?n d? li?u UART
#define RX_BUFFER_SIZE 50
extern char rx_buffer[RX_BUFFER_SIZE];
extern unsigned int rx_index;

#endif /* GLOBAL_H */
