#include "stm_mapping.h"
#include "drivers.h"

/* 1. Kh?i t?o GPIO (LEDs & Button) và Ng?t EXTI */
void GPIO_Init(void) {
    // a. B?t Clock cho GPIOG (LED), GPIOA (Button), SYSCFG (EXTI)
    RCC->AHB1ENR |= (1 << 6); // GPIOG (Bit 6)
    RCC->AHB1ENR |= (1 << 0); // GPIOA (Bit 0)
    RCC->APB2ENR |= (1 << 14); // SYSCFG (Bit 14)

    // b. C?u hình LED (PG13, PG14) là Output
    // PG13: Clear bit 27, Set bit 26 (01: Output)
    GPIOG->MODER &= ~(3U << 26); 
    GPIOG->MODER |=  (1U << 26);
    // PG14: Clear bit 29, Set bit 28
    GPIOG->MODER &= ~(3U << 28); 
    GPIOG->MODER |=  (1U << 28);

    // c. C?u hình Button (PA0) là Input (00) - M?c d?nh là 00 nên không c?n ghi
    // d. Map PA0 vào line EXTI0
    SYSCFG->EXTICR[0] &= ~(0xF); // Clear 4 bit d?u (ch?n PA)

    // e. C?u hình EXTI0
    EXTI->IMR  |= (1 << 0); // Cho phép ng?t line 0
    EXTI->FTSR |= (1 << 0); // Ch?n ng?t c?nh xu?ng (Nh? nút m?i nh?n)
    // EXTI->RTSR |= (1 << 0); // N?u mu?n c?nh lên thì dùng dòng này

    // f. B?t NVIC cho EXTI0 (IRQ #6)
    NVIC_ISER0 |= (1 << 6);
}

/* 2. Kh?i t?o Timer 2 (T?o ng?t 1ms) */
void TIM2_Init(void) {
    // a. B?t Clock cho TIM2 (APB1)
    RCC->APB1ENR |= (1 << 0);

    // b. Tính toán b? chia (Gi? s? Clock = 16MHz)
    // Mu?n 1ms (1kHz). Công th?c: F_timer = F_clk / ((PSC+1)*(ARR+1))
    // Ch?n PSC = 15 -> Timer Clock = 16MHz / 16 = 1MHz (1 ticks = 1us)
    TIM2->PSC = 15;
    
    // Ch?n ARR = 999 -> Ð?m 1000 ticks = 1000us = 1ms
    TIM2->ARR = 999;

    // c. Cho phép ng?t Update (UIE - Bit 0)
    TIM2->DIER |= (1 << 0);

    // d. B?t NVIC cho TIM2 (IRQ #28)
    NVIC_ISER0 |= (1 << 28);

    // e. B?t Timer (CEN - Bit 0)
    TIM2->CR1 |= (1 << 0);
}

/* 3. Kh?i t?o USART1 (115200 baud, 8N1) */
void USART1_Init(void) {
    // a. B?t Clock GPIOA và USART1
    RCC->AHB1ENR |= (1 << 0);  // GPIOA
    RCC->APB2ENR |= (1 << 4);  // USART1

    // b. C?u hình PA9 (TX) và PA10 (RX) ch? d? Alternate Function (AF)
    // PA9 Mode: 10 (AF) -> Bit 19:18
    GPIOA->MODER &= ~(3U << 18);
    GPIOA->MODER |=  (2U << 18);
    // PA10 Mode: 10 (AF) -> Bit 21:20
    GPIOA->MODER &= ~(3U << 20);
    GPIOA->MODER |=  (2U << 20);

    // Ch?n AF7 (USART1) cho PA9 và PA10
    // PA9 (AFRH bit 7:4), PA10 (AFRH bit 11:8)
    GPIOA->AFRH |= (7 << 4) | (7 << 8);

    // c. C?u hình Baudrate 115200 (Clock 16MHz)
    // USARTDIV = 16,000,000 / (16 * 115200) = 8.6805
    // Mantissa = 8. Fraction = 0.6805 * 16 = 10.88 ~ 11 (0xB)
    // => BRR = 0x8B
    USART1->BRR = 0x8B; 

    // d. C?u hình Control Register
    // UE (Enable) | TE (Transmitter) | RE (Receiver) | RXNEIE (RX Interrupt)
    // Bit 13, 3, 2, 5
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2) | (1 << 5);

    // e. B?t NVIC cho USART1 (IRQ #37)
    // 37 n?m ? thanh ghi ISER1 (37 - 32 = 5)
    NVIC_ISER1 |= (1 << 5);
}

void UART_SendChar(char c) {
    // Ch? cho d?n khi TXE (Transmit Data Register Empty) = 1 (Bit 7 SR)
    while (!(USART1->SR & (1 << 7)));
    USART1->DR = c;
}

void UART_SendString(char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}
