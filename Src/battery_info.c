#include "stm32f30x_conf.h"
#include "stm32_it.h"
#include <system_stm32f30x.h>
//#include <core_cm3.h>
#include <stm32f30x.h>
#include "gpio_control.h"
#include "main.h"
#include <stdio.h>

#include "project.h"
#include "i2c_master.h"
#include "i2c_slave.h"
#include "eeprom_control.h"
#include "battery_info.h"
#include "debug_port.h"
#include "vpm_control.h"

int I2C_BatteryRead2Byte(unsigned char ReadAddr, unsigned int *ReturndValue)
{
  return(I2C_READ_WORD(DEF_BQBQ27541_ADDRESS, ReadAddr, ReturndValue));
}
//-----------------------------------------------------------------------------


void BATTERY_TEST()
{
//  unsigned int BATTERY_INFO = 0;
//  if (VAR_BATTERY_EXIST == 1) // 電池存在
//  {
//    I2C_BatteryRead2Byte(0x14, &BATTERY_INFO);
//    DEBUG_PRINT ("Battery %4x\r\n",BATTERY_INFO);
//  }
}
//-----------------------------------------------------------------------------


// 偵測電池溫度是否過溫
void TASK_BATTERY_OVER_TEMPERATURE_DETECTION()
{
  if (VAR_BATTERY_EXIST != 1) // 電池不存在
  {
//    DEBUG_PRINT ("@@: Turn off charger, lose battery\r\n");
    VAR_BATTERY_OVER_TEMPERATURE = 0;
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO; // 關掉Battery Charger
    return;
  }

//temp0909 : need to fine tune ADC value
//temp0909  // 查看CAR Power是否有超過Threshold
//temp0909  if (VAR_CURRENT_V_CB_VALUE <= VAR_VPM_START_CHARGING_THRESHOLD)
//temp0909  {
//temp0909    // 車電太低，不允許充電
//temp0909    DEBUG_PRINT ("@@: Turn off charger, less than threshold\r\n");
//temp0909    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO; // 關掉Battery Charger
//temp0909    return;
//temp0909  }

  if (BAT_INFO_Temperature == 0)
  {
    // 電池過放, 導致VPM讀取不到電池的資料, 此時須強制充電
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI;
    return;
  }

  // 電池存在，看Battery的Over Temperature Flag
  float bat_temperature_C = (BAT_INFO_Temperature/10) - 273.15;

  if ((bat_temperature_C >= 50) || (bat_temperature_C <= 0)) // OVER_TEMP_ALARM
  {
    // 當電池存在且過溫時要切斷充電線路
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO; // 關掉Battery Charger

    if (VAR_BATTERY_OVER_TEMPERATURE == 0) // 之前為沒有Over Temperature
    {
      DEBUG_PRINT ("@@: Turn off charger, over temperature\r\n");
      VAR_BATTERY_OVER_TEMPERATURE = 1; // 設定Temperature Over了
      INC_VPM_EVENT_LOG_CNT(5);  // 觸發Battery Over Temperature
      // =========
      // Notify OS
      // =========
      EVENTQUEUE_INSERT_TO_QUEUE(0x03,0x02); // CLASS = 0x03
                                             // ID = 0000 0010
                                             //             - <-- High Temp.
    }
  }
  else
  {
    // 沒有過溫
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI; // 打開Battery Charger
  }
}
//-----------------------------------------------------------------------------


// 更新Battery的資訊
void BATTERY_INFO_UPDATE()
{
  if (VAR_BATTERY_EXIST == 1) // 電池存在
  {
//    I2C_BatteryRead2Byte(BAT_OFFSET_AtRate, &BAT_INFO_AtRate);
//    I2C_BatteryRead2Byte(BAT_OFFSET_UnfilteredSOC, &BAT_INFO_UnfilteredSOC);
    I2C_BatteryRead2Byte(BAT_OFFSET_Temperature, &BAT_INFO_Temperature);
    I2C_BatteryRead2Byte(BAT_OFFSET_Voltage, &BAT_INFO_Voltage);
    I2C_BatteryRead2Byte(BAT_OFFSET_Flags, &BAT_INFO_Flags);

//    I2C_BatteryRead2Byte(BAT_OFFSET_NomAvailableCapacity, &BAT_INFO_NomAvailableCapacity);
//    I2C_BatteryRead2Byte(BAT_OFFSET_FullAvailableCapacity, &BAT_INFO_FullAvailableCapacity);
//    I2C_BatteryRead2Byte(BAT_OFFSET_RemainingCapacity, &BAT_INFO_RemainingCapacity);
//    I2C_BatteryRead2Byte(BAT_OFFSET_FullChargeCapacity, &BAT_INFO_FullChargeCapacity);
    I2C_BatteryRead2Byte(BAT_OFFSET_Current, &BAT_INFO_Current);
    I2C_BatteryRead2Byte(BAT_OFFSET_AverageCurrent, &BAT_INFO_AverageCurrent);

    I2C_BatteryRead2Byte(BAT_OFFSET_TimeToEmpty, &BAT_INFO_TimeToEmpty);
//    I2C_BatteryRead2Byte(BAT_OFFSET_FilteredFCC, &BAT_INFO_FilteredFCC);
//    I2C_BatteryRead2Byte(BAT_OFFSET_StandbyCurrent, &BAT_INFO_StandbyCurrent);
//    I2C_BatteryRead2Byte(BAT_OFFSET_UnfilteredFCC, &BAT_INFO_UnfilteredFCC);
//    I2C_BatteryRead2Byte(BAT_OFFSET_MaxLoadCurrent, &BAT_INFO_MaxLoadCurrent);
//
//    I2C_BatteryRead2Byte(BAT_OFFSET_UnfilteredRM, &BAT_INFO_UnfilteredRM);
//    I2C_BatteryRead2Byte(BAT_OFFSET_FilteredRM, &BAT_INFO_FilteredRM);
//    I2C_BatteryRead2Byte(BAT_OFFSET_AveragePower, &BAT_INFO_AveragePower);
//    I2C_BatteryRead2Byte(BAT_OFFSET_InternalTemperature, &BAT_INFO_InternalTemperature);
//    I2C_BatteryRead2Byte(BAT_OFFSET_CycleCount, &BAT_INFO_CycleCount);

    I2C_BatteryRead2Byte(BAT_OFFSET_StateOfCharge, &BAT_INFO_StateOfCharge);
//    I2C_BatteryRead2Byte(BAT_OFFSET_StateOfHealth, &BAT_INFO_StateOfHealth);
//    I2C_BatteryRead2Byte(BAT_OFFSET_PassedCharge, &BAT_INFO_PassedCharge);
//    I2C_BatteryRead2Byte(BAT_OFFSET_DOD0, &BAT_INFO_DOD0);
//    I2C_BatteryRead2Byte(BAT_OFFSET_SelfDischargeCurrent, &BAT_INFO_SelfDischargeCurrent);



////    DEBUG_PRINT ("BAT_INFO_AtRate %04x\r\n",BAT_INFO_AtRate);
////    DEBUG_PRINT ("BAT_INFO_UnfilteredSOC %04x\r\n",BAT_INFO_UnfilteredSOC);
//    DEBUG_PRINT ("BAT_INFO_Temperature %04x\r\n",BAT_INFO_Temperature);
//    DEBUG_PRINT ("BAT_INFO_Voltage %04x\r\n",BAT_INFO_Voltage);
//    DEBUG_PRINT ("BAT_INFO_Flags %04x\r\n",BAT_INFO_Flags);
////
////    DEBUG_PRINT ("BAT_INFO_NomAvailableCapacity %04x\r\n",BAT_INFO_NomAvailableCapacity);
////    DEBUG_PRINT ("BAT_INFO_FullAvailableCapacity %04x\r\n",BAT_INFO_FullAvailableCapacity);
////    DEBUG_PRINT ("BAT_INFO_RemainingCapacity %04x\r\n",BAT_INFO_RemainingCapacity);
////    DEBUG_PRINT ("BAT_INFO_FullChargeCapacity %04x\r\n",BAT_INFO_FullChargeCapacity);
//    DEBUG_PRINT ("BAT_INFO_AverageCurrent %04x\r\n",BAT_INFO_AverageCurrent);
////
//    DEBUG_PRINT ("BAT_INFO_TimeToEmpty %04x\r\n",BAT_INFO_TimeToEmpty);
////    DEBUG_PRINT ("BAT_INFO_FilteredFCC %04x\r\n",BAT_INFO_FilteredFCC);
////    DEBUG_PRINT ("BAT_INFO_StandbyCurrent %04x\r\n",BAT_INFO_StandbyCurrent);
////    DEBUG_PRINT ("BAT_INFO_UnfilteredFCC %04x\r\n",BAT_INFO_UnfilteredFCC);
////    DEBUG_PRINT ("BAT_INFO_MaxLoadCurrent %04x\r\n",BAT_INFO_MaxLoadCurrent);
////
////    DEBUG_PRINT ("BAT_INFO_UnfilteredRM %04x\r\n",BAT_INFO_UnfilteredRM);
////    DEBUG_PRINT ("BAT_INFO_FilteredRM %04x\r\n",BAT_INFO_FilteredRM);
////    DEBUG_PRINT ("BAT_INFO_AveragePower %04x\r\n",BAT_INFO_AveragePower);
////    DEBUG_PRINT ("BAT_INFO_InternalTemperature %04x\r\n",BAT_INFO_InternalTemperature);
////    DEBUG_PRINT ("BAT_INFO_CycleCount %04x\r\n",BAT_INFO_CycleCount);
////
//    DEBUG_PRINT ("BAT_INFO_StateOfCharge %04x\r\n",BAT_INFO_StateOfCharge);
////    DEBUG_PRINT ("BAT_INFO_StateOfHealth %04x\r\n",BAT_INFO_StateOfHealth);
////    DEBUG_PRINT ("BAT_INFO_PassedCharge %04x\r\n",BAT_INFO_PassedCharge);
////    DEBUG_PRINT ("BAT_INFO_DOD0 %04x\r\n",BAT_INFO_DOD0);
////    DEBUG_PRINT ("BAT_INFO_SelfDischargeCurrent %04x\r\n",BAT_INFO_SelfDischargeCurrent);
//
//    DEBUG_PRINT ("\r\n");
  }
}
//-----------------------------------------------------------------------------


void TASK_BATTERY_PROTECTION()
{
  float bat_temperature_C = (BAT_INFO_Temperature/10) - 273.15;

  if ((bat_temperature_C >= 68) && (CAR_POWER_EXIST == 0))
  {
    if (VAR_BATTERY_PROTECTION_FLAG == 0) // 之前沒發生過
    {
      VAR_BATTERY_PROTECTION_FLAG = 1;
      VPM_STATE = 4500;
      GPIO_OUTPUT_STATUS_INIT();
    }
  }else
  {
    VAR_BATTERY_PROTECTION_FLAG = 0;
  }
}
//-----------------------------------------------------------------------------


int IS_BAT_UNDER_TEMPERATURE()
{
  unsigned int bat_temperature_K = 0;

  if(VAR_BATTERY_EXIST == 1)                                            //battery exist
  {
    I2C_BatteryRead2Byte(BAT_OFFSET_Temperature, &bat_temperature_K);   //read current temperature
    float bat_temperature_C = (bat_temperature_K/10) - 273.15;
    if(bat_temperature_C <= 60)
      return 1;
    else
      return 0;
  }
  else
  {
    return 1;                                                           //under temperature
  }
}
//-----------------------------------------------------------------------------


void TASK_BATTERY_UNDER_PERCENTAGE_DETECTION()
{
  if ((BAT_INFO_StateOfCharge < 25) && (CAR_POWER_EXIST == 0))
  {
    VAR_BATTERY_UNDER_PERCENTAGE = 1;
  }
  else
  {
    VAR_BATTERY_UNDER_PERCENTAGE = 0;
  }
}
//=============================================================================