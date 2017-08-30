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

// �����ɥΪ��ܼ�
extern volatile int VAR_TIMETICK_DELAY;

// GPS �Ϊ��ܼ�
extern volatile int VAR_GPS_PACKET_READY;
extern volatile int VAR_GPS_CMD_LENGTH;
extern unsigned char GPS_Cmd[GPS_CmdSize];
extern u8 GPS_TxBuffer[TxBufferSize];
extern u8 GPS_RxBuffer[RxBufferSize];
extern u16 GPS_TxS, GPS_TxE; // Tx Start, End Ptr
extern u16 GPS_RxS, GPS_RxE; // Rx Start, End Ptr

// DEBUG �Ϊ��ܼ�
extern volatile int VAR_DEBUG_PACKET_READY;
extern volatile int VAR_DEBUG_CMD_LENGTH;
extern unsigned char DEBUG_Cmd[DEBUG_CmdSize];
extern u8 DEBUG_TxBuffer[TxBufferSize];
extern u8 DEBUG_RxBuffer[RxBufferSize];
extern u16 DEBUG_TxS, DEBUG_TxE; // Tx Start, End Ptr
extern u16 DEBUG_RxS, DEBUG_RxE; // Rx Start, End Ptr

extern unsigned int VAR_BATTERY_EXIST; // Battery�O�_�s�b�A�w�]���s�b
extern unsigned int CAR_POWER_EXIST;   // Car Power�O�_�s�b�A�w�]���s�b

extern unsigned int VAR_CURRENT_V_CB_VALUE; // �ثe��CAR Power Value
extern volatile unsigned int VAR_PCB_TEMP_VALUE; // �ثe��PCB Temperture Value
extern volatile unsigned int VAR_GPS_REF_VALUE;  // �ثe��GPS REF Value
extern volatile unsigned int VAR_GPS_RF_VALUE; // �ثe��GPS RF Value
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

extern volatile int I2C_USER_SET_RTC_TIME_EVENT; // User�z�LI2C�]�wRTC
extern unsigned char VAR_USER_CHANGE_RTC_VALUE; // User�z�LI2C�]�w��RTC�Ȧs���ܼ�
//extern long STM32RTC_CounterValue;

extern volatile int I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT; // User�z�LI2C�]�wWDG_COUNTDOWN_TIMER
extern unsigned int VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE; // User�z�LI2C�]�w��WDG_COUNTDOWN_TIMER�Ȧs���ܼ�

extern volatile int I2C_USER_SET_ALARM_TIME_EVENT; // User�z�LI2C�]�wALARM
extern unsigned char VAR_USER_CHANGE_ALARM_VALUE; // User�z�LI2C�]�w��ALARM�Ȧs���ܼ�

extern volatile int I2C_USER_LOAD_EEPROM_DEFAULT_EVENT;  // User�z�LI2C Load EEPROM

extern volatile int I2C_USER_SET_REAR_VIEW_EVENT;  // User�z�LI2C�]�wRear View Status
extern volatile int VAR_REAR_VIEW_STATUS;  // Rear View Status

extern volatile int I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT; // User�z�LI2C�]�wVPM_POWER_LOW_EVT_DLY
extern unsigned int VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE; // User�z�LI2C�]�w��VPM_POWER_LOW_EVT_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT; // User�z�LI2C�]�wVPM_POWER_LOW_HARD_DLY
extern unsigned int VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE; // User�z�LI2C�]�w��VPM_POWER_LOW_HARD_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_VPM_PWR_ON_DLY_EVENT; // User�z�LI2C�]�wVPM_PWR_ON_DLY
extern unsigned int VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE; // User�z�LI2C�]�w��VPM_PWR_ON_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT; // User�z�LI2C�]�wVPM_PWR_OFF_EVT_DLY
extern unsigned int VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE; // User�z�LI2C�]�w��VPM_PWR_OFF_EVT_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT; // User�z�LI2C�]�wVPM_PWR_OFF_HARD_DLY
extern unsigned int VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE; // User�z�LI2C�]�w��VPM_PWR_OFF_HARD_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT; // User�z�LI2C�]�wVPM_POST_PWR_CHK_DLY
extern unsigned int VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE; // User�z�LI2C�]�w��VPM_POST_PWR_CHK_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT; // User�z�LI2C�]�wPRE_PWR_CHK_VOLT
extern unsigned int VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE; // User�z�LI2C�]�w��PRE_PWR_CHK_VOLT�Ȧs���ܼ�

extern volatile int I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT; // User�z�LI2C�]�wPOST_PWR_CHK_VOLT
extern unsigned int VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE; // User�z�LI2C�]�w��POST_PWR_CHK_VOLT�Ȧs���ܼ�

extern volatile int I2C_USER_SET_UPS_CHARGE_VOLT_EVENT; // User�z�LI2C�]�wUPS_CHARGE_VOLT
extern unsigned int VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE; // User�z�LI2C�]�w��UPS_CHARGE_VOLT�Ȧs���ܼ�

extern volatile int I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT; // User�z�LI2C�]�wVPM_SHUT_DOWN_DLY
extern unsigned int VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE; // User�z�LI2C�]�w��VPM_SHUT_DOWN_DLY�Ȧs���ܼ�

extern volatile int I2C_USER_SET_REFERENCE_VOLT_EVENT; // User�z�LI2C�]�wREFERENCE_VOLT
extern unsigned int VAR_USER_CHANGE_REFERENCE_VOLT_VALUE; // User�z�LI2C�]�w��REFERENCE_VOLT�Ȧs���ܼ�

extern unsigned int VAR_SYSTEM_POWER_SYSTEM_STATE; // �t�Φb��ت��A
              // = 0 at S5, Power Off
              // = 1 at S0, Power On
              // = 2 ar S3, Suspend

extern unsigned int AUTO_DETECT_REAR_VIEW_EVENT;

// �q���O�_�L�Ū��ܼ�
// = 0, �S���L��
// = 1, �L��
extern volatile unsigned int VAR_BATTERY_OVER_TEMPERATURE;

// �q���O�_�C�q���ܼ�
// = 0, �S���C�q
// = 1, �C�q
extern volatile unsigned int VAR_BATTERY_UNDER_PERCENTAGE;

// �q���O�_�L�ťB���q�O�_�s�b���ܼ�
// = 0, �S���L�ũΨ��q�s�b
// = 1, �L�ťB���q���s�b
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

extern unsigned int VAR_IMM_CHANGE_WORKING_MODE_EVENT; // �]�w�t�ΥߧY�i�J���A�ƥ�
              // = 0, No Event
              // = 1, Enter Power-Off Flow Immediately
              // = 2, Enter Suspend Flow Immediately
              // �e���O�t�Φb�}���ɦ��R�O�~����

// WATCHDOG TIMER
extern volatile int VAR_WATCHDOG_STATUS;      // Watchdog�}�ҩ�����
extern volatile int VAR_WATCHDOG_COUNTER;     // Watchdog�˼ƭp�ɾ�
extern volatile int VAR_WATCHDOG_RESET_VALUE; // Watchdog�˼ƭp�ɭ��m��

// Ignition Status Related
extern volatile unsigned int VAR_IGN_ON_OFF;
                      // =0, ���Ignition at Low (Ignition Off)
                      // =1, ���Ignition at High (Ignition On)
extern volatile unsigned int VAR_IGN_OFF_2_ON_EVENT;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Ignition Off->On Event
extern volatile unsigned int VAR_IGN_ON_2_OFF_EVENT;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Ignition On->Off Event

// Power Button Status Related
extern volatile unsigned int VAR_PBT_ON_OFF;
                      // =0, ���Power Button at High (Power Button Off)
                      // =1, ���Power Button at Low (Power Button On)
extern volatile unsigned int VAR_PBT_OFF_2_ON_EVENT;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Power Button Off->On Event
extern volatile unsigned int VAR_PBT_ON_2_OFF_EVENT;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Power Button On->Off Event
extern volatile unsigned int VAR_PBT_COUNTER;
                      // ����Power Button�Q��������(5ms)
extern volatile unsigned int VAR_PBT_POWER_ON_EVENT;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Power Button Power On Event(2s)

// EN LED Status Related
extern volatile unsigned int VAR_EN_LED_HI_LO;
extern volatile unsigned int VAR_EN_LED_LO_2_HI_EVENT;
extern volatile unsigned int VAR_EN_LED_HI_2_LO_EVENT;

// VPM Flow����Variables
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
// ��bS0�ɵo��Power Low��Event�ɭn�p��B�m���Ҧ�
// �o�w�q�bEEPROM $0052�B
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
extern volatile char VAR_SHUTDOWN_STATUS;  // Shutdown�}�ҩ�����
extern volatile int VAR_SHUTDOWN_COUNTER;  // Shutdown�˼ƭp�ɾ�
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

// �s��Serial Number�B
extern unsigned char VAR_SERIAL_NUMBER[10];
extern unsigned char VAR_SERIAL_NUMBER_CHG_EVENT; // �O�_�o�ͧ���Serial Number�ƥ�

extern volatile int VAR_VPM_PREBOOT_VOLTAGE_CHK; // Preboot
extern volatile int VAR_VPM_POSTBOOT_VOLTAGE_CHK; // Postboot

extern unsigned char VAR_VPM_PREBOOT_VOLTAGE_CHK_ENABLE; // Preboot
extern unsigned char VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE; // postboot

// UPS�}�l�R�q���q��Threshold
extern unsigned int VAR_VPM_START_CHARGING_THRESHOLD; // EEPROM $0065, $0066

extern unsigned int VAR_VPM_POST_PWR_CHK_DLY_TIME; // EEPROM $0017, $0018

extern unsigned int VAR_VPM_MCU_1ST_POWERED;

extern volatile int VAR_VPM_CAR_POWER_LOW_HAPPENED;

extern unsigned int VAR_DEBUG_PRINT;

// �p�ƾ����
extern unsigned int VAR_VPM_COUNTER_UPDATE_FAIL;  // �qEEPROM��sCOUNTER�ɬOFAIL�����p
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

// �p��Counter�����ܼ�
extern unsigned char VAR_COUNTER_ENABLE_POWER_ON_COUNT; // �O�_�}�l�i��Power On Counting

// �W�@������t�Ϊ�Event
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