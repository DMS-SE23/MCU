#ifndef __BATTERY_INFO_H
#define __BATTERY_INFO_H

/*BAT Command*/
//#define BAT_OFFSET_AtRate                   0x02
//#define BAT_OFFSET_UnfilteredSOC            0x04
#define BAT_OFFSET_Temperature              0x08        //0x06
#define BAT_OFFSET_Voltage                  0x09        //0x08
#define BAT_OFFSET_Flags                    0x16        //0x0A
//#define BAT_OFFSET_NomAvailableCapacity     0x0C
//#define BAT_OFFSET_FullAvailableCapacity    0x0E
//#define BAT_OFFSET_RemainingCapacity        0x10
//#define BAT_OFFSET_FullChargeCapacity       0x12
#define BAT_OFFSET_Current                  0x0A
#define BAT_OFFSET_AverageCurrent           0x0B        //0x14
#define BAT_OFFSET_TimeToEmpty              0x11        //0x16
//#define BAT_OFFSET_FilteredFCC              0x18
//#define BAT_OFFSET_StandbyCurrent           0x1A
//#define BAT_OFFSET_UnfilteredFCC            0x1C
//#define BAT_OFFSET_MaxLoadCurrent           0x1E
//#define BAT_OFFSET_UnfilteredRM             0x20
//#define BAT_OFFSET_FilteredRM               0x22
//#define BAT_OFFSET_AveragePower             0x24
//#define BAT_OFFSET_InternalTemperature      0x28
//#define BAT_OFFSET_CycleCount               0x2A
#define BAT_OFFSET_StateOfCharge            0x0D        //0x2C
//#define BAT_OFFSET_StateOfHealth            0x2E
//#define BAT_OFFSET_PassedCharge             0x34
//#define BAT_OFFSET_DOD0                     0x36
//#define BAT_OFFSET_SelfDischargeCurrent     0x38

// Function Prototype
int I2C_BatteryRead2Byte (unsigned char ReadAddr, unsigned int *ReturndValue);

void BATTERY_TEST(void);
void BATTERY_INFO_UPDATE(void);
// 盎代筿放琌筁放
void TASK_BATTERY_OVER_TEMPERATURE_DETECTION(void);
void TASK_BATTERY_PROTECTION(void);
int IS_BAT_UNDER_TEMPERATURE(void);
// 盎代筿筿秖琌筁
void TASK_BATTERY_UNDER_PERCENTAGE_DETECTION(void);

#endif