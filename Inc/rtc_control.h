#ifndef __RTC_CONTROL_H
#define __RTC_CONTROL_H

//check if re-initialization is required or not
//extern void RTC_S35390A_INITIALIZATION();
//extern int Get_S35390A_RTCTime(RTC_TIME *rtc_time);
//extern int Set_S35390A_RTCTime(RTC_TIME rtc_time);

void RTC_INITIALIZATION(void); // STM32本身的RTC

void RTC_S35390A_INITIALIZATION(void);
void RTC_S35390A_DUMP_REGISTERS(void);
void TASK_DBG_DUMP_RTC_REGISTERS(void);

// 將RTC的資料格式，轉換成Counter的值
int TRANSLATE_RTC_2_INT (unsigned char rtc_value_format);
// 將Counter的值，轉換成RTC的資料格式
unsigned char TRANSLATE_2_RTC_FORMAT(int counter);

int UPDATE_USER_DEFINED_RTC_VALUE();
void UPDATE_USER_DEFINED_ALARM_VALUE();
void TASK_SYNC_RTC_TIME();
int UPDATE_RTC_FROM_S35390A();
int UPDATE_RTC_TO_S35390A();
void RTC_GetDateTime();
void UPDATE_ALARM();
void SHOW_CURRENT_TIME();

// 清除Alarm Interrupt
void Clear_Alarm_Interrupt(void);
// 啟用ALARM Interrupt
void Enable_Alarm_Interrupt(void);

#endif