/**
  ******************************************************************************
  * @file    main.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************

  ******************************************************************************
  */

#include "main.h"
#include "gpio_control.h"
#include "vpm_control.h"
#include <stdio.h>

void GPIO_OUTPUT_STATUS_INIT()
{
  __OUT_A15_GPIO_OUT_UART_SW_SET_LO;                    //UART_SW       = LOW

  __OUT_B0_GPIO_OUT_LED_RED_SET_HI;			//LED_R-        = HIGH
  __OUT_B1_GPIO_OUT_LED_GREEN_SET_HI;			//LED_G-        = HIGH
  __OUT_B4_GPIO_OUT_5V_EN_SET_LO;			//+V5_EN        = LOW
  __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;            //DA9063_ONKEY  = LOW
  __OUT_B8_GPIO_OUT_CAN_WAKEUP_SET_LO;			//CAN_WAKEUP	= LOW
  __OUT_B9_GPIO_OUT_CAN_RST_SET_LO;			//CAN_RST	= LOW
  __OUT_B12_GPIO_OUT_VPM_INT_SET_LO;                    //VPM_INT       = LOW
  __OUT_B13_GPIO_OUT_PWM_LCD_VPM_SET_LO;		//PWM_LCD_VPM	= LOW
  __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;		//BACKLIGHT_EN	= LOW

  __OUT_C2_GPIO_OUT_UART_DB_SW_SET_LO;			//UART_DB_SW	= LOW
  __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI;		//BAT_CHARGER	= HIGH
  __OUT_C12_GPIO_OUT_DBG_LED_SET_LO;                    //DBG_LED       = LOW

  __OUT_D6_GPIO_OUT_KEYPAD_LED_EN_SET_LO;		//KEYPAD_LED_EN	= LOW
  __OUT_D7_GPIO_OUT_3V_5V_EN_SET_LO;			//3V_5V_EN	= LOW
  __OUT_D9_GPIO_OUT_12VSB_RUN_SET_HI;			//12VSB_RUN	= HIGH
  __OUT_D10_GPIO_OUT_TERM_339_SET_LO;			//TERM_339	= LOW

  //default : LOOPBACK
  __OUT_D11_GPIO_OUT_339_MODE1_SET_LO;			//339_MODE1	= LOW
  __OUT_D12_GPIO_OUT_339_MODE0_SET_LO;			//339_MODE0	= LOW

  __OUT_D14_GPIO_OUT_PWRDN_7186_SET_LO;			//PWRDN_7186	= LOW
  __OUT_D15_GPIO_OUT_RESET_7186_SET_LO;			//RESET_7186	= LOW
  VAR_TRANSCEIVER_STATUS = 0;
}
//-----------------------------------------------------------------------------

void FUNC_GPIO_INIT()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* GPIOB Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  /* GPIOE Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  /* GPIOF Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  /* ADC12 Periph clock enable */
//temp0830  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE); 					// DAC Periph clock enable

  GPIO_OUTPUT_STATUS_INIT();

/* GPIOA ============================================================> */
  // GPIO Group A

    // Input - Floating
      // __IN_A0_GPIO_IN_VPM_WAKE
      // __IN_A1_GPIO_IN_IGNITION_UP_BAR
      // __IN_A4_GPIO_IN_DI_1
      // __IN_A5_GPIO_IN_PWR_ON
      // __IN_A6_GPIO_IN_DI_3
      // __IN_A7_GPIO_IN_RTC_INT2
      // __IN_A8_GPIO_IN_DI_4
      // __IN_A11_GPIO_IN_DI_5
      // __IN_A12_GPIO_IN_DI_6
  GPIO_InitStructure.GPIO_Pin	=       GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 |
                                        GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
                                        GPIO_Pin_8 | GPIO_Pin_11| GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Input - Push Pull HIGH

    // Input - Push Pull LOW
      // __IN_A2_GPIO_IN_SOC_INT
      // __IN_A3_GPIO_IN_DEVICE_RESET_OUT_BAR
  GPIO_InitStructure.GPIO_Pin	=       GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Onput - LOW
      // __OUT_A15_GPIO_OUT_UART_SW
// PA15 Remap as GPIO
//  RCC_AHBPeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

/* GPIOB ============================================================> */
  // GPIO Group B
    // Input - Floating
      // __IN_B10_GPIO_IN_PENDING
      // __IN_B11_GPIO_IN_PENDING
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Output - LOW
      // __OUT_B4_GPIO_OUT_5V_EN
      // __OUT_B8_GPIO_OUT_CAN_WAKEUP
      // __OUT_B9_GPIO_OUT_CAN_RST
      // __OUT_B13_GPIO_OUT_PWM_LCD_VPM
      // __OUT_B14_GPIO_OUT_BACKLIGHT_EN
      // __OUT_B15_GPIO_OUT_CAN_BOOT
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 |
  					GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Output - HIGH
      // __OUT_B0_GPIO_OUT_LED_RED
      // __OUT_B1_GPIO_OUT_LED_GREEN
      // __OUT_B12_GPIO_OUT_VPM_INT
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Output - OD
      // __OUT_B5_GPIO_OUT_DA9063_ONKEY#
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

/* GPIOC ============================================================> */
  // GPIO Group C
    // ADC
      // PC0  - V_CAR_BATT_DET
      // PC1  - PCB_TEPM
      // PC4  - GPS_VCC_REF
      // PC5  - GPS_VCC_RF_R
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 |
                                	GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AN;						// GPIO Analog In/Out Mode
  GPIO_InitStructure.GPIO_PuPd  =       GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Input - Floating
      // __IN_C9_GPIO_IN_BAT_PRES
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Input - Floating, Pull-down
      // __IN_C7_GPIO_IN_EN_LED
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Output - LOW
      // __OUT_C2_GPIO_OUT_UART_DB_SW
      // __OUT_C12_GPIO_OUT_DBG_LED
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_2 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Output - HIGH
      // __OUT_C8_GPIO_OUT_BAT_CHARGER_EN
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


/* GPIOD ============================================================> */
  // GPIO Group D
    // Input - Floating
      // __IN_D2_GPIO_IN_DI_2
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Input - PU
      // __IN_D5_GPIO_IN_12V_HD_FAIL
      // __IN_D8_GPIO_IN_12VSB_PG
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_5 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Output - LOW
      // __OUT_D6_GPIO_OUT_KEYPAD_LED_EN
      // __OUT_D7_GPIO_OUT_3V_5V_EN
      // __OUT_D10_GPIO_OUT_TERM_339
      // __OUT_D11_GPIO_OUT_339_MODE1
      // __OUT_D12_GPIO_OUT_339_MODE0
      // __OUT_D14_GPIO_OUT_PWRDN#_7186
      // __OUT_D15_GPIO_OUT_RESET#_7186
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10|
  					GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_14|
  					GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Output - HIGH
      // __OUT_D9_GPIO_OUT_12VSB_RUN
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);


/* GPIOE ============================================================> */
  // GPIO Group E
    // Input - Push Pull HIGH
      // __IN_E11_GPIO_IN_F_KEY0
      // __IN_E12_GPIO_IN_F_KEY1
      // __IN_E13_GPIO_IN_F_KEY2
      // __IN_E14_GPIO_IN_F_KEY3
      // __IN_E15_GPIO_IN_F_KEY4
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13|
                                        GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
//-----------------------------------------------------------------------------

void TASK_CHECK_BATTERY_PRESENT()
{
  if (VAR_BATTERY_EXIST == 0) // 原本Battery不存在
  {
    if (__IN_C9_GPIO_IN_BAT_PRES_TEST_HI) // Battery存在
    {
      DEBUG_PRINT("@@: Battery Attached\n\r");
      VAR_BATTERY_EXIST = 1;
      VAR_SYSTEM_POWER_STATUS |= 0x10; // bit 4 = 1 for I2C CMD 0x09
    }
  }
  else // 原本Battery存在
  {
    if (__IN_C9_GPIO_IN_BAT_PRES_TEST_LO) // Battery不存在
    {
      DEBUG_PRINT("@@: Battery Disattached\n\r");
      VAR_BATTERY_EXIST = 0;
      VAR_SYSTEM_POWER_STATUS &= 0xEF; // bit 4 = 0 for I2C CMD 0x09
    }
  }
}

//-----------------------------------------------------------------------------

// 控制Power LED
// Power Off: 關閉
//
void TASK_LED_CONTROL()
{
  static int var_LED_state = 0;
  // 需要判斷目前是否為Power-Off
  // 在Power On狀態下
  if (VAR_SYSTEM_POWER_SYSTEM_STATE == 0)           // Power Off State
  {
    // LED Off
    __OUT_B0_GPIO_OUT_LED_RED_SET_HI;
    __OUT_B1_GPIO_OUT_LED_GREEN_SET_HI;
    return;
  }
  if (VAR_SYSTEM_POWER_SYSTEM_STATE == 1)           // Power On (Red)
  {
    __OUT_B0_GPIO_OUT_LED_RED_SET_LO;
    __OUT_B1_GPIO_OUT_LED_GREEN_SET_HI;
    return;
  }
  if (VAR_SYSTEM_POWER_SYSTEM_STATE == 2)           // Power On (Green)
  {
    // 若Car Power不存在
    if (CAR_POWER_EXIST)
    {
      // Car Power Exist
      // Always On
      __OUT_B0_GPIO_OUT_LED_RED_SET_HI;
      __OUT_B1_GPIO_OUT_LED_GREEN_SET_LO;
      var_LED_state = 0;
    }
    else
    {
      // Car Power Lost
      __OUT_B1_GPIO_OUT_LED_GREEN_SET_HI;
      switch (var_LED_state)
      {
        case 0:
        case 1:
        case 2: __OUT_B0_GPIO_OUT_LED_RED_SET_HI; var_LED_state++; break;
        case 3: __OUT_B0_GPIO_OUT_LED_RED_SET_LO; var_LED_state = 0; break;
      }
    }
  }
}
//-----------------------------------------------------------------------------

void TASK_AUTO_DETECT_RESERVE_GEAR()
{
  // DI1 mode return
  if (VAR_AUTO_DETECT_RESERVE_GEAR_STATUS != 1) return;
  
  // System power off return
  if (VAR_SYSTEM_POWER_SYSTEM_STATE != 2) return;

  static int last_di1_status = 1;

  // Last Status is Reserve gear
  if (last_di1_status == 0)
  {
    // Non-Reserve gear
    if (__IN_A4_GPIO_IN_DI_1_TEST_HI)
    {
      last_di1_status = 1;
      AUTO_DETECT_REAR_VIEW_EVENT = 1;
    }
  }
  // Last Status is Non-Reserve gear
  if (last_di1_status == 1)
  {
    // Reserve gear
    if (__IN_A4_GPIO_IN_DI_1_TEST_LO)
    {
      last_di1_status = 0;
      AUTO_DETECT_REAR_VIEW_EVENT = 2;
    }
  }
}
//-----------------------------------------------------------------------------

void TASK_DETECT_EN_LED_BEHAVIOR()
{
  static int last_en_led_status = 0;

  (__IN_C7_GPIO_IN_EN_LED_TEST_HI) ? (VAR_EN_LED_HI_LO = 1) : (VAR_EN_LED_HI_LO = 0);

  if (last_en_led_status == 0)                  // 上次為LO
  {
    if (VAR_EN_LED_HI_LO == 1)                  // 這次為HI
    {
      VAR_EN_LED_LO_2_HI_EVENT = 1;             // 設定發生Event
    }
  }
  else if (last_en_led_status == 1)             // 上次為HI
  {
    if (VAR_EN_LED_HI_LO == 0)                  // 這次為LO
    {
      VAR_EN_LED_HI_2_LO_EVENT = 1;             // 設定發生Event
    }
  }
  // -------------------------------------
  // 紀錄這次的Ignition Status
  last_en_led_status = VAR_EN_LED_HI_LO;
}
//-----------------------------------------------------------------------------

void TASK_DETECT_IGNITION_BEHAVIOR()
{
  static int last_ignition_status = 0;
                                                //Ignition On      : //Ignition Off
  (__IN_A1_GPIO_IN_IGNITION_UP_BAR_TEST_LO) ? (VAR_IGN_ON_OFF = 1) : (VAR_IGN_ON_OFF = 0);

  ////printf ("IGN %d\n\r",VAR_IGN_ON_OFF);
  // -------------------------------------
  // 曾經發生過Ignition Off to On or
  //           Ignition On to Off 之Event
  // 就直接Return
  if ((VAR_IGN_OFF_2_ON_EVENT != 0) ||
      (VAR_IGN_ON_2_OFF_EVENT |= 0)) return;
  // -------------------------------------
  if (last_ignition_status == 0)                // 上次為Off
  {
    if (VAR_IGN_ON_OFF == 1)                    // 這次為On
    {
      VAR_IGN_OFF_2_ON_EVENT = 1;               // 設定發生Event
    }
  }
  if (last_ignition_status == 1)                // 上次為On
  {
    if (VAR_IGN_ON_OFF == 0)                    // 這次為Off
    {
      VAR_IGN_ON_2_OFF_EVENT = 1;               // 設定發生Event
    }
  }
  // -------------------------------------
  // 紀錄這次的Ignition Status
  last_ignition_status = VAR_IGN_ON_OFF;
}
//-----------------------------------------------------------------------------

void TASK_DETECT_POWER_BUTTON_BEHAVIOR()
{
//  if (VPM_STATE != 4000) return;
  
  static int last_power_button_status = 0;
                                       //Power Button On  : //Power Button Off
  (__IN_A5_GPIO_IN_PWR_ON_TEST_LO) ? (VAR_PBT_ON_OFF = 1) : (VAR_PBT_ON_OFF = 0);

  if (last_power_button_status == 0)                // 上次為Off
  {
    if (VAR_PBT_ON_OFF == 1)                    // 這次為On
    {
      VAR_PBT_OFF_2_ON_EVENT = 1;               // 設定發生Event
    }
  }
  if (last_power_button_status == 1)                // 上次為On
  {
    if (VAR_PBT_ON_OFF == 0)                    // 這次為Off
    {
      VAR_PBT_ON_2_OFF_EVENT = 1;               // 設定發生Event
    }
  }
  // -------------------------------------
  // 紀錄這次的Power Button Status
  last_power_button_status = VAR_PBT_ON_OFF;
  // -------------------------------------
  if (VPM_STATE == 2000)
  {
    if (VAR_PBT_ON_OFF == 0)
    {
      VAR_PBT_COUNTER = 0;
    }
    if ((VAR_PBT_OFF_2_ON_EVENT == 1) && (VAR_PBT_ON_OFF == 1))
    {
      VAR_PBT_COUNTER++;
      if (VAR_PBT_COUNTER >= 200)
      {
        VAR_PBT_POWER_ON_EVENT = 1;
      }
    }
  }
  
  if ((VPM_STATE == 4000) || (VPM_STATE == 4300) || (VPM_STATE == 4400) || (VPM_STATE == 4900))
  {
  //  if ((VAR_PBT_ON_2_OFF_EVENT == 1) && (VAR_PBT_ON_OFF == 0))
    if (VAR_PBT_ON_2_OFF_EVENT == 1)
    {
      VAR_PBT_ON_2_OFF_EVENT = 0;
      // 小於一秒：開關Backlight
      if (VAR_PBT_COUNTER < 200)
      {
        if (__MACRO_CHECK_BACKLIGHT_IS_OFF)
        {
          __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
          DEBUG_PRINT ("Turn On Backlight\n\r");
        }
        else
        {
          __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;
          DEBUG_PRINT ("Turn Off Backlight\n\r");
        }
      }
    }
    if (VAR_PBT_ON_OFF == 0)
    {
      VAR_PBT_COUNTER = 0;
      __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
    }
    if ((VAR_PBT_OFF_2_ON_EVENT == 1) && (VAR_PBT_ON_OFF == 1))
    {
      VAR_PBT_COUNTER++;
      //　大於一秒 
      if (VAR_PBT_COUNTER >= 200)
      {
        // 若Backlight仍處於Off, 則開Backlight
        if (__MACRO_CHECK_BACKLIGHT_IS_OFF)
        {
          __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
          DEBUG_PRINT ("Turn On Backlight\n\r");
        }
        // 開始轉發Power Botton Pulse to OS
        __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
      }
      // 大於六秒
      if (VAR_PBT_COUNTER >= 1200)
      {
        VAR_PBT_COUNTER = 1200;
        VPM_STATE = 4500;
      }
      DEBUG_PRINT ("PBT %d\n\r",VAR_PBT_COUNTER);
    }
  }
}
//-----------------------------------------------------------------------------

void TASK_SHUT_DOWN_DETECTION()
{
  if (VAR_SHUTDOWN_STATUS == 0) return;

  if ((VAR_OS_READY == 1) && (VAR_SHUTDOWN_FLAG == 0))
  {
    DEBUG_PRINT ("SHUTDOWN COUNTER %04X\n\r", VAR_SHUTDOWN_COUNTER);
    if (VAR_SHUTDOWN_COUNTER-- <= 0)
    {
      DEBUG_PRINT ("[SD:TIMEOUT]\n\r");
      VAR_OS_READY = 0;
      VAR_SHUTDOWN_FLAG = 1;
    }
  }
}
//-----------------------------------------------------------------------------

void SUSPEND_WAKEUP_PIN_INIT()
{
  //EXTI structure to init EXT
  EXTI_InitTypeDef EXTI_InitStructure;
  //NVIC structure to set up NVIC controller
  NVIC_InitTypeDef NVIC_InitStructure;

  __MACRO_SET_EXTI_PIN(A, 1, EXTI_Trigger_Falling);		// PA1 for Ignition
  __MACRO_SET_EXTI_PIN(A, 5, EXTI_Trigger_Falling);		// PA5 for Power Button
  __MACRO_SET_EXTI_PIN(A, 7, EXTI_Trigger_Falling);		// PA7 for ALARM
}
//-----------------------------------------------------------------------------

void REINIT_AFTER_WAKEUP_RESUME_FROM_SUSPEND()
{
}
//-----------------------------------------------------------------------------

void POWER_OFF_WAKE_UP_PIN_INIT()
{
}
//-----------------------------------------------------------------------------

void REINIT_AFTER_WAKEUP_FROM_POWER_OFF()
{
}
//-----------------------------------------------------------------------------
/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SYSCLKConfig_STOP(void)
{  
  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {}
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08)
  {}
}
//=============================================================================