#ifndef __DEBUG_PORT_H
#define __DEBUG_PORT_H

void DEBUG_PORT_INITIALIZATION(void);
void DEBUG_PRINT(const char *fmt, ...);
// �o�OHeart Beat LED�A�n���b�C�@��Task��
void HEART_BEAT_LED_CONTROL_TASK(void);

void Check_DEBUG_Command();
// �B�zDEBUG Command �{���i�J�I (Command Mode)
void DEBUG_Command_Processing();

#endif 