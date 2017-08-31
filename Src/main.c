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
#include "eeprom_control.h"
#include "stm32f30x_dbgmcu.h"
#include "debug_port.h"

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

// �����ɥΪ��ܼ�
volatile int VAR_TIMETICK_DELAY = 0;

volatile int VAR_TRACE_VPM_STATE = 1;

// DEBUG �Ϊ��ܼ�
volatile int VAR_DEBUG_PACKET_READY = 0;
volatile int VAR_DEBUG_CMD_LENGTH = 0;
unsigned char DEBUG_Cmd[DEBUG_CmdSize];
u8 DEBUG_TxBuffer[TxBufferSize];
u8 DEBUG_RxBuffer[RxBufferSize];
u16 DEBUG_TxS=0, DEBUG_TxE=0; // Tx Start, End Ptr
u16 DEBUG_RxS=0, DEBUG_RxE=0; // Rx Start, End Ptr

volatile unsigned int VAR_BATTERY_EXIST = 0; // Battery�O�_�s�b�A�w�]���s�b
volatile unsigned int CAR_POWER_EXIST = 0;   // Car Power�O�_�s�b�A�w�]���s�b

volatile unsigned char VAR_SYSTEM_POWER_STATUS = 0; // ������I2C 0x09��
                  // SYSTEM_POWER_STATUS Used
                  // bit 4, for BAT1 exist or not
                  // bit 0, for EXTPWR exist or not
                  // ------------------------------
                  // = 0, not exist
                  // = 1, exist

// �q���O�_�L�Ū��ܼ�
// = 0, �S���L��
// = 1, �L��
volatile unsigned int VAR_BATTERY_OVER_TEMPERATURE = 0;

// �q���O�_�C�q���ܼ�
// = 0, �S���C�q
// = 1, �C�q
volatile unsigned int VAR_BATTERY_UNDER_PERCENTAGE = 0;

// �q���O�_�L�ťB���q�O�_�s�b���ܼ�
// = 0, �S���L�ũΨ��q�s�b
// = 1, �L�ťB���q���s�b
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

// WATCHDOG TIMER
volatile int VAR_WATCHDOG_STATUS                            = 0;    // Watchdog�}�ҩ�����
volatile int VAR_WATCHDOG_COUNTER                           = 0;    // Watchdog�˼ƭp�ɾ�
volatile int VAR_WATCHDOG_RESET_VALUE                       = 10;   // Watchdog�˼ƭp�ɭ��m��

// for printf used <--- fputc()
typedef void (*pFunction)(void);

// Power Button Status Related
volatile unsigned int VAR_PBT_ON_OFF = 0;
                      // =0, ���Power Button at High (Power Button Off)
                      // =1, ���Power Button at Low (Power Button On)
volatile unsigned int VAR_PBT_OFF_2_ON_EVENT = 0;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Power Button Off->On Event
volatile unsigned int VAR_PBT_ON_2_OFF_EVENT = 0;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Power Button On->Off Event
volatile unsigned int VAR_PBT_COUNTER = 0;
                      // ����Power Button�Q��������(5ms)
volatile unsigned int VAR_PBT_POWER_ON_EVENT = 0;
                      // =0, ���No Event
                      // =1, ��ܦ��o��Power Button Power On Event(2s)

// EN LED Status Related
volatile unsigned int VAR_EN_LED_HI_LO = 0;
volatile unsigned int VAR_EN_LED_LO_2_HI_EVENT = 0;
volatile unsigned int VAR_EN_LED_HI_2_LO_EVENT = 0;


// VPM Flow����Variables
unsigned int VAR_VPM_PWR_ON_DLY = 0;                                // EEPROM $0010, $0011
unsigned int VAR_VPM_PWR_OFF_EVT_DLY = 0;                           // EEPROM $0012, $0013
unsigned char VAR_VPM_PWR_MODE_CTRL = 0;                            // EEPROM $0016
unsigned int VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY = 0;                  // EEPROM $0014, $0015
unsigned int VAR_SYSTEM_POWER_SYSTEM_STATE = 0;                     // �t�Φb��ت��A
              // = 0 at S5, Power Off
              // = 1 at S0, Power On
              // = 2 ar S3, Suspend
unsigned int VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0;                 // �]�w�t�ΥߧY�i�J���A�ƥ�
              // = 0, No Event
              // = 1, Enter Power-Off Flow Immediately
              // = 2, Enter Suspend Flow Immediately
              // �e���O�t�Φb�}���ɦ��R�O�~����
// ��bS0�ɵo��Power Low��Event�ɭn�p��B�m���Ҧ�
// �o�w�q�bEEPROM $0052�B
// =0 (default), Notify OS and Begin Count Down
// =1, Not Notify OS, begin Count Down
// =2, Notify OS Only
volatile int VAR_VPM_POWER_LOW_AT_S0_MODE = 0;
// Car Power Low Event Delay
unsigned int VAR_VPM_POWER_LOW_EVT_DLY = 0;                         // EEPROM $0021, $0022
// Car Power Low Hard Delay
unsigned int VAR_VPM_POWER_LOW_HARD_DLY = 13;                       // EEPROM $0023, $0024
// UPS�}�l�R�q���q��Threshold
unsigned int VAR_VPM_START_CHARGING_THRESHOLD = 0x2EC;              // EEPROM $0065, $0066

// �p�ƾ����
unsigned int VAR_VPM_COUNTER_UPDATE_FAIL = 0;                       // �qEEPROM��sCOUNTER�ɬOFAIL�����p
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

// �p��Counter�����ܼ�
unsigned char VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;                // �O�_�}�l�i��Power On Counting


unsigned int VAR_DEBUG_PRINT = 0;

// �W�@������t�Ϊ�Event
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

// �s��Serial Number�B
unsigned char VAR_SERIAL_NUMBER[10] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
unsigned char VAR_SERIAL_NUMBER_CHG_EVENT = 0; // �O�_�o�ͧ���Serial Number�ƥ�

// Wakeup Event Mask
unsigned char VAR_WAKEUP_MASK_HI = 0x00;
unsigned char VAR_WAKEUP_MASK_LO = 0x00;

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
//// printf��
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
  if (VAR_1mS_Event) // �C1mS��Event
  {
    VAR_1mS_Event = 0; // Clear 1mS Event
  }
  // �C5mSEvent
  if (VAR_5mS_Counter >= 5)
  {
    VAR_5mS_Event = 1;
    VAR_5mS_Counter = 0;
  }
  // �C10mSEvent
  if (VAR_10mS_Counter >= 10)
  {
    VAR_10mS_Event = 1;
    VAR_10mS_Counter = 0;
  }
  // �C25mSEvent
  if (VAR_25mS_Counter >= 25)
  {
    VAR_25mS_Event = 1;
    VAR_25mS_Counter = 0;
  }
  // �C100mSEvent
  if (VAR_100mS_Counter >= 100)
  {
    //printf ("Timer %d\n\r",VAR_100mS_Counter);
    VAR_100mS_Event = 1;
    VAR_100mS_Counter = 0;
  }
  // �@��Event�o��
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
  // �[�J���I�s
  // unsigned char Get_Platform_String(int type, int index)�~�|�sĶ���s�J
  // �_�h�|�Q�̨Τƥh��~~
  Get_Platform_String(0,0);

  // �I�ssystem_stm32f10x.c���� SystemInit()�PSystemCoreClockUpdate()�A
  // �H�]�w������Clock
  SystemInit();
  SystemCoreClockUpdate();

  // ���_table reset to offset
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

  DBGMCU_Config(DBGMCU_STOP, DISABLE);						// �bDebug Mode�n�[�o��

  // ����1mS�ɰ�SysTick���_
  // 72000000 / 1000 = 72000
  if (SysTick_Config(SystemCoreClock /1000))
  {
    /* Capture error */
    while (1);
  }

  // ��l��System Tick �Τ� Counter
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

  // ��l��Debug Port
  DEBUG_PORT_INITIALIZATION();
  DEBUG_PRINT("\n\r\n\r..: Hello DMS-SE23 VPM\n\r");

  // ��l��GPIO
  FUNC_GPIO_INIT();
  DEBUG_PRINT("..: GPIO Init Complete\n\r");

  //Initial I2C1 Master
  I2C1_Master_Init();

  //Init I2C2 port as Slave transmitter/receiver
  I2C_Slave_Init();

  // ��sVPM�ܼ�
  UPDATE_VPM_VARIABLE_FROM_EEPROM();
  DEBUG_PRINT("..: EEPROM Update Complete\n\r");

  //VPM version
  DEBUG_PRINT("..: VPM version : %01d.%03d\n\r", __DEF_PROJECT_MAIN_VERSION, __DEF_PROJECT_MINER_VERSION);
  DEBUG_PRINT("..: Platform ID : 0x%02X\n\r", PLATFORM_ID);

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
      DEBUG_Command_Processing(); // �B�zAT Command �{���i�J�I (Command Mode)
      //else
      //  AT_Data_Processing(); // �B�zAT Command �{���i�J�I (Data Mode)
      /* �M��AT Command Buffer -----------------------*/
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
  }
}
//=============================================================================