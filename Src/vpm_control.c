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
#include "rtc_control.h"
#include "i2c_master.h"
#include "i2c_slave.h"
#include "adc_channel.h"
#include "eeprom_control.h"
#include "debug_port.h"
#include "adv7186_control.h"
#include "battery_info.h"
#include "pwm_control.h"

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

void DBG_DUMP_COUNT_DOWN_VALUE_EX(long var_VPM_Count_Down_by_10mS_ING_OFF, long var_VPM_Count_Down_by_10mS_Low_Power)
{
  // Debug模式才顯示，否則立即返回
  if (VAR_TRACE_VPM_STATE != 1) return;
  // 每一秒顯示一次
  if (VAR_COUNT_1_SEC == 0)
  {
    VAR_COUNT_1_SEC = 1;
    DEBUG_PRINT ("@@ Count Down for IGN OFF Delay = %d, Count Down for Low Power Delay = %d\n\r", var_VPM_Count_Down_by_10mS_ING_OFF/100, var_VPM_Count_Down_by_10mS_Low_Power/100);
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
  static long var_VPM_Count_Down_by_10mS_temp;
  static long var_VPM_Count_Down_by_10mS_IGN_OFF;
  static long var_VPM_Count_Down_by_10mS_Low_Power;
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
            __DEBUG_VPM_TRACE("@@: VPM (0000) TREK-734 VPM Start\n\r");
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
            // 清除Ignition Events，跳到下一狀態才會
            // 開始偵測Ignition由Off to On之Event
            VAR_IGN_OFF_2_ON_EVENT = 0;
            VAR_IGN_ON_2_OFF_EVENT = 0;
            VAR_PBT_OFF_2_ON_EVENT = 0;
            VAR_PBT_ON_2_OFF_EVENT = 0;
            // Initial shutdown variables
            VAR_SHUTDOWN_STATUS = 0;
            VAR_SHUTDOWN_COUNTER = VAR_VPM_SHUT_DOWN_DLY;
            VAR_SHUTDOWN_FLAG = 0;
            // Initial last wakeup event source
            VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x00;
            // Turn Off Power LED
            VAR_SYSTEM_POWER_SYSTEM_STATE = 0;
            // 關閉POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;
            // 清除Alarm Flag
            Clear_Alarm_Interrupt();
            // 若有需要啟用Alarm Wakeup
            if ((VAR_WAKEUP_MASK_HI & 0x10) != 0)
            {
              Enable_Alarm_Interrupt();
            }
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
              // 清除Ignition Off to On Event
              VAR_IGN_OFF_2_ON_EVENT = 0;
              VAR_IGN_ON_2_OFF_EVENT = 0;
              // Clear Event Queue
              //VAR_EVENT_INPUT = 0;
              //VAR_EVENT_OUTPUT = 0;
              //VAR_EVENT_SIZE = 0;
              I2C_DeInit(I2C1);                                             // Deinit I2C1
              DMA_DeInit(DMA1_Channel1);                                    // Deinit DMA1
              ADC_DeInit(ADC1);                                             // Deinit ADC
              SUSPEND_WAKEUP_PIN_INIT();
              // STOP MODE
              PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
              // STANDBY MODE
              //PWR_EnterSTANDBYMode();
              // 清醒之後
              SYSCLKConfig_STOP();
              REINIT_AFTER_WAKEUP_RESUME_FROM_SUSPEND();
              ADC_Configuration();                                         // Init ADC & DMA
              I2C1_Master_Init();                                           // Init I2C
              // 設定進入Power Off 2秒後Sleep
              var_VPM_Count_Down_by_10mS = 100 * (long)2;
              VAR_COUNT_1_SEC = 0;                                          // 顯示倒數值初始化
              return;
            }
            //-----------------------------------------------------------------
            // Ignition -> don't care
            // Power Button pressed, power on directly
            if (VAR_PBT_POWER_ON_EVENT == 1)                             //power button pressed over 2s
            {
              VAR_PBT_POWER_ON_EVENT = 0;
              if (IS_BAT_UNDER_TEMPERATURE() == 1 || CAR_POWER_EXIST == 1)
              {
                __MACRO_VPM_TRACE(">>: VPM->2100 TREK-734 Power Button Triggered\n\r");
                VAR_VPM_CAR_POWER_LOW_HAPPENED = 0;
                VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x10;         // Power button pressed event
                VAR_VPM_MCU_1ST_POWERED = 0;                                // MCU is Not 1st Powered
                __MACRO_CHANGE_VPM_STATE_TO(2100);
                return;
              }
              else
              {
                __MACRO_VPM_TRACE(">>: VPM->1900 BAT Over Temperature & Car Power Lose\n\r");
                __MACRO_CHANGE_VPM_STATE_TO(1900);
                return;
              }
            }
            //-----------------------------------------------------------------
            // 需要忽略Ignition Status時
            // 這是因為按下Reset Button or Software Reset，直接開機進去
            // 不等Ignition的變化
            if (var_boot_source_pass_wait_ignition == 1)
            {
              if (IS_BAT_UNDER_TEMPERATURE() == 1 || CAR_POWER_EXIST == 1)
              {
                __MACRO_VPM_TRACE(">>: VPM->2100 Reset or SW RST\n\r");
                var_boot_source_pass_wait_ignition = 0;
                VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x40; 		// Reset Button or Software Reset Event
                VAR_VPM_MCU_1ST_POWERED = 0;  								// MCU is Not 1st Powered
                __MACRO_CHANGE_VPM_STATE_TO(2100);  						// 進入檢查是否要做Pre-boot Car Power Check
                return;
              }
              else
              {
                __MACRO_VPM_TRACE(">>: VPM->1900 BAT Over Temperature & Car Power Lose\n\r");
                var_boot_source_pass_wait_ignition = 0;
                __MACRO_CHANGE_VPM_STATE_TO(1900);
                return;
              }
            }
            //-----------------------------------------------------------------
            // Priority = 1 [第一個判斷順序]
            // 當有Ignition Off -> On Event發生 (邊緣觸發)
            if ((VAR_IGN_OFF_2_ON_EVENT) && ((VAR_WAKEUP_MASK_LO & 0x02) != 0))
            {
              if (IS_BAT_UNDER_TEMPERATURE() == 1 || CAR_POWER_EXIST == 1)
              {
                __MACRO_VPM_TRACE(">>: VPM->2020 Ignition Off -> On\n\r");
                // 清除Ignition Off-2-High Event
                VAR_IGN_OFF_2_ON_EVENT = 0;
                VAR_IGN_ON_2_OFF_EVENT = 0;
                VAR_VPM_CAR_POWER_LOW_HAPPENED = 0;
                VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x01;         // Ignition Off to On Event
                VAR_VPM_MCU_1ST_POWERED = 0;                                // MCU is Not 1st Powered
                __MACRO_CHANGE_VPM_STATE_TO(2020);                          // 進入Set Timeout = Ignition On Delay
                return;
              }
              else
              {
                __MACRO_VPM_TRACE(">>: VPM->1900 BAT Over Temperature & Car Power Lose\n\r");
                __MACRO_CHANGE_VPM_STATE_TO(1900);
                return;
              }
            }
            //-----------------------------------------------------------------
            // Priority = 2 [第二個判斷順序]
            // KEEP_ALIVE = 1 & Ignition = On，直接開機
            if ((VAR_VPM_KEEP_ALIVE == 1) && (VAR_IGN_ON_OFF == 1) && (VAR_VPM_CAR_POWER_LOW_HAPPENED == 0))
            {
              if (IS_BAT_UNDER_TEMPERATURE() == 1 || CAR_POWER_EXIST == 1)
              {
                __MACRO_VPM_TRACE(">>: VPM->2100 KEEP_ALIVE=1 & Ignition=On\n\r");
                VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x02;         // Keep Alive Event
                VAR_VPM_MCU_1ST_POWERED = 0;                                // MCU is Not 1st Powered
                INC_VPM_EVENT_LOG_CNT(9);                         // 觸發Keep Alive On Power On
                __MACRO_CHANGE_VPM_STATE_TO(2098);                          // 進入Set Delay Time = Write LOG to EEPROM Delay
                return;
              }
              else
              {
                __MACRO_VPM_TRACE(">>: VPM->1900 BAT Over Temperature & Car Power Lose\n\r");
                __MACRO_CHANGE_VPM_STATE_TO(1900);
                return;
              }
            }
            //-----------------------------------------------------------------
            // Priority = 3 [第三個判斷順序]
            // AT_MODE = 1時，當Ignition為ON且MCU 1st Powered時就需要開機
            if ((VAR_VPM_AT_MODE == 1) && (VAR_IGN_ON_OFF == 1) && (VAR_VPM_MCU_1ST_POWERED == 1))
            {
              if (IS_BAT_UNDER_TEMPERATURE() == 1 || CAR_POWER_EXIST == 1)
              {
                __MACRO_VPM_TRACE(">>: VPM->2020 AT_MODE=1 & Ignition=On & MCU 1st Powered\n\r");
                VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x03;         // AT Mode Event
                VAR_VPM_MCU_1ST_POWERED = 0;                                // MCU is Not 1st Powered
                __MACRO_CHANGE_VPM_STATE_TO(2020);                          // 進入Set Timeout = Ignition On Delay
                return;
              }
              else
              {
                __MACRO_VPM_TRACE(">>: VPM->1900 BAT Over Temperature & Car Power Lose\n\r");
                __MACRO_CHANGE_VPM_STATE_TO(1900);
                return;
              }
            }
            //-----------------------------------------------------------------
            // 判斷是否以ALARM開機
            if ((__IN_A7_GPIO_IN_RTC_INT2_TEST_LO) && ((VAR_WAKEUP_MASK_HI & 0x10) != 0))
            {
              if (IS_BAT_UNDER_TEMPERATURE() == 1 || CAR_POWER_EXIST == 1)
              {
                __MACRO_VPM_TRACE(">>: VPM->2100 RTC Alarm Triggered\n\r");
                VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF = 0x30;         // Alarm Event
                VAR_VPM_MCU_1ST_POWERED = 0;                                // MCU is Not 1st Powered
                VAR_SHUTDOWN_STATUS = VAR_SHUTDOWN_EN;
                __MACRO_CHANGE_VPM_STATE_TO(2100);
                break;
              }
              else
              {
                __MACRO_VPM_TRACE(">>: VPM->1900 BAT Over Temperature & Car Power Lose\n\r");
                __MACRO_CHANGE_VPM_STATE_TO(1900);
                return;
              }
            }
            //-----------------------------------------------------------------
            // 若AT_MODE = 0, 又有Ignition On to Off Event發生
            // 要清除掉此Event
            if ((VAR_VPM_AT_MODE == 0) && (VAR_IGN_ON_2_OFF_EVENT))
            {
              __MACRO_VPM_TRACE("><: Clear Off-2-On EVENT (AT_MODE=0, Ignition On -> Off)\n\r");
              VAR_IGN_OFF_2_ON_EVENT = 0; // 清除Ignition Off-2-High Event
              VAR_IGN_ON_2_OFF_EVENT = 0;
            }
            if ((VAR_VPM_AT_MODE == 1)  && (VAR_IGN_ON_2_OFF_EVENT))
            {
              __MACRO_VPM_TRACE("><: Clear On-2-Off EVENT (AT_MODE=1, Ignition On -> Off)\n\r");
              VAR_IGN_OFF_2_ON_EVENT = 0; // 清除Ignition Off-2-High Event
              VAR_IGN_ON_2_OFF_EVENT = 0;
            }
            //-----------------------------------------------------------------

            break;
    case 2020:  // Set Delay Time = Ignition On Delay
            __DEBUG_VPM_TRACE("@@: VPM (2020) Set Ignition On Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100 * (long)VAR_VPM_PWR_ON_DLY;
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            __MACRO_CHANGE_VPM_STATE_TO(2040);
            break;
    case 2040:  // Wait for Ignition On Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (2040) Wait Ignition On Delay Timeout\n\r");
            // 印出Counter的倒數值
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS);
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->2100 Check Pre-boot Car Power Check Enable or Not\n\r");
              INC_VPM_EVENT_LOG_CNT(1); // 觸發Ignition On Power On
              __MACRO_CHANGE_VPM_STATE_TO(2098); // 進入Set Delay Time = Write LOG to EEPROM Delay
              return;
            }
            if (VAR_IGN_ON_OFF == 0) // Ignition Off
            {
              __MACRO_VPM_TRACE(">>: VPM->1900 Ignition Off\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(1900); // 回到Initial Some Detection Variables
              return;
            }
            break;
    case 2098:  // Set Delay Time = Write LOG to EEPROM Delay
            __DEBUG_VPM_TRACE("@@: VPM (2098) Set Write LOG to EEPROM Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100; // 要等1S
            __MACRO_CHANGE_VPM_STATE_TO(2099);
            break;
    case 2099:  // Wait for Write LOG to EEPROM Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (2099) Wait for Write LOG to EEPROM Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_CHANGE_VPM_STATE_TO(2100);  // 進入檢查是否要做Pre-boot Car Power Check
              return;
            }
            break;
    case 2100:  // 檢查是否要做Pre-boot Car Power Check
            __DEBUG_VPM_TRACE("@@: VPM (2100) Check is Enable do Pre-boot Car Power Check\n\r");
            if (VAR_VPM_PREBOOT_VOLTAGE_CHK_ENABLE == 0)
            {
              // 不做Pre-boot Car Power Check
              __MACRO_VPM_TRACE(">>: VPM->2300 Pre-boot Voltage Check Disabled\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(2300);  // 進入Sync RTC Time from S35390A
              return;
            }
            // 要做Pre-boot Car Power Check
            __MACRO_VPM_TRACE(">>: VPM->2200 Pre-boot Voltage Check Enabled\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(2200);  // 進入Pre-boot Car Power Voltage Check
            break;
    case 2200:  // Pre-boot Car Power Voltage Check
            __DEBUG_VPM_TRACE("@@: VPM (2200) Pre-boot Car Power Check\n\r");
            DEBUG_PRINT("CUR PWR: %04X, Pre-boot: %04X\n\r",VAR_CURRENT_V_CB_VALUE,VAR_VPM_PREBOOT_VOLTAGE_CHK);
            if (VAR_CURRENT_V_CB_VALUE < VAR_VPM_PREBOOT_VOLTAGE_CHK)
            {
              __MACRO_VPM_TRACE(">>: VPM->1000 Pre-Boot Voltage Check Fail\n\r");
              INC_VPM_EVENT_LOG_CNT(6); // 觸發Pre-boot Car Power Check Generate Power Off
              __MACRO_CHANGE_VPM_STATE_TO(998);  // 回到Set Delay Time = Write LOG to EEPROM Delay
            }
            else
            {
              __MACRO_VPM_TRACE(">>: VPM->2300 Pre-Boot Voltage Check OK, Power Normal\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(2300);  // 進入Sync RTC Time from S35390A
            }
            break;
    case 2300:  // Sync RTC Time from S35390A
            __DEBUG_VPM_TRACE("@@: VPM (2300) Sync RTC Time from S35390A\n\r");
            UPDATE_RTC_FROM_S35390A();
            __MACRO_CHANGE_VPM_STATE_TO(3000);
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
            __OUT_D9_GPIO_OUT_12VSB_RUN_SET_LO;
            __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
//temp0923             if (VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF != 0x30)
//temp0923             {
//temp0923               __OUT_B4_GPIO_OUT_5V_EN_SET_HI;
//temp0923 DEBUG_PRINT ("** B4_GPIO_OUT_5V_EN = HIGH\n\r");
//temp0923             }
//            var_VPM_Count_Down_by_10mS = 100; // 要等1000mS
            __MACRO_CHANGE_VPM_STATE_TO(3010);
            break;
    case 3010:  // Do Power On Sequence 2
            __DEBUG_VPM_TRACE("@@: VPM (3010) Power On Sequence 2\n\r");
            if (__IN_D8_GPIO_IN_12VSB_PG_TEST_HI)
            {
              __OUT_D7_GPIO_OUT_3V_5V_EN_SET_HI;
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
              var_VPM_Count_Down_by_10mS = 100;						// 要等1000mS
              __MACRO_CHANGE_VPM_STATE_TO(3015);
              return;
            }
            break;
    case 3015:  // Do Power On Sequence 3
            __DEBUG_VPM_TRACE("@@: VPM (3015) Power On Sequence 3\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 90)					// 100ms
            {
              __OUT_B4_GPIO_OUT_5V_EN_SET_HI;
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
              __MACRO_CHANGE_VPM_STATE_TO(3020);
               return;
            }
            break;
    case 3020:  // Do Power On Sequence 4
            __DEBUG_VPM_TRACE("@@: VPM (3020) Power On Sequence 4\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
              __MACRO_CHANGE_VPM_STATE_TO(3025);
              return;
        	}
            break;
    case 3025:  // Do Power On Sequence 5
            __DEBUG_VPM_TRACE("@@: VPM (3025) Power On Sequence 5\n\r");
            DEBUG_PRINT("@@: Hello TREK-734 VPM Power On\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(3030);
            break;
    case 3030:  // Set Delay Time = PMIC Init Delay
            __DEBUG_VPM_TRACE("@@: VPM (3030) Set PMIC Init Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 10; // 要等100mS
            __MACRO_CHANGE_VPM_STATE_TO(3040);
            break;
    case 3040:  // Wait PMIC Init Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (3040) Wait for PMIC Init Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_CHANGE_VPM_STATE_TO(3050);  // 進入Set Delay Time = LCD Backlight Enable Check Delay
              return;
            }
            break;
    case 3050:  // Set Delay Time = LCD Backlight Enable Check Delay
            __DEBUG_VPM_TRACE("@@: VPM (3050) Set LCD Backlight Enable Check Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 1000;              // 要等10S
            VAR_EN_LED_LO_2_HI_EVENT = 0;
            VAR_OS_READY = 0;
            __MACRO_CHANGE_VPM_STATE_TO(3060);
            break;
    case 3060:  // Wait for LCD Backlight Enable Check Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (3060) Wait for LCD Backlight Enable Check Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4850 LCD Backlight Enable is Abnormal\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4850);            // 進入Turn Off Backlight and Peripheral Powers
              return;
            }
            if (VAR_EN_LED_LO_2_HI_EVENT == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->3100 LCD Backlight Enable is Normal\n\r");
              VAR_EN_LED_LO_2_HI_EVENT = 0;
              VAR_OS_READY = 1;
              __MACRO_CHANGE_VPM_STATE_TO(3100);            // 進入Turn On Power LED
              return;
            }
            break;
    case 3100:  // Turn On Power LED
            __DEBUG_VPM_TRACE("@@: VPM (3100) Turn On Power LED\n\r");
            // Turn On Power LED (Red)
            VAR_SYSTEM_POWER_SYSTEM_STATE = 1;
            // 打開POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 1;
            __MACRO_CHANGE_VPM_STATE_TO(3110);
            break;
    case 3110:  // Turn On Peripheral and Related Powers
            __DEBUG_VPM_TRACE("@@: VPM (3110) Turn On Peripheral and Related Powers\n\r");
            __OUT_A15_GPIO_OUT_UART_SW_SET_HI;
            __OUT_B12_GPIO_OUT_VPM_INT_SET_HI;              //LOW : issue interrupt to OS
            __OUT_D11_GPIO_OUT_339_MODE1_SET_LO;            //339_MODE1		= LOW
            __OUT_D12_GPIO_OUT_339_MODE0_SET_HI;            //339_MODE0		= HIGH
            VAR_TRANSCEIVER_STATUS = 1;
            __OUT_B15_GPIO_OUT_CAN_BOOT_SET_HI;             // CAN_BOOT
            SET_PWM_WIDTH(VAR_FUNCTION_KEY_PWM_PERIOD, VAR_FUNCTION_KEY_BRIGHTNESS);    // Set Function Key Brightness PWM
            __OUT_D6_GPIO_OUT_KEYPAD_LED_EN_SET_HI;         // Function Key Backlight Turn On
            __MACRO_CHANGE_VPM_STATE_TO(3115);
            break;
    case 3115:  // Turn On CAN Bus
            __DEBUG_VPM_TRACE("@@: VPM (3115) Turn On CAN Bus\n\r");
            __OUT_B9_GPIO_OUT_CAN_RST_SET_HI;               // CAN_RST#
            __MACRO_CHANGE_VPM_STATE_TO(3300);
            break;
    case 3300:  // ADV7186 Configuration1 (LVDS)
            __DEBUG_VPM_TRACE("@@: VPM (3300) ADV7186 Configuration1 (LVDS)\n\r");
            __OUT_D14_GPIO_OUT_PWRDN_7186_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3310);
            break;
    case 3310:  // ADV7186 Configuration2 (LVDS)
            __DEBUG_VPM_TRACE("@@: VPM (3310) ADV7186 Configuration2 (LVDS)\n\r");
            __OUT_D15_GPIO_OUT_RESET_7186_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(3320);
            break;
    case 3320:  // ADV7186 Configuration3 (LVDS)
            __DEBUG_VPM_TRACE("@@: VPM (3320) ADV7186 Configuration3 (LVDS)\n\r");
            ADV7186_LVDS();
            __MACRO_CHANGE_VPM_STATE_TO(3330);
            break;
    case 3330:  // Set Delay Time = 500ms
            __DEBUG_VPM_TRACE("@@: VPM (3330) Set 500mS Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 50;                // 要等500mS
            __MACRO_CHANGE_VPM_STATE_TO(3340);
            break;
    case 3340:  // Wait for 500mS Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (3340) Wait for 500mS Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              // Don't turn on backlight if wakeup by RTC
              if (VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF != 0x30)
              {
                __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
                // Turn On Power LED (Green)
                VAR_SYSTEM_POWER_SYSTEM_STATE = 2;
              }
              __MACRO_CHANGE_VPM_STATE_TO(3400);  // 進入檢查是否做Post-boot Car Power Check
              return;
            }
            break;
    case 3400:  // 檢查是否做Post-boot Car Power Check
            __DEBUG_VPM_TRACE("@@: VPM (3400) Check is Enable do Post-boot Car Power Check\n\r");
            if (VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE == 0)
            {
              // 不做Post-boot Car Power Check
              __MACRO_VPM_TRACE(">>: VPM->3500 Post-boot Voltage Check Disabled\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(3500);  // 進入Clear Ignition Events
              return;
            }
            // 要做Post-boot Car Power Check
            __MACRO_VPM_TRACE(">>: VPM->3420 Post-boot Voltage Check Enabled\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(3420);  // 進入設定Delay Time = VAR_VPM_POST_PWR_CHK_DLY_TIME
            break;
    case 3420:  // Set Delay Time = VAR_VPM_POST_PWR_CHK_DLY_TIME
            __DEBUG_VPM_TRACE("@@: VPM (3420) Set Delay Time = VAR_VPM_POST_PWR_CHK_DLY_TIME\n\r");
            var_VPM_Count_Down_by_10mS = 10 * (long)VAR_VPM_POST_PWR_CHK_DLY_TIME;
            __MACRO_CHANGE_VPM_STATE_TO(3440);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 3440:  // Wait for VPM Post-boot Car Power Check Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (3440) Wait VPM Post-boot Car Power Check Delay Timeout\n\r");
            // 印出Counter的倒數值 ----------------------
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS);
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->3300 Timeout, Check Post-boot Power\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(3460); // 進入Post-boot Car Power Voltage Check
              return;
            }
            break;
    case 3460:  // Post-boot Car Power Voltage Check
            __DEBUG_VPM_TRACE("@@: VPM (3460) Post-boot Car Power Check\n\r");
            // 強迫獲得Power Value
//            TASK_UPDATE_DETECTED_CAR_POWER_VALUE();
            DEBUG_PRINT("CUR PWR: %04X, Post-boot: %04X\n\r",VAR_CURRENT_V_CB_VALUE,VAR_VPM_POSTBOOT_VOLTAGE_CHK);
            // Pre-Boot Voltage Check
            if (VAR_CURRENT_V_CB_VALUE < VAR_VPM_POSTBOOT_VOLTAGE_CHK)
            {
              __MACRO_VPM_TRACE(">>: VPM->4850 Post-Boot Voltage Check Fail\n\r");
              INC_VPM_EVENT_LOG_CNT(7);                     // 觸發Post-boot Car Power Check Generate Power Off
              __MACRO_CHANGE_VPM_STATE_TO(4698);            // 進入Set Delay Time = Write LOG to EEPROM Delay
            }
            else
            {
              __MACRO_VPM_TRACE(">>: VPM->3500 Post-Boot Voltage Check OK, Power is Normal\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(3500);            // 進入Clear Ignition Events
            }
            break;
    case 3500:  // Clear Ignition and Power Button Events
            __DEBUG_VPM_TRACE("@@: VPM (3500) Clear Ignition and Power Button Events\n\r");
            VAR_IGN_OFF_2_ON_EVENT = 0;
            VAR_IGN_ON_2_OFF_EVENT = 0;
            VAR_PBT_OFF_2_ON_EVENT = 0;
            VAR_PBT_ON_2_OFF_EVENT = 0;
            VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0;
            __MACRO_CHANGE_VPM_STATE_TO(4000);
            break;
    case 4000:  // Power On State
            __DEBUG_VPM_TRACE("@@: VPM (4000) Power On State\n\r");
            // 清掉 Ignition Off to On Event
            if (VAR_IGN_OFF_2_ON_EVENT == 1)
            {
              VAR_IGN_OFF_2_ON_EVENT = 0;
              VAR_SHUTDOWN_STATUS = 0; // Disable shutdown count down
              // Turn on backlight if wakeup by RTC
              if (VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF == 0x30)
              {
                __OUT_B4_GPIO_OUT_5V_EN_SET_HI;
                __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
                // Turn On Power LED (Green)
                VAR_SYSTEM_POWER_SYSTEM_STATE = 2;
              }
            }
            // Shut Down Flag Enable
            if (VAR_SHUTDOWN_FLAG == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Shut Down Flag Enable\n\r");
              VAR_SHUTDOWN_FLAG = 0;
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x08);    // Interrupt OS : Shutdown Flag Enable
              __MACRO_CHANGE_VPM_STATE_TO(4510); // 進入Set Delay Time = Ignition Off Power Off Hard Delay
              return;
            }
            // Event of Ignition Off
            if ((VAR_IGN_ON_2_OFF_EVENT == 1) && ((VAR_SHUTDOWN_MASK & 0x02) != 0)) // Ignition On -> Off
            {
              __MACRO_VPM_TRACE(">>: VPM->4100 Ignition On -> Off\n\r");
              VAR_IGN_ON_2_OFF_EVENT = 0;
              __MACRO_CHANGE_VPM_STATE_TO(4100); // 進入Set Delay Time = Ignition Off Event Delay
              return;
            }
            // 下命令進入Power-Off
            if (VAR_IMM_CHANGE_WORKING_MODE_EVENT == 1)
            {
              // 立即進入Notify OS Power-Off
              __MACRO_VPM_TRACE(">>: VPM->4510 CMD Force Enter POWER DOWN FLOW\n\r");
              VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0; // 處理過了
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            // CAR POWER Low通知
            if ((VAR_CURRENT_V_CB_VALUE < VAR_VPM_POSTBOOT_VOLTAGE_CHK) && (VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE == 1))
            {
              __MACRO_VPM_TRACE(">>: VPM->4200 Car Power Low\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4200);  // 進入Set Delay Time = Car Power Low Event Delay
              return;
            }
            // Battery Under Percentage
            if (VAR_BATTERY_UNDER_PERCENTAGE == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Battery Under Percentage\n\r");
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x04);    // Interrupt OS : Battery Low
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            break;
    case 4100:  // Set Delay Time = Ignition Off Event Delay
            __DEBUG_VPM_TRACE("@@: VPM (4100) Set Ignition Off Event Delay Time\n\r");
            var_VPM_Count_Down_by_10mS_IGN_OFF = 100 * (long)VAR_VPM_PWR_OFF_EVT_DLY;
            __MACRO_CHANGE_VPM_STATE_TO(4400);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 4400:  // Wait for Ignition Off Event Delay Timeout & Check Ignition Status
            __DEBUG_VPM_TRACE("@@: VPM (4400) Wait Ignition Off Event Delay Timeout\n\r");
            // 印出Counter的倒數值 ----------------------
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS_IGN_OFF);
            if (var_VPM_Count_Down_by_10mS_IGN_OFF-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Timeout, Set Timeout = IGN_OFF_POWER_OFF_HARD_DLY\n\r");
              INC_VPM_EVENT_LOG_CNT(4); // 觸發Ignition OFF to Off
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x01);    // Interrupt OS : Ignition Off
              __MACRO_CHANGE_VPM_STATE_TO(4508); // 進入Set Delay Time = Write LOG to EEPROM Delay
              return;
            }
            //Ignition On
            if (VAR_IGN_ON_OFF == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->3500 Ignition Off Event Cancel\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(3500); // 回到Clear Ignition Events
              return;
            }
            // 下命令進入Power-Off
            if (VAR_IMM_CHANGE_WORKING_MODE_EVENT == 1)
            {
              // 立即進入Notify OS Power-Off
              __MACRO_VPM_TRACE(">>: VPM->4510 CMD Force Enter POWER DOWN FLOW\n\r");
              VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0; // 處理過了
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            // CAR POWER Low通知
            if ((VAR_CURRENT_V_CB_VALUE < VAR_VPM_POSTBOOT_VOLTAGE_CHK) && (VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE == 1))
            {
              __MACRO_VPM_TRACE(">>: VPM->4900 Car Power Low\n\r");
              var_VPM_Count_Down_by_10mS_Low_Power = 100 * (long)VAR_VPM_POWER_LOW_EVT_DLY;
              __MACRO_CHANGE_VPM_STATE_TO(4900);  // 進入Set Delay Time = Ignition Off Event or Car Power Low Delay Timeout
              VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
              return;
            }
            // Battery Under Percentage
            if (VAR_BATTERY_UNDER_PERCENTAGE == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Battery Under Percentage\n\r");
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x04);    // Interrupt OS : Battery Low
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            break;
    case 4200: // Set Delay Time = Car Power Low Event Delay
            __DEBUG_VPM_TRACE("@@: VPM (4200) Set Car Power Low Event Delay Time\n\r");
            var_VPM_Count_Down_by_10mS_Low_Power = 100 * (long)VAR_VPM_POWER_LOW_EVT_DLY;
            __MACRO_CHANGE_VPM_STATE_TO(4300);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 4300: // Wait for Car Power Low Event Delay Timeout and Check Car Power Status
            __DEBUG_VPM_TRACE("@@: VPM (4300) Wait for Car Power Low Event Delay Timeout and Check Car Power Status\n\r");
            // 印出Counter的倒數值 ----------------------
            DBG_DUMP_COUNT_DOWN_VALUE(var_VPM_Count_Down_by_10mS_Low_Power);
            // 清掉 Ignition Off to On Event
            if (VAR_IGN_OFF_2_ON_EVENT == 1)
            {
              VAR_IGN_OFF_2_ON_EVENT = 0;
              VAR_SHUTDOWN_STATUS = 0; // Disable shutdown count down
              // Turn on backlight if wakeup by RTC
              if (VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF == 0x30)
              {
                __OUT_B4_GPIO_OUT_5V_EN_SET_HI;
                __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
                // Turn On Power LED (Green)
                VAR_SYSTEM_POWER_SYSTEM_STATE = 2;
              }
            }
            // Timeout
            if (var_VPM_Count_Down_by_10mS_Low_Power-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4520 Timeout, Check Car Power Low Event Mode\n\r");
              VAR_VPM_CAR_POWER_LOW_HAPPENED = 1;
              INC_VPM_EVENT_LOG_CNT(3); // 觸發Car Power Low to Off
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x02);    // Interrupt OS : Car Power Low
              __MACRO_CHANGE_VPM_STATE_TO(4518); // 進入Set Delay Time = Write LOG to EEPROM Delay
              return;
            }
            // Shut Down Flag Enable
            if (VAR_SHUTDOWN_FLAG == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Shut Down Flag Enable\n\r");
              VAR_SHUTDOWN_FLAG = 0;
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x08);    // Interrupt OS : Shutdown Flag Enable
              __MACRO_CHANGE_VPM_STATE_TO(4510); // 進入Set Delay Time = Ignition Off Power Off Hard Delay
              return;
            }
            // Post-boot Voltage Check Changing Disable
            if (VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE == 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->3500 Post-boot Voltage Check Changing Disable\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(3500);  // 回到Clear Ignition Events
              return;
            }
            // Car Power Going Normal
            if (VAR_CURRENT_V_CB_VALUE >= VAR_VPM_POSTBOOT_VOLTAGE_CHK)
            {
              __MACRO_VPM_TRACE(">>: VPM->3500 Car Power Going Normal\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(3500);  // 回到Clear Ignition Events
              return;
            }
            // Ignition On to Off Event
            if ((VAR_IGN_ON_2_OFF_EVENT == 1) && ((VAR_SHUTDOWN_MASK & 0x02) != 0))
            {
              __MACRO_VPM_TRACE(">>: VPM->; Ignition On to Off Event\n\r");
              VAR_IGN_ON_2_OFF_EVENT = 0;
              var_VPM_Count_Down_by_10mS_IGN_OFF = 100 * (long)VAR_VPM_PWR_OFF_EVT_DLY;
              __MACRO_CHANGE_VPM_STATE_TO(4900); // 進入Set Delay Time = Ignition Off Event or Car Power Low Delay Timeout
              VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
              return;
            }
            // 下命令進入Power-Off
            if (VAR_IMM_CHANGE_WORKING_MODE_EVENT == 1)
            {
              // 立即進入Notify OS Power-Off
              __MACRO_VPM_TRACE(">>: VPM->4510 CMD Force Enter POWER DOWN FLOW\n\r");
              VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0; // 處理過了
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            // Battery Under Percentage
            if (VAR_BATTERY_UNDER_PERCENTAGE == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Battery Under Percentage\n\r");
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x04);    // Interrupt OS : Battery Low
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            break;
    case 4508:  // Set Delay Time = Write LOG to EEPROM Delay
            __DEBUG_VPM_TRACE("@@: VPM (4508) Set Write LOG to EEPROM Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100; // 要等1S
            __MACRO_CHANGE_VPM_STATE_TO(4509);
            break;
    case 4509:  // Wait for Write LOG to EEPROM Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (4509) Wait for Write LOG to EEPROM Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_CHANGE_VPM_STATE_TO(4510);  // 進入Set Delay Time = Ignition Off Power Off Hard Delay
              return;
            }
            break;
    case 4510:  // Set Delay Time = Ignition Off Power Off Hard Delay
            __DEBUG_VPM_TRACE("@@: VPM (4510) Set Ignition Off Power Off Hard Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100 * (long)VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY;
            __MACRO_CHANGE_VPM_STATE_TO(4800);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 4518:  // Set Delay Time = Write LOG to EEPROM Delay
            __DEBUG_VPM_TRACE("@@: VPM (4518) Set Write LOG to EEPROM Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100; // 要等1S
            __MACRO_CHANGE_VPM_STATE_TO(4519);
            break;
    case 4519:  // Wait for Write LOG to EEPROM Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (4519) Wait for Write LOG to EEPROM Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_CHANGE_VPM_STATE_TO(4520);  // 進入Set Delay Time = Car Power Low Hard Delay
              return;
            }
            break;
    case 4520: // Set Delay Time = Car Power Low Hard Delay
            __DEBUG_VPM_TRACE("@@: VPM (4520) Set Car Power Low Hard Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100 * (long)VAR_VPM_POWER_LOW_HARD_DLY;
            __MACRO_CHANGE_VPM_STATE_TO(4800);
            VAR_COUNT_1_SEC = 0; // 顯示倒數值初始化
            break;
    case 4698:  // Set Delay Time = Write LOG to EEPROM Delay
            __DEBUG_VPM_TRACE("@@: VPM (4698) Set Write LOG to EEPROM Delay Time\n\r");
            var_VPM_Count_Down_by_10mS = 100;               // 要等1S
            __MACRO_CHANGE_VPM_STATE_TO(4699);
            break;
    case 4699:  // Wait for Write LOG to EEPROM Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (4699) Wait for Write LOG to EEPROM Delay Timeout\n\r");
            if (var_VPM_Count_Down_by_10mS-- <= 0)
            {
              __MACRO_CHANGE_VPM_STATE_TO(4850);            // 進入Turn Off Backlight and Peripheral Powers
              return;
            }
            break;
    case 4700:  // Start Power Off Sequence (6s)
            __DEBUG_VPM_TRACE("@@: VPM (4700) Start Power Off Sequence (6s)\n\r");
            __MACRO_CHANGE_VPM_STATE_TO(4705);
            break;
    case 4705:  // Do Power Off Sequence 1 (6s)
            __DEBUG_VPM_TRACE("@@: VPM (4705) Power Off Sequence 1 (6s)\n\r");
            __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
            __MACRO_CHANGE_VPM_STATE_TO(4710);
            break;
    case 4710:  // Do Power Off Sequence 2 (6s)
            __DEBUG_VPM_TRACE("@@: VPM (4710) Power Off Sequence 2 (6s)\n\r");
            __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
            var_VPM_Count_Down_by_10mS_temp = 600; // 要等6000mS~6s
            __MACRO_CHANGE_VPM_STATE_TO(4715);
            break;
    case 4715:  // Do Power Off Sequence 3 (6s)
            __DEBUG_VPM_TRACE("@@: VPM (4715) Power Off Sequence 3 (6s)\n\r");
            if (var_VPM_Count_Down_by_10mS_temp-- <= 0)
            {
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
              __MACRO_CHANGE_VPM_STATE_TO(4720);
              return;
            }
            break;
    case 4720:  // Do Power Off Sequence 4 (6s)
            __DEBUG_VPM_TRACE("@@: VPM (4720) Power Off Sequence 4 (6s)\n\r");
            DEBUG_PRINT("@@: Hello TREK-734 VPM Power Off\r\n");
            __MACRO_CHANGE_VPM_STATE_TO(4500);
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
            if (__IN_C7_GPIO_IN_EN_LED_TEST_LO)                             // LCD Backlight Going Low
            {
              __MACRO_VPM_TRACE(">>: VPM->4850 LCD Backlight Going Low\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4850);                            // 進入Turn Off Backlight and Peripheral Powers
              return;
            }
            break;
    case 4850:  // Turn Off Backlight and Peripheral Powers
            __DEBUG_VPM_TRACE("@@: VPM (4850) Turn Off Backlight and Peripheral Powers\n\r");
            // Turn Off Backlight
            __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;
            __OUT_D15_GPIO_OUT_RESET_7186_SET_LO;
            __OUT_D14_GPIO_OUT_PWRDN_7186_SET_LO;
            __OUT_A15_GPIO_OUT_UART_SW_SET_LO;
            VAR_OS_READY = 0;
            SET_PWM_WIDTH(VAR_FUNCTION_KEY_PWM_PERIOD, 0);    // Set Function Key Brightness PWM
            __OUT_D6_GPIO_OUT_KEYPAD_LED_EN_SET_LO;         // Function Key Backlight Turn Off
            __MACRO_CHANGE_VPM_STATE_TO(4700);
            break;
    case 4500:  // Turn Off Power LED
            __DEBUG_VPM_TRACE("@@: VPM (4500) Turn Off Power LED\n\r");
            __OUT_B4_GPIO_OUT_5V_EN_SET_LO;
            __OUT_D7_GPIO_OUT_3V_5V_EN_SET_LO;
            // wait to check dealy time
            __OUT_D9_GPIO_OUT_12VSB_RUN_SET_HI;
            GPIO_OUTPUT_STATUS_INIT();

            // Turn Off Power LED
            VAR_SYSTEM_POWER_SYSTEM_STATE = 0;
            // 關閉POWER ON 時間 COUNT
            VAR_COUNTER_ENABLE_POWER_ON_COUNT = 0;
            // 寫回POWER_ON時間COUNT
            WRITEBACK_POWER_ON_TIME(0);
            __MACRO_CHANGE_VPM_STATE_TO(1000);
            break;
    case 4900:  // Wait for Ignition Off Event or Car Power Low Delay Timeout
            __DEBUG_VPM_TRACE("@@: VPM (4900) Wait Ignition Off Event or Car Power Low Delay Timeout\n\r");
            // 印出Counter的倒數值 ----------------------
            DBG_DUMP_COUNT_DOWN_VALUE_EX(var_VPM_Count_Down_by_10mS_IGN_OFF, var_VPM_Count_Down_by_10mS_Low_Power);
            // IGN OFF Timeout
            if (var_VPM_Count_Down_by_10mS_IGN_OFF-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Timeout, Set Timeout = IGN_OFF_POWER_OFF_HARD_DLY\n\r");
              INC_VPM_EVENT_LOG_CNT(4); // 觸發Ignition OFF to Off
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x01);    // Interrupt OS : Ignition Off
              __MACRO_CHANGE_VPM_STATE_TO(4508); // 進入Set Delay Time = Write LOG to EEPROM Delay
              return;
            }
            // Low Power Timeout
            if (var_VPM_Count_Down_by_10mS_Low_Power-- <= 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4520 Timeout, Check Car Power Low Event Mode\n\r");
              VAR_VPM_CAR_POWER_LOW_HAPPENED = 1;
              INC_VPM_EVENT_LOG_CNT(3); // 觸發Car Power Low to Off
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x02);    // Interrupt OS : Car Power Low
              __MACRO_CHANGE_VPM_STATE_TO(4518); // 進入Set Delay Time = Write LOG to EEPROM Delay
              return;
            }
            // Post-boot Voltage Check Changing Disable
            if (VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE == 0)
            {
              __MACRO_VPM_TRACE(">>: VPM->4400 Post-boot Voltage Check Changing Disable\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4400);  // 回到Wait for Ignition Off Event Delay Timeout & Check Ignition Status
              return;
            }
            // Car Power Going Normal
            if (VAR_CURRENT_V_CB_VALUE >= VAR_VPM_POSTBOOT_VOLTAGE_CHK)
            {
              __MACRO_VPM_TRACE(">>: VPM->4400 Car Power Going Normal\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4400);  // 回到Wait for Ignition Off Event Delay Timeout & Check Ignition Status
              return;
            }
            // Ignition On
            if (VAR_IGN_ON_OFF == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4300 Ignition Off Event Cancel\n\r");
              __MACRO_CHANGE_VPM_STATE_TO(4300); // 回到Wait for Car Power Low Event Delay Timeout and Check Car Power Status
              return;
            }
            // 下命令進入Power-Off
            if (VAR_IMM_CHANGE_WORKING_MODE_EVENT == 1)
            {
              // 立即進入Notify OS Power-Off
              __MACRO_VPM_TRACE(">>: VPM->4510 CMD Force Enter POWER DOWN FLOW\n\r");
              VAR_IMM_CHANGE_WORKING_MODE_EVENT = 0; // 處理過了
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            // Battery Under Percentage
            if (VAR_BATTERY_UNDER_PERCENTAGE == 1)
            {
              __MACRO_VPM_TRACE(">>: VPM->4510 Battery Under Percentage\n\r");
              EVENTQUEUE_INSERT_TO_QUEUE(0x08,0x04);    // Interrupt OS : Battery Low
              __MACRO_CHANGE_VPM_STATE_TO(4510);
              return;
            }
            break;
  }
}
//=============================================================================