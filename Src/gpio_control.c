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

#include "includes.h"

void GPIO_OUTPUT_STATUS_INIT()
{
  __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_LO;
  __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_LO;
  __OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_LO;
  __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
  __OUT_C3_GPIO_OUT_DBG_GPIO_SET_LO;
  __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO;
  __OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_LO;
  __OUT_E2_GPIO_OUT_LED_PWR_G_SET_HI;
  __OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_HI;
  __OUT_E4_GPIO_OUT_LED_RED_SET_HI;
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

  GPIO_OUTPUT_STATUS_INIT();

/* GPIOA ============================================================> */
  // GPIO Group A
    // Input - Floating
      // __IN_A0_GPIO_IN_UP_POWER_KEY_BAR
      // __IN_A6_GPIO_IN_UP_CPU_STATUS
  GPIO_InitStructure.GPIO_Pin	=       GPIO_Pin_0 | GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    // Onput - Push-pull
      // __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR
      // __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR
      // __OUT_A11_GPIO_OUT_UP_I2C_INT_BAR
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

/* GPIOB ============================================================> */
  // GPIO Group B
    // Input - Floating
      // __IN_B1_GPIO_IN_DC_IN_DETECT
      // __IN_B2_GPIO_IN_PG_VCC_3V3
      // __IN_B12_GPIO_IN_BAT_PRES
      // __IN_B14_GPIO_IN_UP_BLK_EN
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12|
                                        GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Output - Push-pull
      // __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

/* GPIOC ============================================================> */
  // GPIO Group C
    // Input - Floating
      // __IN_C13_GPIO_IN_VPM_PC13
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Output - Push-pull
      // __OUT_C3_GPIO_OUT_DBG_GPIO
      // __OUT_C8_GPIO_OUT_BAT_CHARGER_EN
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_3 | GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

/* GPIOD ============================================================> */
  // GPIO Group D  
    // Output - Push-pull
      // __OUT_D15_GPIO_OUT_PMIC_ON_REQ
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

/* GPIOE ============================================================> */
  // GPIO Group E  
    // Output - Push-pull
      // __OUT_E2_GPIO_OUT_LED_PWR_G
      // __OUT_E3_GPIO_OUT_LED_BAT_GREEN
      // __OUT_E4_GPIO_OUT_LED_RED
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
//-----------------------------------------------------------------------------

void TASK_POWER_LED_CONTROL()
{
  if ((CAR_POWER_EXIST == 1))   // DC in
  {
    // Turn on power LED
    __OUT_E2_GPIO_OUT_LED_PWR_G_SET_LO;
  }
  else
  {
    __OUT_E2_GPIO_OUT_LED_PWR_G_SET_HI;
  }
}

void TASK_BATTERY_LED_CONTROL()
{
  static int var_LED_state = 0;
  
  if (VAR_BATTERY_STATE == 0)           // Battery disattached
  {
    // Turn off battery LED
    __OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_HI;
  }
  else if (VAR_BATTERY_STATE == 1)      // Battery not fully charged
  {
    // Blinking battery LED
    if (var_LED_state)
    {
      __OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_HI;
      var_LED_state = 0;
    }
    else
    {
      __OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_LO;
      var_LED_state = 1;
    }
  }
  else if (VAR_BATTERY_STATE == 2)      // Battery fully charged
  {
    // Turn on battery LED
    __OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_LO;
  }
  else if (VAR_BATTERY_STATE == 3)      // DC out, and Battery attached
  {
    // Turn off battery LED
    __OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_HI;
  }
}

void SUSPEND_WAKEUP_PIN_INIT()
{
  //EXTI structure to init EXT
  EXTI_InitTypeDef EXTI_InitStructure;
  //NVIC structure to set up NVIC controller
  NVIC_InitTypeDef NVIC_InitStructure;

  __MACRO_SET_EXTI_PIN(A, 0, EXTI_Trigger_Falling);		// PA0 for Power Button
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