#include "stm32f30x.h"
#include "gpio_control.h"
#include "config.h"
#include "common.h"

void GPIO_OUTPUT_STATUS_INIT()
{
  __OUT_B0_GPIO_OUT_LED_RED_SET_HI;                     //LED_R-        = HIGH
  __OUT_B1_GPIO_OUT_LED_GREEN_SET_HI;                   //LED_G-        = HIGH
  __OUT_B4_GPIO_OUT_5V_EN_SET_LO;                       //+V5_EN        = LOW
  __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;            //DA9063_ONKEY# = LOW
  __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;               //BACKLIGHT_EN	= LOW
  
  __OUT_D7_GPIO_OUT_3V_5V_EN_SET_LO;                    //3V_5V_EN      = LOW
  __OUT_D9_GPIO_OUT_12VSB_RUN_SET_HI;                   //12VSB_RUN     = HIGH
  __OUT_D14_GPIO_OUT_PWRDN_7186_SET_LO;                 //PWRDN_7186	= LOW
  __OUT_D15_GPIO_OUT_RESET_7186_SET_LO;                 //RESET_7186	= LOW
}

void GPIO_INIT()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOB Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  
  
  
  /* GPIOB ============================================================> */
    // Output - HIGH
      // __OUT_B0_GPIO_OUT_LED_RED
      // __OUT_B1_GPIO_OUT_LED_GREEN
      // __OUT_B12_GPIO_OUT_VPM_INT
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_0 | GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Output - LOW
      // __OUT_B14_GPIO_OUT_BACKLIGHT_EN
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
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
    // Input - Floating, Pull-down
      // __IN_C7_GPIO_IN_EN_LED
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* GPIOD ============================================================> */
  //GPIO Group D==============================
    // Output - LOW
      // __OUT_D7_GPIO_OUT_3V_5V_EN
      // __OUT_D14_GPIO_OUT_PWRDN#_7186
      // __OUT_D15_GPIO_OUT_RESET#_7186
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_7 | GPIO_Pin_14 | GPIO_Pin_15;
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
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Input - PU
      // __IN_D8_GPIO_IN_12VSB_PG
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
//-----------------------------------------------------------------------------


void BUTTON_INIT()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configure Button pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//-----------------------------------------------------------------------------


uint8_t CHECK_BUTTON_STATE()
{
	uint8_t val;

 	val = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

	return val;
}
//=============================================================================