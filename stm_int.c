#include "stm_mapping.h"
#include "global.h"

/* Bi?n extern t? stopwatch.h d? d?o tr?ng thái nút nh?n */
extern void Stopwatch_Toggle(void);

/* 1. Ng?t Timer 2 (1ms) */
void TIM2_IRQHandler(void) {
    // Ki?m tra c? UIF (Update Interrupt Flag) - Bit 0
    if ((TIM2->SR & 0x01) != 0) {
        // Xóa c? (R?t quan tr?ng!)
        TIM2->SR &= ~0x01;
        
        // B?t c? cho Main x? lý
        flag_timer_1ms = 1;
    }
}

/* 2. Ng?t Nút nh?n PA0 (EXTI Line 0) */
void EXTI0_IRQHandler(void) {
    // Ki?m tra c? PR (Pending Register) - Bit 0
    if ((EXTI->PR & 0x01) != 0) {
        // Xóa c? (Ghi s? 1 vào bit dó d? xóa)
        EXTI->PR |= 0x01;
        
        // Báo c? nút nh?n
        flag_button_press = 1;
    }
}

/* 3. Ng?t UART1 (Nh?n d? li?u) */
void USART1_IRQHandler(void) {
    // Ki?m tra c? RXNE (Read Data Not Empty) - Bit 5 SR
    if ((USART1->SR & (1 << 5)) != 0) {
        char c = (char)(USART1->DR); // Ð?c d? li?u (T? d?ng xóa c? RXNE)
        
        // Logic nh?n chu?i vào Buffer
        if (rx_index < RX_BUFFER_SIZE - 1) {
            // N?u g?p ký t? k?t thúc l?nh (Enter)
            if (c == '\n' || c == '\r') {
                rx_buffer[rx_index] = '\0'; // K?t thúc chu?i chu?n C
                flag_uart_ready = 1;        // Báo Main x? lý
            } 
            else {
                rx_buffer[rx_index++] = c;  // Luu ký t?
            }
        } else {
            // Tràn buffer -> Reset
            rx_index = 0;
        }
    }
}
