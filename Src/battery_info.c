#include "includes.h"

int I2C_BatteryRead2Byte(unsigned char ReadAddr, unsigned int *ReturndValue)
{
  return(I2C_READ_WORD(DEF_BQ40Z50_ADDRESS, ReadAddr, ReturndValue));
}

int I2C_BatteryWrite2Byte(unsigned char WriteAddr, unsigned int SettingValue)
{
  return(I2C_WRITE_WORD(DEF_BQ40Z50_ADDRESS, WriteAddr, SettingValue));
}

int I2C_BatteryReadNByte(unsigned char ReadAddr, unsigned int ReadSize, unsigned char *ReturndValue)
{
  return(I2C_READ_NBYTE(DEF_BQ40Z50_ADDRESS, ReadAddr, ReadSize, ReturndValue));
}
//-----------------------------------------------------------------------------

// 更新Battery的資訊
void BATTERY_INFO_UPDATE()
{
  if (VAR_BATTERY_EXIST == 1) // 電池存在
  {
//    I2C_BatteryRead2Byte(BAT_OFFSET_RemainingCapacityAlarm, &BAT_INFO_RemainingCapacityAlarm);
//    I2C_BatteryRead2Byte(BAT_OFFSET_RemainingTimeAlarm, &BAT_INFO_RemainingTimeAlarm);
//    I2C_BatteryRead2Byte(BAT_OFFSET_BatteryMode, &BAT_INFO_BatteryMode);
    I2C_BatteryRead2Byte(BAT_OFFSET_AtRate, &BAT_INFO_AtRate);
//    I2C_BatteryRead2Byte(BAT_OFFSET_AtRateTimeToFull, &BAT_INFO_AtRateTimeToFull);
//
    I2C_BatteryRead2Byte(BAT_OFFSET_AtRateTimeToEmpty, &BAT_INFO_AtRateTimeToEmpty);
//    I2C_BatteryRead2Byte(BAT_OFFSET_AtRateOK, &BAT_INFO_AtRateOK);
    I2C_BatteryRead2Byte(BAT_OFFSET_Temperature, &BAT_INFO_Temperature);
    I2C_BatteryRead2Byte(BAT_OFFSET_Voltage, &BAT_INFO_Voltage);
    I2C_BatteryRead2Byte(BAT_OFFSET_Current, &BAT_INFO_Current);
//    I2C_BatteryRead2Byte(BAT_OFFSET_AverageCurrent, &BAT_INFO_AverageCurrent);

//    I2C_BatteryRead2Byte(BAT_OFFSET_MaxError, &BAT_INFO_MaxError);
    I2C_BatteryRead2Byte(BAT_OFFSET_RelativeStateOfCharge, &BAT_INFO_RelativeStateOfCharge);
//    I2C_BatteryRead2Byte(BAT_OFFSET_AbsoluteStateOfCharge, &BAT_INFO_AbsoluteStateOfCharge);
    I2C_BatteryRead2Byte(BAT_OFFSET_RemainingCapacity, &BAT_INFO_RemainingCapacity);
    I2C_BatteryRead2Byte(BAT_OFFSET_FullChargeCapacity, &BAT_INFO_FullChargeCapacity);

//    I2C_BatteryRead2Byte(BAT_OFFSET_RunTimeToEmpty, &BAT_INFO_RunTimeToEmpty);
    I2C_BatteryRead2Byte(BAT_OFFSET_AverageTimeToEmpty, &BAT_INFO_AverageTimeToEmpty);
    I2C_BatteryRead2Byte(BAT_OFFSET_AverageTimeToFull, &BAT_INFO_AverageTimeToFull);
//    I2C_BatteryRead2Byte(BAT_OFFSET_ChargingCurrent, &BAT_INFO_ChargingCurrent);
//    I2C_BatteryRead2Byte(BAT_OFFSET_ChargingVoltage, &BAT_INFO_ChargingVoltage);

    I2C_BatteryRead2Byte(BAT_OFFSET_BatteryStatus, &BAT_INFO_BatteryStatus);
    I2C_BatteryRead2Byte(BAT_OFFSET_CycleCount, &BAT_INFO_CycleCount);
//    I2C_BatteryRead2Byte(BAT_OFFSET_DesignCapacity, &BAT_INFO_DesignCapacity);
//    I2C_BatteryRead2Byte(BAT_OFFSET_DesignVoltage, &BAT_INFO_DesignVoltage);
//    I2C_BatteryRead2Byte(BAT_OFFSET_SpecificationInfo, &BAT_INFO_SpecificationInfo);
//    
//    I2C_BatteryRead2Byte(BAT_OFFSET_ManufacturerDate, &BAT_INFO_ManufacturerDate);
    I2C_BatteryRead2Byte(BAT_OFFSET_SerialNumber, &BAT_INFO_SerialNumber);
    
    I2C_BatteryReadNByte(BAT_OFFSET_ManufacturerName, 21, BAT_INFO_ManufacturerName);
    I2C_BatteryReadNByte(BAT_OFFSET_DeviceName, 21, BAT_INFO_DeviceName);
    I2C_BatteryRead2Byte(BAT_OFFSET_StateOfHealth, &BAT_INFO_StateOfHealth);
  }
  else
  {
    CLEAR_BATTERY_INFO();
  }
}
//-----------------------------------------------------------------------------

void SHOW_BATTERY_INFO()
{
  if (VAR_BATTERY_EXIST == 1) // 電池存在
  {
//    DEBUG_PRINT("BAT_OFFSET_RemainingCapacityAlarm %04x\r\n",BAT_INFO_RemainingCapacityAlarm);
//    DEBUG_PRINT("BAT_OFFSET_RemainingTimeAlarm %04x\r\n",BAT_INFO_RemainingTimeAlarm);
//    DEBUG_PRINT("BAT_OFFSET_BatteryMode %04x\r\n",BAT_INFO_BatteryMode);
//    DEBUG_PRINT("BAT_OFFSET_AtRate %04x\r\n",BAT_INFO_AtRate);
//    DEBUG_PRINT("BAT_OFFSET_AtRateTimeToFull %04x\r\n",BAT_INFO_AtRateTimeToFull);
//
//    DEBUG_PRINT("BAT_OFFSET_AtRateTimeToEmpty %04x\r\n",BAT_INFO_AtRateTimeToEmpty);
//    DEBUG_PRINT("BAT_OFFSET_AtRateOK %04x\r\n",BAT_INFO_AtRateOK);
//    DEBUG_PRINT("BAT_OFFSET_Temperature %04x\r\n",BAT_INFO_Temperature);
//    DEBUG_PRINT("BAT_OFFSET_Voltage %04x\r\n",BAT_INFO_Voltage);
//    DEBUG_PRINT("BAT_OFFSET_Current %04x\r\n",BAT_INFO_Current);
//    DEBUG_PRINT("BAT_OFFSET_AverageCurrent %04x\r\n",BAT_INFO_AverageCurrent);
//
//    DEBUG_PRINT("BAT_OFFSET_MaxError %04x\r\n",BAT_INFO_MaxError);
//    DEBUG_PRINT("BAT_OFFSET_RelativeStateOfCharge %04x\r\n",BAT_INFO_RelativeStateOfCharge);
//    DEBUG_PRINT("BAT_OFFSET_AbsoluteStateOfCharge %04x\r\n",BAT_INFO_AbsoluteStateOfCharge);
//    DEBUG_PRINT("BAT_OFFSET_RemainingCapacity %04x\r\n",BAT_INFO_RemainingCapacity);
//    DEBUG_PRINT("BAT_OFFSET_FullChargeCapacity %04x\r\n",BAT_INFO_FullChargeCapacity);
//
//    DEBUG_PRINT("BAT_OFFSET_RunTimeToEmpty %04x\r\n",BAT_INFO_RunTimeToEmpty);
//    DEBUG_PRINT("BAT_OFFSET_AverageTimeToEmpty %04x\r\n",BAT_INFO_AverageTimeToEmpty);
//    DEBUG_PRINT("BAT_OFFSET_AverageTimeToFull %04x\r\n",BAT_INFO_AverageTimeToFull);
//    DEBUG_PRINT("BAT_OFFSET_ChargingCurrent %04x\r\n",BAT_INFO_ChargingCurrent);
//    DEBUG_PRINT("BAT_OFFSET_ChargingVoltage %04x\r\n",BAT_INFO_ChargingVoltage);
//
//    DEBUG_PRINT("BAT_OFFSET_BatteryStatus %04x\r\n",BAT_INFO_BatteryStatus);
//    DEBUG_PRINT("BAT_OFFSET_CycleCount %04x\r\n",BAT_INFO_CycleCount);
//    DEBUG_PRINT("BAT_OFFSET_DesignCapacity %04x\r\n",BAT_INFOT_DesignCapacity);
//    DEBUG_PRINT("BAT_OFFSET_DesignVoltage %04x\r\n",BAT_INFO_DesignVoltage);
//    DEBUG_PRINT("BAT_OFFSET_SpecificationInfo %04x\r\n",BAT_INFO_SpecificationInfo);
//    
//    DEBUG_PRINT("BAT_OFFSET_ManufacturerDate %04x\r\n",BAT_INFO_ManufacturerDate);
//    DEBUG_PRINT("BAT_OFFSET_SerialNumber %04x\r\n",BAT_INFO_SerialNumber);
//
//    DEBUG_PRINT ("\r\n");
    
//    static int count = 0;

//    if (count++ > 10) {
//      float bat_temperature_C = ((float)BAT_INFO_Temperature / 10) - 273.15;
//      int bat_current = BAT_INFO_Current;
//    
//      if ((bat_current & 0x8000) == 0x8000)
//      {
//        bat_current -= 0xFFFF;
//      }
//    
//      DEBUG_PRINT ("Bat Info: ");
//      DEBUG_PRINT ("StateOfCharge=%d%; ",BAT_INFO_RelativeStateOfCharge);
//      DEBUG_PRINT ("Temp=%.1fC; ",bat_temperature_C);
//      DEBUG_PRINT ("Volt=%dmV; ",BAT_INFO_Voltage);
//      DEBUG_PRINT ("Curr=%dmA; ",bat_current);
//      
//      if (VAR_BATTERY_STATE == 1)
//      {
//        DEBUG_PRINT ("TimeToFull=%dmin; ",BAT_INFO_AverageTimeToFull);
//      }
//      else if (VAR_BATTERY_STATE == 2)
//      {
//      }
//      else if (VAR_BATTERY_STATE == 3)
//      {
//        DEBUG_PRINT ("TimeToEmpty=%dmin; ",BAT_INFO_AverageTimeToEmpty);
//      }
//
//      DEBUG_PRINT ("\r\n");
//      
//      count = 0;
//    }
  }
}

void TASK_BATTERY_CHARGE_CONTROL()
{ 
  if (VAR_BATTERY_EXIST == 0)   // Battery disattached
  {
    // Stop charge
    VAR_BATTERY_STATE = 0;
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO;
    return;
  }
  
  if (CAR_POWER_EXIST == 0)     // DC out, and Battery attached
  {
    // Stop charge
    VAR_BATTERY_STATE = 3;
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO;
    return;
  }
  
  if ((BAT_INFO_BatteryStatus & 0x0020) == 0x0020)      // Battery fully charged
  {
    // Stop charge
    VAR_BATTERY_STATE = 2;
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO;
  }
  else  // Battery not fully charged
  {
    // Start charge
    VAR_BATTERY_STATE = 1;
    __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI;
  }
}

void CLEAR_BATTERY_INFO()
{
  BAT_INFO_Temperature = 0;
  BAT_INFO_Voltage = 0;
  BAT_INFO_Current = 0;
  BAT_INFO_RelativeStateOfCharge = 0;
  BAT_INFO_AverageTimeToEmpty = 0;
  BAT_INFO_AverageTimeToFull = 0;
  BAT_INFO_BatteryStatus = 0;
  BAT_INFO_RemainingCapacity = 0;
  BAT_INFO_FullChargeCapacity = 0;
  BAT_INFO_CycleCount = 0;
  BAT_INFO_SerialNumber = 0;
  memset(BAT_INFO_ManufacturerName, '\0', sizeof(BAT_INFO_ManufacturerName));
  memset(BAT_INFO_DeviceName, '\0', sizeof(BAT_INFO_DeviceName));
  BAT_INFO_StateOfHealth = 0;
}
//=============================================================================