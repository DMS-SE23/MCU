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

#include "includes.h"

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
    DEBUG_PRINT ("@@ Count Down for Delay = %d\r\n",var_VPM_Count_Down_by_10mS/100);
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
                DEBUG_PRINT("@@: VPM (%d) UNKNOWN STATE, HANGING...\r\n",VPM_STATE);
                VAR_DEBUG_PRINT = 1;
              }
            }
            break;
    case 0: // Start
            __DEBUG_VPM_TRACE("@@: VPM (0000) DMS-SE23 MCU Start\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(200);
            break;
    case 200: // Initialization
            __DEBUG_VPM_TRACE("@@: VPM (0200) Initial VPM System\r\n");
            // 檢查開機的來源
            if (RCC_GetFlagStatus(RCC_FLAG_SFTRST))             // Power on from IDE
            {
              __MACRO_VPM_TRACE(">>: Software Restart\r\n");
              var_boot_source_pass_wait_ignition = 1;
            }
            else
            if (RCC_GetFlagStatus(RCC_FLAG_PORRST))             // Power on from power cable
            {
              __MACRO_VPM_TRACE(">>: Power In Start\r\n");
            }
            else                                                //                    ___
            if (RCC_GetFlagStatus(RCC_FLAG_PINRST))             // 此為按RESET PIN => RST
            {
              __MACRO_VPM_TRACE(">>: Reset Pin Start\r\n");
              var_boot_source_pass_wait_ignition = 1;
            }
            else
            {
              __MACRO_VPM_TRACE(">>: Unknown Reset Start\r\n");
            }
            // The flags must be cleared manually after use
            RCC_ClearFlag();
            __MACRO_CHANGE_VPM_STATE_TO(300);
            break;
    case 300: // Read VPM Parameter from EEPROM，但事實上已經自EEPROM讀回到VPM了
            // 開始進入VPM State Machine
            __DEBUG_VPM_TRACE("@@: VPM (0300) Load VPM Parameter from EEPROM\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(1000);
            break;
    // ******************************** //
    // ***** OFF MODE STATES ********** //
    // ******************************** //
    case 1000:  // Start Power Off Sequence
            __DEBUG_VPM_TRACE("@@: VPM (1000) Start Power Off Sequence\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(1100);
            break;
    case 1100:  // Turn Off Peripheral and Related Powers
            __DEBUG_VPM_TRACE("@@: VPM (1100) Turn Off Peripheral and Related Powers\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(1900);
            break;
    case 1900:  // Initial Some Detection Variables
            __DEBUG_VPM_TRACE("@@: VPM (1900) Clear Detection Variables\r\n");
            // 清除Power Button Events，跳到下一狀態才會
            // 開始偵測Power Button On之Event 
            VAR_POWER_BUTTON_POWER_ON_EVENT = 0;
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
            __MACRO_CHANGE_VPM_STATE_TO(2000);
            break;
    case 2000:  // Power Off State
            // Wait System Events
            __DEBUG_VPM_TRACE("@@: VPM (2000) Power Off State\r\n");
            //-----------------------------------------------------------------
            // 需要忽略Power Button Status時
            // 這是因為按下Reset Button or Software Reset，直接開機進去
            // 不等Power Button的變化
            if (var_boot_source_pass_wait_ignition == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->3000 Reset or SW RST\r\n");
              var_boot_source_pass_wait_ignition = 0;
              VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x40;       // Reset Button or Software Reset Event
              __MACRO_CHANGE_VPM_STATE_TO(3000);        // 進入Start Power On Sequence
              return;
            }
            //-----------------------------------------------------------------
            // 判斷是否以Power Button開機
            if (VAR_POWER_BUTTON_POWER_ON_EVENT == 1)
            {
              VAR_POWER_BUTTON_POWER_ON_EVENT = 0;
              __MACRO_VPM_TRACE(">>: VPM->3000 Power Button Triggered\r\n");
              VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x10;       // Power Button Event
              __MACRO_CHANGE_VPM_STATE_TO(3000);        // 進入Start Power On Sequence
              return;
            }
            break;
    // ******************************** //
    // ***** POWER-ON MODE STATES ***** //
    // ******************************** //
    case 3000:  // Start Power On Sequence
            __DEBUG_VPM_TRACE("@@: VPM (3000) Start Power On Sequence\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(3005);
            break;
    case 3005:  // Do Power On Sequence 1
            __DEBUG_VPM_TRACE("@@: VPM (3005) Power On Sequence 1\r\n");
            __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_HI;
            __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3010);
            break;
    case 3010:  // Do Power On Sequence 2
            __DEBUG_VPM_TRACE("@@: VPM (3010) Power On Sequence 2\r\n");
            __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_LO;
            var_VPM_Count_Down_by_10mS = 80;
            __MACRO_CHANGE_VPM_STATE_TO(3015);
            break;
    case 3015:  // Do Power On Sequence 3
            __DEBUG_VPM_TRACE("@@: VPM (3015) Power On Sequence 3\r\n");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
              __MACRO_CHANGE_VPM_STATE_TO(3020);
               return;
            }
            break;
    case 3020:  // Do Power On Sequence 4
            __DEBUG_VPM_TRACE("@@: VPM (3020) Power On Sequence 4\r\n");
            __OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3025);
            break;
    case 3025:  // Do Power On Sequence 5
            __DEBUG_VPM_TRACE("@@: VPM (3025) Power On Sequence 5\r\n");
            if (__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_HI)
            {
              __MACRO_CHANGE_VPM_STATE_TO(3030);
               return;
            }
            break;
    case 3030:  // Do Power On Sequence 6
            __DEBUG_VPM_TRACE("@@: VPM (3030) Power On Sequence 6\r\n");
            __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3035);
            break;
    case 3035:  // Do Power On Sequence 7
            __DEBUG_VPM_TRACE("@@: VPM (3035) Power On Sequence 7\r\n");
            DEBUG_PRINT("@@: Hello DMS-SE23 MCU Power On\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(3100);
            break;
    case 3100:  // Turn On Power LED
            __DEBUG_VPM_TRACE("@@: VPM (3100) Turn On Power LED\r\n");
            // Turn On Power LED
            VAR_SYSTEM_POWER_SYSTEM_STATE = 1;
            __MACRO_CHANGE_VPM_STATE_TO(3300);
            break;
    case 3300:  // Turn On Peripheral and Related Powers
            __DEBUG_VPM_TRACE("@@: VPM (3300) Turn On Peripheral and Related Powers\r\n");
            // 打開POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 1;
            __MACRO_CHANGE_VPM_STATE_TO(3500);
            break;
    case 3500:  // Clear Power Button Events
            __DEBUG_VPM_TRACE("@@: VPM (3500) Clear Power Button Events\r\n");
            VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0;
            __MACRO_CHANGE_VPM_STATE_TO(4000);
            break;
    case 4000:  // Power On State
            __DEBUG_VPM_TRACE("@@: VPM (4000) Power On State\r\n");
            // 下命令進入Power-Off
            if (VAR_IMM_CHANGE_WORKING_MODE_EVENT == 1)
            {
              // 立即進入Notify OS Power-Off
              __MACRO_VPM_TRACE(">>: VPM->4510 CMD Force Enter POWER DOWN FLOW\r\n");
              VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0; // 處理過了
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            break;
    case 4510:  // Set Delay Time = Ignition Off Power Off Hard Delay
            __DEBUG_VPM_TRACE("@@: VPM (4510) Set Ignition Off Power Off Hard Delay Time\r\n");
            var_VPM_Count_Down_by_10mS = 100 * (long)60;
            __MACRO_CHANGE_VPM_STATE_TO(4800);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 4800:  // Wait for Power Off Flow Hard Delay Time Out or LCD Backlight Going Low
            __DEBUG_VPM_TRACE("@@: VPM (4800) Wait Power Off Flow Hard Delay Time Out or LCD Backlight Going Low\r\n");
            // 印出Counter的倒數值 ----------------------
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS);
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4850 Timeout, Turn Off Power LED\r\n");
              __MACRO_CHANGE_VPM_STATE_TO(4850);                            // 進入Turn Off Backlight and Peripheral Powers
              return;
            }
//            if (__IN_B12_GPIO_IN_BLK_EN_TEST_LO)                             // LCD Backlight Going Low
//            {
//              __MACRO_VPM_TRACE(">>: VPM->4850 LCD Backlight Going Low\r\n");
//              __MACRO_CHANGE_VPM_STATE_TO(4850);                            // 進入Turn Off Backlight and Peripheral Powers
//              return;
//            }
            break;
    case 4850:  // Turn Off Backlight and Peripheral Powers
            __DEBUG_VPM_TRACE("@@: VPM (4850) Turn Off Backlight and Peripheral Powers\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(4500);
            break;
    case 4500:  // Turn Off Power LED
            __DEBUG_VPM_TRACE("@@: VPM (4500) Turn Off Power LED\r\n");
            // Init GPO
            GPIO_OUTPUT_STATUS_INIT();
            // 關閉中斷服務
            VAR_INTERRUPT_STATUS = 0;
            // 關閉POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;
            // 寫回POWER_ON時間COUNT
            WRITEBACK_POWER_ON_TIME(0);
            __MACRO_CHANGE_VPM_STATE_TO(1000);
            break;
  }
}
//=============================================================================