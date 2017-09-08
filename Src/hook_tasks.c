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
#include "event_control.h"
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
  GPI_EventManager();
  TASK_VPM_CONTROL();
}

// 25mS Hook主函式
void TASK_Hook_25mS()
{
//  TASK_EEPROM_UPDATE_BYTES(); // 每25mS Write EEPROM一次
}

// 100mS Hook主函式
void TASK_Hook_100mS()
{
  TASK_BATTERY_CHARGE_CONTROL();
}

// 1S Hook主函式
void TASK_Hook_1S()
{
  HEART_BEAT_LED_CONTROL_TASK();
  TASK_POWER_LED_CONTROL();
  BATTERY_INFO_UPDATE();
  TASK_BATTERY_LED_CONTROL();
  TASK_WATCHDOG();
  TASK_COUNTING_POWER_ON_MINUTES();

/*** For DEBUG only ***/
  SHOW_BATTERY_INFO();
}