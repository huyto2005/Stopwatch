#include "stm_mapping.h"
#include "global.h"
#include "drivers.h"
#include "stopwatch.h"
#include "command_parser.h"

/* Hàm x? lý nút nh?n trong Main (d? d?m b?o tính d?ng b?) */
void Button_Process(void) {
    if (flag_button_press) {
        // Toggle tr?ng thái d?ng h?
        Stopwatch_Toggle();
        
        // Nháy dèn xanh d? báo hi?u
        GPIOG->ODR ^= (1 << 13); 
        
        // Reset c?
        flag_button_press = 0;
    }
}

int main(void) {
    // 1. Kh?i t?o ph?n c?ng
    GPIO_Init();
    USART1_Init();
    TIM2_Init();
    
    // 2. Kh?i t?o logic ph?n m?m
    Stopwatch_Init();

    // G?i thông báo kh?i d?ng
    UART_SendString("SYSTEM READY. Waiting for commands...\r\n");

    // 3. Vòng l?p vô t?n (Super Loop)
    while (1) {
        // Task 1: C?p nh?t th?i gian & G?i UART d?nh k?
        Stopwatch_Update();

        // Task 2: X? lý l?nh t? máy tính (START, STOP...)
        Command_Process();

        // Task 3: X? lý nút nh?n
        Button_Process();
    }
    
    return 0;
}