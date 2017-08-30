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

#ifndef MAIN_C
#define MAIN_C

#include "stm32f30x_syscfg.h"
#include "stm32f30x_exti.h"
#include "stm32f30x_conf.h"
#include "stm32f30x_dbgmcu.h"
#include "project.h"
#include "eeprom_control.h"
#include "debug_port.h"
#include <stdarg.h>
#include <stdio.h>        // for printf
#include <stdlib.h>

// Variable for 100uS Tick Event, indicates the event occurse
// This variable is rise high in time tick interrupt

// Event Flags
extern int VAR_1mS_Event;
extern int VAR_5mS_Event;
extern int VAR_10mS_Event;
extern int VAR_25mS_Event;
extern int VAR_100mS_Event;
extern int VAR_1S_Event;
// Event Counting Used Variables
extern int VAR_1mS_Counter;
extern int VAR_5mS_Counter;
extern int VAR_10mS_Counter;
extern int VAR_25mS_Counter;
extern int VAR_100mS_Counter;
extern int VAR_1S_Counter;

// 做延時用的變數
extern volatile int VAR_TIMETICK_DELAY;

// GPS 用的變數
extern volatile int VAR_GPS_PACKET_READY;
extern volatile int VAR_GPS_CMD_LENGTH;
extern unsigned char GPS_Cmd[GPS_CmdSize];
extern u8 GPS_TxBuffer[TxBufferSize];
extern u8 GPS_RxBuffer[RxBufferSize];
extern u16 GPS_TxS, GPS_TxE; // Tx Start, End Ptr
extern u16 GPS_RxS, GPS_RxE; // Rx Start, End Ptr

// DEBUG 用的變數
extern volatile int VAR_DEBUG_PACKET_READY;
extern volatile int VAR_DEBUG_CMD_LENGTH;
extern unsigned char DEBUG_Cmd[DEBUG_CmdSize];
extern u8 DEBUG_TxBuffer[TxBufferSize];
extern u8 DEBUG_RxBuffer[RxBufferSize];
extern u16 DEBUG_TxS, DEBUG_TxE; // Tx Start, End Ptr
extern u16 DEBUG_RxS, DEBUG_RxE; // Rx Start, End Ptr

extern unsigned int VAR_BATTERY_EXIST; // Battery是否存在，預設不存在
extern unsigned int CAR_POWER_EXIST;   // Car Power是否存在，預設不存在

extern unsigned int VAR_CURRENT_V_CB_VALUE; // 目前之CAR Power Value
extern volatile unsigned int VAR_PCB_TEMP_VALUE; // 目前之PCB Temperture Value
extern volatile unsigned int VAR_GPS_REF_VALUE;  // 目前之GPS REF Value
extern volatile unsigned int VAR_GPS_RF_VALUE; // 目前之GPS RF Value
extern volatile unsigned int VAR_ODOMETER_VALUE; // Odometer counter
extern volatile unsigned char VAR_SYSTEM_POWER_STATUS;

// Multi-protocol Transceiver Status
extern volatile unsigned int VAR_TRANSCEIVER_STATUS;

// State machine for dump RTC registers
// = 0, Not Dump
// = 1, Start Dump
//extern volatile unsigned int START_DUMP_RTC_S35390A_REGISTERS = 0;
extern volatile unsigned int VAR_START_DUMP_RTC_S35390A_REGISTERS;

// Car Power System Information
extern unsigned char VAR_SYSI2C_SYS_INFO;       // EEPROM $0025
extern unsigned int VAR_SYSI2C_SYS_INFO_CHANGE;

extern volatile int I2C_USER_SET_RTC_TIME_EVENT; // User透過I2C設定RTC
extern unsigned char VAR_USER_CHANGE_RTC_VALUE; // User透過I2C設定之RTC暫存用變數
//extern long STM32RTC_CounterValue;

extern volatile int I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT; // User透過I2C設定WDG_COUNTDOWN_TIMER
extern unsigned int VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE; // User透過I2C設定之WDG_COUNTDOWN_TIMER暫存用變數

extern volatile int I2C_USER_SET_ALARM_TIME_EVENT; // User透過I2C設定ALARM
extern unsigned char VAR_USER_CHANGE_ALARM_VALUE; // User透過I2C設定之ALARM暫存用變數

extern volatile int I2C_USER_LOAD_EEPROM_DEFAULT_EVENT;  // User透過I2C Load EEPROM

extern volatile int I2C_USER_SET_REAR_VIEW_EVENT;  // User透過I2C設定Rear View Status
extern volatile int VAR_REAR_VIEW_STATUS;  // Rear View Status

extern volatile int I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT; // User透過I2C設定VPM_POWER_LOW_EVT_DLY
extern unsigned int VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE; // User透過I2C設定之VPM_POWER_LOW_EVT_DLY暫存用變數

extern volatile int I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT; // User透過I2C設定VPM_POWER_LOW_HARD_DLY
extern unsigned int VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE; // User透過I2C設定之VPM_POWER_LOW_HARD_DLY暫存用變數

extern volatile int I2C_USER_SET_VPM_PWR_ON_DLY_EVENT; // User透過I2C設定VPM_PWR_ON_DLY
extern unsigned int VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE; // User透過I2C設定之VPM_PWR_ON_DLY暫存用變數

extern volatile int I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT; // User透過I2C設定VPM_PWR_OFF_EVT_DLY
extern unsigned int VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE; // User透過I2C設定之VPM_PWR_OFF_EVT_DLY暫存用變數

extern volatile int I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT; // User透過I2C設定VPM_PWR_OFF_HARD_DLY
extern unsigned int VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE; // User透過I2C設定之VPM_PWR_OFF_HARD_DLY暫存用變數

extern volatile int I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT; // User透過I2C設定VPM_POST_PWR_CHK_DLY
extern unsigned int VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE; // User透過I2C設定之VPM_POST_PWR_CHK_DLY暫存用變數

extern volatile int I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT; // User透過I2C設定PRE_PWR_CHK_VOLT
extern unsigned int VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE; // User透過I2C設定之PRE_PWR_CHK_VOLT暫存用變數

extern volatile int I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT; // User透過I2C設定POST_PWR_CHK_VOLT
extern unsigned int VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE; // User透過I2C設定之POST_PWR_CHK_VOLT暫存用變數

extern volatile int I2C_USER_SET_UPS_CHARGE_VOLT_EVENT; // User透過I2C設定UPS_CHARGE_VOLT
extern unsigned int VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE; // User透過I2C設定之UPS_CHARGE_VOLT暫存用變數

extern volatile int I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT; // User透過I2C設定VPM_SHUT_DOWN_DLY
extern unsigned int VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE; // User透過I2C設定之VPM_SHUT_DOWN_DLY暫存用變數

extern volatile int I2C_USER_SET_REFERENCE_VOLT_EVENT; // User透過I2C設定REFERENCE_VOLT
extern unsigned int VAR_USER_CHANGE_REFERENCE_VOLT_VALUE; // User透過I2C設定之REFERENCE_VOLT暫存用變數

extern unsigned int VAR_SYSTEM_POWER_SYSTEM_STATE; // 系統在何種狀態
              // = 0 at S5, Power Off
              // = 1 at S0, Power On
              // = 2 ar S3, Suspend

extern unsigned int AUTO_DETECT_REAR_VIEW_EVENT;

// 電池是否過溫的變數
// = 0, 沒有過溫
// = 1, 過溫
extern volatile unsigned int VAR_BATTERY_OVER_TEMPERATURE;

// 電池是否低電的變數
// = 0, 沒有低電
// = 1, 低電
extern volatile unsigned int VAR_BATTERY_UNDER_PERCENTAGE;

// 電池是否過溫且車電是否存在的變數
// = 0, 沒有過溫或車電存在
// = 1, 過溫且車電不存在
extern volatile unsigned int VAR_BATTERY_PROTECTION_FLAG;

// for battery information
extern unsigned int BAT_INFO_Temperature;           //0x08
extern unsigned int BAT_INFO_Voltage;               //0x09
extern unsigned int BAT_INFO_Flags;                 //0x16
extern unsigned int BAT_INFO_Current;               //0x0A
extern unsigned int BAT_INFO_AverageCurrent;        //0x0B
extern unsigned int BAT_INFO_TimeToEmpty;           //0x11
extern unsigned int BAT_INFO_StateOfCharge;         //0x0D

extern volatile int VAR_TRACE_VPM_STATE;

extern unsigned int VAR_IMM_CHANGE_WORKING_MODE_EVENT; // 設定系統立即進入狀態事件
              // = 0, No Event
              // = 1, Enter Power-Off Flow Immediately
              // = 2, Enter Suspend Flow Immediately
              // 前提是系統在開機時此命令才有效

// WATCHDOG TIMER
extern volatile int VAR_WATCHDOG_STATUS;      // Watchdog開啟或關閉
extern volatile int VAR_WATCHDOG_COUNTER;     // Watchdog倒數計時器
extern volatile int VAR_WATCHDOG_RESET_VALUE; // Watchdog倒數計時重置值

// Ignition Status Related
extern volatile unsigned int VAR_IGN_ON_OFF;
                      // =0, 表示Ignition at Low (Ignition Off)
                      // =1, 表示Ignition at High (Ignition On)
extern volatile unsigned int VAR_IGN_OFF_2_ON_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Ignition Off->On Event
extern volatile unsigned int VAR_IGN_ON_2_OFF_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Ignition On->Off Event

// Power Button Status Related
extern volatile unsigned int VAR_PBT_ON_OFF;
                      // =0, 表示Power Button at High (Power Button Off)
                      // =1, 表示Power Button at Low (Power Button On)
extern volatile unsigned int VAR_PBT_OFF_2_ON_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Off->On Event
extern volatile unsigned int VAR_PBT_ON_2_OFF_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button On->Off Event
extern volatile unsigned int VAR_PBT_COUNTER;
                      // 紀錄Power Button被按壓次數(5ms)
extern volatile unsigned int VAR_PBT_POWER_ON_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Power On Event(2s)

// EN LED Status Related
extern volatile unsigned int VAR_EN_LED_HI_LO;
extern volatile unsigned int VAR_EN_LED_LO_2_HI_EVENT;
extern volatile unsigned int VAR_EN_LED_HI_2_LO_EVENT;

// VPM Flow相關Variables
// VPM Variables
extern unsigned int VAR_VPM_PWR_ON_DLY;        // EEPROM $0010, $0011
extern unsigned int VAR_VPM_PWR_OFF_EVT_DLY;   // EEPROM $0012, $0013
extern unsigned char VAR_VPM_PWR_MODE_CTRL;    // EEPROM $0016
extern unsigned int VAR_VPM_AT_MODE;
                      // =0, AT_MODE = 0 (Off)
                      // =1, AT_MODE = 1 (On)
extern unsigned int VAR_VPM_KEEP_ALIVE;
                      // =0, KEEP_ALIVE = 0 (Off)
                      // =1, KEEP_ALIVE = 1 (On)
extern unsigned int VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY; // EEPROM $0014, $0015
// 當在S0時發生Power Low的Event時要如何處置之模式
// 這定義在EEPROM $0052處
// =0 (default), Notify OS and Begin Count Down
// =1, Not Notify OS, begin Count Down
// =2, Notify OS Only
extern volatile int VAR_VPM_POWER_LOW_AT_S0_MODE;
// Car Power Low Event Delay
extern unsigned int VAR_VPM_POWER_LOW_EVT_DLY;   // EEPROM $0021, $0022
// Car Power Low Hard Delay
extern unsigned int VAR_VPM_POWER_LOW_HARD_DLY;   // EEPROM $0023, $0024
// Power Off Event Mask
// bit7 : RFU
// bit6 : RFU
// bit5 : RFU
// bit4 : RFU
// bit3 : RFU
// bit2 : RFU
// bit1 : Ignition Wakeup Enable (Default On)
// bit0 : RFU
extern unsigned char VAR_SHUTDOWN_MASK;  // EEPROM $0026
// Shut Down Delay
extern unsigned int VAR_VPM_SHUT_DOWN_DLY; // EEPROM $0029, $002A
// Shut Down Function Status
extern unsigned char VAR_SHUTDOWN_EN;  // EEPROM $002B
// DI Input Type Control
// Trek-734_removed extern unsigned char VAR_DI_INPUT_TYPE_CTR;  // EEPROM $002C
// Reference Voltage
extern unsigned int VAR_REFERENCE_VOLTAGE; // EEPROM $0071, $0072

// SHUTDOWN TIMER
extern volatile char VAR_SHUTDOWN_STATUS;  // Shutdown開啟或關閉
extern volatile int VAR_SHUTDOWN_COUNTER;  // Shutdown倒數計時器
extern volatile char VAR_SHUTDOWN_FLAG;  // Shutdown flag
extern volatile char VAR_OS_READY; // OS ready or not

// EEPROM Write Used
extern volatile unsigned int VAR_EEPROM_WRITE_EVENT;
              // =0, No Write Event
              // =1, One Byte Write Event
              // Max is 16 Bytes
extern unsigned int VAR_EEPROM_WRITE_QUEUE_ADDR_HI[16];
extern unsigned int VAR_EEPROM_WRITE_QUEUE_ADDR_LO[16];
extern unsigned char VAR_EEPROM_WRITE_QUEUE_DATA[16];
extern unsigned char VAR_EEPROM_USER_DEFAULT_STATUS;
extern unsigned char VAR_EEPROM_MAGIC_ID_HI;
extern unsigned char VAR_EEPROM_MAGIC_ID_LO;

// 存放Serial Number處
extern unsigned char VAR_SERIAL_NUMBER[10];
extern unsigned char VAR_SERIAL_NUMBER_CHG_EVENT; // 是否發生改變Serial Number事件

extern volatile int VAR_VPM_PREBOOT_VOLTAGE_CHK; // Preboot
extern volatile int VAR_VPM_POSTBOOT_VOLTAGE_CHK; // Postboot

extern unsigned char VAR_VPM_PREBOOT_VOLTAGE_CHK_ENABLE; // Preboot
extern unsigned char VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE; // postboot

// UPS開始充電之電壓Threshold
extern unsigned int VAR_VPM_START_CHARGING_THRESHOLD; // EEPROM $0065, $0066

extern unsigned int VAR_VPM_POST_PWR_CHK_DLY_TIME; // EEPROM $0017, $0018

extern unsigned int VAR_VPM_MCU_1ST_POWERED;

extern volatile int VAR_VPM_CAR_POWER_LOW_HAPPENED;

extern unsigned int VAR_DEBUG_PRINT;

// 計數器資料
extern unsigned int VAR_VPM_COUNTER_UPDATE_FAIL;  // 從EEPROM更新COUNTER時是FAIL的情況
                                               // =0, Success
                                               // =1, Fail Occurs
extern unsigned int VAR_VPM_PWR_ON_TIME_YEAR;             // EEPROM $00C0, $00C1
extern unsigned int VAR_VPM_PWR_ON_MIN_HIGH_WORD;         // EEPROM $00C2, $00C3
extern unsigned int VAR_VPM_PWR_ON_MIN_LOW_WORD;          // EEPROM $00C4, $00C5
extern unsigned int VAR_VPM_PWR_ON_IGN_CNT_ACC;           // EEPROM $00C6, $00C7
extern unsigned int VAR_VPM_PWR_ON_IGN_CNT;               // EEPROM $00C8, $00C9
extern unsigned int VAR_VPM_PWR_LOW_OFF_CNT_ACC;          // EEPROM $00CE, $00CF
extern unsigned int VAR_VPM_PWR_LOW_OFF_CNT;              // EEPROM $00D0, $00D1
extern unsigned int VAR_VPM_IGN_OFF_CNT_ACC;              // EEPROM $00D2, $00D3
extern unsigned int VAR_VPM_IGN_OFF_CNT;                  // EEPROM $00D4, $00D5
extern unsigned int VAR_VPM_BAT_OVR_TMP_CNT_ACC;          // EEPROM $00D6, $00D7
extern unsigned int VAR_VPM_BAT_OVR_TMP_CNT;              // EEPROM $00D8, $00D9
extern unsigned int VAR_VPM_PRE_VCHK_OFF_CNT_ACC;         // EEPROM $00DA, $00DB
extern unsigned int VAR_VPM_PRE_VCHK_OFF_CNT;             // EEPROM $00DC, $00DD
extern unsigned int VAR_VPM_POST_VCHK_OFF_CNT_ACC;        // EEPROM $00DE, $00DF
extern unsigned int VAR_VPM_POST_VCHK_OFF_CNT;            // EEPROM $00E0, $00E1
extern unsigned int VAR_VPM_WDOG_RST_CNT_ACC;             // EEPROM $00E2, $00E3
extern unsigned int VAR_VPM_WDOG_RST_CNT;                 // EEPROM $00E4, $00E5
extern unsigned int VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC;    // EEPROM $00E6, $00E7
extern unsigned int VAR_VPM_PWR_ON_KEEP_ALIVE_CNT;        // EEPROM $00E8, $00E9

// 計算Counter相關變數
extern unsigned char VAR_COUNTER_ENABLE_POWER_ON_COUNT; // 是否開始進行Power On Counting

// 上一次喚醒系統的Event
extern unsigned char VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF;
                      // =0x00, No last wakeup event occurs
                      // =0x01, Ignition Off to On event occurs
                      // =0x10, Power-Button event occurs
                      // =0x20, DI1 event occurs
                      // =0x21, DI2 event occurs
                      // =0x22, DI3 event occurs
                      // =0x23, DI4 event occurs

// Wakeup Event Mask
extern unsigned char VAR_WAKEUP_MASK_HI;
extern unsigned char VAR_WAKEUP_MASK_LO;

// LCD & Brightness Control Status Related
extern unsigned int VAR_FUNCTION_KEY_BRIGHTNESS;   // Function Key Brightness
extern unsigned int VAR_FUNCTION_KEY_PWM_PERIOD;   // Function Key PWM Period

extern volatile unsigned char VAR_AUTO_DETECT_RESERVE_GEAR_STATUS;
                      // =0, DI Mode
                      // =1, Auto Detect Reserve Gear Mode

// Current GPS Status
extern volatile unsigned char VAR_GPS_ANTENNA_STATUS;

// Event Queue Used
extern unsigned char VAR_EVENT_QUEUS[__DEF_EVENT_QUEUE_SIZE][2];
extern int VAR_EVENT_INPUT;
extern int VAR_EVENT_OUTPUT;
extern int VAR_EVENT_SIZE;
extern int VAR_ALREADY_NOTIFY_OS_CAR_POWER_LOW;
extern int VAR_INTERRUPT_STATUS;

// I2C Master Used
extern long TimeOutCounter;
extern unsigned char LastErrorOccurse;

#endif