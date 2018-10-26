#ifndef __BATTERY_INFO_H
#define __BATTERY_INFO_H

/*BAT Command*/
#define BAT_OFFSET_RemainingCapacityAlarm       0x01
#define BAT_OFFSET_RemainingTimeAlarm           0x02
#define BAT_OFFSET_BatteryMode                  0x03
#define BAT_OFFSET_AtRate                       0x04
#define BAT_OFFSET_AtRateTimeToFull             0x05
#define BAT_OFFSET_AtRateTimeToEmpty            0x06
#define BAT_OFFSET_AtRateOK                     0x07
#define BAT_OFFSET_Temperature                  0x08
#define BAT_OFFSET_Voltage                      0x09
#define BAT_OFFSET_Current                      0x0A
#define BAT_OFFSET_AverageCurrent               0x0B
#define BAT_OFFSET_MaxError                     0x0C
#define BAT_OFFSET_RelativeStateOfCharge        0x0D
#define BAT_OFFSET_AbsoluteStateOfCharge        0x0E
#define BAT_OFFSET_RemainingCapacity            0x0F
#define BAT_OFFSET_FullChargeCapacity           0x10
#define BAT_OFFSET_RunTimeToEmpty               0x11
#define BAT_OFFSET_AverageTimeToEmpty           0x12
#define BAT_OFFSET_AverageTimeToFull            0x13
#define BAT_OFFSET_ChargingCurrent              0x14
#define BAT_OFFSET_ChargingVoltage              0x15
#define BAT_OFFSET_BatteryStatus                0x16
#define BAT_OFFSET_CycleCount                   0x17
#define BAT_OFFSET_DesignCapacity               0x18
#define BAT_OFFSET_DesignVoltage                0x19
#define BAT_OFFSET_SpecificationInfo            0x1A
#define BAT_OFFSET_ManufacturerDate             0x1B
#define BAT_OFFSET_SerialNumber                 0x1C
#define BAT_OFFSET_ManufacturerName             0x20
#define BAT_OFFSET_DeviceName                   0x21
#define BAT_OFFSET_StateOfHealth                0x4F

// Function Prototype
int I2C_BatteryRead2Byte (unsigned char ReadAddr, unsigned int *ReturndValue);
int I2C_BatteryWrite2Byte(unsigned char WriteAddr, unsigned int SettingValue);
int I2C_BatteryReadNByte(unsigned char ReadAddr, unsigned int ReadSize, unsigned char *ReturndValue);

void BATTERY_INFO_UPDATE(void);
void SHOW_BATTERY_INFO(void);
void TASK_BATTERY_CHARGE_CONTROL(void);
void CLEAR_BATTERY_INFO(void);

#endif