#ifndef GPIO_CONTROL
#define GPIO_CONTROL

//LED Control
#define __MACRO_CHECK_IS_LED_ON                GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_2)

//GPIO Output Control
/*** GPIOA ***/
#define	__OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_HI	        GPIOA->BSRR=0x00000001<<1
#define	__OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_LO	        GPIOA->BRR=0x00000001<<1
#define	__OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_HI	                GPIOA->BSRR=0x00000001<<7
#define	__OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_LO	                GPIOA->BRR=0x00000001<<7
/*** GPIOB ***/
#define	__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_HI	                GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
#define	__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_LO	                !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
#define	__OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI                  GPIOB->BSRR=0x00000001<<5
#define	__OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_LO      	        GPIOB->BRR=0x00000001<<5
#define	__IN_B14_GPIO_IN_UP_BLK_EN_TEST_HI	                GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define	__IN_B14_GPIO_IN_UP_BLK_EN_TEST_LO	                !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
/*** GPIOC ***/
#define	__OUT_C3_GPIO_OUT_DBG_GPIO_SET_HI			GPIOC->BSRR=0x00000001<<3
#define	__OUT_C3_GPIO_OUT_DBG_GPIO_SET_LO			GPIOC->BRR=0x00000001<<3
#define __IN_C13_GPIO_IN_VPM_PC13_TEST_HI                       GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define __IN_C13_GPIO_IN_VPM_PC13_TEST_LO                       !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
/*** GPIOD ***/
#define	__OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_HI			GPIOD->BSRR=0x00000001<<15
#define	__OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_LO			GPIOD->BRR=0x00000001<<15
/*** GPIOE ***/
#define	__OUT_E2_GPIO_OUT_LED_PWR_G_SET_HI			GPIOE->BSRR=0x00000001<<2
#define	__OUT_E2_GPIO_OUT_LED_PWR_G_SET_LO			GPIOE->BRR=0x00000001<<2

//Define Bootloader timer variable======================
extern volatile uint16_t VAR_TIMETICK_DELAY;
//======================================================

// ªì©l¤ÆGPIO PIN¸}
extern void GPIO_OUTPUT_STATUS_INIT(void);
extern void BUTTON_INIT(void);
extern void GPIO_INIT(void);
extern uint8_t CHECK_BUTTON_STATE(void);

#endif // GPIO_CONTROL
