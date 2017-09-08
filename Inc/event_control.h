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
extern int POWER_BUTTON_ON_DEBOUNCE;
extern int POWER_BUTTON_OVERRIDE_DEBOUNCE;
extern int DC_IN_DEBOUNCE;
extern int BATTERY_ATTACHED_DEBOUNCE;

//Debounce Counter
extern volatile int POWER_BUTTON_ON_COUNTER;
extern volatile int POWER_BUTTON_OVERRIDE_COUNTER;
extern volatile int DC_IN_COUNTER;
extern volatile int BATTERY_ATTACHED_COUNTER;

//Debounce Status
extern unsigned char POWER_BUTTON_ON_STATUS;
extern unsigned char POWER_BUTTON_OVERRIDE_STATUS;
extern unsigned char DC_IN_STATUS;
extern unsigned char BATTERY_ATTACHED_STATUS;

//Power Button On Pressed Event
void PWR_Button_On_Pressed(void);
//Power Button On Released Event
void PWR_Button_On_Released(void);

//Power Button Override Pressed Event
void PWR_Button_Override_Pressed(void);
//Power Button Override Released Event
void PWR_Button_Override_Released(void);

//DC In
void DC_IN(void);
//DC Out
void DC_OUT(void);

//Battery Attached
void BATTERY_ATTACHED(void);
//Battery Disattached
void BATTERY_DISATTACHED(void);

// 10ms Hook
void GPI_EventManager(void);

#endif
