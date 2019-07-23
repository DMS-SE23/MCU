#ifndef PROJECT_H
#define PROJECT_H
// 主版本, 0~255
#define __DEF_PROJECT_MAIN_VERSION   0
// 次版本, 0~255
#define __DEF_PROJECT_MINER_VERSION  103

#define __DEF_PROJECT_NAME_STRING_SIZE 12
// 平台名稱, 12個字元組 (12 Bytes)
#define __DEF_PROJECT_NAME_1  'D'
#define __DEF_PROJECT_NAME_2  'M'
#define __DEF_PROJECT_NAME_3  'S'
#define __DEF_PROJECT_NAME_4  '-'
#define __DEF_PROJECT_NAME_5  'S'
#define __DEF_PROJECT_NAME_6  'E'
#define __DEF_PROJECT_NAME_7  '2'
#define __DEF_PROJECT_NAME_8  '3'
// 配合PCB版號
#define __DEF_PROJECT_NAME_9 '-'
#define __DEF_PROJECT_NAME_10 'A'
#define __DEF_PROJECT_NAME_11 '0'
#define __DEF_PROJECT_NAME_12 '2'

// VPM EVENT TIMEOUT DELAY MAX
#define __DEF_WDG_COUNTDOWN_TIMER_MIN         1
#define __DEF_WDG_COUNTDOWN_TIMER_MAX         65535

#define __DEF_EVENT_QUEUE_SIZE      64

// 定義取得某一位數的值，並轉成ASC II Code
// 取得百位數
#define __MACRO_GET_DIGITAL_IN_HUNDREDS(x) ((x/100)%10)+0x30
// 取得十位數
#define __MACRO_GET_DIGITAL_IN_TENS(x)     ((x/10)%10)+0x30
// 取得個位數
#define __MACRO_GET_DIGITAL_IN_UNITS(x)    (x%10)+0x30

// Define for Protocol
#define RxBufferSize  128
#define TxBufferSize  128
#define DEBUG_CmdSize 128

// Define for I2C1
#define DEF_BQ40Z50_ADDRESS  0x16                         //0001-0110 => W
                                                          //0001-0111 => R
// ----------------------
// EEPROM讀寫相關巨集
// ----------------------
// Wait Inverse Event 巨集
#define TIME_OUT_COUNT_FOR_I2C  0x17FF

    // Wait Inverse Event 巨集
#define WAIT_I2C_iEVENT(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C;\
        while(!I2C_CheckEvent(I2C1, x))\
          {if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};
    // Wait Event 巨集
#define WAIT_I2C_FLAG_STATUS(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C; \
        while(I2C_GetFlagStatus(I2C1, x)) \
          { if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};

    // Wait Inverse Event 巨集
#define EEPROM_WAIT_I2C_iEVENT(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C;\
        while(!I2C_CheckEvent(I2C1, x))\
          {if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};
    // Wait Event 巨集
#define EEPROM_WAIT_I2C_FLAG_STATUS(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C; \
        while(I2C_GetFlagStatus(I2C1, x)) \
          { if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};

    // Wait Inverse Event 巨集
#define RTC_WAIT_I2C_iEVENT(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C;\
        while(!I2C_CheckEvent(I2C1, x))\
          {if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};
    // Wait Event 巨集
#define RTC_WAIT_I2C_FLAG_STATUS(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C; \
        while(I2C_GetFlagStatus(I2C1, x)) \
          { if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};

    // Wait Inverse Event 巨集
#define BATTERY_WAIT_I2C_iEVENT(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C;\
        while(!I2C_CheckEvent(I2C1, x))\
          {if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};
    // Wait Event 巨集
#define BATTERY_WAIT_I2C_FLAG_STATUS(x)\
        TimeOutCounter = TIME_OUT_COUNT_FOR_I2C; \
        while(I2C_GetFlagStatus(I2C1, x)) \
          { if (TimeOutCounter-- == 0) {I2C_GenerateSTOP(I2C1, ENABLE); LastErrorOccurse = 1; return (-1);}};

// ----------------------
// GPIO設定輸出狀態與輸入測試巨集
// ----------------------
/*** GPIOA ***/
#define	__IN_A0_GPIO_IN_UP_POWER_KEY_BAR_TEST_HI	        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define	__IN_A0_GPIO_IN_UP_POWER_KEY_BAR_TEST_LO	        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define	__OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_HI	        GPIOA->BSRR=0x00000001<<1
#define	__OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_LO	        GPIOA->BRR=0x00000001<<1
#define	__IN_A6_GPIO_IN_UP_CPU_STATUS_TEST_HI    	        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define	__IN_A6_GPIO_IN_UP_CPU_STATUS_TEST_LO	                !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define	__OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_HI	                GPIOA->BSRR=0x00000001<<7
#define	__OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_LO	                GPIOA->BRR=0x00000001<<7
#define	__OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_HI	        GPIOA->BSRR=0x00000001<<11
#define	__OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_LO	        GPIOA->BRR=0x00000001<<11
/*** GPIOB ***/
#define	__IN_B1_GPIO_IN_DC_IN_DETECT_TEST_HI	                GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define	__IN_B1_GPIO_IN_DC_IN_DETECT_TEST_LO	                !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define	__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_HI	                GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
#define	__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_LO	                !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
#define	__OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI                  GPIOB->BSRR=0x00000001<<5
#define	__OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_LO      	        GPIOB->BRR=0x00000001<<5
#define	__IN_B12_GPIO_IN_BAT_PRES_TEST_HI	                GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define	__IN_B12_GPIO_IN_BAT_PRES_TEST_LO	                !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define	__IN_B14_GPIO_IN_UP_BLK_EN_TEST_HI	                GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define	__IN_B14_GPIO_IN_UP_BLK_EN_TEST_LO	                !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
/*** GPIOC ***/
#define	__OUT_C3_GPIO_OUT_DBG_GPIO_SET_HI			GPIOC->BSRR=0x00000001<<3
#define	__OUT_C3_GPIO_OUT_DBG_GPIO_SET_LO			GPIOC->BRR=0x00000001<<3
#define	__OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI                 GPIOC->BSRR=0x00000001<<8
#define	__OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO	                GPIOC->BRR=0x00000001<<8
#define __IN_C13_GPIO_IN_VPM_PC13_TEST_HI                       GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define __IN_C13_GPIO_IN_VPM_PC13_TEST_LO                       !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
/*** GPIOD ***/
#define	__OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_HI			GPIOD->BSRR=0x00000001<<15
#define	__OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_LO			GPIOD->BRR=0x00000001<<15
/*** GPIOE ***/
#define	__OUT_E2_GPIO_OUT_LED_PWR_G_SET_HI			GPIOE->BSRR=0x00000001<<2
#define	__OUT_E2_GPIO_OUT_LED_PWR_G_SET_LO			GPIOE->BRR=0x00000001<<2
#define	__OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_HI                  GPIOE->BSRR=0x00000001<<3
#define	__OUT_E3_GPIO_OUT_LED_BAT_GREEN_SET_LO	                GPIOE->BRR=0x00000001<<3
#define	__OUT_E4_GPIO_OUT_LED_RED_SET_HI                        GPIOE->BSRR=0x00000001<<4
#define	__OUT_E4_GPIO_OUT_LED_RED_SET_LO	                GPIOE->BRR=0x00000001<<4
#define	__OUT_E8_GPIO_OUT_AMP_DISABLE_SET_HI			GPIOE->BSRR=0x00000001<<8
#define	__OUT_E8_GPIO_OUT_AMP_DISABLE_SET_LO			GPIOE->BRR=0x00000001<<8
#define	__OUT_E9_GPIO_OUT_AMP_MUTE_SET_HI                       GPIOE->BSRR=0x00000001<<9
#define	__OUT_E9_GPIO_OUT_AMP_MUTE_SET_LO	                GPIOE->BRR=0x00000001<<9
#define	__OUT_E10_GPIO_OUT_AMP_12V_EN_SET_HI                    GPIOE->BSRR=0x00000001<<10
#define	__OUT_E10_GPIO_OUT_AMP_12V_EN_SET_LO	                GPIOE->BRR=0x00000001<<10

// 定義CPU Serial Number位址 =================================================================
#define DevID_SNo0     *((u32 *)0x1FFFF7AC);
#define DevID_SNo1     *((u32 *)0x1FFFF7B0);
#define DevID_SNo2     *((u32 *)0x1FFFF7B4);


// 定義 Shared Data with Boot Loader
#pragma location=0x20007FF8
__no_init volatile int BOOT_SIG;

// 定義 for Check是否為Reboot
#pragma location=0x20007FF0
__no_init volatile int BOOT_CHECK_REBOOT;

//定義Bootloader Starting address in flash
#define BootloaderAddress 0x8000000

//Information data offset in flash, count from backwards
#define FLASH_SIZE		(0x40000)
#define CMP_TAG_OFFSET		0x00
#define CHKSUM_TAG_OFFSET	0x02
#define USR_AREA_CHKSUM_OFFSET	0x06
#define AP_VERSION_OFFSET	0x0A
#define USR_AP_END_ADD_OFFSET	0x0C
#define BSL_INFO_OFFSET		0x10
#define BSL_VERSION_OFFSET	0x20
//Information data size
#define COMPLETE_TAG_SIZE	0x02
#define CHKSUM_TAG_SIZE		0x04
#define USR_AREA_CHKSUM_SIZE    0x04
#define AP_VERSION_SIZE		0x02
#define AP_END_ADD_SIZE		0x04
#define BSL_INFO_SIZE		0x10
#define BSL_VERSION_SIZE	0x02
//Checksum Tag Definition
#define CHK_ALL_TIME		0xFFFFFFFF
#define CHK_ONCE		0x0000FFFF
#define DO_NOT_CHK		0x00000000
//Complete Tag Definition
#define COMPLETED		0xAAAA
#define NOT_COMPLETED		0xFFFF

#define __MACRO_VPM_TRACE(x) if (VAR_TRACE_VPM_STATE == 1) DEBUG_PRINT(x);
#define __DEBUG_VPM_TRACE(x) if (VAR_DEBUG_PRINT == 0) { if (VAR_TRACE_VPM_STATE == 1) { DEBUG_PRINT(x); VAR_DEBUG_PRINT = 1;}}
#define __MACRO_CHANGE_VPM_STATE_TO(x) VPM_STATE =x; VAR_DEBUG_PRINT=0;

// Delay mS 的 Macro
#define __MACRO_DELAY_mS(x)\
  VAR_TIMETICK_DELAY = x;\
  while (VAR_TIMETICK_DELAY){}

// ----------------------
// 定義回傳成功失敗之定義
// ----------------------
#define __RETURN_SUCCESS    0
#define __RETURN_FAIL       -1

// Timer Tick用巨集

// EXTI 相關MACRO
#define MACRO_IRQ0	EXTI0_IRQn
#define MACRO_IRQ1	EXTI1_IRQn
#define MACRO_IRQ3	EXTI3_IRQn
#define MACRO_IRQ4	EXTI4_IRQn
#define MACRO_IRQ5	EXTI9_5_IRQn
#define MACRO_IRQ6	EXTI9_5_IRQn
#define MACRO_IRQ7	EXTI9_5_IRQn
#define MACRO_IRQ8	EXTI9_5_IRQn
#define MACRO_IRQ9	EXTI9_5_IRQn
#define MACRO_IRQ10	EXTI15_10_IRQn
#define MACRO_IRQ11	EXTI15_10_IRQn
#define MACRO_IRQ12	EXTI15_10_IRQn
#define MACRO_IRQ13	EXTI15_10_IRQn
#define MACRO_IRQ14	EXTI15_10_IRQn
#define MACRO_IRQ15	EXTI15_10_IRQn

#define __MACRO_SET_EXTI_PIN(x,y,z) \
  /* Clear pending */\
  EXTI_ClearITPendingBit(EXTI_PinSource##y);\
  /* Configure Button EXTI line */\
  EXTI_InitStructure.EXTI_Line = EXTI_PinSource##y;\
  /* select interrupt mode */\
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;\
  /* generate interrupt on rising edge */\
  EXTI_InitStructure.EXTI_Trigger = z;\
  /* enable EXTI line */\
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;\
  /* send values to registers */\
  EXTI_Init(&EXTI_InitStructure);\
  /* ================================ */\
  /* select NVIC channel to configure */\
  NVIC_InitStructure.NVIC_IRQChannel = MACRO_IRQ##y;\
  /* set priority to lowest */\
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;\
  /* set subpriority to lowest */\
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;\
  /* enable IRQ channel */\
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;\
  /* update NVIC registers */\
  NVIC_Init(&NVIC_InitStructure);

#endif