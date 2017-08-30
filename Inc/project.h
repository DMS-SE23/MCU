#ifndef PROJECT_H
#define PROJECT_H
// 主版本, 0~255
#define __DEF_PROJECT_MAIN_VERSION   0
// 次版本, 0~255
#define __DEF_PROJECT_MINER_VERSION  93

#define __DEF_PROJECT_NAME_STRING_SIZE 12
// 平台名稱, 12個字元組 (12 Bytes)
#define __DEF_PROJECT_NAME_1  'T'
#define __DEF_PROJECT_NAME_2  'R'
#define __DEF_PROJECT_NAME_3  'E'
#define __DEF_PROJECT_NAME_4  'K'
#define __DEF_PROJECT_NAME_5  '-'
#define __DEF_PROJECT_NAME_6  '7'
#define __DEF_PROJECT_NAME_7  '3'
#define __DEF_PROJECT_NAME_8  '4'
// 配合PCB版號
#define __DEF_PROJECT_NAME_9 '-'
#define __DEF_PROJECT_NAME_10 'A'
#define __DEF_PROJECT_NAME_11 '0'
#define __DEF_PROJECT_NAME_12 '1'

// 12V的CAR POWER PROTECTION RANGE
// |(10.11V)--------(11.43V)--------(12.26V)|
//   MIN             DEFAULT         MAX
// 0x05C4 = 12.26V
// 0x0560 = 11.43V
// 0x04C1 = 10.11V
#define __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_H     0x05
#define __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_L     0xC4
#define __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_H     0x04
#define __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_L     0xC1
#define __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H 0x05
#define __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L 0x60
// 24V的CAR POWER PROTECTION RANGE
// |(21.09V)--------(22.42V)--------(23.29V)|
//   MIN             DEFAULT         MAX
// 0x0ABD = 23.29V
// 0x0A56 = 22.42V
// 0x09B9 = 21.09V
#define __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_H     0x0A
#define __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_L     0xBD
#define __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_H     0x09
#define __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_L     0xB9
#define __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H 0x0A
#define __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L 0x56

#define __DEF_12V_CAR_PWR_VOLTAGE_UNIT            0.008306
#define __DEF_24V_CAR_PWR_VOLTAGE_UNIT            0.008472

// VPM EVENT TIMEOUT DELAY MAX
#define __DEF_PWR_ON_DLY_MIN                  0x0001
#define __DEF_PWR_ON_DLY_MAX                  0x4650
#define __DEF_PWR_OFF_EVT_DLY_MIN             0x0001
#define __DEF_PWR_OFF_EVT_DLY_MAX             0x4650
#define __DEF_IGN_OFF_PWR_OFF_HARD_DLY_MIN    0x0001
#define __DEF_IGN_OFF_PWR_OFF_HARD_DLY_MAX    0x4650
#define __DEF_POWER_LOW_EVT_DLY_MIN           0x0001
#define __DEF_POWER_LOW_EVT_DLY_MAX           0x0E10
#define __DEF_POWER_LOW_HARD_DLY_MIN          0x0001
#define __DEF_POWER_LOW_HARD_DLY_MAX          0x0E10
#define __DEF_POST_PWR_CHK_DLY_MIN            0x0001
#define __DEF_POST_PWR_CHK_DLY_MAX            0x4650
#define __DEF_WDG_COUNTDOWN_TIMER_MIN         0x0001
#define __DEF_WDG_COUNTDOWN_TIMER_MAX         0xFFFF
#define __DEF_SHUT_DOWN_DLY_MIN               0x0001
#define __DEF_SHUT_DOWN_DLY_MAX               0x0E10

#define __DEF_EVENT_QUEUE_SIZE      64
#define __DEF_SERIAL_NUMBER_SIZE    10

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
#define GPS_CmdSize   128
#define DEBUG_CmdSize 128

// Define for I2C1
#define DEF_BQBQ27541_ADDRESS  0x16                         //0001-0110 => W
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
#define	__IN_A0_GPIO_IN_VPM_WAKE_TEST_HI	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define	__IN_A0_GPIO_IN_VPM_WAKE_TEST_LO	                !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define	__IN_A1_GPIO_IN_IGNITION_UP_BAR_TEST_HI	                GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define	__IN_A1_GPIO_IN_IGNITION_UP_BAR_TEST_LO	                !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define	__IN_A2_GPIO_IN_SOC_INT_TEST_HI 	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define	__IN_A2_GPIO_IN_SOC_INT_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define	__IN_A3_GPIO_IN_DEVICE_RESET_OUT_BAR_TEST_HI	        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define	__IN_A3_GPIO_IN_DEVICE_RESET_OUT_BAR_TEST_LO            !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define	__IN_A4_GPIO_IN_DI_1_TEST_HI     	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define	__IN_A4_GPIO_IN_DI_1_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define	__IN_A5_GPIO_IN_PWR_ON_TEST_HI     	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define	__IN_A5_GPIO_IN_PWR_ON_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define	__IN_A6_GPIO_IN_DI_3_TEST_HI    	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define	__IN_A6_GPIO_IN_DI_3_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define	__IN_A7_GPIO_IN_RTC_INT2_TEST_HI 	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define	__IN_A7_GPIO_IN_RTC_INT2_TEST_LO	                !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define	__IN_A8_GPIO_IN_DI_4_TEST_HI    	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define	__IN_A8_GPIO_IN_DI_4_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define	__IN_A11_GPIO_IN_DI_5_TEST_HI    	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define	__IN_A11_GPIO_IN_DI_5_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define	__IN_A12_GPIO_IN_DI_6_TEST_HI    	        	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define	__IN_A12_GPIO_IN_DI_6_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define	__OUT_A15_GPIO_OUT_UART_SW_SET_HI	        	GPIOA->BSRR=0x00000001<<15
#define	__OUT_A15_GPIO_OUT_UART_SW_SET_LO	        	GPIOA->BRR=0x00000001<<15
/*** GPIOB ***/
#define	__OUT_B0_GPIO_OUT_LED_RED_SET_HI	        	GPIOB->BSRR=0x00000001<<0
#define	__OUT_B0_GPIO_OUT_LED_RED_SET_LO	        	GPIOB->BRR=0x00000001<<0
#define	__OUT_B1_GPIO_OUT_LED_GREEN_SET_HI	        	GPIOB->BSRR=0x00000001<<1
#define	__OUT_B1_GPIO_OUT_LED_GREEN_SET_LO	        	GPIOB->BRR=0x00000001<<1
#define	__OUT_B4_GPIO_OUT_5V_EN_SET_HI	                        GPIOB->BSRR=0x00000001<<4
#define	__OUT_B4_GPIO_OUT_5V_EN_SET_LO	                        GPIOB->BRR=0x00000001<<4
#define	__OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI               GPIOB->BSRR=0x00000001<<5
#define	__OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO      	        GPIOB->BRR=0x00000001<<5
#define	__OUT_B8_GPIO_OUT_CAN_WAKEUP_SET_HI		        GPIOB->BSRR=0x00000001<<8
#define	__OUT_B8_GPIO_OUT_CAN_WAKEUP_SET_LO			GPIOB->BRR=0x00000001<<8
#define	__OUT_B9_GPIO_OUT_CAN_RST_SET_HI			GPIOB->BSRR=0x00000001<<9
#define	__OUT_B9_GPIO_OUT_CAN_RST_SET_LO			GPIOB->BRR=0x00000001<<9
#define __OUT_B12_GPIO_OUT_VPM_INT_SET_HI                       GPIOB->BSRR=0x00000001<<12
#define __OUT_B12_GPIO_OUT_VPM_INT_SET_LO                       GPIOB->BRR=0x00000001<<12
#define	__OUT_B13_GPIO_OUT_PWM_LCD_VPM_SET_HI			GPIOB->BSRR=0x00000001<<13
#define	__OUT_B13_GPIO_OUT_PWM_LCD_VPM_SET_LO			GPIOB->BRR=0x00000001<<13
#define	__OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI                  GPIOB->BSRR=0x00000001<<14
#define	__OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO	                GPIOB->BRR=0x00000001<<14
#define	__OUT_B15_GPIO_OUT_CAN_BOOT_SET_HI			GPIOB->BSRR=0x00000001<<15
#define	__OUT_B15_GPIO_OUT_CAN_BOOT_SET_LO			GPIOB->BRR=0x00000001<<15
/*** GPIOC ***/
#define	__IN_C0_GPIO_IN_V_CAR_BATT_DET_TEST_HI			GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define	__IN_C0_GPIO_IN_V_CAR_BATT_DET_TEST_LO		        !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define	__IN_C1_GPIO_IN_PCB_TEPM_TEST_HI			GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define	__IN_C1_GPIO_IN_PCB_TEPM_TEST_LO			!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define	__OUT_C2_GPIO_OUT_UART_DB_SW_SET_HI			GPIOC->BSRR=0x00000001<<2
#define	__OUT_C2_GPIO_OUT_UART_DB_SW_SET_LO			GPIOC->BRR=0x00000001<<2
#define	__IN_C4_GPIO_IN_GPS_VCC_REF_TEST_HI    	                GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
#define	__IN_C4_GPIO_IN_GPS_VCC_REF_TEST_LO	                !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
#define	__IN_C5_GPIO_IN_GPS_VCC_RF_R_TEST_HI			GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define	__IN_C5_GPIO_IN_GPS_VCC_RF_R_TEST_LO		        !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define	__IN_C7_GPIO_IN_EN_LED_TEST_HI    	        	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define	__IN_C7_GPIO_IN_EN_LED_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define	__OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI                 GPIOC->BSRR=0x00000001<<8
#define	__OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO	                GPIOC->BRR=0x00000001<<8
#define	__IN_C9_GPIO_IN_BAT_PRES_TEST_HI    	                GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define	__IN_C9_GPIO_IN_BAT_PRES_TEST_LO	                !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define __OUT_C12_GPIO_OUT_DBG_LED_SET_HI                       GPIOC->BSRR=0x00000001<<12
#define __OUT_C12_GPIO_OUT_DBG_LED_SET_LO                       GPIOC->BRR=0x00000001<<12
/*** GPIOD ***/
#define	__IN_D2_GPIO_IN_DI_2_TEST_HI     	        	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define	__IN_D2_GPIO_IN_DI_2_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define	__IN_D5_GPIO_IN_12V_HD_FAIL_TEST_HI			GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)
#define	__IN_D5_GPIO_IN_12V_HD_FAIL_TEST_LO			!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)
#define	__OUT_D6_GPIO_OUT_KEYPAD_LED_EN_SET_HI			GPIOD->BSRR=0x00000001<<6
#define	__OUT_D6_GPIO_OUT_KEYPAD_LED_EN_SET_LO			GPIOD->BRR=0x00000001<<6
#define	__OUT_D7_GPIO_OUT_3V_5V_EN_SET_HI			GPIOD->BSRR=0x00000001<<7
#define	__OUT_D7_GPIO_OUT_3V_5V_EN_SET_LO			GPIOD->BRR=0x00000001<<7
#define	__IN_D8_GPIO_IN_12VSB_PG_TEST_HI			GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)
#define	__IN_D8_GPIO_IN_12VSB_PG_TEST_LO			!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)
#define	__OUT_D9_GPIO_OUT_12VSB_RUN_SET_HI			GPIOD->BSRR=0x00000001<<9
#define	__OUT_D9_GPIO_OUT_12VSB_RUN_SET_LO			GPIOD->BRR=0x00000001<<9
#define	__OUT_D10_GPIO_OUT_TERM_339_SET_HI			GPIOD->BSRR=0x00000001<<10
#define	__OUT_D10_GPIO_OUT_TERM_339_SET_LO			GPIOD->BRR=0x00000001<<10
#define	__OUT_D11_GPIO_OUT_339_MODE1_SET_HI			GPIOD->BSRR=0x00000001<<11
#define	__OUT_D11_GPIO_OUT_339_MODE1_SET_LO			GPIOD->BRR=0x00000001<<11
#define	__OUT_D12_GPIO_OUT_339_MODE0_SET_HI			GPIOD->BSRR=0x00000001<<12
#define	__OUT_D12_GPIO_OUT_339_MODE0_SET_LO			GPIOD->BRR=0x00000001<<12
#define	__OUT_D14_GPIO_OUT_PWRDN_7186_SET_HI			GPIOD->BSRR=0x00000001<<14
#define	__OUT_D14_GPIO_OUT_PWRDN_7186_SET_LO			GPIOD->BRR=0x00000001<<14
#define	__OUT_D15_GPIO_OUT_RESET_7186_SET_HI			GPIOD->BSRR=0x00000001<<15
#define	__OUT_D15_GPIO_OUT_RESET_7186_SET_LO			GPIOD->BRR=0x00000001<<15
/*** GPIOE ***/
#define	__IN_E11_GPIO_IN_F_KEY0_TEST_HI				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)
#define	__IN_E11_GPIO_IN_F_KEY0_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)
#define	__IN_E12_GPIO_IN_F_KEY1_TEST_HI				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define	__IN_E12_GPIO_IN_F_KEY1_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define	__IN_E13_GPIO_IN_F_KEY2_TEST_HI				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define	__IN_E13_GPIO_IN_F_KEY2_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define	__IN_E14_GPIO_IN_F_KEY3_TEST_HI				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)
#define	__IN_E14_GPIO_IN_F_KEY3_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)
#define	__IN_E15_GPIO_IN_F_KEY4_TEST_HI				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)
#define	__IN_E15_GPIO_IN_F_KEY4_TEST_LO	                        !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)
          
#define __MACRO_CHECK_BACKLIGHT_IS_ON                           GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14)
#define __MACRO_CHECK_BACKLIGHT_IS_OFF                          !GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14)

// 定義CPU Serial Number位址 =================================================================
#define DevID_SNo0     *((u16 *)0x1FFFF7E8);
#define DevID_SNo1     *((u32 *)0x1FFFF7EC);
#define DevID_SNo2     *((u32 *)0x1FFFF7F0);


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

// RTC結構之定義
typedef union
{
  unsigned char rtc_data[7];
  struct
  {
    unsigned char Year;
    unsigned char Month;
    unsigned char DayOfMonth;
    unsigned char DayOfWeek;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
  };
} RTC_TIME;

// ALARM結構之定義
typedef union
{
  unsigned char alarm_data[3];
  struct
  {
    unsigned char DayOfWeek;
    unsigned char Hour;
    unsigned char Minute;
  };
} ALARM_TIME;

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