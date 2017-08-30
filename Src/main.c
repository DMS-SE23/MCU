/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Custom HID demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f30x_conf.h"
// User Define Header Files
#include "project.h"
#include "gpio_control.h"
#include "hook_tasks.h"
#include "protocol.h"
#include "i2c_slave.h"
#include "i2c_master.h"
#include <stdio.h>        				            // for printf
#include <stdarg.h>
#include "adc_channel.h"                            // for ADC
//temp0816 #include "dac_channel.h"                 // for DAC
#include "rtc_control.h"
#include "eeprom_control.h"
#include "stm32f30x_dbgmcu.h"
#include "debug_port.h"
//temp0816#include "gps_port.h"
#include "adv7186_control.h"
#include "odometer_control.h"
#include "pwm_control.h"
//-----------------------------------------------------------------------------

// Event Related Variable
volatile int VAR_1mS_Event = 0;
volatile int VAR_5mS_Event = 0;
volatile int VAR_10mS_Event = 0;
volatile int VAR_25mS_Event = 0;
volatile int VAR_100mS_Event = 0;
volatile int VAR_1S_Event = 0;

volatile int VAR_1mS_Counter = 0;
volatile int VAR_5mS_Counter = 0;
volatile int VAR_10mS_Counter = 0;
volatile int VAR_25mS_Counter = 0;
volatile int VAR_100mS_Counter = 0;
volatile int VAR_1S_Counter = 0;

// 做延時用的變數
volatile int VAR_TIMETICK_DELAY = 0;

volatile int VAR_TRACE_VPM_STATE = 1;

// GPS 用的變數
volatile int VAR_GPS_PACKET_READY = 0;
volatile int VAR_GPS_CMD_LENGTH = 0;
unsigned char GPS_Cmd[GPS_CmdSize];
u8 GPS_TxBuffer[TxBufferSize];
u8 GPS_RxBuffer[RxBufferSize];
u16 GPS_TxS=0, GPS_TxE=0; // Tx Start, End Ptr
u16 GPS_RxS=0, GPS_RxE=0; // Rx Start, End Ptr

// DEBUG 用的變數
volatile int VAR_DEBUG_PACKET_READY = 0;
volatile int VAR_DEBUG_CMD_LENGTH = 0;
unsigned char DEBUG_Cmd[DEBUG_CmdSize];
u8 DEBUG_TxBuffer[TxBufferSize];
u8 DEBUG_RxBuffer[RxBufferSize];
u16 DEBUG_TxS=0, DEBUG_TxE=0; // Tx Start, End Ptr
u16 DEBUG_RxS=0, DEBUG_RxE=0; // Rx Start, End Ptr

volatile unsigned int VAR_BATTERY_EXIST = 0; // Battery是否存在，預設不存在
volatile unsigned int CAR_POWER_EXIST = 0;   // Car Power是否存在，預設不存在

volatile unsigned char VAR_SYSTEM_POWER_STATUS = 0; // 供應給I2C 0x09用
                  // SYSTEM_POWER_STATUS Used
                  // bit 4, for BAT1 exist or not
                  // bit 0, for EXTPWR exist or not
                  // ------------------------------
                  // = 0, not exist
                  // = 1, exist

// 目前所偵測到之CAR Power Value
unsigned int VAR_CURRENT_V_CB_VALUE = 0;

// 目前所偵測到之PCB Temperture Value
volatile unsigned int VAR_PCB_TEMP_VALUE = 0;

// 目前所偵測到之GPS REF Value
volatile unsigned int VAR_GPS_REF_VALUE = 0;

// 目前所偵測到之GPS RF Value
volatile unsigned int VAR_GPS_RF_VALUE = 0;

// Odometer counter
volatile unsigned int VAR_ODOMETER_VALUE = 0;

// Multi-protocol Transceiver Status
volatile unsigned int VAR_TRANSCEIVER_STATUS = 0;

// 電池是否過溫的變數
// = 0, 沒有過溫
// = 1, 過溫
volatile unsigned int VAR_BATTERY_OVER_TEMPERATURE = 0;

// 電池是否低電的變數
// = 0, 沒有低電
// = 1, 低電
volatile unsigned int VAR_BATTERY_UNDER_PERCENTAGE = 0;

// 電池是否過溫且車電是否存在的變數
// = 0, 沒有過溫或車電存在
// = 1, 過溫且車電不存在
volatile unsigned int VAR_BATTERY_PROTECTION_FLAG = 0;

// for battery information
//unsigned int BAT_INFO_AtRate = 0;                   // 0x02, 0x03
//unsigned int BAT_INFO_UnfilteredSOC = 0;            // 0x04, 0x05
unsigned int BAT_INFO_Temperature = 0;              // 0x06, 0x07
unsigned int BAT_INFO_Voltage = 0;                  // 0x08, 0x09
unsigned int BAT_INFO_Flags = 0;                    // 0x0A, 0x0B
//unsigned int BAT_INFO_NomAvailableCapacity = 0;     // 0x0C, 0x0D
//unsigned int BAT_INFO_FullAvailableCapacity = 0;    // 0x0E, 0x0F
//unsigned int BAT_INFO_RemainingCapacity = 0;        // 0x10, 0x11
//unsigned int BAT_INFO_FullChargeCapacity = 0;       // 0x12, 0x13
unsigned int BAT_INFO_Current = 0;                  // 0x14, 0x15
unsigned int BAT_INFO_AverageCurrent = 0;           // 0x14, 0x15
unsigned int BAT_INFO_TimeToEmpty = 0;              // 0x16, 0x17
//unsigned int BAT_INFO_FilteredFCC = 0;              // 0x18, 0x19
//unsigned int BAT_INFO_StandbyCurrent = 0;           // 0x1A, 0x1B
//unsigned int BAT_INFO_UnfilteredFCC = 0;            // 0x1C, 0x1D
//unsigned int BAT_INFO_MaxLoadCurrent = 0;           // 0x1E, 0x1F
//unsigned int BAT_INFO_UnfilteredRM = 0;             // 0x20, 0x21
//unsigned int BAT_INFO_FilteredRM = 0;               // 0x22, 0x23
//unsigned int BAT_INFO_AveragePower = 0;             // 0x24, 0x25
//unsigned int BAT_INFO_InternalTemperature = 0;      // 0x28, 0x29
//unsigned int BAT_INFO_CycleCount = 0;               // 0x2A, 0x2B
unsigned int BAT_INFO_StateOfCharge = 0;            // 0x2C, 0x2D
//unsigned int BAT_INFO_StateOfHealth = 0;            // 0x2E, 0x2F
//unsigned int BAT_INFO_PassedCharge = 0;             // 0x34, 0x35
//unsigned int BAT_INFO_DOD0 = 0;                     // 0x36, 0x37
//unsigned int BAT_INFO_SelfDischargeCurrent = 0;     // 0x38, 0x39

// Car Power System Information
unsigned char VAR_SYSI2C_SYS_INFO = 0;  // EEPROM $0025
unsigned int VAR_SYSI2C_SYS_INFO_CHANGE = 0;

/*RTC Time Informaiton - include STM32 internal RTC and external S35390A RTC----------*/
RTC_TIME VAR_RTC_TIME_S35390A;
ALARM_TIME VAR_ALARM_TIME;
//temp0816 long STM32RTC_CounterValue; // STM32 RTC Counter Value

// WATCHDOG TIMER
volatile int VAR_WATCHDOG_STATUS                            = 0;    // Watchdog開啟或關閉
volatile int VAR_WATCHDOG_COUNTER                           = 0;    // Watchdog倒數計時器
volatile int VAR_WATCHDOG_RESET_VALUE                       = 10;   // Watchdog倒數計時重置值

//I2C
volatile int I2C_USER_SET_RTC_TIME_EVENT                    = 0;    // User透過I2C設定RTC
unsigned char VAR_USER_CHANGE_RTC_VALUE                     = 0;    // User透過I2C設定之RTC暫存用變數

volatile int I2C_USER_SET_ALARM_TIME_EVENT                  = 0;    // User透過I2C設定ALARM
unsigned char VAR_USER_CHANGE_ALARM_VALUE                   = 0;    // User透過I2C設定之ALARM暫存用變數

volatile int I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT         = 0;    // User透過I2C設定WDG_COUNTDOWN_TIMER
unsigned int VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE      = 0;    // User透過I2C設定之WDG_COUNTDOWN_TIMER暫存用變數

volatile int I2C_USER_LOAD_EEPROM_DEFAULT_EVENT             = 0;    // User透過I2C Load EEPROM

volatile int I2C_USER_SET_REAR_VIEW_EVENT                   = 0;    // User透過I2C設定Rear View Status
volatile int VAR_REAR_VIEW_STATUS                           = 0;    // Rear View Status

volatile int I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT       = 0;    // User透過I2C設定VPM_POWER_LOW_EVT_DLY
unsigned int VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE    = 0;    // User透過I2C設定之VPM_POWER_LOW_EVT_DLY暫存用變數

volatile int I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT      = 0;    // User透過I2C設定VPM_POWER_LOW_HARD_DLY
unsigned int VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE   = 0;    // User透過I2C設定之VPM_POWER_LOW_HARD_DLY暫存用變數

volatile int I2C_USER_SET_VPM_PWR_ON_DLY_EVENT              = 0;    // User透過I2C設定VPM_PWR_ON_DLY
unsigned int VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE           = 0;    // User透過I2C設定之VPM_PWR_ON_DLY暫存用變數

volatile int I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT         = 0;    // User透過I2C設定VPM_PWR_OFF_EVT_DLY
unsigned int VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE      = 0;    // User透過I2C設定之VPM_PWR_OFF_EVT_DLY暫存用變數

volatile int I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT        = 0;    // User透過I2C設定VPM_PWR_OFF_HARD_DLY
unsigned int VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE     = 0;    // User透過I2C設定之VPM_PWR_OFF_HARD_DLY暫存用變數

volatile int I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT        = 0;    // User透過I2C設定VPM_POST_PWR_CHK_DLY
unsigned int VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE     = 0;    // User透過I2C設定之VPM_POST_PWR_CHK_DLY暫存用變數

volatile int I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT            = 0;    // User透過I2C設定PRE_PWR_CHK_VOLT
unsigned int VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE         = 0;    // User透過I2C設定之PRE_PWR_CHK_VOLT暫存用變數

volatile int I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT           = 0;    // User透過I2C設定POST_PWR_CHK_VOLT
unsigned int VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE        = 0;    // User透過I2C設定之POST_PWR_CHK_VOLT暫存用變數

volatile int I2C_USER_SET_UPS_CHARGE_VOLT_EVENT             = 0;    // User透過I2C設定UPS_CHARGE_VOLT
unsigned int VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE          = 0;    // User透過I2C設定之UPS_CHARGE_VOLT暫存用變數

volatile int I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT           = 0;    // User透過I2C設定VPM_SHUT_DOWN_DLY
unsigned int VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE        = 0;    // User透過I2C設定之VPM_SHUT_DOWN_DLY暫存用變數

volatile int I2C_USER_SET_REFERENCE_VOLT_EVENT              = 0;    // User透過I2C設定REFERENCE_VOLT
unsigned int VAR_USER_CHANGE_REFERENCE_VOLT_VALUE           = 0;    // User透過I2C設定之REFERENCE_VOLT暫存用變數

// State machine for dump RTC registers
// = 0, Not Dump
// = 1, Start Dump
volatile unsigned int VAR_START_DUMP_RTC_S35390A_REGISTERS = 0;
// for printf used <--- fputc()
typedef void (*pFunction)(void);

// Ignition Status Related
volatile unsigned int VAR_IGN_ON_OFF = 0;
                      // =0, 表示Ignition at Low (Ignition Off)
                      // =1, 表示Ignition at High (Ignition On)
volatile unsigned int VAR_IGN_OFF_2_ON_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Ignition Off->On Event
volatile unsigned int VAR_IGN_ON_2_OFF_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Ignition On->Off Event

// Power Button Status Related
volatile unsigned int VAR_PBT_ON_OFF = 0;
                      // =0, 表示Power Button at High (Power Button Off)
                      // =1, 表示Power Button at Low (Power Button On)
volatile unsigned int VAR_PBT_OFF_2_ON_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Off->On Event
volatile unsigned int VAR_PBT_ON_2_OFF_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button On->Off Event
volatile unsigned int VAR_PBT_COUNTER = 0;
                      // 紀錄Power Button被按壓次數(5ms)
volatile unsigned int VAR_PBT_POWER_ON_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Power On Event(2s)

// EN LED Status Related
volatile unsigned int VAR_EN_LED_HI_LO = 0;
volatile unsigned int VAR_EN_LED_LO_2_HI_EVENT = 0;
volatile unsigned int VAR_EN_LED_HI_2_LO_EVENT = 0;

//rtc_time_s35390a;
//RTC_TIME rtc_time_stm32;

// VPM Flow相關Variables
unsigned int VAR_VPM_PWR_ON_DLY = 0;                                // EEPROM $0010, $0011
unsigned int VAR_VPM_PWR_OFF_EVT_DLY = 0;                           // EEPROM $0012, $0013
unsigned char VAR_VPM_PWR_MODE_CTRL = 0;                            // EEPROM $0016
unsigned int VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY = 0;                  // EEPROM $0014, $0015
unsigned int VAR_SYSTEM_POWER_SYSTEM_STATE = 0;                     // 系統在何種狀態
              // = 0 at S5, Power Off
              // = 1 at S0, Power On
              // = 2 ar S3, Suspend
unsigned int AUTO_DETECT_REAR_VIEW_EVENT = 0;

unsigned int VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0;                 // 設定系統立即進入狀態事件
              // = 0, No Event
              // = 1, Enter Power-Off Flow Immediately
              // = 2, Enter Suspend Flow Immediately
              // 前提是系統在開機時此命令才有效
unsigned int VAR_VPM_AT_MODE = 0;
                      // =0, AT_MODE = 0 (Off)
                      // =1, AT_MODE = 1 (On)
unsigned int VAR_VPM_KEEP_ALIVE = 0;
                      // =0, KEEP_ALIVE = 0 (Off)
                      // =1, KEEP_ALIVE = 1 (On)
volatile int VAR_VPM_PREBOOT_VOLTAGE_CHK = 742; // Preboot
volatile int VAR_VPM_POSTBOOT_VOLTAGE_CHK = 742; // Postboot
                      // 預設時以12V的預設值為預設值
unsigned char VAR_VPM_PREBOOT_VOLTAGE_CHK_ENABLE = 0;
unsigned char VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE = 0;
// 當在S0時發生Power Low的Event時要如何處置之模式
// 這定義在EEPROM $0052處
// =0 (default), Notify OS and Begin Count Down
// =1, Not Notify OS, begin Count Down
// =2, Notify OS Only
volatile int VAR_VPM_POWER_LOW_AT_S0_MODE = 0;
// Car Power Low Event Delay
unsigned int VAR_VPM_POWER_LOW_EVT_DLY = 0;                         // EEPROM $0021, $0022
// Car Power Low Hard Delay
unsigned int VAR_VPM_POWER_LOW_HARD_DLY = 13;                       // EEPROM $0023, $0024
// UPS開始充電之電壓Threshold
unsigned int VAR_VPM_START_CHARGING_THRESHOLD = 0x2EC;              // EEPROM $0065, $0066

unsigned int VAR_VPM_POST_PWR_CHK_DLY_TIME = 2;                     // EEPROM $0017, $0018
// Power Off Event Mask
// bit7 : RFU
// bit6 : RFU
// bit5 : RFU
// bit4 : RFU
// bit3 : RFU
// bit2 : RFU
// bit1 : Ignition Wakeup Enable (Default On)
// bit0 : RFU
unsigned char VAR_SHUTDOWN_MASK = 0;                                // EEPROM $0026
// Shut Down Delay
unsigned int VAR_VPM_SHUT_DOWN_DLY = 0;                             // EEPROM $0029, $002A
// Shut Down Function Status
unsigned char VAR_SHUTDOWN_EN = 0;                                  // EEPROM $002B
// DI Input Type Control
// Trek-734_removed unsigned char VAR_DI_INPUT_TYPE_CTR = 0;                            // EEPROM $002C
// Reference Voltage
unsigned int VAR_REFERENCE_VOLTAGE = 0;                             // EEPROM $0071, $0072

// SHUTDOWN TIMER
volatile char VAR_SHUTDOWN_STATUS = 0;                              // Shutdown開啟或關閉
volatile int VAR_SHUTDOWN_COUNTER = 0;                              // Shutdown倒數計時器
volatile char VAR_SHUTDOWN_FLAG = 0;                                // Shutdown flag
volatile char VAR_OS_READY = 0;                                     // OS ready or not

unsigned int VAR_VPM_MCU_1ST_POWERED = 1;
                      // =0, MCU is not 1st powered
                      // =1, MCU is 1st powered

volatile int VAR_VPM_CAR_POWER_LOW_HAPPENED = 0;
                      // =0, Car power low does not happened
                      // =1, Car power low happened

// 計數器資料
unsigned int VAR_VPM_COUNTER_UPDATE_FAIL = 0;                       // 從EEPROM更新COUNTER時是FAIL的情況
                                                                    // =0, Success
                                                                    // =1, Fail Occurs
unsigned int VAR_VPM_PWR_ON_TIME_YEAR = 0;                          // EEPROM $00C0, $00C1
unsigned int VAR_VPM_PWR_ON_MIN_HIGH_WORD = 0;                      // EEPROM $00C2, $00C3
unsigned int VAR_VPM_PWR_ON_MIN_LOW_WORD = 0;                       // EEPROM $00C4, $00C5
unsigned int VAR_VPM_PWR_ON_IGN_CNT_ACC = 0;                        // EEPROM $00C6, $00C7
unsigned int VAR_VPM_PWR_ON_IGN_CNT = 0;                            // EEPROM $00C8, $00C9
unsigned int VAR_VPM_PWR_LOW_OFF_CNT_ACC = 0;                       // EEPROM $00CE, $00CF
unsigned int VAR_VPM_PWR_LOW_OFF_CNT = 0;                           // EEPROM $00D0, $00D1
unsigned int VAR_VPM_IGN_OFF_CNT_ACC = 0;                           // EEPROM $00D2, $00D3
unsigned int VAR_VPM_IGN_OFF_CNT = 0;                               // EEPROM $00D4, $00D5
unsigned int VAR_VPM_BAT_OVR_TMP_CNT_ACC = 0;                       // EEPROM $00D6, $00D7
unsigned int VAR_VPM_BAT_OVR_TMP_CNT = 0;                           // EEPROM $00D8, $00D9
unsigned int VAR_VPM_PRE_VCHK_OFF_CNT_ACC = 0;                      // EEPROM $00DA, $00DB
unsigned int VAR_VPM_PRE_VCHK_OFF_CNT = 0;                          // EEPROM $00DC, $00DD
unsigned int VAR_VPM_POST_VCHK_OFF_CNT_ACC = 0;                     // EEPROM $00DE, $00DF
unsigned int VAR_VPM_POST_VCHK_OFF_CNT = 0;                         // EEPROM $00E0, $00E1
unsigned int VAR_VPM_WDOG_RST_CNT_ACC = 0;                          // EEPROM $00E2, $00E3
unsigned int VAR_VPM_WDOG_RST_CNT = 0;                              // EEPROM $00E4, $00E5
unsigned int VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC = 0;                 // EEPROM $00E6, $00E7
unsigned int VAR_VPM_PWR_ON_KEEP_ALIVE_CNT = 0;                     // EEPROM $00E8, $00E9

// 計算Counter相關變數
unsigned char VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;                // 是否開始進行Power On Counting


unsigned int VAR_DEBUG_PRINT = 0;

// 上一次喚醒系統的Event
unsigned char VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0;
                      // =0x00, No last wakeup event occurs
                      // =0x01, Ignition Off to On event occurs
                      // =0x20, DI1 event occurs
                      // =0x21, DI2 event occurs
                      // =0x22, DI3 event occurs
                      // =0x23, DI4 event occurs

// EEPROM Write Used
volatile unsigned int VAR_EEPROM_WRITE_EVENT = 0;
              // =0, No Write Event
              // =1, One Byte Write Event
              // Max is 16 Bytes
unsigned int VAR_EEPROM_WRITE_QUEUE_ADDR_HI[16];
unsigned int VAR_EEPROM_WRITE_QUEUE_ADDR_LO[16];
unsigned char VAR_EEPROM_WRITE_QUEUE_DATA[16];
unsigned char VAR_EEPROM_USER_DEFAULT_STATUS = 0;
unsigned char VAR_EEPROM_MAGIC_ID_HI = 0;
unsigned char VAR_EEPROM_MAGIC_ID_LO = 0;

// 存放Serial Number處
unsigned char VAR_SERIAL_NUMBER[10] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
unsigned char VAR_SERIAL_NUMBER_CHG_EVENT = 0; // 是否發生改變Serial Number事件

// Wakeup Event Mask
unsigned char VAR_WAKEUP_MASK_HI = 0x00;
unsigned char VAR_WAKEUP_MASK_LO = 0x00;

// LCD & Brightness Control Status Related
unsigned int VAR_FUNCTION_KEY_BRIGHTNESS = 0;   // Function Key Brightness
unsigned int VAR_FUNCTION_KEY_PWM_PERIOD = 0;   // Function Key PWM Period

volatile unsigned char VAR_AUTO_DETECT_RESERVE_GEAR_STATUS = 1;
                      // =0, DI Mode
                      // =1, Auto Detect Reserve Gear Mode

// Current GPS Status
volatile unsigned char VAR_GPS_ANTENNA_STATUS = 0;

//-----------------------------------------------------------------------------

// Event Queue Used
unsigned char VAR_EVENT_QUEUS[__DEF_EVENT_QUEUE_SIZE][2];   //project.h => size=32
                                                            //[0] = evt_class
                                                            //[1] = evt_id
int VAR_EVENT_INPUT                     = 0;
int VAR_EVENT_OUTPUT                    = 0;
int VAR_EVENT_SIZE                      = 0;
int VAR_ALREADY_NOTIFY_OS_CAR_POWER_LOW = 0;
int VAR_INTERRUPT_STATUS                = 0;
//-----------------------------------------------------------------------------

// I2C Master Used
long TimeOutCounter;
unsigned char LastErrorOccurse = 0;
//-----------------------------------------------------------------------------
//
//// Diagnostic Used Variable
//
//// printf用
//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(USART1, (uint8_t) ch);
//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//    {}
//  return ch;
//}
//-----------------------------------------------------------------------------


/*I2C Slave event status---------------------------------------------------------*/
//I2C event is set to "1" after the i2c stop event detected in i2c interrupt handler to indicate a complete i2c packet is received.
//or after a NACK event detected in i2c err interrupt handler to indicate the host has
EventStatus i2c_event= NOEVENT;
uint8_t i2c_cmd;

// The timer base is 100uS (0.1mS)
void TASK_Main_Event_Generator()
{
  if (VAR_1mS_Event) // 每1mS之Event
  {
    VAR_1mS_Event = 0; // Clear 1mS Event
  }
  // 每5mSEvent
  if (VAR_5mS_Counter >= 5)
  {
    VAR_5mS_Event = 1;
    VAR_5mS_Counter = 0;
  }
  // 每10mSEvent
  if (VAR_10mS_Counter >= 10)
  {
    VAR_10mS_Event = 1;
    VAR_10mS_Counter = 0;
  }
  // 每25mSEvent
  if (VAR_25mS_Counter >= 25)
  {
    VAR_25mS_Event = 1;
    VAR_25mS_Counter = 0;
  }
  // 每100mSEvent
  if (VAR_100mS_Counter >= 100)
  {
    //printf ("Timer %d\n\r",VAR_100mS_Counter);
    VAR_100mS_Event = 1;
    VAR_100mS_Counter = 0;
  }
  // 一秒的Event發生
  if (VAR_1S_Counter >= 1000)
  {
    //printf ("Timer %d\n\r",VAR_1S_Counter);
    VAR_1S_Event = 1;
    VAR_1S_Counter = 0;
  }
}
//-----------------------------------------------------------------------------


// Main Program
int main(void)
{
  // 加入此呼叫
  // unsigned char Get_Platform_String(int type, int index)才會編譯器編入
  // 否則會被最佳化去掉~~
  Get_Platform_String(0,0);

  // 呼叫system_stm32f10x.c中的 SystemInit()與SystemCoreClockUpdate()，
  // 以設定相關之Clock
  SystemInit();
  SystemCoreClockUpdate();

  // 中斷table reset to offset
  #ifdef  VECT_TAB_RAM
     /* Set the Vector Table base location at 0x20000000 */
     NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
  #else  /* VECT_TAB_FLASH  */
     /* Set the Vector Table base location at 0x08000000 */
     #ifdef NDEBUG
       NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2800);
     #else
       NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
     #endif
  #endif

  DBGMCU_Config(DBGMCU_STOP, DISABLE);						// 在Debug Mode要加這行

  // 產生1mS時基之SysTick中斷
  // 72000000 / 1000 = 72000
  if (SysTick_Config(SystemCoreClock /1000))
  {
    /* Capture error */
    while (1);
  }

  // 初始化System Tick 用之 Counter
  VAR_1mS_Counter = 0;
  VAR_5mS_Counter = 0;
  VAR_10mS_Counter = 0;
  VAR_25mS_Counter = 0;
  VAR_100mS_Counter = 0;
  VAR_1S_Counter = 0;
  //
  VAR_1mS_Event = 0;
  VAR_5mS_Event = 0;
  VAR_10mS_Event = 0;
  VAR_25mS_Event = 0;
  VAR_100mS_Event = 0;
  VAR_1S_Event = 0;

  // 初始化Debug Port
  DEBUG_PORT_INITIALIZATION();
  DEBUG_PRINT("\n\r\n\r..: Hello TREK-734 VPM\n\r");

  RTC_INITIALIZATION();
  DEBUG_PRINT("..: STM32 RTC Init Complete\n\r");

  // 初始化GPIO
  FUNC_GPIO_INIT();
  DEBUG_PRINT("..: GPIO Init Complete\n\r");

//temp0816   GPS_PORT_INITIALIZATION();
//temp0816   DEBUG_PRINT("..: GPS Port Init Complete\n\r");

  //Initial I2C1 Master
  I2C1_Master_Init();

  //Init I2C2 port as Slave transmitter/receiver
  I2C_Slave_Init();

  // 初始化ADC1
  ADC_Configuration();
  DEBUG_PRINT("..: ADC Init Complete\n\r");

  // 初始化DAC2
//temp0816   DAC2_Configuration();
//temp0816   DEBUG_PRINT("..: DAC Init Complete\n\r");

  // 初始化TIM1
//temp0817  Odometer_Configuration();
//temp0816   DEBUG_PRINT("..: TIM Init Complete\n\r");

  // 初始化PWM
  INIT_PWM_GPIO();
  VAR_FUNCTION_KEY_BRIGHTNESS = 0;
  SET_PWM_WIDTH(INIT_PWM(10000), VAR_FUNCTION_KEY_BRIGHTNESS);  // frequency is 10kHz
  DEBUG_PRINT("..: PWM Init Complete\n\r");
  
  // 初始化External RTC
  RTC_S35390A_INITIALIZATION();
  DEBUG_PRINT("..: RTC_S35390A Init Complete\n\r");

  // 將External RTC的時間更新到STM32中
  (UPDATE_RTC_FROM_S35390A() == __RETURN_SUCCESS) ? \
  DEBUG_PRINT("..: RTC Update Complete\n\r") : \
  DEBUG_PRINT("..: RTC Update Failed!\n\r");

  UPDATE_ALARM();
  DEBUG_PRINT("..: ALARM Update Complete\n\r");

  // 更新VPM變數
  UPDATE_VPM_VARIABLE_FROM_EEPROM();
  DEBUG_PRINT("..: EEPROM Update Complete\n\r");

  //VPM version
  DEBUG_PRINT("..: VPM version : %01d.%03d\n\r", __DEF_PROJECT_MAIN_VERSION, __DEF_PROJECT_MINER_VERSION);
  DEBUG_PRINT("..: Platform ID : 0x%02X\n\r", PLATFORM_ID);

  //for test
//  DUMP_EEPROM_CONTENT();

  while(1)
  {
    TASK_Main_Event_Generator();  // 100uS Event
    // 5 mS
    if (VAR_5mS_Event == 1)
    {
      TASK_Hook_5mS();
      VAR_5mS_Event = 0;
    }
    // 10 mS
    if (VAR_10mS_Event == 1)
    {
      TASK_Hook_10mS();
      VAR_10mS_Event = 0;
    }
    // 25 mS
    if (VAR_25mS_Event == 1)
    {
      TASK_Hook_25mS();
      VAR_25mS_Event = 0;
    }
    // 100 mS
    if (VAR_100mS_Event == 1)
    {
      TASK_Hook_100mS();
      VAR_100mS_Event = 0;
    }
    // 1 Second
    if (VAR_1S_Event == 1)
    {
      TASK_Hook_1S();
      VAR_1S_Event = 0;
    }
//--------------------------------------
//temp0816    // GPS Command
//temp0816    if (VAR_GPS_PACKET_READY == 0)
//temp0816    {
//temp0816      GPS_CHECK_PACKET();
//temp0816    }
//temp0816    else
//temp0816    {
//temp0816      GPS_PROCESSING_PACKET();
//temp0816      VAR_GPS_PACKET_READY = 0; // 處理好了
//temp0816      VAR_GPS_CMD_LENGTH = 0; // TREK-30X Received Buffer歸零
//temp0816    }
//--------------------------------------
    // DEBUG Command
    if (VAR_DEBUG_PACKET_READY == 0)
    {
      Check_DEBUG_Command();
    }
    else
    {
      //if (WORKING_MODE == 0)
      DEBUG_Command_Processing(); // 處理AT Command 程式進入點 (Command Mode)
      //else
      //  AT_Data_Processing(); // 處理AT Command 程式進入點 (Data Mode)
      /* 清除AT Command Buffer -----------------------*/
      VAR_DEBUG_PACKET_READY = 0;
      VAR_DEBUG_CMD_LENGTH = 0;
    }
//--------------------------------------
    //I2C Slave Command
    if (i2c_event == EVENT_NOTYET_READ)
    {
      i2c_cmd = Read_I2C_Slave_Cmd();
      i2c_event = EVENT_READ;

      I2C_Slave_Command_Processing(i2c_cmd);
    }

/*** RTC Related ***/
//    if (I2C_USER_SET_RTC_TIME_EVENT == 1)
//    {
//      // User透過I2C設定了RTC的時間
//      UPDATE_RTC_TO_S35390A(); // 更新RTC時間
//      I2C_USER_SET_RTC_TIME_EVENT = 0;
//    }
//    if (I2C_USER_SET_RTC_TIME_EVENT == 2)
//    { // User改變時
//      UPDATE_RTC_FROM_S35390A();
//      VAR_RTC_TIME_S35390A.Hour = VAR_USER_CHANGE_RTC_VALUE;
//      UPDATE_USER_DEFINED_RTC_VALUE();                      // 更新RTC時間
//      UPDATE_RTC_FROM_S35390A();
//      I2C_USER_SET_RTC_TIME_EVENT = 0;
//    }
//    if (I2C_USER_SET_RTC_TIME_EVENT == 3)
//    { // User改變分
//      UPDATE_RTC_FROM_S35390A();
//      VAR_RTC_TIME_S35390A.Minute = VAR_USER_CHANGE_RTC_VALUE;
//      UPDATE_USER_DEFINED_RTC_VALUE();                      // 更新RTC時間
//      UPDATE_RTC_FROM_S35390A();
//      I2C_USER_SET_RTC_TIME_EVENT = 0;
//    }
//    if (I2C_USER_SET_RTC_TIME_EVENT == 4)
//    { // User改變秒
//      UPDATE_RTC_FROM_S35390A();
//      VAR_RTC_TIME_S35390A.Second = VAR_USER_CHANGE_RTC_VALUE;
//      UPDATE_USER_DEFINED_RTC_VALUE();                      // 更新RTC時間
//      UPDATE_RTC_FROM_S35390A();
//      I2C_USER_SET_RTC_TIME_EVENT = 0;
//    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 1)                   // External RTC Set, YEAR
    { // wait
      // SDK will issue 7 interrupts to set RTC, YEAR-MONTH-DAY-WEEK-HOUR-MINUTE-SECOND
      // The interrupt will cause I2C failed, if the I2C is processing
    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 2)                   // External RTC Set, MONTH
    { // wait
      // SDK will issue 7 interrupts to set RTC, YEAR-MONTH-DAY-WEEK-HOUR-MINUTE-SECOND
      // The interrupt will cause I2C failed, if the I2C is processing
    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 3)                   // External RTC Set, DAY
    { // wait
      // SDK will issue 7 interrupts to set RTC, YEAR-MONTH-DAY-WEEK-HOUR-MINUTE-SECOND
      // The interrupt will cause I2C failed, if the I2C is processing
    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 4)                   // External RTC Set, WEEK
    { // wait
      // SDK will issue 7 interrupts to set RTC, YEAR-MONTH-DAY-WEEK-HOUR-MINUTE-SECOND
      // The interrupt will cause I2C failed, if the I2C is processing
    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 5)                   // External RTC Set, HOUR
    { // wait
      // SDK will issue 7 interrupts to set RTC, YEAR-MONTH-DAY-WEEK-HOUR-MINUTE-SECOND
      // The interrupt will cause I2C failed, if the I2C is processing
    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 6)                   // External RTC Set, MINUTE
    { // wait
      // SDK will issue 7 interrupts to set RTC, YEAR-MONTH-DAY-WEEK-HOUR-MINUTE-SECOND
      // The interrupt will cause I2C failed, if the I2C is processing
    }
    if (I2C_USER_SET_RTC_TIME_EVENT == 7)                   // External RTC Set, SECOND
    {
      UPDATE_RTC_TO_S35390A();                              // External RTC Set
      I2C_USER_SET_RTC_TIME_EVENT = 0;
    }
//--------------------------------------

    // 有改變Serial Number Event發生
    if (VAR_SERIAL_NUMBER_CHG_EVENT != 0)
    {
      EEPROM_UPDATE_SERIAL_NUMBER(VAR_SERIAL_NUMBER_CHG_EVENT);
      VAR_SERIAL_NUMBER_CHG_EVENT = 0;
    }
//--------------------------------------

/* RTC Related */
    if (I2C_USER_SET_ALARM_TIME_EVENT == 1)
    {
      VAR_ALARM_TIME.DayOfWeek = VAR_USER_CHANGE_ALARM_VALUE;
      UPDATE_USER_DEFINED_ALARM_VALUE();
      UPDATE_ALARM();
      I2C_USER_SET_ALARM_TIME_EVENT = 0;
    }
    if (I2C_USER_SET_ALARM_TIME_EVENT == 2)
    {
      VAR_ALARM_TIME.Hour = VAR_USER_CHANGE_ALARM_VALUE;
      UPDATE_USER_DEFINED_ALARM_VALUE();
      UPDATE_ALARM();
      I2C_USER_SET_ALARM_TIME_EVENT = 0;
    }
    if (I2C_USER_SET_ALARM_TIME_EVENT == 3)
    {
      VAR_ALARM_TIME.Minute = VAR_USER_CHANGE_ALARM_VALUE;
      UPDATE_USER_DEFINED_ALARM_VALUE();
      UPDATE_ALARM();
      I2C_USER_SET_ALARM_TIME_EVENT = 0;
    }

    if (I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT == 1)
    {
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE >= __DEF_WDG_COUNTDOWN_TIMER_MIN) &&
          (VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE <= __DEF_WDG_COUNTDOWN_TIMER_MAX))
      {
        VAR_WATCHDOG_RESET_VALUE = VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x50; // Watchdog Countdown Timer High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_WATCHDOG_RESET_VALUE>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x51; // Watchdog Countdown Timer Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_WATCHDOG_RESET_VALUE & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
    }
//--------------------------------------

    if (I2C_USER_LOAD_EEPROM_DEFAULT_EVENT == 1)
    {
      // 回復到預設值
      // 不會清掉機器序號與WWAN MAC Address
      EEPROM_LOAD_FACTORY_DEFAULT();
      I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 0;
    }
    if (I2C_USER_LOAD_EEPROM_DEFAULT_EVENT == 2)
    {
      // Save User Default to EEPROM
      EEPROM_SAVE_USER_DEFAULT();
      I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 0;
    }
    if (I2C_USER_LOAD_EEPROM_DEFAULT_EVENT == 3)
    {
      // Load User Default to EEPROM
      EEPROM_LOAD_USER_DEFAULT();
      UPDATE_VPM_VARIABLE_FROM_EEPROM();
      I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 0;
    }
    if (I2C_USER_LOAD_EEPROM_DEFAULT_EVENT == 4)
    {
      // 回復到工廠預設值
      EEPROM_FILL_DEFAULT_VALUE();
      I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 0;
    }
//--------------------------------------

   if (I2C_USER_SET_REAR_VIEW_EVENT == 1 || AUTO_DETECT_REAR_VIEW_EVENT == 1)
   {
     __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;
     __MACRO_DELAY_mS(5);
     ADV7186_RESET();
     __MACRO_DELAY_mS(5);
     ADV7186_LVDS();
     __MACRO_DELAY_mS(300);
     __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
     VAR_REAR_VIEW_STATUS = 0;
     I2C_USER_SET_REAR_VIEW_EVENT = 0;
     AUTO_DETECT_REAR_VIEW_EVENT = 0;
   }
   if (I2C_USER_SET_REAR_VIEW_EVENT == 2 || AUTO_DETECT_REAR_VIEW_EVENT == 2)
   {
     __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;
     __MACRO_DELAY_mS(5);
     ADV7186_RESET();
     __MACRO_DELAY_mS(5);
     ADV7186_CVBS();
     __MACRO_DELAY_mS(300);
     __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
     VAR_REAR_VIEW_STATUS = 1;
     I2C_USER_SET_REAR_VIEW_EVENT = 0;
     AUTO_DETECT_REAR_VIEW_EVENT = 0;
   }
//--------------------------------------

    /* I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT - User set : Power LOW event delay event */
    if (I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE >= __DEF_POWER_LOW_EVT_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE <= __DEF_POWER_LOW_EVT_DLY_MAX))
      {
        VAR_VPM_POWER_LOW_EVT_DLY = VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x21; // Car Power Low Event Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POWER_LOW_EVT_DLY>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x22; // Car Power Low Event Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POWER_LOW_EVT_DLY & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT - User set : Power LOW hard delay event */
    if (I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE >= __DEF_POWER_LOW_HARD_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE <= __DEF_POWER_LOW_HARD_DLY_MAX))
      {
        VAR_VPM_POWER_LOW_HARD_DLY = VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x23; // Car Power Low Hard Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POWER_LOW_HARD_DLY>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x24; // Car Power Low Hard Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POWER_LOW_HARD_DLY & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_VPM_PWR_ON_DLY_EVENT - User set : Power ON delay event */
    if (I2C_USER_SET_VPM_PWR_ON_DLY_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_VPM_PWR_ON_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE >= __DEF_PWR_ON_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE <= __DEF_PWR_ON_DLY_MAX))
      {
        VAR_VPM_PWR_ON_DLY = VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x10; // Ignition On Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_ON_DLY>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x11; // Ignition On Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_ON_DLY & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT - User set : Power OFF event delay event */
    if (I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE >= __DEF_PWR_OFF_EVT_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE <= __DEF_PWR_OFF_EVT_DLY_MAX))
      {
        VAR_VPM_PWR_OFF_EVT_DLY = VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x12; // Ignition Off Event Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_OFF_EVT_DLY>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x13; // Ignition Off Event Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_OFF_EVT_DLY & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT - User set : Power OFF hard delay event */
    if (I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE >= __DEF_IGN_OFF_PWR_OFF_HARD_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE <= __DEF_IGN_OFF_PWR_OFF_HARD_DLY_MAX))
      {
        VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY = VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x14; // Ignition Off Run Power Off Flow Hard Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x15; // Ignition Off Run Power Off Flow Hard Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT - User set : Post power check delay event */
    if (I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE >= __DEF_POST_PWR_CHK_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE <= __DEF_POST_PWR_CHK_DLY_MAX))
      {
        VAR_VPM_POST_PWR_CHK_DLY_TIME = VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x17; // Post-boot Power Check Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POST_PWR_CHK_DLY_TIME>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x18; // Post-boot Power Check Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POST_PWR_CHK_DLY_TIME & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT - User set : Pre-power check voltage event */
    if (I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT == 2)
    {
      if (((VAR_SYSI2C_SYS_INFO>>6) & 0x03) == 0)
      {
        // 24V Car Power System
        if ((VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE >= VAR_VPM_POSTBOOT_VOLTAGE_CHK) &&
            (VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE <= ((__DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_H<<8)|__DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_L)))
        {
          VAR_VPM_PREBOOT_VOLTAGE_CHK = VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x59; // 24V Pre-Boot Voltage Check High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PREBOOT_VOLTAGE_CHK>>8) & 0xFF;
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x5A; // 24V Pre-Boot Voltage Check Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PREBOOT_VOLTAGE_CHK & 0xFF;
          VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
        }
      }
      else
      {
        // 12V Car Power System
        if ((VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE >= VAR_VPM_POSTBOOT_VOLTAGE_CHK) &&
            (VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE <= ((__DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_H<<8)|__DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_L)))
        {
          VAR_VPM_PREBOOT_VOLTAGE_CHK = VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x55; // 12V Pre-Boot Voltage Check High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PREBOOT_VOLTAGE_CHK>>8) & 0xFF;
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x56; // 12V Pre-Boot Voltage Check Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PREBOOT_VOLTAGE_CHK & 0xFF;
          VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
        }
      }
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
    }
    /* I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT - User set : Post power check voltage event */
    if (I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT == 2)
    {
      if (((VAR_SYSI2C_SYS_INFO>>6) & 0x03) == 0)
      {
        // 24V Car Power System
        if ((VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE >= ((__DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_H<<8)|__DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_L)) &&
            (VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE <= VAR_VPM_PREBOOT_VOLTAGE_CHK))
        {
          VAR_VPM_POSTBOOT_VOLTAGE_CHK = VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x5B; // 24V Post-Boot Voltage Check High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POSTBOOT_VOLTAGE_CHK>>8) & 0xFF;
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x5C; // 24V Post-Boot Voltage Check Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POSTBOOT_VOLTAGE_CHK & 0xFF;
          VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
        }
      }
      else
      {
        // 12V Car Power System
        if ((VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE >= ((__DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_H<<8)|__DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_L)) &&
            (VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE <= VAR_VPM_PREBOOT_VOLTAGE_CHK))
        {
          VAR_VPM_POSTBOOT_VOLTAGE_CHK = VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x57; // 12V Post-Boot Voltage Check High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POSTBOOT_VOLTAGE_CHK>>8) & 0xFF;
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x58; // 12V Post-Boot Voltage Check Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POSTBOOT_VOLTAGE_CHK & 0xFF;
          VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
        }
      }
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
    }
    /* I2C_USER_SET_UPS_CHARGE_VOLT_EVENT - User set : UPS change voltage event */
    if (I2C_USER_SET_UPS_CHARGE_VOLT_EVENT == 1)
    {
      I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
      I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
      I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
      I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
      I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
    }
    if (I2C_USER_SET_UPS_CHARGE_VOLT_EVENT == 2)
    {
      if (((VAR_SYSI2C_SYS_INFO>>6) & 0x03) == 0)
      {
        // 24V Car Power System
        if ((VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE >= ((__DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_H<<8)|__DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_L)) &&
            (VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE <= ((__DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_H<<8)|__DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_L)))
        {
          VAR_VPM_START_CHARGING_THRESHOLD = VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x67; // 24V UPS Start Charge Voltage High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_START_CHARGING_THRESHOLD>>8) & 0xFF;
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x68; // 24V UPS Start Charge Voltage Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_START_CHARGING_THRESHOLD & 0xFF;
          VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
        }
      }
      else
      {
        // 12V Car Power System
        if ((VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE >= ((__DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_H<<8)|__DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_L)) &&
            (VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE <= ((__DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_H<<8)|__DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_L)))
        {
          VAR_VPM_START_CHARGING_THRESHOLD = VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x65; // 12V UPS Start Charge Voltage High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_START_CHARGING_THRESHOLD>>8) & 0xFF;
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x66; // 12V UPS Start Charge Voltage Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_START_CHARGING_THRESHOLD & 0xFF;
          VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
        }
      }
      I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
    }
    /* I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT - User set : Shutdown delay event */
    if (I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT == 1)
    {
    }
    if (I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT == 2)
    {
      if ((VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE >= __DEF_SHUT_DOWN_DLY_MIN) &&
          (VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE <= __DEF_SHUT_DOWN_DLY_MAX))
      {
        VAR_VPM_SHUT_DOWN_DLY = VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE;
        // 更新EEPROM的內容
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x29; // Shut Down Delay High Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_SHUT_DOWN_DLY>>8) & 0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x2A; // Shut Down Delay Low Byte
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_SHUT_DOWN_DLY & 0xFF;
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      }
      I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT = 0;
    }
    /* I2C_USER_SET_REFERENCE_VOLT_EVENT - User set : Reference voltage event */
    if (I2C_USER_SET_REFERENCE_VOLT_EVENT == 1)
    {
    }
    if (I2C_USER_SET_REFERENCE_VOLT_EVENT == 2)
    {
      VAR_REFERENCE_VOLTAGE = VAR_USER_CHANGE_REFERENCE_VOLT_VALUE;
      DAC_SetChannel2Data(DAC_Align_12b_R, VAR_REFERENCE_VOLTAGE);
      // 更新EEPROM的內容
      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x71; // Reference Voltage High Byte
      VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_REFERENCE_VOLTAGE>>8) & 0xFF;
      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x72; // Reference Voltage Low Byte
      VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_REFERENCE_VOLTAGE & 0xFF;
      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte
      I2C_USER_SET_REFERENCE_VOLT_EVENT = 0;
    }
      
    if (VAR_SYSI2C_SYS_INFO_CHANGE == 1)
    {
      VAR_SYSI2C_SYS_INFO_CHANGE = 0;
      REFRESH_VPM_LOW_VOLTAGE_PROTECTION_VALUE();
      REFRESH_UPS_START_CHARGE_VOLTAGE();
    }
  }
}
//=============================================================================