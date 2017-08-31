#include "project.h"
#include "stm32f30x_conf.h"
#include <stdio.h>        // for printf
#include "main.h"
#include "debug_port.h"
#include "hotkey_control.h"
#include "i2c_slave.h"
//-----------------------------------------------------------------------------

uint8_t hotkey_pressed = 0;                    //bit 0 = 1 => hotkey1 trigger => HOTKEY_HOME             0x01
                                               //bit 1 = 1 => hotkey2 trigger => HOTKEY_KEY_MENU         0x02
                                               //bit 2 = 1 => hotkey3 trigger => HOTKEY_BACK             0x04
                                               //bit 3 = 1 => hotkey4 trigger => HOTKEY_DOWN             0x08
                                               //bit 4 = 1 => hotkey5 trigger => HOTKEY_UP               0x10
                                               //bit 5 = 1 => hotkey6 trigger => Reserved
                                               //bit 6 = 1 => hotkey7 trigger => Reserved
                                               //bit 7 = 1 => hotkey8 trigger => HOTKEY_BACK+HOTKEY_UP   0x80
uint8_t hotkey_released = 0;                   //bit 0 = 1 => hotkey1 trigger
                                               //bit 1 = 1 => hotkey2 trigger
                                               //bit 2 = 1 => hotkey3 trigger
                                               //bit 3 = 1 => hotkey4 trigger
                                               //bit 4 = 1 => hotkey5 trigger
                                               //bit 5 = 1 => hotkey6 trigger
                                               //bit 6 = 1 => hotkey7 trigger
                                               //bit 7 = 1 => hotkey8 trigger
uint8_t hkevent_triggered = 0;                 //bit 0 = 1 => hotkey1 trigger
                                               //bit 1 = 1 => hotkey2 trigger
                                               //bit 2 = 1 => hotkey3 trigger
                                               //bit 3 = 1 => hotkey4 trigger
                                               //bit 4 = 1 => hotkey5 trigger
                                               //bit 5 = 1 => hotkey6 trigger
                                               //bit 6 = 1 => hotkey7 trigger
                                               //bit 7 = 1 => hotkey8 trigger
//uint8_t hotkey_trigger_timer = 0xFF;
uint8_t F_KEY4_trigger_timer    = 0xFF;
uint8_t F_KEY3_trigger_timer    = 0xFF;
uint8_t F_KEY2_trigger_timer    = 0xFF;
uint8_t F_KEY1_trigger_timer    = 0xFF;
uint8_t F_KEY0_trigger_timer    = 0xFF;
uint8_t CombKEY01_trigger_timer = 0xFF;
uint8_t UART_DBG_switch         = 1;
//-----------------------------------------------------------------------------


void TASK_KEYBOARD_SCAN()
{

//  /*** MENU ***/
//  if(__IN_E15_GPIO_IN_F_KEY4_TEST_LO &
//     __IN_E14_GPIO_IN_F_KEY3_TEST_HI &
//     __IN_E13_GPIO_IN_F_KEY2_TEST_HI &
//     __IN_E12_GPIO_IN_F_KEY1_TEST_HI &
//     __IN_E11_GPIO_IN_F_KEY0_TEST_HI)
//  {
//    if((hkevent_triggered&HOTKEY_KEY_MENU)!=HOTKEY_KEY_MENU)            //event not issue yet
//    {
//      if((hotkey_pressed&HOTKEY_KEY_MENU)!=HOTKEY_KEY_MENU)             //not recorded
//      {
//        hotkey_pressed |= HOTKEY_KEY_MENU;
//        F_KEY4_trigger_timer = (uint8_t)hotkey_trigger_timermax;
//        DEBUG_PRINT("Hotkey MENU pressed! \r\n");
//      }
//      else if((hotkey_pressed&HOTKEY_KEY_MENU)==HOTKEY_KEY_MENU)        //already recorded, counting
//      {
//        if(F_KEY4_trigger_timer == 0)
//        {
//          EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_KEY_MENU);
//          hkevent_triggered |= HOTKEY_KEY_MENU;                         //event issued
//          DEBUG_PRINT("Hotkey MENU event issued! \r\n");
//        }
//        else
//          F_KEY4_trigger_timer--;
//      }
//    }
//  }
//  else if(__IN_E15_GPIO_IN_F_KEY4_TEST_HI)
//  {
//    if(((hotkey_pressed&HOTKEY_KEY_MENU)==HOTKEY_KEY_MENU) && (F_KEY4_trigger_timer==0))
//    {
//      DEBUG_PRINT("Hotkey MENU released! \r\n");
//      EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_BUTTON_UP);
//      DEBUG_PRINT("Hotkey MENU BUTTON_UP event issued! \r\n");
//    }
//    hotkey_pressed &= ~HOTKEY_KEY_MENU;
//    hkevent_triggered &= ~HOTKEY_KEY_MENU;
//    F_KEY4_trigger_timer = 0xFF;
//  }
//  //-----------------
//  /*** next page ***/
//  if(__IN_E15_GPIO_IN_F_KEY4_TEST_HI &
//     __IN_E14_GPIO_IN_F_KEY3_TEST_LO &
//     __IN_E13_GPIO_IN_F_KEY2_TEST_HI &
//     __IN_E12_GPIO_IN_F_KEY1_TEST_HI &
//     __IN_E11_GPIO_IN_F_KEY0_TEST_HI)
//  {
//    if((hkevent_triggered&HOTKEY_DOWN)!=HOTKEY_DOWN)                    //event not issue yet
//    {
//      if((hotkey_pressed&HOTKEY_DOWN)!=HOTKEY_DOWN)                     //not recorded
//      {
//        hotkey_pressed |= HOTKEY_DOWN;
//        F_KEY3_trigger_timer = (uint8_t)hotkey_trigger_timermax;
//        DEBUG_PRINT("Hotkey DOWN pressed! \r\n");
//      }
//      else if((hotkey_pressed&HOTKEY_DOWN)==HOTKEY_DOWN)                //already recorded, counting
//      {
//        if(F_KEY3_trigger_timer == 0)
//        {
//          EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_DOWN);
//          hkevent_triggered |= HOTKEY_DOWN;                             //event issued
//          DEBUG_PRINT("Hotkey DOWN event issued! \r\n");
//        }
//        else
//          F_KEY3_trigger_timer--;
//      }
//    }
//  }
//  else if(__IN_E14_GPIO_IN_F_KEY3_TEST_HI)
//  {
//    if(((hotkey_pressed&HOTKEY_DOWN)==HOTKEY_DOWN) && (F_KEY3_trigger_timer==0))
//    {
//      DEBUG_PRINT("Hotkey DOWN released! \r\n");
//      EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_BUTTON_UP);
//      DEBUG_PRINT("Hotkey DOWN BUTTON_UP event issued! \r\n");
//    }
//    hotkey_pressed &= ~HOTKEY_DOWN;
//    hkevent_triggered &= ~HOTKEY_DOWN;
//    F_KEY3_trigger_timer = 0xFF;
//  }
//  //------------
//  /*** HOME ***/
//  if(__IN_E15_GPIO_IN_F_KEY4_TEST_HI &
//     __IN_E14_GPIO_IN_F_KEY3_TEST_HI &
//     __IN_E13_GPIO_IN_F_KEY2_TEST_LO &
//     __IN_E12_GPIO_IN_F_KEY1_TEST_HI &
//     __IN_E11_GPIO_IN_F_KEY0_TEST_HI)
//  {
//    if((hkevent_triggered&HOTKEY_HOME)!=HOTKEY_HOME)                    //event not issue yet
//    {
//      if((hotkey_pressed&HOTKEY_HOME)!=HOTKEY_HOME)                     //not recorded
//      {
//        hotkey_pressed |= HOTKEY_HOME;
//        F_KEY2_trigger_timer = (uint8_t)hotkey_trigger_timermax;
//        DEBUG_PRINT("Hotkey HOME pressed! \r\n");
//      }
//      else if((hotkey_pressed&HOTKEY_HOME)==HOTKEY_HOME)                //already recorded, counting
//      {
//        if(F_KEY2_trigger_timer == 0)
//        {
//          EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_HOME);
//          hkevent_triggered |= HOTKEY_HOME;                             //event issued
//          DEBUG_PRINT("Hotkey HOME event issued! \r\n");
//        }
//        else
//          F_KEY2_trigger_timer--;
//      }
//    }
//  }
//  else if(__IN_E13_GPIO_IN_F_KEY2_TEST_HI)
//  {
//    if(((hotkey_pressed&HOTKEY_HOME)==HOTKEY_HOME) && (F_KEY2_trigger_timer==0))
//    {
//      DEBUG_PRINT("Hotkey HOME released! \r\n");
//      EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_BUTTON_UP);
//      DEBUG_PRINT("Hotkey HOME BUTTON_UP event issued! \r\n");
//    }
//    hotkey_pressed &= ~HOTKEY_HOME;
//    hkevent_triggered &= ~HOTKEY_HOME;
//    F_KEY2_trigger_timer = 0xFF;
//  }
//  //---------------------
//  /*** previous page ***/
//  if(__IN_E15_GPIO_IN_F_KEY4_TEST_HI &
//     __IN_E14_GPIO_IN_F_KEY3_TEST_HI &
//     __IN_E13_GPIO_IN_F_KEY2_TEST_HI &
//     __IN_E12_GPIO_IN_F_KEY1_TEST_LO &
//     __IN_E11_GPIO_IN_F_KEY0_TEST_HI)
//  {
//    if((hkevent_triggered&HOTKEY_UP)!=HOTKEY_UP)                        //event not issue yet
//    {
//      if((hotkey_pressed&HOTKEY_UP)!=HOTKEY_UP)                         //not recorded
//      {
//        hotkey_pressed |= HOTKEY_UP;
//        F_KEY1_trigger_timer = (uint8_t)hotkey_trigger_timermax;
//        DEBUG_PRINT("Hotkey UP pressed! \r\n");
//      }
//      else if((hotkey_pressed&HOTKEY_UP)==HOTKEY_UP)                    //already recorded, counting
//      {
//        if(F_KEY1_trigger_timer == 0)
//        {
//          EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_UP);
//          hkevent_triggered |= HOTKEY_UP;                               //event issued
//          DEBUG_PRINT("Hotkey UP event issued! \r\n");
//        }
//        else
//          F_KEY1_trigger_timer--;
//      }
//    }
//  }
//  else if(__IN_E12_GPIO_IN_F_KEY1_TEST_HI)
//  {
//    if(((hotkey_pressed&HOTKEY_UP)==HOTKEY_UP) && (F_KEY1_trigger_timer==0))
//    {
//      DEBUG_PRINT("Hotkey UP released! \r\n");
//      EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_BUTTON_UP);
//      DEBUG_PRINT("Hotkey UP BUTTON_UP event issued! \r\n");
//    }
//    hotkey_pressed &= ~HOTKEY_UP;
//    hkevent_triggered &= ~HOTKEY_UP;
//    F_KEY1_trigger_timer = 0xFF;
//  }
//  //---------------------
//  /*** previous item ***/
//  if(__IN_E15_GPIO_IN_F_KEY4_TEST_HI &
//     __IN_E14_GPIO_IN_F_KEY3_TEST_HI &
//     __IN_E13_GPIO_IN_F_KEY2_TEST_HI &
//     __IN_E12_GPIO_IN_F_KEY1_TEST_HI &
//     __IN_E11_GPIO_IN_F_KEY0_TEST_LO)
//  {
//    if((hkevent_triggered&HOTKEY_BACK)!=HOTKEY_BACK)                    //event not issue yet
//    {
//      if((hotkey_pressed&HOTKEY_BACK)!=HOTKEY_BACK)                     //not recorded
//      {
//        hotkey_pressed |= HOTKEY_BACK;
//        F_KEY0_trigger_timer = (uint8_t)hotkey_trigger_timermax;
//        DEBUG_PRINT("Hotkey BACK pressed! \r\n");
//      }
//      else if((hotkey_pressed&HOTKEY_BACK)==HOTKEY_BACK)                //already recorded, counting
//      {
//        if(F_KEY0_trigger_timer == 0)
//        {
//          EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_BACK);
//          hkevent_triggered |= HOTKEY_BACK;                             //event issued
//          DEBUG_PRINT("Hotkey BACK event issued! \r\n");
//        }
//        else
//          F_KEY0_trigger_timer--;
//      }
//    }
//  }
//  else if(__IN_E11_GPIO_IN_F_KEY0_TEST_HI)
//  {
//    if(((hotkey_pressed&HOTKEY_BACK)==HOTKEY_BACK) && (F_KEY0_trigger_timer==0))
//    {
//      DEBUG_PRINT("Hotkey BACK released! \r\n");
//      EVENTQUEUE_INSERT_TO_QUEUE((unsigned char)INT_EVENT_CLASS, (unsigned char)HOTKEY_BUTTON_UP);
//      DEBUG_PRINT("Hotkey BACK BUTTON_UP event issued! \r\n");
//    }
//    hotkey_pressed &= ~HOTKEY_BACK;
//    hkevent_triggered &= ~HOTKEY_BACK;
//    F_KEY0_trigger_timer = 0xFF;
//  }
//  //---------------------------------------------------------------------------
//  /*** combination key (F_KEY0 + F_KEY1) ***/
//  if(__IN_E11_GPIO_IN_F_KEY0_TEST_LO && __IN_E12_GPIO_IN_F_KEY1_TEST_LO)
//  {
//    if((hotkey_pressed&COMB_BACK_UP) == 0)                              //not recorded
//    {
//      hotkey_pressed |= COMB_BACK_UP;
//      CombKEY01_trigger_timer = (uint8_t)hotkey_trigger_timermax;
//      DEBUG_PRINT("Hotkey BACK+UP pressed! \r\n");
//    }
//    else
//    {
//      if(CombKEY01_trigger_timer != 0)
//        CombKEY01_trigger_timer--;
//    }
//  }
//  else if(__IN_E11_GPIO_IN_F_KEY0_TEST_HI && __IN_E12_GPIO_IN_F_KEY1_TEST_HI)
//  {
//    if(((hotkey_pressed&COMB_BACK_UP)==COMB_BACK_UP) && (CombKEY01_trigger_timer==0))
//    {
//      (UART_DBG_switch == 1) ? (__OUT_C2_GPIO_OUT_UART_DB_SW_SET_HI) : (__OUT_C2_GPIO_OUT_UART_DB_SW_SET_LO);
//      UART_DBG_switch ^= 1;
//
//      DEBUG_PRINT("CPU Debug Port switch => %d!\r\n", UART_DBG_switch);
//      (UART_DBG_switch==1) ? \
//      (DEBUG_PRINT("Message change to external RS-232 connector = 1\r\n")) : \
//      (DEBUG_PRINT("Message change to internal RS-232 connector = 0\r\n"));
//    }
//    hotkey_pressed &= ~COMB_BACK_UP;
//    CombKEY01_trigger_timer = 0xFF;
//  }
}
//=============================================================================