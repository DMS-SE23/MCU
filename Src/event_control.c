#include "main.h"
#include "event_control.h"

//Debounce Time
int POWER_BUTTON_ON_DEBOUNCE            = T_DEBOUNCE_100ms;     //Power Button On
int POWER_BUTTON_OVERRIDE_DEBOUNCE      = T_DEBOUNCE_7s;        //Power Button Override
int DC_IN_DEBOUNCE                      = T_DEBOUNCE_100ms;     //DC In
int BATTERY_ATTACHED_DEBOUNCE           = T_DEBOUNCE_100ms;     //Battery Attached

//Debounce Counter
volatile int POWER_BUTTON_ON_COUNTER            = 0;    //Power Button On
volatile int POWER_BUTTON_OVERRIDE_COUNTER      = 0;    //Power Button Override
volatile int DC_IN_COUNTER                      = 0;    //DC In
volatile int BATTERY_ATTACHED_COUNTER           = 0;    //Battery Attached

//Debounce Status
unsigned char POWER_BUTTON_ON_STATUS            = 1;    //Power Button On
unsigned char POWER_BUTTON_OVERRIDE_STATUS      = 1;    //Power Button Override
unsigned char DC_IN_STATUS                      = 1;    //DC In
unsigned char BATTERY_ATTACHED_STATUS           = 0;    //Battery Attached

//Power Button On Pressed Event
void PWR_Button_On_Pressed(void)
{
  VAR_POWER_BUTTON_POWER_ON_EVENT = 1;
}

//Power Button On Released Event
void PWR_Button_On_Released(void)
{
}

//Power Button Override Pressed Event
void PWR_Button_Override_Pressed(void)
{
  DEBUG_PRINT("@@: Power Button Override\r\n");
  VAR_POWER_BUTTON_OVERRIDE_EVENT = 1;
  POWER_BUTTON_OVERRIDE_DEBOUNCE = T_DEBOUNCE_100ms;
}

//Power Button Override Released Event
void PWR_Button_Override_Released(void)
{
  POWER_BUTTON_OVERRIDE_DEBOUNCE = T_DEBOUNCE_7s;
}

//DC In
void DC_IN(void)
{
  DEBUG_PRINT("@@: DC IN\r\n");
  CAR_POWER_EXIST = 1;
}

//DC Out
void DC_OUT(void)
{
  DEBUG_PRINT("@@: DC OUT\r\n");
  CAR_POWER_EXIST = 0;
}

//Battery Attached
void BATTERY_ATTACHED(void)
{
  DEBUG_PRINT("@@: Battery Attached\r\n");
  VAR_BATTERY_EXIST = 1;
}

//Battery Disattached
void BATTERY_DISATTACHED(void)
{
  DEBUG_PRINT("@@: Battery Disattached\r\n");
  VAR_BATTERY_EXIST = 0;
}

void NullEvent(void)
{
}

const struct Event_handler Debounce_Check[] = 
{
  //Power Button On
  {&POWER_BUTTON_ON_STATUS, &POWER_BUTTON_ON_COUNTER, &POWER_BUTTON_ON_DEBOUNCE, GPIOA, GPIO_Pin_0, PWR_Button_On_Pressed, PWR_Button_On_Released, NullEvent},
  //Power Button Override
  {&POWER_BUTTON_OVERRIDE_STATUS, &POWER_BUTTON_OVERRIDE_COUNTER, &POWER_BUTTON_OVERRIDE_DEBOUNCE, GPIOA, GPIO_Pin_0, PWR_Button_Override_Pressed, PWR_Button_Override_Released, NullEvent},
  //DC In
  {&DC_IN_STATUS, &DC_IN_COUNTER, &DC_IN_DEBOUNCE, GPIOB, GPIO_Pin_1, DC_IN, DC_OUT, NullEvent},
  //Battery Attached
  {&BATTERY_ATTACHED_STATUS, &BATTERY_ATTACHED_COUNTER, &BATTERY_ATTACHED_DEBOUNCE, GPIOB, GPIO_Pin_12, BATTERY_DISATTACHED, BATTERY_ATTACHED, NullEvent},
};



// 10ms Hook
void GPI_EventManager(void)
{
  uint8_t new_state, old_state, di_size, i;
  volatile int *counter;
  unsigned char *status;

  di_size = sizeof(Debounce_Check) / sizeof(Debounce_Check[0]);

  for (i = 0; i < di_size; i++)
  {
    counter = Debounce_Check[i].DeBounce_Counter;
    status = Debounce_Check[i].status;
    
    new_state = GPIO_ReadInputDataBit(Debounce_Check[i].GPIOx, Debounce_Check[i].GPIO_Pin);
    old_state = (*status);
    
    if(new_state != old_state)
    {
      if (*counter == 0x00)
      {
        (*counter) = (*Debounce_Check[i].DeBounce_Time);
      }
      else
      {
        (*counter)--;
        if (*counter == 0x00)
        {
          if (new_state == 0x00)
          {
            Debounce_Check[i].gpi_hi_to_lo_func();
            (*status) = new_state;
          }
          else
          {
            Debounce_Check[i].gpi_low_to_hi_func();
            (*status) = new_state;
          }
        }
      }
    }
    else
    {
      (*counter) = 0x00;
    }
    Debounce_Check[i].di_nochange();
  }
}

