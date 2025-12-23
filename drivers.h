#ifndef DRIVERS_H
#define DRIVERS_H

void GPIO_Init(void);
void TIM2_Init(void);
void USART1_Init(void);
void UART_SendChar(char c);
void UART_SendString(char *str);

#endif