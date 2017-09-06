#ifndef __EVENT_CONTROL_H
#define __EVENT_CONTROL_H


//Event Definition==========================================
#define T_DEBOUNCE_100ms	10
#define T_DEBOUNCE_7s	        700

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
extern int T_GPI_DEBOUNCE_1;

//Debounce Counter
extern volatile int POWER_BUTTON_ON_COUNTER;
extern volatile int POWER_BUTTON_OVERRIDE_COUNTER;

//Debounce Status
extern unsigned char POWER_BUTTON_ON_STATUS;
extern unsigned char POWER_BUTTON_OVERRIDE_STATUS;

//Power Button On Pressed Event
void PWR_Button_On_Pressed(void);
//Power Button On Released Event
void PWR_Button_On_Released(void);

//Power Button Override Pressed Event
void PWR_Button_Override_Pressed(void);
//Power Button Override Released Event
void PWR_Button_Override_Released(void);

// 10ms Hook
void GPI_EventManager(void);


#endif
