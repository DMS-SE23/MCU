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

#include "main.h"
#include "vpm_control.h"
#include "gpio_control.h"
#include "i2c_master.h"
#include "i2c_slave.h"
#include "eeprom_control.h"
#include "debug_port.h"

/*set to zero at initial state*/
int VPM_STATE = 0;

int VAR_COUNT_1_SEC = 0;

void DBG_DUMP_COUNT_DOWN_VALUE(long var_VPM_Count_Down_by_10mS)
{
  // Debug模式才顯示，否則立即返回
  if (VAR_TRACE_VPM_STATE != 1) return;
  // 每一秒顯示一次
  if (VAR_COUNT_1_SEC == 0)
  {
    VAR_COUNT_1_SEC = 1;
    DEBUG_PRINT ("@@ Count Down for Delay = %d\n\r",var_VPM_Count_Down_by_10mS/100);
  }
  else
  {
    VAR_COUNT_1_SEC++;
    if (VAR_COUNT_1_SEC >= 100) VAR_COUNT_1_SEC = 0;
  }
}

void TASK_VPM_CONTROL()
{
  static long var_VPM_Count_Down_by_10mS;
  static int var_boot_source_pass_wait_ignition = 0;

  // ========== VPM 狀態機 ========== //
  switch (VPM_STATE)
  {
    default:  // 若有不存在之State，先在此無窮迴圈繞
            if (VAR_DEBUG_PRINT == 0)
            {
              if (VAR_TRACE_VPM_STATE == 1)
              {
                DEBUG_PRINT("@@: VPM (%d) UNKNOWN STATE, HANGING...\n\r",VPM_STATE);
                VAR_DEBUG_PRINT = 1;
              }
            }
            break;
    case 0: // Start
            __DEBUG_VPM_TRACE("@@: VPM (0000) DMS-SE23 MCU Start\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(200);
            /*
            // Testing Debug用之部分
            VAR_VPM_KEEP_ALIVE = 0; // KEEP_ALIVE = OFF
            //VAR_VPM_KEEP_ALIVE = 1; // KEEP_ALIVE = ON
            VAR_WAKEUP_MASK_HI |= 0x04; // DI1 Wakeup
            VAR_WAKEUP_MASK_HI |= 0x08; // DI2 Wakeup
            //VAR_WAKEUP_MASK_HI |= 0x10; // RTC Wakeup
            //VAR_VPM_PREBOOT_VOLTAGE_CHK_ENABLE = 1; // Do Pre Boot Car Power Check
            //VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE = 1; // Do Post Boot Car Power Check
            */
            break;
    case 200: // Initialization
            __DEBUG_VPM_TRACE("@@: VPM (0200) Initial VPM System\n\r");
            // 檢查開機的來源
            if (RCC_GetFlagStatus(RCC_FLAG_SFTRST))             // Power on from IDE
            {
              __MACRO_VPM_TRACE(">>: Software Restart\n\r");
              var_boot_source_pass_wait_ignition = 1;
            }
            else
            if (RCC_GetFlagStatus(RCC_FLAG_PORRST))             // Power on from power cable
            {
              __MACRO_VPM_TRACE(">>: Power In Start\n\r");
            }
            else                                                //                    ___
            if (RCC_GetFlagStatus(RCC_FLAG_PINRST))             // 此為按RESET PIN => RST
            {
              __MACRO_VPM_TRACE(">>: Reset Pin Start\n\r");
              var_boot_source_pass_wait_ignition = 1;
            }
            else
            {
              __MACRO_VPM_TRACE(">>: Unknown Reset Start\n\r");
            }
            // The flags must be cleared manually after use
            RCC_ClearFlag();
            __MACRO_CHANGE_VPM_STATE_TO(300);
            break;
    case 300: // Read VPM Parameter from EEPROM，但事實上已經自EEPROM讀回到VPM了
            // 開始進入VPM State Machine
            __DEBUG_VPM_TRACE("@@: VPM (0300) Load VPM Parameter from EEPROM\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(1000);
            break;
    // ******************************** //
    // ***** OFF MODE STATES ********** //
    // ******************************** //
    case 998:  // Set Delay Time = Write LOG to EEPROM Delay
            __DEBUG_VPM_TRACE("@@: VPM (0998) Set Write LOG to EEPROM Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100; // 要等1S
            __MACRO_CHANGE_VPM_STATE_TO(999);
            break;
    case 999:  // Wait for Write LOG to EEPROM Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (0999) Wait for Write LOG to EEPROM Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_CHANGE_VPM_STATE_TO(1000);  // 進入Start Power Off Sequence
              return;
            }
            break;
    case 1000:  // Start Power Off Sequence
            __DEBUG_VPM_TRACE("@@: VPM (1000) Start Power Off Sequence\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(1100);
            break;
    case 1100:  // Turn Off Peripheral and Related Powers
            __DEBUG_VPM_TRACE("@@: VPM (1100) Turn Off Peripheral and Related Powers\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(1900);
            break;
    case 1900:  // Initial Some Detection Variables
            __DEBUG_VPM_TRACE("@@: VPM (1900) Clear Detection Variables\n\r");
            // Initial last wakeup event source
            VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x00;
            // Turn Off Power LED
            VAR_SYSTEM_POWER_SYSTEM_STATE = 0;
            // 關閉POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;
            // Clear Event Queue
            //VAR_EVENT_INPUT = 0;
            //VAR_EVENT_OUTPUT = 0;
            //VAR_EVENT_SIZE = 0;
            // 設定進入Power Off 2秒後Sleep
            var_VPM_Count_Down_by_10mS = 100 * (long)2;
            __MACRO_CHANGE_VPM_STATE_TO(2000);
            VAR_COUNT_1_SEC = 0;                                            // 顯示倒數值初始化
            break;
    case 2000:  // Power Off State
            // Wait System Events
            __DEBUG_VPM_TRACE("@@: VPM (2000) Power Off State\n\r");
            // 印出Counter的倒數值
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS);
            // 進入Power Off State後2秒進入MCU Sleep
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              DEBUG_PRINT("><: VPM->2000 MCU Sleep\n\r");
              // Clear Event Queue
              //VAR_EVENT_INPUT = 0;
              //VAR_EVENT_OUTPUT = 0;
              //VAR_EVENT_SIZE = 0;
              I2C_DeInit(I2C1);                                             // Deinit I2C1
              SUSPEND_WAKEUP_PIN_INIT();
              // STOP MODE
//              PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
              // STANDBY MODE
              //PWR_EnterSTANDBYMode();
              // 清醒之後
              SYSCLKConfig_STOP();
              REINIT_AFTER_WAKEUP_RESUME_FROM_SUSPEND();
              I2C1_Master_Init();                                           // Init I2C
              // 設定進入Power Off 2秒後Sleep
              var_VPM_Count_Down_by_10mS = 100 * (long)2;
              VAR_COUNT_1_SEC = 0;                                          // 顯示倒數值初始化
              return;
            }
            //-----------------------------------------------------------------
            // 需要忽略Power Button Status時
            // 這是因為按下Reset Button or Software Reset，直接開機進去
            // 不等Power Button的變化
            if (var_boot_source_pass_wait_ignition == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->3000 Reset or SW RST\n\r");
              var_boot_source_pass_wait_ignition = 0;
              VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x40;       // Reset Button or Software Reset Event
              __MACRO_CHANGE_VPM_STATE_TO(3000);        // 進入Start Power On Sequence
              return;
            }
            //-----------------------------------------------------------------
            // 判斷是否以Power Button開機
            if (__IN_A0_GPIO_IN_UP_POWER_KEY_BAR_TEST_LO)
            {
              __MACRO_VPM_TRACE(">>: VPM->3000 Power Button Triggered\n\r");
              VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x10;       // Power Button Event
              __MACRO_CHANGE_VPM_STATE_TO(3000);        // 進入Start Power On Sequence
              return;
            }
            break;
    // ******************************** //
    // ***** POWER-ON MODE STATES ***** //
    // ******************************** //
    case 3000:  // Start Power On Sequence
            __DEBUG_VPM_TRACE("@@: VPM (3000) Start Power On Sequence\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(3005);
            break;
    case 3005:  // Do Power On Sequence 1
            __DEBUG_VPM_TRACE("@@: VPM (3005) Power On Sequence 1\n\r");
            __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_HI;
            __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3010);
            break;
    case 3010:  // Do Power On Sequence 2
            __DEBUG_VPM_TRACE("@@: VPM (3010) Power On Sequence 2\n\r");
            __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_LO;
            var_VPM_Count_Down_by_10mS = 80;
            __MACRO_CHANGE_VPM_STATE_TO(3015);
            break;
    case 3015:  // Do Power On Sequence 3
            __DEBUG_VPM_TRACE("@@: VPM (3015) Power On Sequence 3\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
              __MACRO_CHANGE_VPM_STATE_TO(3020);
               return;
            }
            break;
    case 3020:  // Do Power On Sequence 4
            __DEBUG_VPM_TRACE("@@: VPM (3020) Power On Sequence 4\n\r");
            __OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3025);
            break;
    case 3025:  // Do Power On Sequence 5
            __DEBUG_VPM_TRACE("@@: VPM (3025) Power On Sequence 5\n\r");
            if (__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_HI)
            {
              __MACRO_CHANGE_VPM_STATE_TO(3030);
               return;
            }
            break;
    case 3030:  // Do Power On Sequence 6
            __DEBUG_VPM_TRACE("@@: VPM (3030) Power On Sequence 6\n\r");
            __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3035);
            break;
    case 3035:  // Do Power On Sequence 7
            __DEBUG_VPM_TRACE("@@: VPM (3035) Power On Sequence 7\n\r");
            DEBUG_PRINT("@@: Hello DMS-SE23 MCU Power On\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(3100);
            break;
    case 3100:  // Turn On Power LED
            __DEBUG_VPM_TRACE("@@: VPM (3100) Turn On Power LED\n\r");
            // Turn On Power LED (Red)
            VAR_SYSTEM_POWER_SYSTEM_STATE = 1;
            __MACRO_CHANGE_VPM_STATE_TO(3300);
            break;
    case 3300:  // Turn On Peripheral and Related Powers
            __DEBUG_VPM_TRACE("@@: VPM (3300) Turn On Peripheral and Related Powers\n\r");
            // Turn On Power LED (Green)
            VAR_SYSTEM_POWER_SYSTEM_STATE = 2;
            // 打開POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 1;
            __MACRO_CHANGE_VPM_STATE_TO(3500);
            break;
    case 3500:  // Clear Ignition and Power Button Events
            __DEBUG_VPM_TRACE("@@: VPM (3500) Clear Ignition and Power Button Events\n\r");
            VAR_PBT_OFF_2_ON_EVENT = 0;
            VAR_PBT_ON_2_OFF_EVENT = 0;
            VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0;
            __MACRO_CHANGE_VPM_STATE_TO(4000);
            break;
    case 4000:  // Power On State
            __DEBUG_VPM_TRACE("@@: VPM (4000) Power On State\n\r");
            // 下命令進入Power-Off
            if (VAR_IMM_CHANGE_WORKING_MODE_EVENT == 1)
            {
              // 立即進入Notify OS Power-Off
              __MACRO_VPM_TRACE(">>: VPM->4510 CMD Force Enter POWER DOWN FLOW\n\r");
              VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0; // 處理過了
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            break;
    case 4510:  // Set Delay Time = Ignition Off Power Off Hard Delay
            __DEBUG_VPM_TRACE("@@: VPM (4510) Set Ignition Off Power Off Hard Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100 * (long)60;
            __MACRO_CHANGE_VPM_STATE_TO(4800);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 4800:  // Wait for Power Off Flow Hard Delay Time Out or LCD Backlight Going Low
            __DEBUG_VPM_TRACE("@@: VPM (4800) Wait Power Off Flow Hard Delay Time Out or LCD Backlight Going Low\n\r");
            // 印出Counter的倒數值 ----------------------
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS);
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4850 Timeout, Turn Off Power LED\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4850);                            // 進入Turn Off Backlight and Peripheral Powers
              return;
            }
//            if (__IN_B12_GPIO_IN_BLK_EN_TEST_LO)                             // LCD Backlight Going Low
//            {
//              __MACRO_VPM_TRACE(">>: VPM->4850 LCD Backlight Going Low\n\r");
//              __MACRO_CHANGE_VPM_STATE_TO(4850);                            // 進入Turn Off Backlight and Peripheral Powers
//              return;
//            }
            break;
    case 4850:  // Turn Off Backlight and Peripheral Powers
            __DEBUG_VPM_TRACE("@@: VPM (4850) Turn Off Backlight and Peripheral Powers\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(4500);
            break;
    case 4500:  // Turn Off Power LED
            __DEBUG_VPM_TRACE("@@: VPM (4500) Turn Off Power LED\n\r");
            GPIO_OUTPUT_STATUS_INIT();
            // Turn Off Power LED
            VAR_SYSTEM_POWER_SYSTEM_STATE = 0;
            // 關閉POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;
            // 寫回POWER_ON時間COUNT
            WRITEBACK_POWER_ON_TIME(0);
            __MACRO_CHANGE_VPM_STATE_TO(1000);
            break;
  }
}
//=============================================================================