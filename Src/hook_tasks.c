#include "includes.h"

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
  TASK_EEPROM_UPDATE_BYTES(); // 每25mS Write EEPROM一次
}

// 100mS Hook主函式
void TASK_Hook_100mS()
{
  TASK_BATTERY_CHARGE_CONTROL();
  TASK_UPDATE_DETECTED_ADC_VALUE();
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
  SHOW_ADC_INFO();
}