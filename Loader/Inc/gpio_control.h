#ifndef GPIO_CONTROL
#define GPIO_CONTROL

//LED Control
#define __MACRO_CHECK_IS_LED_ON                GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0)

//GPIO Output Control
/*GPIO Group A----------------------------------------------------------------*/

/*GPIO Group B----------------------------------------------------------------*/
#define	__OUT_B0_GPIO_OUT_LED_RED_SET_HI	        GPIOB->BSRR=0x00000001<<0
#define	__OUT_B0_GPIO_OUT_LED_RED_SET_LO	        GPIOB->BRR =0x00000001<<0
#define	__OUT_B1_GPIO_OUT_LED_GREEN_SET_HI	        GPIOB->BSRR=0x00000001<<1
#define	__OUT_B1_GPIO_OUT_LED_GREEN_SET_LO	        GPIOB->BRR =0x00000001<<1
#define	__OUT_B4_GPIO_OUT_5V_EN_SET_HI	                GPIOB->BSRR=0x00000001<<4
#define	__OUT_B4_GPIO_OUT_5V_EN_SET_LO	                GPIOB->BRR =0x00000001<<4
#define	__OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI       GPIOB->BSRR=0x00000001<<5
#define	__OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO       GPIOB->BRR =0x00000001<<5
#define	__OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI          GPIOB->BSRR=0x00000001<<14
#define	__OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO	        GPIOB->BRR =0x00000001<<14

/*GPIO Group C----------------------------------------------------------------*/
#define	__IN_C7_GPIO_IN_EN_LED_TEST_HI    	        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define	__IN_C7_GPIO_IN_EN_LED_TEST_LO	                !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define	__IN_C13_GPIO_IN_ENTER_BOTLOADER_TEST_HI        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define	__IN_C13_GPIO_IN_ENTER_BOTLOADER_TEST_LO        !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)

/*GPIO Group D----------------------------------------------------------------*/
#define	__OUT_D7_GPIO_OUT_3V_5V_EN_SET_HI		GPIOD->BSRR=0x00000001<<7
#define	__OUT_D7_GPIO_OUT_3V_5V_EN_SET_LO		GPIOD->BRR =0x00000001<<7
#define	__IN_D8_GPIO_IN_12VSB_PG_TEST_HI		GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)
#define	__IN_D8_GPIO_IN_12VSB_PG_TEST_LO	        !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)
#define	__OUT_D9_GPIO_OUT_12VSB_RUN_SET_HI              GPIOD->BSRR=0x00000001<<9
#define	__OUT_D9_GPIO_OUT_12VSB_RUN_SET_LO		GPIOD->BRR =0x00000001<<9
#define	__OUT_D14_GPIO_OUT_PWRDN_7186_SET_HI            GPIOD->BSRR=0x00000001<<14
#define	__OUT_D14_GPIO_OUT_PWRDN_7186_SET_LO    	GPIOD->BRR =0x00000001<<14
#define	__OUT_D15_GPIO_OUT_RESET_7186_SET_HI            GPIOD->BSRR=0x00000001<<15
#define	__OUT_D15_GPIO_OUT_RESET_7186_SET_LO            GPIOD->BRR =0x00000001<<15

//Define Bootloader timer variable======================
extern volatile uint16_t VAR_TIMETICK_DELAY;
//======================================================

// ªì©l¤ÆGPIO PIN¸}
extern void BUTTON_INIT(void);
extern void GPIO_INIT(void);
extern uint8_t CHECK_BUTTON_STATE(void);

#endif // GPIO_CONTROL
