#include "command_parser.h"
#include "global.h"
#include "stopwatch.h"
#include "drivers.h" // Ð? dùng hàm g?i UART

/* Hàm t? vi?t: So sánh 2 chu?i (Thay th? strcmp)
 * Tr? v? 1 n?u gi?ng nhau, 0 n?u khác nhau
 */
int StringCompare(char *s1, char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    // C? 2 ph?i cùng k?t thúc thì m?i g?i là gi?ng
    if (*s1 == '\0' && *s2 == '\0') return 1;
    return 0;
}

void Command_Process(void) {
    if (flag_uart_ready) {
        // 1. X? lý chu?i nh?n du?c (Xóa ký t? xu?ng dòng \r\n n?u có)
        // Gi? s? rx_buffer ch?a "START\r\n" -> C?t thành "START"
        // Trong hàm ng?t ta s? x? lý vi?c thêm \0 vào cu?i chu?i r?i.
        
        // 2. So sánh l?nh
        if (StringCompare(rx_buffer, "START")) {
            Stopwatch_Start();
        }
        else if (StringCompare(rx_buffer, "STOP")) {
            Stopwatch_Stop();
        }
        else if (StringCompare(rx_buffer, "RESET")) {
            Stopwatch_Reset();
        }
        else if (StringCompare(rx_buffer, "LAP")) {
            Stopwatch_Lap();
        }
        else if (StringCompare(rx_buffer, "LAPS?")) {
            Stopwatch_ShowLaps();
        }
        else {
            UART_SendString("ERROR: Unknown Command\r\n");
        }

        // 3. Reset tr?ng thái nh?n
        flag_uart_ready = 0;
        rx_index = 0;
        // Xóa buffer d? tránh nh? l?nh cu (Optional)
        for(int i=0; i<RX_BUFFER_SIZE; i++) rx_buffer[i] = 0;
    }
}