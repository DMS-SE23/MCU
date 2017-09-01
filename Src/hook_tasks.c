#include "stm32f30x_conf.h"
// User Define Header Files
#include "project.h"
#include "gpio_control.h"
#include "vpm_control.h"
#include "main.h"
#include "stm32f30x_adc.h"
#include "stdio.h"
#include "stm32f30x_rtc.h"
#include "battery_info.h"
#include "watchdog.h"
#include "eeprom_control.h"
#include "hotkey_control.h"
#include "i2c_slave.h"

// ===============================
// Hook Tasks Main Processing Area
// ===============================

// 5mS Hook主函式
void TASK_Hook_5mS()
{
}

// 10mS Hook主函式
void TASK_Hook_10mS()
{
  TASK_VPM_CONTROL();
//  TASK_KEYBOARD_SCAN();
}

// 25mS Hook主函式
void TASK_Hook_25mS()
{
//  TASK_EEPROM_UPDATE_BYTES(); // 每25mS Write EEPROM一次
}

// 100mS Hook主函式
void TASK_Hook_100mS()
{
  TASK_CHECK_BATTERY_PRESENT(); // 檢查電池是否存在
}

// 1S Hook主函式
void TASK_Hook_1S()
{
  HEART_BEAT_LED_CONTROL_TASK();
  BATTERY_INFO_UPDATE();
  TASK_LED_CONTROL();
  TASK_WATCHDOG();
//  TASK_BATTERY_OVER_TEMPERATURE_DETECTION();
  TASK_COUNTING_POWER_ON_MINUTES();
//  TASK_BATTERY_PROTECTION();
//  TASK_BATTERY_UNDER_PERCENTAGE_DETECTION();

/*** For DEBUG only ***/
//  I2C_Slave_TX_UserDEBUG();

  //SHOW_CURRENT_TIME();
  //BATTERY_TEST();
//  DEBUG_PRINT ("VAR_CURRENT_V_CB_VALUE = %d", VAR_CURRENT_V_CB_VALUE);
//  DEBUG_PRINT ("  VAR_PCB_TEMP_VALUE = %d", VAR_PCB_TEMP_VALUE);
//  DEBUG_PRINT ("  VAR_GPS_REF_VALUE = %d", VAR_GPS_REF_VALUE);
//  DEBUG_PRINT ("  VAR_GPS_RF_VALUE = %d\r\n", VAR_GPS_RF_VALUE);

//  TASK_DBG_DUMP_RTC_REGISTERS();
  
//  if (VAR_BRIGHTNESS_VALUE-- <= 0)
//  {
//    VAR_BRIGHTNESS_VALUE = 100;
//  }
//  SET_PWM_WIDTH(INIT_PWM(200), VAR_BRIGHTNESS_VALUE);
//  DEBUG_PRINT ("  VAR_BRIGHTNESS_VALUE = %d\r\n", VAR_BRIGHTNESS_VALUE);
}