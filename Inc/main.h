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

// Variable for 100uS Tick Event, indicates the event occurse
// This variable is rise high in time tick interrupt

// Event Flags
extern volatile int VAR_1mS_Event;
extern volatile int VAR_5mS_Event;
extern volatile int VAR_10mS_Event;
extern volatile int VAR_25mS_Event;
extern volatile int VAR_100mS_Event;
extern volatile int VAR_1S_Event;
// Event Counting Used Variables
extern volatile int VAR_1mS_Counter;
extern volatile int VAR_5mS_Counter;
extern volatile int VAR_10mS_Counter;
extern volatile int VAR_25mS_Counter;
extern volatile int VAR_100mS_Counter;
extern volatile int VAR_1S_Counter;

// 做延時用的變數
extern volatile int VAR_TIMETICK_DELAY;

// DEBUG 用的變數
extern volatile int VAR_DEBUG_PACKET_READY;
extern volatile int VAR_DEBUG_CMD_LENGTH;
extern unsigned char DEBUG_Cmd[DEBUG_CmdSize];
extern u8 DEBUG_TxBuffer[TxBufferSize];
extern u8 DEBUG_RxBuffer[RxBufferSize];
extern u16 DEBUG_TxS, DEBUG_TxE; // Tx Start, End Ptr
extern u16 DEBUG_RxS, DEBUG_RxE; // Rx Start, End Ptr

// 目前所偵測到之Current
extern volatile unsigned int VAR_CURRENT_SENSE_VALUE;

extern unsigned char VAR_BATTERY_EXIST; // Battery是否存在，預設不存在
extern unsigned char CAR_POWER_EXIST;   // Car Power是否存在，預設不存在

extern unsigned int VAR_SYSTEM_POWER_SYSTEM_STATE; // 系統在何種狀態
              // = 0 at S5, Power Off
              // = 1 at S0, Power On
              // = 2 ar S3, Suspend
extern unsigned char VAR_BATTERY_STATE;  // 電池在何種狀態
              // = 0, Battery disattached
              // = 1, Battery not fully charged
              // = 2, Battery fully charged
              // = 3, DC out, and Battery attached

// for battery information
extern unsigned int BAT_INFO_RemainingCapacityAlarm;      // 0x01
extern unsigned int BAT_INFO_RemainingTimeAlarm;          // 0x02
extern unsigned int BAT_INFO_BatteryMode;                 // 0x03
extern unsigned int BAT_INFO_AtRate;                      // 0x04
extern unsigned int BAT_INFO_AtRateTimeToFull;            // 0x05
extern unsigned int BAT_INFO_AtRateTimeToEmpty;           // 0x06
extern unsigned int BAT_INFO_AtRateOK;                    // 0x07
extern unsigned int BAT_INFO_Temperature;                 // 0x08
extern unsigned int BAT_INFO_Voltage;                     // 0x09
extern unsigned int BAT_INFO_Current;                     // 0x0A
extern unsigned int BAT_INFO_AverageCurrent;              // 0x0B
extern unsigned int BAT_INFO_MaxError;                    // 0x0C
extern unsigned int BAT_INFO_RelativeStateOfCharge;       // 0x0D
extern unsigned int BAT_INFO_AbsoluteStateOfCharge;       // 0x0E
extern unsigned int BAT_INFO_RemainingCapacity;           // 0x0F
extern unsigned int BAT_INFO_FullChargeCapacity;          // 0x10
extern unsigned int BAT_INFO_RunTimeToEmpty;              // 0x11
extern unsigned int BAT_INFO_AverageTimeToEmpty;          // 0x12
extern unsigned int BAT_INFO_AverageTimeToFull;           // 0x13
extern unsigned int BAT_INFO_ChargingCurrent;             // 0x14
extern unsigned int BAT_INFO_ChargingVoltage;             // 0x15
extern unsigned int BAT_INFO_BatteryStatus;               // 0x16
extern unsigned int BAT_INFO_CycleCount;                  // 0x17
extern unsigned int BAT_INFO_DesignCapacity;              // 0x18
extern unsigned int BAT_INFO_DesignVoltage;               // 0x19
extern unsigned int BAT_INFO_SpecificationInfo;           // 0x1A
extern unsigned int BAT_INFO_ManufacturerDate;            // 0x1B
extern unsigned int BAT_INFO_SerialNumber;                // 0x1C

extern volatile int VAR_TRACE_VPM_STATE;

extern unsigned int VAR_IMM_CHANGE_WORKING_MODE_EVENT; // 設定系統立即進入狀態事件
              // = 0, No Event
              // = 1, Enter Power-Off Flow Immediately
              // = 2, Enter Suspend Flow Immediately
              // 前提是系統在開機時此命令才有效

// WATCHDOG TIMER
extern volatile unsigned char VAR_WATCHDOG_STATUS;      // Watchdog開啟或關閉
extern volatile int VAR_WATCHDOG_COUNTER;     // Watchdog倒數計時器
extern volatile int VAR_WATCHDOG_RESET_VALUE; // Watchdog倒數計時重置值

// Power Button Status Related
extern volatile unsigned int VAR_POWER_BUTTON_POWER_ON_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Power On Event
extern volatile unsigned int VAR_POWER_BUTTON_OVERRIDE_EVENT;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Override Event

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
extern unsigned char VAR_SERIAL_NUMBER_BUFFER[10];
extern unsigned char VAR_SERIAL_NUMBER_CHG_EVENT; // 是否發生改變Serial Number事件

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
                      // =0x10, Power-Button event occurs
                      // =0x40, Reset Button or Software Reset Event

// Event Queue Used
extern unsigned char VAR_EVENT_QUEUS[__DEF_EVENT_QUEUE_SIZE][2];
extern int VAR_EVENT_INPUT;
extern int VAR_EVENT_OUTPUT;
extern int VAR_EVENT_SIZE;
extern int VAR_ALREADY_NOTIFY_OS_CAR_POWER_LOW;
extern unsigned char VAR_INTERRUPT_STATUS;

// I2C Master Used
extern long TimeOutCounter;
extern unsigned char LastErrorOccurse;

#endif