#include "stopwatch.h"
#include "global.h"

/* Khai báo hàm g?i UART (S? d?nh nghia bên uart_driver ho?c main) */
extern void UART_SendString(char *str);

/* Bi?n n?i b? qu?n lý th?i gian */
unsigned int hh = 0, mm = 0, ss = 0, ms = 0;
unsigned int is_running = 0; // 0: Stop, 1: Start

/* Bi?n luu tr? Laps (M?ng 2 chi?u: 8 laps x 4 giá tr?) */
unsigned int laps[8][4]; 
unsigned int lap_count = 0;

/* Hàm ph? tr?: Chuy?n s? sang chu?i 2 ch? s? (VD: 5 -> "05") */
void IntTo2Char(unsigned int val, char *buf) {
    buf[0] = (val / 10) + '0';
    buf[1] = (val % 10) + '0';
}

/* Hàm ph? tr?: T?o chu?i th?i gian "TIME: hh:mm:ss\r\n" */
void Format_Time_String(char *buffer, unsigned int h, unsigned int m, unsigned int s) {
    // "TIME: "
    buffer[0] = 'T'; buffer[1] = 'I'; buffer[2] = 'M'; buffer[3] = 'E'; buffer[4] = ':'; buffer[5] = ' ';
    
    // hh
    IntTo2Char(h, &buffer[6]);
    buffer[8] = ':';
    
    // mm
    IntTo2Char(m, &buffer[9]);
    buffer[11] = ':';
    
    // ss
    IntTo2Char(s, &buffer[12]);
    
    // "\r\n"
    buffer[14] = '\r';
    buffer[15] = '\n';
    buffer[16] = '\0'; // Null terminator
}

void Stopwatch_Init(void) {
    hh = 0; mm = 0; ss = 0; ms = 0;
    is_running = 0;
    lap_count = 0;
}

void Stopwatch_Update(void) {
    // Ki?m tra c? 1ms t? ng?t Timer
    if (flag_timer_1ms) {
        flag_timer_1ms = 0; // Xóa c? dã x? lý

        if (is_running) {
            ms++;
            if (ms >= 1000) {
                ms = 0;
                ss++;
                
                // Yêu c?u d? bài: G?i "TIME: hh:mm:ss" m?i 1 giây
                char timeStr[20];
                Format_Time_String(timeStr, hh, mm, ss);
                UART_SendString(timeStr);

                if (ss >= 60) {
                    ss = 0;
                    mm++;
                    if (mm >= 60) {
                        mm = 0;
                        hh++;
                        if (hh >= 24) hh = 0;
                    }
                }
            }
        }
    }
}

void Stopwatch_Start(void) {
    is_running = 1;
    UART_SendString("CMD: STARTED\r\n");
}

void Stopwatch_Stop(void) {
    is_running = 0;
    UART_SendString("CMD: STOPPED\r\n");
}

void Stopwatch_Reset(void) {
    is_running = 0;
    hh = 0; mm = 0; ss = 0; ms = 0;
    lap_count = 0; // Xóa luôn danh sách Lap
    UART_SendString("CMD: RESET OK\r\n");
}

void Stopwatch_Toggle(void) {
    if (is_running) Stopwatch_Stop();
    else Stopwatch_Start();
}

void Stopwatch_Lap(void) {
    if (lap_count < 8) {
        laps[lap_count][0] = hh;
        laps[lap_count][1] = mm;
        laps[lap_count][2] = ss;
        laps[lap_count][3] = ms;
        lap_count++;
        UART_SendString("CMD: LAP SAVED\r\n");
    } else {
        UART_SendString("CMD: LAP FULL (Max 8)\r\n");
    }
}

void Stopwatch_ShowLaps(void) {
    UART_SendString("--- LAP LIST ---\r\n");
    char lapStr[30]; // "LAP x: hh:mm:ss.ms\r\n"
    unsigned int i;
    for (i = 0; i < lap_count; i++) {
        // T? ch? hàm in Lap th? công vì không dùng sprintf
        lapStr[0] = 'L'; lapStr[1] = 'A'; lapStr[2] = 'P'; lapStr[3] = ' ';
        lapStr[4] = i + '1'; // S? th? t? lap (1-8)
        lapStr[5] = ':'; lapStr[6] = ' ';
        
        IntTo2Char(laps[i][0], &lapStr[7]); // hh
        lapStr[9] = ':';
        IntTo2Char(laps[i][1], &lapStr[10]); // mm
        lapStr[12] = ':';
        IntTo2Char(laps[i][2], &lapStr[13]); // ss
        lapStr[15] = '.';
        
        // ms (l?y 2 s? d?u c?a ms cho g?n)
        IntTo2Char(laps[i][3]/10, &lapStr[16]); 
        
        lapStr[18] = '\r'; lapStr[19] = '\n'; lapStr[20] = '\0';
        
        UART_SendString(lapStr);
    }
}

