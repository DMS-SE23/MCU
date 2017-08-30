#ifndef __DEBUG_PORT_H
#define __DEBUG_PORT_H

void DEBUG_PORT_INITIALIZATION(void);
void DEBUG_PRINT(const char *fmt, ...);
// 這是Heart Beat LED，要掛在每一秒的Task中
void HEART_BEAT_LED_CONTROL_TASK(void);

void Check_DEBUG_Command();
// 處理DEBUG Command 程式進入點 (Command Mode)
void DEBUG_Command_Processing();

#endif 