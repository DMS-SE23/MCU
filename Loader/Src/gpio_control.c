#include "includes.h"

void GPIO_OUTPUT_STATUS_INIT()
{
  __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_LO;
  __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_LO;
  __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
  __OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_LO;
  __OUT_E2_GPIO_OUT_LED_PWR_G_SET_HI;
}

void GPIO_INIT()
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
    // Onput - Push-pull
      // __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR
      // __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_1 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

/* GPIOB ============================================================> */
  // GPIO Group B
    // Input - Floating
      // __IN_B2_GPIO_IN_PG_VCC_3V3
      // __IN_B14_GPIO_IN_UP_BLK_EN
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_2 | GPIO_Pin_14;
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
  GPIO_InitStructure.GPIO_Pin	=	GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
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