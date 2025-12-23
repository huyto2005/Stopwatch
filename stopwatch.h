#ifndef STOPWATCH_H
#define STOPWATCH_H

/* Kh?i t?o các giá tr? ban d?u */
void Stopwatch_Init(void);

/* Hàm c?p nh?t th?i gian (G?i liên t?c trong while(1)) */
void Stopwatch_Update(void);

/* Các hàm di?u khi?n logic */
void Stopwatch_Start(void);
void Stopwatch_Stop(void);
void Stopwatch_Reset(void);
void Stopwatch_Toggle(void); // Ð?o tr?ng thái Start/Stop (cho nút b?m)
void Stopwatch_Lap(void);
void Stopwatch_ShowLaps(void);

#endif /* STOPWATCH_H */

