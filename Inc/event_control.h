#ifndef __EVENT_CONTROL_H
#define __EVENT_CONTROL_H


//Event Definition==========================================
#define T_DEBOUNCE_10ms	1
#define T_DEBOUNCE_50ms	5
#define T_DEBOUNCE_100ms	10
#define T_DEBOUNCE_150ms	15
#define T_DEBOUNCE_200ms	20
#define T_DEBOUNCE_250ms	25
#define T_DEBOUNCE_300ms	30
#define T_DEBOUNCE_350ms	35
#define T_DEBOUNCE_400ms	40
#define T_DEBOUNCE_450ms	45
#define T_DEBOUNCE_500ms	50
#define T_DEBOUNCE_1s	100
#define T_DEBOUNCE_2s	200
#define T_DEBOUNCE_5s	500

struct Event_handler {
  unsigned char* status;
  volatile int* DeBounce_Counter;
  int* DeBounce_Time;
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
  void (*gpi_hi_to_lo_func)();
  void (*gpi_low_to_hi_func)();
  void (*di_nochange)();
};

//Debounce Time
extern int T_GPI_DEBOUNCE_0;

//Debounce Counter
extern volatile int POWER_BUTTON_COUNTER;

//Debounce Status
extern unsigned char POWER_BUTTON_STATUS;

//Override Counter
extern volatile int POWER_BUTTON_OVERRIDE_COUNTER;

// 10ms Hook
void GPI_EventManager(void);


#endif
