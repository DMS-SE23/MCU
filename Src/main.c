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

#include "includes.h"

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

// DEBUG 用的變數
volatile int VAR_DEBUG_PACKET_READY = 0;
volatile int VAR_DEBUG_CMD_LENGTH = 0;
unsigned char DEBUG_Cmd[DEBUG_CmdSize];
u8 DEBUG_TxBuffer[TxBufferSize];
u8 DEBUG_RxBuffer[RxBufferSize];
u16 DEBUG_TxS=0, DEBUG_TxE=0; // Tx Start, End Ptr
u16 DEBUG_RxS=0, DEBUG_RxE=0; // Rx Start, End Ptr

// 目前所偵測到之Current
volatile unsigned int VAR_CURRENT_SENSE_VALUE = 0;

unsigned char VAR_BATTERY_EXIST = 0; // Battery是否存在，預設不存在
unsigned char CAR_POWER_EXIST = 0;   // Car Power是否存在，預設不存在

// for battery information
unsigned int BAT_INFO_RemainingCapacityAlarm = 0;     // 0x01
unsigned int BAT_INFO_RemainingTimeAlarm = 0;         // 0x02
unsigned int BAT_INFO_BatteryMode = 0;                // 0x03
unsigned int BAT_INFO_AtRate = 0;                     // 0x04
unsigned int BAT_INFO_AtRateTimeToFull = 0;           // 0x05
unsigned int BAT_INFO_AtRateTimeToEmpty = 0;          // 0x06
unsigned int BAT_INFO_AtRateOK = 0;                   // 0x07
unsigned int BAT_INFO_Temperature = 0;                // 0x08
unsigned int BAT_INFO_Voltage = 0;                    // 0x09
unsigned int BAT_INFO_Current = 0;                    // 0x0A
unsigned int BAT_INFO_AverageCurrent = 0;             // 0x0B
unsigned int BAT_INFO_MaxError = 0;                   // 0x0C
unsigned int BAT_INFO_RelativeStateOfCharge = 0;      // 0x0D
unsigned int BAT_INFO_AbsoluteStateOfCharge = 0;      // 0x0E
unsigned int BAT_INFO_RemainingCapacity = 0;          // 0x0F
unsigned int BAT_INFO_FullChargeCapacity = 0;         // 0x10
unsigned int BAT_INFO_RunTimeToEmpty = 0;             // 0x11
unsigned int BAT_INFO_AverageTimeToEmpty = 0;         // 0x12
unsigned int BAT_INFO_AverageTimeToFull = 0;          // 0x13
unsigned int BAT_INFO_ChargingCurrent = 0;            // 0x14
unsigned int BAT_INFO_ChargingVoltage = 0;            // 0x15
unsigned int BAT_INFO_BatteryStatus = 0;              // 0x16
unsigned int BAT_INFO_CycleCount = 0;                 // 0x17
unsigned int BAT_INFO_DesignCapacity = 0;             // 0x18
unsigned int BAT_INFO_DesignVoltage = 0;              // 0x19
unsigned int BAT_INFO_SpecificationInfo = 0;          // 0x1A
unsigned int BAT_INFO_ManufacturerDate = 0;           // 0x1B
unsigned int BAT_INFO_SerialNumber = 0;               // 0x1C
unsigned char BAT_INFO_ManufacturerName[21];          // 0x20
unsigned char BAT_INFO_DeviceName[21];                // 0x21
unsigned int BAT_INFO_StateOfHealth = 0;              // 0x4F
unsigned char BAT_INFO_FullSerialNumber[10] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // 0x00 0x70 0x00             

// AMP Mute Status
unsigned char VAR_AMP_MUTE_STATUS = 0;

// WATCHDOG TIMER
volatile unsigned char VAR_WATCHDOG_STATUS                  = 0;    // Watchdog開啟或關閉
volatile int VAR_WATCHDOG_COUNTER                           = 0;    // Watchdog倒數計時器
volatile int VAR_WATCHDOG_RESET_VALUE                       = 10;   // Watchdog倒數計時重置值

// for printf used <--- fputc()
typedef void (*pFunction)(void);

// Power Button Status Related
volatile unsigned int VAR_POWER_BUTTON_POWER_ON_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Power On Event
volatile unsigned int VAR_POWER_BUTTON_OVERRIDE_EVENT = 0;
                      // =0, 表示No Event
                      // =1, 表示有發生Power Button Override Event

// VPM Flow相關Variables
unsigned int VAR_SYSTEM_POWER_SYSTEM_STATE = 0;                     // 系統在何種狀態
              // = 0 at S5, Power Off
              // = 1 at S0, Power On
              // = 2 ar S3, Suspend
unsigned char VAR_BATTERY_STATE = 0;                                // 電池在何種狀態
              // = 0, Battery disattached
              // = 1, Battery not fully charged
              // = 2, Battery fully charged
              // = 3, DC out, and Battery attached
unsigned int VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0;                 // 設定系統立即進入狀態事件
              // = 0, No Event
              // = 1, Enter Power-Off Flow Immediately
              // = 2, Enter Suspend Flow Immediately
              // 前提是系統在開機時此命令才有效

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
                      // =0x10, Power-Button event occurs
                      // =0x40, Reset Button or Software Reset Event

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
unsigned char VAR_SERIAL_NUMBER_BUFFER[10] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
unsigned char VAR_SERIAL_NUMBER_CHG_EVENT = 0; // 是否發生改變Serial Number事件

//-----------------------------------------------------------------------------

// Event Queue Used
unsigned char VAR_EVENT_QUEUS[__DEF_EVENT_QUEUE_SIZE][2];   //project.h => size=32
                                                            //[0] = evt_class
                                                            //[1] = evt_id
int VAR_EVENT_INPUT                     = 0;
int VAR_EVENT_OUTPUT                    = 0;
int VAR_EVENT_SIZE                      = 0;
int VAR_ALREADY_NOTIFY_OS_CAR_POWER_LOW = 0;
unsigned char VAR_INTERRUPT_STATUS      = 0;
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
    VAR_100mS_Event = 1;
    VAR_100mS_Counter = 0;
  }
  // 一秒的Event發生
  if (VAR_1S_Counter >= 1000)
  {
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
  DEBUG_PRINT("\r\n\r\n..: Hello DMS-SE23 VPM\r\n");

  // 初始化GPIO
  FUNC_GPIO_INIT();
  DEBUG_PRINT("..: GPIO Init Complete\r\n");
  
  // 初始化ADC
  ADC_Configuration();
  DEBUG_PRINT("..: ADC Init Complete\r\n");

  //Initial I2C1 Master
  I2C1_Master_Init();

  //Init I2C2 port as Slave transmitter/receiver
  I2C_Slave_Init();

  // 更新VPM變數
  UPDATE_VPM_VARIABLE_FROM_EEPROM();
  DEBUG_PRINT("..: EEPROM Update Complete\r\n");

  //VPM version
  DEBUG_PRINT("..: VPM version : %01d.%03d\r\n", __DEF_PROJECT_MAIN_VERSION, __DEF_PROJECT_MINER_VERSION);
  DEBUG_PRINT("..: Platform ID : 0x%02X\r\n", PLATFORM_ID);

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
//--------------------------------------    
    if (VAR_POWER_BUTTON_OVERRIDE_EVENT == 1)
    {
      VAR_POWER_BUTTON_OVERRIDE_EVENT = 0;
      VPM_STATE = 4850;
    }
//-------------------------------------- 
    if (VAR_SERIAL_NUMBER_CHG_EVENT == 1)
    {
      EEPROM_UPDATE_SERIAL_NUMBER();
      VAR_SERIAL_NUMBER_CHG_EVENT = 0;
    }
  }
}
//=============================================================================