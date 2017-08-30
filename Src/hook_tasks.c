#include "stm32f30x_conf.h"
// User Define Header Files
#include "project.h"
#include "gpio_control.h"
#include "vpm_control.h"
#include "main.h"

#include "adc_channel.h"
#include "dac_channel.h"
#include "stm32f30x_adc.h"
#include "stdio.h"
#include "stm32f30x_rtc.h"

#include "battery_info.h"

#include "rtc_control.h"
#include "watchdog.h"
#include "eeprom_control.h"
#include "hotkey_control.h"
#include "i2c_slave.h"
#include "pwm_control.h"

// ===============================
// Hook Tasks Main Processing Area
// ===============================

// 5mS Hook�D�禡
void TASK_Hook_5mS()
{
  TASK_DETECT_IGNITION_BEHAVIOR();
  TASK_DETECT_POWER_BUTTON_BEHAVIOR();
  TASK_AUTO_DETECT_RESERVE_GEAR();
  TASK_DETECT_EN_LED_BEHAVIOR();
}

// 10mS Hook�D�禡
void TASK_Hook_10mS()
{
  TASK_VPM_CONTROL();
//  TASK_DBG_DUMP_RTC_REGISTERS();
  TASK_KEYBOARD_SCAN();
}

// 25mS Hook�D�禡
void TASK_Hook_25mS()
{
  TASK_EEPROM_UPDATE_BYTES(); // �C25mS Write EEPROM�@��
}

// 100mS Hook�D�禡
void TASK_Hook_100mS()
{
  TASK_CHECK_BATTERY_PRESENT(); // �ˬd�q���O�_�s�b
//temp0817  TASK_UPDATE_FORCE_DAC_VALUE();  // ��sDAC Value
  TASK_UPDATE_DETECTED_ADC_VALUE(); // ��sADC Value
}

// 1S Hook�D�禡
void TASK_Hook_1S()
{
  // �C���s�@�� RTC Counter �� STM32RTC_CounterValue��
//temp0816  STM32RTC_CounterValue = RTC_GetCounter();
  HEART_BEAT_LED_CONTROL_TASK();
  BATTERY_INFO_UPDATE();
//  TASK_SYNC_RTC_TIME();
  TASK_LED_CONTROL();
  TASK_WATCHDOG();
  TASK_BATTERY_OVER_TEMPERATURE_DETECTION();
  TASK_COUNTING_POWER_ON_MINUTES();
  TASK_BATTERY_PROTECTION();
  TASK_SHUT_DOWN_DETECTION();
  TASK_BATTERY_UNDER_PERCENTAGE_DETECTION();

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