#ifndef HOOK_TASKS_C
#define HOOK_TASKS_C


extern volatile int VAR_1mS_Event; 
extern volatile int VAR_10mS_Event;

extern volatile unsigned int VAR_1mS_Counter;
extern volatile unsigned int VAR_10mS_Counter;

//===============================
// Hook Task Variables
//===============================

//TASK_POWER_UP
extern unsigned char sequence;

// ===============================
// Hook Tasks Main Processing Area
// ===============================

// 10mS Hook¥D¨ç¦¡
extern void TASK_Hook_10mS(int power_type);

#endif