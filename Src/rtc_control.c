#include "main.h"
#include "stm32f30x_rtc.h"
#include "i2c_master.h"
#include <stdio.h>

#define USE_RTC_HSE 1

//Global time stored the s35390a rtc time
extern RTC_TIME VAR_RTC_TIME_S35390A; // 這定義在main.c中
                                      // 需要include "project.h" 的結構宣告
extern ALARM_TIME VAR_ALARM_TIME;

  //unsigned char count = 5;

  // Status 1 of S-35390A
  // 7654-3210
  // |||| ||||
  // |||| |||+--- POC
  // |||| ||+---- BLD
  // |||| |+----- INT2
  // |||| +------ INT1
  // |||+-------- SC1
  // ||+--------- SC2
  // |+---------- 12/24
  // +----------- RESET
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Status 2 of S-35390A
  // 7654-3210
  // |||| ||||
  // |||| |||+--- TEST
  // |||| ||+---- INT2AE
  // |||| |+----- INT2ME
  // |||| +------ INT2FE
  // |||+-------- 32kE
  // ||+--------- INT1AE
  // |+---------- INT1ME
  // +----------- INT1FE

  //If POC = 1 or BLD = 1
  // 當電源接上時(也就是裝上電池)，會設置 POC=1；
  //   在此時需要設置 RESET=1，讓 chip reset
  // 當電源拔掉時(也就是突然接觸不良)，會設置 BLD=1；
  //   在此時需要設置 RESET=1，讓 chip reset
  // 當有不明原因讓 chip 進入 test mode 時，會設置 TEST=1；
  //   在此時需要設置 RESET=1，讓 chip reset

  /*
  // (1) 讀取RTC S35390A的 Status
  I2C_RTC_READ_NBYTE(0x60,1,&status1); // Read Status 1
  // (2) Compare POC bit
  if ( (status1 & 0x01) == 0) // POC == 0
  {
    // (3) 看BLD bit
    if ( (status1 & 0x01) == 0) // BLD = 0
    {
      // (4) Read Real-Time Data
      return;
    }
  }
  else
  {
    // (5) 等0.5秒以上
  }
  // (6)
  */
//-----------------------------------------------------------------------------

//Private variables -----------------------------------------------------------
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
//-----------------------------------------------------------------------------


/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_STM32Config(void)
{

    //Enable the PWR APB1 clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    //Allow access to RTC
    PWR_BackupAccessCmd(ENABLE);

    //Reset backup domain
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

#if USE_RTC_LSI  
    //LSI used as RTC source clock
    RCC_LSICmd(ENABLE);
    //Wait till LSI is ready
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    //Select the RTC clock source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    SynchPrediv = 0x18F;
    AsynchPrediv = 0x63;

    //Enable the RTC Clock
    RCC_RTCCLKCmd(ENABLE);

    //Wait for RTC APB registers synchronisation
    RTC_WaitForSynchro();
#endif
    
#if USE_RTC_HSE
    
    // 使用HSE為Clock Source
    RCC_HSEConfig(RCC_HSE_ON);
    // 等待HSE Ready
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    // Refer:
    // http://wenku.baidu.com/view/419e5a2c0066f5335a81214c.html
    // --------------------------------------------------
    // (HSE/32) --+
    // (LSE) ------+---> RTC Clock Source
    // (LSI RC) ---+
    // 目前Select HSE/32，原生自External Oscillator 8MHz
    // --------------------------------------------------
    // LSE為32.768KHz
    // LSI RC為內部RC震盪，大約40KHz
    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div32);
    // RTC的Clock開啟
    RCC_RTCCLKCmd(ENABLE);
    // RTC等待同步
    RTC_WaitForSynchro();
    
    // 8MHz / 32 = 1/4 MHz = 250KHz
    // PREDIV_A = 124 (div 125) 
    // PREDIV_S = 1999 (div 2000)
    AsynchPrediv    = 124;
    SynchPrediv     = 1999;

#endif    
}
//-----------------------------------------------------------------------------


void RTC_STM32TimeRegulate(void)
{
	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;

	//If not config before, set to 2000-01-01 00:00:00 Saturday
	RTC_DateStructure.RTC_Year = 0;
	RTC_DateStructure.RTC_Month = 1;
	RTC_DateStructure.RTC_Date = 1;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = 0;
	RTC_TimeStructure.RTC_Minutes = 0;
	RTC_TimeStructure.RTC_Seconds = 0;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

	//Indicator for the RTC configuration
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x53CA);

	DEBUG_PRINT("..: RTC Time Regulate Completed.\n\r");
}
//-----------------------------------------------------------------------------


void RTC_INITIALIZATION()
{

	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_InitTypeDef RTC_InitStructure;

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x53CA)
	{
        //RTC configuration
        RTC_STM32Config();

        //Configure the RTC data register and RTC prescaler
        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

        //Check on RTC init
        if (RTC_Init(&RTC_InitStructure) == ERROR)
        {
            DEBUG_PRINT("..: STM32 RTC Prescaler Config failed\n\r");
        }

        //Configure the time register
        RTC_STM32TimeRegulate();
	}
	else
	{

        DEBUG_PRINT("..: No need to configure RTC.\n\r");

        //RTC configuration
        RTC_STM32Config();
	}

    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    
    DEBUG_PRINT("..: The current internal time is :  %02d-%02d-%02d %0.2d:%0.2d:%0.2d \n\r",
                RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date,
                RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}
//-----------------------------------------------------------------------------


void RTC_S35390A_INITIALIZATION()
{
  unsigned char status1;
  unsigned char status2;
  //unsigned char herz[3] = {0x40,0xff,0xff};

  // 先採用Rachel的Flow
  unsigned char count = 5;
  while (count > 0)
  {
    I2C_RTC_READ_NBYTE(0x60,1,&status1);            // Read Status 1
    I2C_RTC_READ_NBYTE(0x62,1,&status2);            // Read Status 2

    if ((status1 & (0x03)) || (status2 & 0x01))
    {
      // status1 bit 1, 0
      // status2 bit 0
      status1 = 0x80;
      I2C_RTC_WRITE_NBYTE(0x60,1,&status1);         // Reset
      for (int i=0; i<200000; i++) {}               // Delay for a while
    }
    else
      break;
    count--;
  }
  status1 = 0x40; // bit 6 = 1 for 24H
  I2C_RTC_WRITE_NBYTE(0x60,1,&status1);
  for (int i=0; i<200000; i++) {}       // Delay for a while
  //status2 = 0x08; // bit 1 = 0 for INT2AE
//  //I2C_RTC_WRITE_NBYTE(0x62,1,&status2);
//  //for (int i=0; i<200000; i++) {}       // Delay for a while
//  //I2C_RTC_WRITE_NBYTE(0x6A,1,herz);
//  for (int i=0; i<200000; i++) {}       // Delay for a while
//  status2 = 0x02; // bit 1 = 1 for INT2AE
//  I2C_RTC_WRITE_NBYTE(0x62,1,&status2);
}
//-----------------------------------------------------------------------------


// 將RTC的資料格式，轉換成Counter的值
int TRANSLATE_RTC_2_INT (unsigned char rtc_value_format)
{
  int return_value = 0;
  if (rtc_value_format & 0x80) return_value += 1;
  if (rtc_value_format & 0x40) return_value += 2;
  if (rtc_value_format & 0x20) return_value += 4;
  if (rtc_value_format & 0x10) return_value += 8;
  if (rtc_value_format & 0x08) return_value += 10;
  if (rtc_value_format & 0x04) return_value += 20;
  if (rtc_value_format & 0x02) return_value += 40;
  if (rtc_value_format & 0x01) return_value += 80;
  return (return_value);
}
//-----------------------------------------------------------------------------


// 將Counter的值，轉換成RTC的資料格式
unsigned char TRANSLATE_2_RTC_FORMAT(int counter)
{
  unsigned char return_value = 0x00;
  int current_value;
  current_value = counter % 100; // 先取小於100
  if (current_value >= 80) { current_value -= 80; return_value |= 0x01; }
  if (current_value >= 40) { current_value -= 40; return_value |= 0x02; }
  if (current_value >= 20) { current_value -= 20; return_value |= 0x04; }
  if (current_value >= 10) { current_value -= 10; return_value |= 0x08; }
  if (current_value >=  8) { current_value -=  8; return_value |= 0x10; }
  if (current_value >=  4) { current_value -=  4; return_value |= 0x20; }
  if (current_value >=  2) { current_value -=  2; return_value |= 0x40; }
  if (current_value >=  1) {                      return_value |= 0x80; }
                             // 最後一個只測試，不需減
  return (return_value);
}
//-----------------------------------------------------------------------------


// Update STM RTC from S-35390A
int UPDATE_RTC_FROM_S35390A()
{

  RTC_DateTypeDef RTC_DateStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  uint8_t DayOfWeek;

  // Read RTC Information from S-35390A
  if(I2C_RTC_READ_NBYTE(0x64, 7, VAR_RTC_TIME_S35390A.rtc_data) == __RETURN_FAIL)
    return (__RETURN_FAIL);

  DEBUG_PRINT("**: Update external RTC to internal RTC ...\n\r");
//  /*** for debug ***/
//  DEBUG_PRINT("**: External RTC time :             0x%02x-0x%02x-0x%02x-0x%02x 0x%02x:0x%02x:0x%02x\n\r",
//				VAR_RTC_TIME_S35390A.Year,
//				VAR_RTC_TIME_S35390A.Month,
//				VAR_RTC_TIME_S35390A.DayOfMonth,
//				VAR_RTC_TIME_S35390A.DayOfWeek,
//				VAR_RTC_TIME_S35390A.Hour,
//				VAR_RTC_TIME_S35390A.Minute,
//				VAR_RTC_TIME_S35390A.Second);
//
//  DEBUG_PRINT("**: External RTC time :             %02d-%02d-%02d-%02d %0.2d:%0.2d:%0.2d\n\r",
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Year),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Month & 0xF8),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfMonth & 0xFC),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfWeek & 0xE0),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Hour & 0xFC),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Minute & 0xFE),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Second & 0xFE));
//  /*** for debug ***/

  DayOfWeek = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfWeek & 0xE0);                      //W1 W2 W4 x  - x   x   x   x   : W = W1*1+W2*2+W4*4
  if(DayOfWeek == 0) DayOfWeek = 7;

  RTC_DateStructure.RTC_Year    = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Year);              //Y1 Y2 Y4 Y8 - Y10 Y20 Y40 Y80 : Y = Y1*1+Y2*2+Y4*4+Y8*8+Y10*10+Y20*20+Y40*40+Y80*80
  RTC_DateStructure.RTC_Month   = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Month & 0xF8);      //M1 M2 M4 M8 - M10 x   x   x   : M = M1*1+M2*2+M4*4+M8*8+M10*10
  RTC_DateStructure.RTC_Date    = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfMonth & 0xFC); //D1 D2 D4 D8 - D10 D20 x   x   : D = D1*1+D2*2+D4*4+D10*10+D20*20
  RTC_DateStructure.RTC_WeekDay = DayOfWeek;
  while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR);
//  DEBUG_PRINT("**: Internal RTC Date update SUCCESS!!!\n\r");

  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Hour & 0xFC);       //H1 H2 H4 H8 - H10 H20 AM/PM x : H = H1*1+H2*2+H4*4+H10*10+H20*20
  RTC_TimeStructure.RTC_Minutes = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Minute & 0xFE);     //m1 m2 m4 m8 - m10 m20 m40 x   : m = m1*1+m2*2+m4*4+m10*10+m20*20+m40*40
  RTC_TimeStructure.RTC_Seconds = (uint8_t)TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Second & 0xFE);     //s1 s2 s4 s8 - s10 s20 s40 x   : s = s1*1+s2*2+s4*4+s10*10+s20*20+s40*40
  while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR);
//  DEBUG_PRINT("**: Internal RTC time update SUCCESS!!!\n\r");

//  /*** for debug ***/
//  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//
//  DEBUG_PRINT("**: Be updated internal RTC time :  %02d-%02d-%02d-%02d %0.2d:%0.2d:%0.2d\n\r",
//				RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_WeekDay,
//				RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
//  /*** for debug ***/
  DEBUG_PRINT("**: Internal RTC Time Set Completed.\n\r");

  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


// Update STM RTC to S-35390A
int UPDATE_RTC_TO_S35390A()
{
  RTC_DateTypeDef RTC_DateStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  uint8_t DayOfWeek;
  
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  
  DEBUG_PRINT("**: Update internal RTC to external RTC ...\n\r");
//  DEBUG_PRINT("**: The internal RTC time :         %02d-%02d-%02d-%02d %0.2d:%0.2d:%0.2d\n\r",
//				  RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_WeekDay,
//				  RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);

  // 換算成RTC的時間格式
  (RTC_DateStructure.RTC_WeekDay == 7) ? (DayOfWeek = 0) : (DayOfWeek = RTC_DateStructure.RTC_WeekDay);

  VAR_RTC_TIME_S35390A.Year         = TRANSLATE_2_RTC_FORMAT(RTC_DateStructure.RTC_Year);       //Y1 Y2 Y4 Y8 - Y10 Y20 Y40 Y80 : Y = Y1*1+Y2*2+Y4*4+Y8*8+Y10*10+Y20*20+Y40*40+Y80*80
  VAR_RTC_TIME_S35390A.Month        = TRANSLATE_2_RTC_FORMAT(RTC_DateStructure.RTC_Month);      //M1 M2 M4 M8 - M10 x   x   x   : M = M1*1+M2*2+M4*4+M8*8+M10*10
  VAR_RTC_TIME_S35390A.DayOfMonth   = TRANSLATE_2_RTC_FORMAT(RTC_DateStructure.RTC_Date);       //D1 D2 D4 D8 - D10 D20 x   x   : D = D1*1+D2*2+D4*4+D10*10+D20*20
  VAR_RTC_TIME_S35390A.DayOfWeek    = TRANSLATE_2_RTC_FORMAT(DayOfWeek);                        //W1 W2 W4 x  - x   x   x   x   : W = W1*1+W2*2+W4*4

  VAR_RTC_TIME_S35390A.Hour         = TRANSLATE_2_RTC_FORMAT(RTC_TimeStructure.RTC_Hours);      //H1 H2 H4 H8 - H10 H20 AM/PM x : H = H1*1+H2*2+H4*4+H10*10+H20*20
  VAR_RTC_TIME_S35390A.Minute       = TRANSLATE_2_RTC_FORMAT(RTC_TimeStructure.RTC_Minutes);    //m1 m2 m4 m8 - m10 m20 m40 x   : m = m1*1+m2*2+m4*4+m10*10+m20*20+m40*40
  VAR_RTC_TIME_S35390A.Second       = TRANSLATE_2_RTC_FORMAT(RTC_TimeStructure.RTC_Seconds);    //s1 s2 s4 s8 - s10 s20 s40 x   : s = s1*1+s2*2+s4*4+s10*10+s20*20+s40*40

  if (I2C_RTC_WRITE_NBYTE(0x64, 7, VAR_RTC_TIME_S35390A.rtc_data) == __RETURN_FAIL)
      return (__RETURN_FAIL);

  DEBUG_PRINT("**: Update external RTC completed : %02d-%02d-%02d-%02d %0.2d:%0.2d:%0.2d\n\r",
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Year),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Month & 0xF8),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfMonth & 0xFC),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfWeek & 0xE0),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Hour & 0xFC),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Minute & 0xFE),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Second & 0xFE));

///*** for debug ***/
//  // Read RTC Information from S-35390A
//  if(I2C_RTC_READ_NBYTE(0x64, 7, VAR_RTC_TIME_S35390A.rtc_data) == __RETURN_FAIL)
//    return (__RETURN_FAIL);
//
//  DEBUG_PRINT("**: DBG External RTC time :  0x%02x-0x%02x-0x%02x-0x%02x 0x%02x:0x%02x:0x%02x\n\r",
//				VAR_RTC_TIME_S35390A.Year,
//				VAR_RTC_TIME_S35390A.Month,
//				VAR_RTC_TIME_S35390A.DayOfMonth,
//				VAR_RTC_TIME_S35390A.DayOfWeek,
//				VAR_RTC_TIME_S35390A.Hour,
//				VAR_RTC_TIME_S35390A.Minute,
//				VAR_RTC_TIME_S35390A.Second);
//
//  DEBUG_PRINT("**: DBG External RTC time :  %02d-%02d-%02d-%02d %0.2d:%0.2d:%0.2d\n\r",
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Year),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Month & 0xF8),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfMonth & 0xFC),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfWeek & 0xE0),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Hour & 0xFC),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Minute & 0xFE),
//				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Second & 0xFE));
///*** for debug ***/

  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


// Debug用命令
void RTC_S35390A_DUMP_REGISTERS()
{
  unsigned char data[7];
  I2C_RTC_READ_NBYTE(0x60,1,data);
  printf ("STATUS 1: %02X\n\r", data[0]);
  I2C_RTC_READ_NBYTE(0x62,1,data);
  printf ("STATUS 2: %02X\n\r", data[0]);
  I2C_RTC_READ_NBYTE(0x64,7,data);
  printf ("RTC 1: %02X %02X %02X %02X %02X %02X %02X\n\r", data[0], data[1],
          data[2], data[3], data[4], data[5], data[6]);
  I2C_RTC_READ_NBYTE(0x66,3,data);
  printf ("RTC 2: %02X %02X %02X\n\r", data[0], data[1], data[2]);
  I2C_RTC_READ_NBYTE(0x68,3,data);
  printf ("INT 1: %02X %02X %02X\n\r", data[0], data[1], data[2]);
  I2C_RTC_READ_NBYTE(0x6A,3,data);
  printf ("INT 2: %02X %02X %02X\n\r", data[0], data[1], data[2]);
  I2C_RTC_READ_NBYTE(0x6C,1,data);
  printf ("Correction: %02X\n\r", data[0]);
  I2C_RTC_READ_NBYTE(0x6E,1,data);
  printf ("Register: %02X\n\r", data[0]);
}
//-----------------------------------------------------------------------------


void SHOW_CURRENT_TIME()
{
  //uint32_t CounterValue;
  // Sync一次RTC的時間
  UPDATE_RTC_FROM_S35390A();
  /// 計算Counter值
  // Hour 要Mask 1111 1100 (0xFC)
  //                    |
  //                    +---> 這是 L(AM)H(PM) Bit
  VAR_RTC_TIME_S35390A.Hour = VAR_RTC_TIME_S35390A.Hour & 0xFC;
  // 來源是S35390A，必須換算RTC的時間格式成為Integer Value
  for (int iCount =0; iCount < 7; iCount++)
  {
    DEBUG_PRINT ("%02X", TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.rtc_data[iCount]));
  }
  DEBUG_PRINT ("\n\r");
}
//-----------------------------------------------------------------------------


void RTC_GetDateTime()
{
  if(I2C_RTC_READ_NBYTE(0x64, 7, VAR_RTC_TIME_S35390A.rtc_data) == __RETURN_SUCCESS)
    DEBUG_PRINT("**: Get External RTC time :  %02d-%02d-%02d-%02d %0.2d:%0.2d:%0.2d\n\r",
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Year),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Month & 0xF8),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfMonth & 0xFC),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.DayOfWeek & 0xE0),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Hour & 0xFC),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Minute & 0xFE),
				TRANSLATE_RTC_2_INT(VAR_RTC_TIME_S35390A.Second & 0xFE));
}
//-----------------------------------------------------------------------------


/* External RTC Set date & time */
int UPDATE_USER_DEFINED_RTC_VALUE()
{
  // 更新來自於User定義之RTC值
  if (I2C_RTC_WRITE_NBYTE(0x64, 7, VAR_RTC_TIME_S35390A.rtc_data) == __RETURN_FAIL)
      return (__RETURN_FAIL);

  DEBUG_PRINT ("**: Set External RTC time :  %02X-%02X-%02X %02X %02X:%02X:%02X OK!\n\r",
                VAR_RTC_TIME_S35390A.rtc_data[0],
                VAR_RTC_TIME_S35390A.rtc_data[1],
                VAR_RTC_TIME_S35390A.rtc_data[2],
                VAR_RTC_TIME_S35390A.rtc_data[3],
                VAR_RTC_TIME_S35390A.rtc_data[4],
                VAR_RTC_TIME_S35390A.rtc_data[5],
                VAR_RTC_TIME_S35390A.rtc_data[6]);

//  printf ("%02X-%02X-%02X %02X %02X:%02X:%02X"
//                         , VAR_RTC_TIME_S35390A.rtc_data[0]
//                         , VAR_RTC_TIME_S35390A.rtc_data[1]
//                         , VAR_RTC_TIME_S35390A.rtc_data[2]
//                         , VAR_RTC_TIME_S35390A.rtc_data[3]
//                         , VAR_RTC_TIME_S35390A.rtc_data[4]
//                         , VAR_RTC_TIME_S35390A.rtc_data[5]
//                         , VAR_RTC_TIME_S35390A.rtc_data[6]);
//  printf ("OK\n\r");
  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


void UPDATE_USER_DEFINED_ALARM_VALUE()
{
  unsigned char old_status2, status2;

  // 讀取舊的Status 2 Register
  I2C_RTC_READ_NBYTE(0x62,1,&old_status2);
  // 設定INT2AE = 1，才能選到ALARM TIME
  status2 = 0x02; // bit 1 = 1 for INT2AE
  I2C_RTC_WRITE_NBYTE(0x62,1,&status2);
  //
  I2C_RTC_WRITE_NBYTE(0x6A,3,VAR_ALARM_TIME.alarm_data);
  // 回復原本Status 2 Register
  I2C_RTC_WRITE_NBYTE(0x62,1,&old_status2);
}
//-----------------------------------------------------------------------------


void UPDATE_ALARM()
{
  unsigned char old_status2, status2;

  // 讀取舊的Status 2 Register
  I2C_RTC_READ_NBYTE(0x62,1,&old_status2);
  // 設定INT2AE = 1，才能選到ALARM TIME
  status2 = 0x02; // bit 1 = 1 for INT2AE
  I2C_RTC_WRITE_NBYTE(0x62,1,&status2);
  // 讀取INT2 Register Value
  I2C_RTC_READ_NBYTE(0x6A,3,VAR_ALARM_TIME.alarm_data);
  // 回復原本Status 2 Register
  I2C_RTC_WRITE_NBYTE(0x62,1,&old_status2);
}
//-----------------------------------------------------------------------------


// Debug用，切割Task
void TASK_DBG_DUMP_RTC_REGISTERS()
{
  static unsigned char data[7];
//  if (VAR_START_DUMP_RTC_S35390A_REGISTERS == 0) return;
  VAR_START_DUMP_RTC_S35390A_REGISTERS = 5;
  switch (VAR_START_DUMP_RTC_S35390A_REGISTERS)
  {
    case 1: I2C_RTC_READ_NBYTE(0x60,1,data);
            break;
    case 2: DEBUG_PRINT ("STATUS 1: %02X\n\r", data[0]);
            break;
    case 3: I2C_RTC_READ_NBYTE(0x62,1,data);
            break;
    case 4: DEBUG_PRINT ("STATUS 2: %02X\n\r", data[0]);
            break;
    case 5:
            I2C_RTC_READ_NBYTE(0x64,7,data);

            VAR_RTC_TIME_S35390A.Year = TRANSLATE_2_RTC_FORMAT(16);
            VAR_RTC_TIME_S35390A.Month = TRANSLATE_2_RTC_FORMAT(9);
            VAR_RTC_TIME_S35390A.DayOfMonth = TRANSLATE_2_RTC_FORMAT(29);
            VAR_RTC_TIME_S35390A.DayOfWeek = TRANSLATE_2_RTC_FORMAT(4);
            VAR_RTC_TIME_S35390A.Hour = TRANSLATE_2_RTC_FORMAT(14);
            VAR_RTC_TIME_S35390A.Minute = TRANSLATE_2_RTC_FORMAT(5);
            VAR_RTC_TIME_S35390A.Second = TRANSLATE_2_RTC_FORMAT(45);
            I2C_RTC_WRITE_NBYTE(0x64, 7, VAR_RTC_TIME_S35390A.rtc_data);
            UPDATE_RTC_FROM_S35390A();
            break;
    case 6: DEBUG_PRINT ("RTC 1: %02X %02X %02X %02X %02X %02X %02X\n\r", data[0], data[1],
          data[2], data[3], data[4], data[5], data[6]);
            break;
    case 7: I2C_RTC_READ_NBYTE(0x66,3,data);
            break;
    case 8: DEBUG_PRINT ("RTC 2: %02X %02X %02X\n\r", data[0], data[1], data[2]);
            break;
    case 9: I2C_RTC_READ_NBYTE(0x68,3,data);
            break;
    case 10: DEBUG_PRINT ("INT 1: %02X %02X %02X\n\r", data[0], data[1], data[2]);
            break;
    case 11: I2C_RTC_READ_NBYTE(0x6A,3,data);
            break;
    case 12: DEBUG_PRINT ("INT 2: %02X %02X %02X\n\r", data[0], data[1], data[2]);
            break;
    case 13: I2C_RTC_READ_NBYTE(0x6C,1,data);
            break;
    case 14: DEBUG_PRINT ("Correction: %02X\n\r", data[0]);
            break;
    case 15: I2C_RTC_READ_NBYTE(0x6E,1,data);
            break;
    case 16: DEBUG_PRINT ("Register: %02X\n\r", data[0]);
            break;
    case 17: VAR_START_DUMP_RTC_S35390A_REGISTERS = 0;
             return;
             break;
  }
//  VAR_START_DUMP_RTC_S35390A_REGISTERS++;
}
//-----------------------------------------------------------------------------


void TASK_SYNC_RTC_TIME()
{
  static uint8_t LastHour;
  RTC_TimeTypeDef RTC_SyncTimeStructure;

//  if ((STM32RTC_CounterValue / 3600) == LastHour) return;
  // 與前一小時不同
//  LastHour = STM32RTC_CounterValue / 3600;

  RTC_GetTime(RTC_Format_BIN, &RTC_SyncTimeStructure);
  if(RTC_SyncTimeStructure.RTC_Hours == LastHour) return;
  LastHour = RTC_SyncTimeStructure.RTC_Hours;

  DEBUG_PRINT ("**: Sync RTC Time from S35390, H:%d\n\r", RTC_SyncTimeStructure.RTC_Hours);
  // 每一小時同步一次
  UPDATE_RTC_FROM_S35390A();
}
//-----------------------------------------------------------------------------


// 清除Alarm Interrupt
void Clear_Alarm_Interrupt(void)
{
  unsigned char status;
  // 讀走Regitster Status 1
  I2C_RTC_READ_NBYTE(0x60,1,&status);
  // 取消ALARM Interrupt
  status = 0x00; // INT2AE = 0
  I2C_RTC_WRITE_NBYTE(0x62,1,&status);
}

// 啟用ALARM Interrupt
void Enable_Alarm_Interrupt(void)
{
  unsigned char status;
  status = 0x02; // INT2AE = 0
  I2C_RTC_WRITE_NBYTE(0x62,1,&status);
}
//=============================================================================