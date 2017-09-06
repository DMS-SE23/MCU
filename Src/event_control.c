#include "main.h"
#include "event_control.h"

//Debounce Time
int T_GPI_DEBOUNCE_0 = T_DEBOUNCE_100ms;		//Power Button On
int T_GPI_DEBOUNCE_1 = T_DEBOUNCE_7s;		        //Power Button Override

//Debounce Counter
volatile int POWER_BUTTON_ON_COUNTER = 0;		//Power Button On
volatile int POWER_BUTTON_OVERRIDE_COUNTER = 0;         //Power Button Override

//Debounce Status
unsigned char POWER_BUTTON_ON_STATUS = 1;		//Power Button On
unsigned char POWER_BUTTON_OVERRIDE_STATUS = 1;		//Power Button Override

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
  VAR_POWER_BUTTON_OVERRIDE_EVENT = 1;
  T_GPI_DEBOUNCE_1 = T_DEBOUNCE_100ms;
}

//Power Button Override Released Event
void PWR_Button_Override_Released(void)
{
  T_GPI_DEBOUNCE_1 = T_DEBOUNCE_7s;
}

void NullEvent(void)
{
}

const struct Event_handler Debounce_Check[] = 
{
  //Power Button On
  {&POWER_BUTTON_ON_STATUS, &POWER_BUTTON_ON_COUNTER, &T_GPI_DEBOUNCE_0, GPIOA, GPIO_Pin_0, PWR_Button_On_Pressed, PWR_Button_On_Released, NullEvent},
  //Power Button Override
  {&POWER_BUTTON_OVERRIDE_STATUS, &POWER_BUTTON_OVERRIDE_COUNTER, &T_GPI_DEBOUNCE_1, GPIOA, GPIO_Pin_0, PWR_Button_Override_Pressed, PWR_Button_Override_Released, NullEvent},
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

