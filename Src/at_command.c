//#include "stm32f10x_conf.h"
//#include "stm32f10x_it.h"
//#include <system_stm32f10x.h>
//#include <core_cm3.h>
//#include <stm32f10x.h>
//#include "gpio_control.h"
//#include <stdio.h>
//
//
//#include "project.h"
//#include "main.h"
//#include "protocol.h"
//#include "battery_info.h"
//#include "i2c_master.h"
//#include "rtc_control.h"
//#include "watchdog.h"
//#include "eeprom_control.h"
//#include "i2c_slave.h"
//
//extern RTC_TIME VAR_RTC_TIME_S35390A; // 這定義在main.c中       
//                                      // 需要include "project.h" 的結構宣告
//
//typedef  void (*pFunction)(void);
//
//void peripheral_irq_disable()
//{
//	//Disable UART1 interrupt
//	NVIC_DisableIRQ(USART1_IRQn);
//	//Disable I2C2 interrupt
//	NVIC_DisableIRQ(I2C2_EV_IRQn);
//	NVIC_DisableIRQ(I2C2_ER_IRQn);
//	//Disable Ignition interrupt
//	NVIC_DisableIRQ(EXTI1_IRQn);
//        //Disable Power Button interrupt
//	NVIC_DisableIRQ(EXTI2_IRQn);
//        //Disable DI1/DI2 interrupt
//	NVIC_DisableIRQ(EXTI3_IRQn);
//        NVIC_DisableIRQ(EXTI4_IRQn);
//        //Disable G-sensor/WWAN/ALARM interrupt
//        NVIC_DisableIRQ(EXTI9_5_IRQn);
//}
//
//int HEX_2DIGIT_TO_INT (unsigned char hi, unsigned char lo)
//{
//  int return_value = 0;
//  // High byte
//  if ((hi >= '0') && (hi <= '9'))
//  {
//    return_value = (int)(hi - '0') * 16;
//  }
//  else if ((hi >= 'A') && (hi <= 'F'))
//  {
//    return_value = ((int)(hi - 'A') + 10) * 16;
//  }
//  else
//  {
//    return (256); // 合理Value 0~255，若是大於255就是有Error
//  }
//  // Low Byte
//  if ((lo >= '0') && (lo <= '9'))
//  {
//    return_value += (int)(lo - '0');
//  }
//  else if ((lo >= 'A') && (lo <= 'F'))
//  {
//    return_value += (int)(lo - 'A') + 10;
//  }
//  else
//  {
//    return (256); // 合理Value 0~255，若是大於255就是有Error
//  }
//  return (return_value);
//}
//
//void INT_TO_HEX_4DIGIT (int number)
//{  
//  unsigned char return_value[4];
//
//  return_value[0] = number/4096;
//  number = number%4096;
//  return_value[1] = number/256;
//  number = number%256;
//  return_value[2] = number/16;
//  return_value[3] = number%16;
//  
//  printf ("%X%X%X%X\n\r", return_value[0], return_value[1], return_value[2], return_value[3]);
//}
//void ATCMD_ATF2()
//{
//  uint32_t read_address;
//  uint32_t checksum_tag;
//  
//  //Check Checksum Tag
//  read_address = BootloaderAddress + FLASH_SIZE - (CHKSUM_TAG_OFFSET + CHKSUM_TAG_SIZE);
//  checksum_tag= *((uint32_t*)read_address);
//  if (checksum_tag == CHK_ALL_TIME)
//  {
//  	printf("0");
//  }
//  else if (checksum_tag == CHK_ONCE)
//  {
//  	printf("1");
//  }
//  else if (checksum_tag == DO_NOT_CHK)
//  {
//  	printf("2");
//  }
//  
//  printf ("\r\n");
//  return;
//}
//
//void ATCMD_ATFD()
//{
//    printf ("Enter Bootloader Mode\r\n");
//#ifdef NDEBUG // Release Mode才存在支援
//    BOOT_SIG = 0xbabecafe;
//	//USART_DeInit(USART1);
//	//NVIC_DisableIRQ(USART1_IRQn);
////        __disable_irq();
//	peripheral_irq_disable();
//	u32 JumpAddress = *(__IO u32*) (BootloaderAddress + 4);
//	pFunction Jump_To_Bootloader = (pFunction) JumpAddress;
//	__set_MSP(*(__IO u32*) BootloaderAddress);
//	Jump_To_Bootloader();
//#endif // of NDEBUG
//    return;
//
//}
//
//void ATCMD_ATSN()
//{
//  unsigned int uiSN_Temp1;
//  unsigned long ulSN_Temp2;
//  uiSN_Temp1 = DevID_SNo0;
//  printf ("%04X", uiSN_Temp1);
//  ulSN_Temp2 = DevID_SNo1;
//  printf ("%08X", ulSN_Temp2);
//  ulSN_Temp2 = DevID_SNo2;
//  printf ("%08X\r\n", ulSN_Temp2);
//  return;
//}

//void ATCMD_EET()
//{
//  // Get Type & Size of EEPROM
//  printf ("24LC512,10000\n\r");
//}
//
//void ATCMD_EEG(unsigned char add1,
//               unsigned char add2,
//               unsigned char add3,
//               unsigned char add4)
//{
//  int hi_addr, lo_addr;
//  unsigned char ReturnValue;
//  hi_addr = HEX_2DIGIT_TO_INT(add1,add2);
//  lo_addr = HEX_2DIGIT_TO_INT(add3,add4);
//  // AT24LC02最多256 Bytes (0x0000 ~ 0x00FF)
//  if (hi_addr > 0) { printf("ERR-E03\n\r"); return; }
//  if (lo_addr > 255) { printf("ERR-E03\n\r"); return; }
//  if (I2C_EEPROM_24LC512_READ_BYTE(0xA0,hi_addr&0xFF,lo_addr&0xFF,&ReturnValue) == 0)
//  {
//    printf ("%02X\n\r",ReturnValue);
//    return;
//  }
//  printf ("ERR-G02\n\r");
//}
//
//void ATCMD_EES(unsigned char add1,
//               unsigned char add2,
//               unsigned char add3,
//               unsigned char add4,
//               unsigned char dat1,
//               unsigned char dat2)
//{
//  int hi_addr, lo_addr;
//  int write_value;
//  hi_addr = HEX_2DIGIT_TO_INT(add1,add2);
//  lo_addr = HEX_2DIGIT_TO_INT(add3,add4);
//  // AT24LC02最多256 Bytes (0x0000 ~ 0x00FF)
//  if (hi_addr > 0) { printf("ERR-E03\n\r"); return; }
//  if (lo_addr > 255) { printf("ERR-E03\n\r"); return; }
//  //
//  write_value = HEX_2DIGIT_TO_INT(dat1,dat2);
//  if (write_value > 255) { printf("ERR-E03\n\r"); return; }
//  //
//  if (I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,hi_addr&0xFF,lo_addr&0xFF,write_value&0xFF) == 0)
//  {
//    printf ("OK\n\r");
//    return;
//  }
//  printf ("ERR-G02\n\r");
//}
//
//void ATCMD_DBG01()
//{
//  VAR_START_DUMP_RTC_S35390A_REGISTERS = 1;
//  // RTC_S35390A_DUMP_REGISTERS();
//}
//
//void ATCMD_MCPR(unsigned char power_system)
//{
//  if (power_system == '2') // 24V
//  {
//    printf ("%02X%02X%02X%02X%02X%02X\n\r",
//            __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_H,
//            __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_L,
//            __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H,
//            __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L,
//            __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_H,
//            __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_L);
//    return;
//  }
//  else
//  if (power_system == '3') // 12V
//  {
//    printf ("%02X%02X%02X%02X%02X%02X\n\r",
//            __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_H,
//            __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_L,
//            __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H,
//            __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L,
//            __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_H,
//            __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_L);
//    return;
//  }
//  printf ("ERR-M02\n\r");
//}
//
//void ATCMD_MCPG(unsigned char voltage_system, unsigned char voltage_check_time)
//{
//  unsigned char ReturnValueH, ReturnValueL;
//  if (voltage_system == '2') // 24V System
//  {
//    if (voltage_check_time == '0')
//    { // PRE_BOOT @ 24V
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x59,&ReturnValueH);
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5A,&ReturnValueL);
//      printf("%02X%02X\n\r",ReturnValueH,ReturnValueL);
//      return;
//    }
//    if (voltage_check_time == '1')
//    { // POST_BOOT @ 24V
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5B,&ReturnValueH);
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5C,&ReturnValueL);
//      printf("%02X%02X\n\r",ReturnValueH,ReturnValueL);
//      return;
//    }
//  }
//  if (voltage_system == '3') // 12V system
//  {
//    if (voltage_check_time == '0')
//    { // PRE_BOOT @ 12V
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x55,&ReturnValueH);
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x56,&ReturnValueL);
//      printf("%02X%02X\n\r",ReturnValueH,ReturnValueL);
//      return;
//    }
//    if (voltage_check_time == '1')
//    { // POST_BOOT @ 12V
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x57,&ReturnValueH);
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x58,&ReturnValueL);
//      printf("%02X%02X\n\r",ReturnValueH,ReturnValueL);
//      return;
//    }
//  }
//  printf ("ERR-M02\n\r");
//}
//
//// 設定Voltage Check為Default Value
//void ATCMD_MCPD(unsigned char voltage_system)
//{
//  if (VAR_EEPROM_WRITE_EVENT != 0)
//  {
//    printf ("ERR-E04\n\r");
//    return;
//  }
//  if (voltage_system == '2') // 24V system
//  {
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x59;
//    VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x05;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x5A;
//    VAR_EEPROM_WRITE_QUEUE_DATA[1] = 0x89;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x5B;
//    VAR_EEPROM_WRITE_QUEUE_DATA[2] = 0x05;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x5C;
//    VAR_EEPROM_WRITE_QUEUE_DATA[3] = 0x89;
//    VAR_EEPROM_WRITE_EVENT = 4;
//    printf ("OK\n\r");
//    return;
//  }
//  if (voltage_system == '3') // 12V system
//  {
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x55;
//    VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x02;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x56;
//    VAR_EEPROM_WRITE_QUEUE_DATA[1] = 0xE6;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x57;
//    VAR_EEPROM_WRITE_QUEUE_DATA[2] = 0x02;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
//    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x58;
//    VAR_EEPROM_WRITE_QUEUE_DATA[3] = 0xE6;
//    VAR_EEPROM_WRITE_EVENT = 4;
//    printf ("OK\n\r");
//    return;
//  }
//  printf ("ERR-M02\n\r"); 
//}
//                 
//void ATCMD_MCPS(unsigned char voltage_system, 
//                unsigned char voltage_check_time,
//                unsigned char value_h_h,
//                unsigned char value_h_l,
//                unsigned char value_l_h,
//                unsigned char value_l_l)
//{
//  int high_value, low_value, complete_value, ReturnValueAll;
//  unsigned char ReturnValueH, ReturnValueL;
//  unsigned int car_power_system;
//  if (VAR_EEPROM_WRITE_EVENT != 0)
//  {
//    printf ("ERR-E04\n\r");
//    return;
//  }
//  car_power_system = (VAR_SYSI2C_SYS_INFO>>6) & 0x0003;
//  high_value = HEX_2DIGIT_TO_INT(value_h_h, value_h_l);
//  low_value = HEX_2DIGIT_TO_INT(value_l_h, value_l_l);
//  if (high_value > 255) { // 參數錯誤
//                          printf ("ERR-M02\n\r");
//                          return;
//                        }
//  if (low_value > 255) { // 參數錯誤
//                          printf ("ERR-M02\n\r");
//                          return;
//                        }
//  complete_value = high_value * 256 + low_value; // 要設定之值
//  // 判斷邊界值
//  if (voltage_system == '2') // 24V System
//  {
//    if (voltage_check_time == '0')
//    { // PRE_BOOT @ 24V
//      // Condition is PRE_BOOT >= POST_BOOT >= MIN 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5B,&ReturnValueH); // Read Post-Boot Value 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5C,&ReturnValueL); // Read Post-Boot Value
//      ReturnValueAll = ReturnValueH * 256 + ReturnValueL;
//      if (complete_value < ReturnValueAll)
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      if (complete_value > (__DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_H*256 + 
//                            __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_L))
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      // Range Valid
//      if (car_power_system == 2)
//      {
//        VAR_VPM_PREBOOT_VOLTAGE_CHK = complete_value;
//      }
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x59;
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = high_value;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x5A;
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = low_value;
//      VAR_EEPROM_WRITE_EVENT = 2;
//      printf ("OK\n\r");
//      return;
//    }
//    if (voltage_check_time == '1')
//    { // POST_BOOT @ 24V
//      // Condition is PRE_BOOT >= POST_BOOT >= MIN 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x59,&ReturnValueH); // Read Pre-Boot Value 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5A,&ReturnValueL); // Read Pre-Boot Value
//      ReturnValueAll = ReturnValueH * 256 + ReturnValueL;
//      if (complete_value > ReturnValueAll)
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      if (complete_value < (__DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_H*256 + 
//                            __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_L))
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      // Range Valid
//      if (car_power_system == 2)
//      {
//        VAR_VPM_POSTBOOT_VOLTAGE_CHK = complete_value;
//      }
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x5B;
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = high_value;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x5C;
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = low_value;
//      VAR_EEPROM_WRITE_EVENT = 2;
//      printf ("OK\n\r");
//      return;
//    }
//  }
//  if (voltage_system == '3') // 12V system
//  {
//    if (voltage_check_time == '0')
//    { // PRE_BOOT @ 12V
//      // Condition is PRE_BOOT >= POST_BOOT >= MIN 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x57,&ReturnValueH); // Read Post-Boot Value 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x58,&ReturnValueL); // Read Post-Boot Value
//      ReturnValueAll = ReturnValueH * 256 + ReturnValueL;
//      if (complete_value < ReturnValueAll)
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      if (complete_value > (__DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_H*256 + 
//                            __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_L))
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      // Range Valid
//      if (car_power_system == 3)
//      {
//        VAR_VPM_PREBOOT_VOLTAGE_CHK = complete_value;
//      }
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x55;
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = high_value;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x56;
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = low_value;
//      VAR_EEPROM_WRITE_EVENT = 2;
//      printf ("OK\n\r");
//      return;
//    }
//    if (voltage_check_time == '1')
//    { // POST_BOOT @ 12V
//      // Condition is PRE_BOOT >= POST_BOOT >= MIN 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x55,&ReturnValueH); // Read Pre-Boot Value 
//      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x56,&ReturnValueL); // Read Pre-Boot Value
//      ReturnValueAll = ReturnValueH * 256 + ReturnValueL;
//      if (complete_value > ReturnValueAll)
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      if (complete_value < (__DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_H*256 + 
//                            __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_L))
//      {
//        printf("ERR-M03\n\r"); // Range Error
//        return;
//      }
//      // Range Valid
//      if (car_power_system == 3)
//      {
//        VAR_VPM_POSTBOOT_VOLTAGE_CHK = complete_value;
//      }
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x57;
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = high_value;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x58;
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = low_value;
//      VAR_EEPROM_WRITE_EVENT = 2;
//      printf ("OK\n\r");
//      return;
//    }
//  }
//  printf ("ERR-M02\n\r");
//}
//
//// 設定VPM Event Timeout Delay
//void ATCMD_MCUS(unsigned char time_type,
//                unsigned char data_hh,
//                unsigned char data_hl,
//                unsigned char data_lh,
//                unsigned char data_ll)
//{
//  // 上次EEPROM設定未完成
//  if (VAR_EEPROM_WRITE_EVENT != 0) { printf ("ERR-E04\n\r"); return;}
//  
//  unsigned int HighValue, LowValue, ReturnValue;
//  
//  HighValue = HEX_2DIGIT_TO_INT(data_hh, data_hl);
//  LowValue = HEX_2DIGIT_TO_INT(data_lh, data_ll);
//  if (HighValue > 255) { printf ("ERR-M07\n\r"); return;}
//  if (LowValue > 255) { printf ("ERR-M07\n\r"); return;}
//  
//  ReturnValue = (HighValue*256)+LowValue;
//  
//  switch (time_type)
//  {
//    // Car Power Low Event Delay
//    case '0':
//      if (ReturnValue < __DEF_POWER_LOW_EVT_DLY_MIN) { printf ("ERR-M07\n\r"); return;}
//      if (ReturnValue > __DEF_POWER_LOW_EVT_DLY_MAX) { printf ("ERR-M07\n\r"); return;}
//      VAR_VPM_POWER_LOW_EVT_DLY = ReturnValue;
//      // 更新EEPROM的內容
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x21; // Car Power Low Event Delay High Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = HighValue&0xFF;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x22; // Car Power Low Event Delay Low Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = LowValue&0xFF;
//      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte 
//      printf ("OK\n\r");
//      return;
//    // Car Power Low Hard Delay
//    case '1':
//      if (ReturnValue < __DEF_POWER_LOW_HARD_DLY_MIN) { printf ("ERR-M07\n\r"); return;}
//      if (ReturnValue > __DEF_POWER_LOW_HARD_DLY_MAX) { printf ("ERR-M07\n\r"); return;}
//      VAR_VPM_POWER_LOW_HARD_DLY = ReturnValue;
//      // 更新EEPROM的內容
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x23; // Car Power Low Hard Delay High Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = HighValue&0xFF;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x24; // Car Power Low Hard Delay Low Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = LowValue&0xFF;
//      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte 
//      printf ("OK\n\r");
//      return;
//    // Ignition On Delay
//    case '2':
//      if (ReturnValue < __DEF_PWR_ON_DLY_MIN) { printf ("ERR-M07\n\r"); return;}
//      if (ReturnValue > __DEF_PWR_ON_DLY_MAX) { printf ("ERR-M07\n\r"); return;}
//      VAR_VPM_PWR_ON_DLY = ReturnValue;
//      // 更新EEPROM的內容
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x10; // Ignition On Delay High Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = HighValue&0xFF;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x11; // Ignition On Delay Low Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = LowValue&0xFF;
//      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte 
//      printf ("OK\n\r");
//      return;
//    // Ignition Off Event Delay
//    case '3':
//      if (ReturnValue < __DEF_PWR_OFF_EVT_DLY_MIN) { printf ("ERR-M07\n\r"); return;}
//      if (ReturnValue > __DEF_PWR_OFF_EVT_DLY_MAX) { printf ("ERR-M07\n\r"); return;}
//      VAR_VPM_PWR_OFF_EVT_DLY = ReturnValue;
//      // 更新EEPROM的內容
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x12; // Ignition Off Event Delay High Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = HighValue&0xFF;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x13; // Ignition Off Event Delay Low Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = LowValue&0xFF;
//      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte 
//      printf ("OK\n\r");
//      return;
//    // Ignition Off Hard Delay for Power Off
//    case '4':
//      if (ReturnValue < __DEF_IGN_OFF_PWR_OFF_HARD_DLY_MIN) { printf ("ERR-M07\n\r"); return;}
//      if (ReturnValue > __DEF_IGN_OFF_PWR_OFF_HARD_DLY_MAX) { printf ("ERR-M07\n\r"); return;}
//      VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY = ReturnValue;
//      // 更新EEPROM的內容
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x14; // Ignition Off Hard Delay High Byte for Power Off
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = HighValue&0xFF;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x15; // Ignition Off Hard Delay Low Byte for Power Off
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = LowValue&0xFF;
//      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte 
//      printf ("OK\n\r");
//      return;
//    // Post-boot Power Check Delay
//    case '5':
//      if (ReturnValue < __DEF_POST_PWR_CHK_DLY_MIN) { printf ("ERR-M07\n\r"); return;}
//      if (ReturnValue > __DEF_POST_PWR_CHK_DLY_MAX) { printf ("ERR-M07\n\r"); return;}
//      VAR_VPM_POST_PWR_CHK_DLY_TIME = ReturnValue;
//      // 更新EEPROM的內容
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x17; // Post-boot Power Check Delay High Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[0] = HighValue&0xFF;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
//      VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x18; // Post-boot Power Check Delay Low Byte
//      VAR_EEPROM_WRITE_QUEUE_DATA[1] = LowValue&0xFF;
//      VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte 
//      printf ("OK\n\r");
//      return;
//    default:
//      printf ("ERR-M08\n\r");
//      return;
//  }
//}
//// Get Alarm Time Setting
//void ATCMD_RTAG()
//{
//  unsigned char old_status2, status2;
//  unsigned char data[3];
//  // 讀取舊的Status 2 Register
//  I2C_RTC_READ_NBYTE(0x62,1,&old_status2);
//  // 設定INT2AE = 1，才能選到ALARM TIME
//  status2 = 0x02; // bit 1 = 1 for INT2AE
//  I2C_RTC_WRITE_NBYTE(0x62,1,&status2);
//  // 讀取INT2 Register Value
//  I2C_RTC_READ_NBYTE(0x6A,3,data);
//  printf ("%02X%02X%02X\n\r", data[0], data[1], data[2]);
//  // 回復原本Status 2 Register 
//  I2C_RTC_WRITE_NBYTE(0x62,1,&old_status2);
//}
//
//void ATCMD_RTAS(unsigned char date_h, unsigned char date_l,
//                unsigned char hour_h, unsigned char hour_l,
//                unsigned char min_h, unsigned char min_l)
//{
//  unsigned char data[3];
//  unsigned int ReturnValue;
//  unsigned char old_status2, status2;
//  // Check Date
//  ReturnValue = HEX_2DIGIT_TO_INT(date_h, date_l);
//  if (ReturnValue > 255)
//  {
//    printf ("ERR-R01\n\r"); return;
//  }
//  data[0] = ReturnValue & 0xFF;
//  // Check Hour
//  ReturnValue = HEX_2DIGIT_TO_INT(hour_h, hour_l);
//  if (ReturnValue > 255)
//  {
//    printf ("ERR-R01\n\r"); return;
//  }
//  data[1] = ReturnValue & 0xFF;
//  // Check Hour
//  ReturnValue = HEX_2DIGIT_TO_INT(min_h, min_l);
//  if (ReturnValue > 255)
//  {
//    printf ("ERR-R01\n\r"); return;
//  }
//  data[2] = ReturnValue & 0xFF;
//  
//  // 讀取舊的Status 2 Register
//  I2C_RTC_READ_NBYTE(0x62,1,&old_status2);
//  // 設定INT2AE = 1，才能選到ALARM TIME
//  status2 = 0x02; // bit 1 = 1 for INT2AE
//  I2C_RTC_WRITE_NBYTE(0x62,1,&status2);
//  //
//  I2C_RTC_WRITE_NBYTE(0x6A,3,data);
//  // 回復原本Status 2 Register 
//  I2C_RTC_WRITE_NBYTE(0x62,1,&old_status2);
//  // Done
//  printf ("OK\n\r");
//}
//     
//// Clear Alarm INT
//void ATCMD_RTAC(void)
//{
//  unsigned char status;
//  // 讀走Regitster Status 1
//  I2C_RTC_READ_NBYTE(0x60,1,&status);
//  // 取消ALARM Interrupt 
//  status = 0x00; // INT2AE = 0
//  I2C_RTC_WRITE_NBYTE(0x62,1,&status);
//  printf ("OK\n\r");
//}
//
//void ATCMD_RTAE(void)
//{
//  unsigned char status;
//  status = 0x02; // INT2AE = 1
//  I2C_RTC_WRITE_NBYTE(0x62,1,&status);
//  printf ("OK\n\r");
//}
//
//// Reset RTC
//void ATCMD_RTRST()
//{
//  unsigned char status;
//  status = 0x80; // RESET = 1
//  // 對RTC進行重置
//  I2C_RTC_WRITE_NBYTE(0x60,1,&status);
//  status = 0x40; // bit 6 = 1 for 24H
//  // 設為24Hhor制
//  I2C_RTC_WRITE_NBYTE(0x60,1,&status);
//  printf ("OK\n\r");
//}
//
//// Load EEPROM to Factory Default Bypass SN
//void ATCMD_EEDF()
//{
//  EEPROM_LOAD_FACTORY_DEFAULT();
//  UPDATE_VPM_VARIABLE_FROM_EEPROM();
//  printf ("OK\n\r");
//}
//
//// Save EEPROM to User Default
//void ATCMD_EEUS()
//{
//  EEPROM_SAVE_USER_DEFAULT();
//  printf ("OK\n\r");
//}
//
//// Load EEPROM to User Default
//void ATCMD_EEUL()
//{
//  if(EEPROM_LOAD_USER_DEFAULT())
//  {
//    printf ("ERR-E05\n\r");
//    return;
//  }
//  
//  UPDATE_VPM_VARIABLE_FROM_EEPROM();
//  printf ("OK\n\r");
//}
//
//// Reset EEPROM to Factory Default
//void ATCMD_EEFA()
//{
//  //printf ("S1\n\r");
//  EEPROM_FILL_DEFAULT_VALUE();
//  //printf ("S2\n\r");
//  UPDATE_VPM_VARIABLE_FROM_EEPROM();
//  printf ("OK\n\r");
//}
//
//// Get Working Mode for Ignition Off
//void ATCMD_IGMG()
//{
//  printf ("%d\n\r",VAR_IGN_OFF_WORKING_MODE & 0xFF);
//}
//
//// Set Working Mode for Ignition Off
//void ATCMD_IGMS(unsigned char mode_set)
//{
//  if (mode_set == '1')
//  {
//    VAR_IGN_OFF_WORKING_MODE = 1; // Goto Power Off
//    printf ("OK\n\r");
//    return;
//  }
//  if (mode_set == '2')
//  {
//    VAR_IGN_OFF_WORKING_MODE = 2; // Goto Suspend
//    printf ("OK\n\r");
//    return;
//  }
//  printf ("ERR-M04\n\r");
//}
//
//// 獲得ALARM INT Pin的目前電壓準位
//void ATCMD_RTAI()
//{
//  if (__MACRO_CHECK_IS_PA7_RTC_INT2_HI)
//  {
//    printf ("1\n\r");
//  }
//  else
//  {
//    printf ("0\n\r");
//  }
//}
//
//// Erase EEPROM Contents
//void ATCMD_EERS()
//{
//  EEPROM_ERASE_VALUE();
//  printf ("OK\n\r");
//}
//
//// Debug Command for Dump Event Queue
//void ATCMD_DBG03()
//{
//  EVENTQUEUE_DUMP_CONTENTS();
//}
//
//void ATCMD_EVQA (unsigned evt_cls_h,
//                 unsigned evt_cls_l,
//                 unsigned evt_idx_h,
//                 unsigned evt_idx_l)
//{
//  unsigned int evt_cls, evt_idx;
//  // Check Event Class
//  evt_cls = HEX_2DIGIT_TO_INT(evt_cls_h, evt_cls_l);
//  if (evt_cls > 255)
//  {
//    printf ("ERR-Q01\n\r"); return;
//  }
//  // Check Event Index
//  evt_idx = HEX_2DIGIT_TO_INT(evt_idx_h, evt_idx_l);
//  if (evt_idx > 255)
//  {
//    printf ("ERR-Q01\n\r"); return;
//  }
//  if ( EVENTQUEUE_INSERT_TO_QUEUE(evt_cls, evt_idx) == 0)
//  {
//    printf ("OK\n\r");
//  }
//  else
//  {
//    printf ("ERR-Q02\n\r");
//  }
//}
//                 
//void ATCMD_EVQG()
//{
//  printf ("%04X\n\r", EVENTQUEUE_DELETE_FROM_QUEUE());
//}
//
//// Event Queue Flush
//void ATCMD_EVQF()
//{
//  VAR_EVENT_INPUT = 0;
//  VAR_EVENT_OUTPUT = 0;
//  VAR_EVENT_SIZE = 0;
//  // 清完後沒有Event了，取消Interrupt
//  if (VAR_EVENT_SIZE == 0) __EN_uP_PB12_TO_OS_INT_LO;
//  printf ("OK\n\r");
//}
//
//// Dump Parameters Restore From EEPROM
//void ATCMD_EEPP()
//{
//  DUMP_EEPROM_PARAMETER_VALUES();
//}
//
//// 獲得當CAR Power Low採取之流程
//void ATCMD_MCLG()
//{
//  printf ("%1d\n\r",VAR_VPM_POWER_LOW_AT_S0_MODE);
//}
//
//// 設定當CAR Power Low採取之流程
//void ATCMD_MCLS(unsigned char method)
//{
//  switch (method)
//  {
//    case '0': VAR_VPM_POWER_LOW_AT_S0_MODE = 0;
//            // 對EEPROM寫入
//            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x52; // Addr
//            VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x00;
//            VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
//            printf ("OK\n\r");
//            break;
//    case '1': VAR_VPM_POWER_LOW_AT_S0_MODE = 1;
//            // 對EEPROM寫入
//            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x52; // Addr
//            VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x01;
//            VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
//            printf ("OK\n\r");
//            break;
//    case '2': VAR_VPM_POWER_LOW_AT_S0_MODE = 2;
//            // 對EEPROM寫入
//            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x52; // Addr
//            VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x02;
//            VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
//            printf ("OK\n\r");
//            break;
//            // 模式3 (Default Mode)，當Power Low時，通知OS，不管Ignition
//            // 情況
//    case '3': VAR_VPM_POWER_LOW_AT_S0_MODE = 3;
//            // 對EEPROM寫入
//            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
//            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x52; // Addr
//            VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x03;
//            VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
//            printf ("OK\n\r");
//            break;
//    default:
//            printf ("ERR-M05\n\r");
//            break;
//  }
//}
//
//void ATCMD_EEPC(void)
//{
//  DUMP_CURRENT_COUNTERS();
//}
//
//// Get Ignition On Power On Event Count
//void ATCMD_DIAG01()
//{
//  double result = (VAR_VPM_PWR_ON_IGN_CNT_ACC<<16)+VAR_VPM_PWR_ON_IGN_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Power Button Power On Event Count
//void ATCMD_DIAG02()
//{
//  double result = (VAR_VPM_PWR_ON_PWBTN_CNT_ACC<<16)+VAR_VPM_PWR_ON_PWBTN_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Car Power Low -> Off Event Count
//void ATCMD_DIAG03()
//{
//  double result = (VAR_VPM_PWR_LOW_OFF_CNT_ACC<<16)+VAR_VPM_PWR_LOW_OFF_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Ignition Off -> Off Event Count
//void ATCMD_DIAG04()
//{
//  double result = (VAR_VPM_IGN_OFF_CNT_ACC<<16)+VAR_VPM_IGN_OFF_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Battery Over Temperature Count
//void ATCMD_DIAG05()
//{
//  double result = (VAR_VPM_BAT_OVR_TMP_CNT_ACC<<16)+VAR_VPM_BAT_OVR_TMP_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Pre-boot Car Power Check Generate Power Off Count
//void ATCMD_DIAG06()
//{
//  double result = (VAR_VPM_PRE_VCHK_OFF_CNT_ACC<<16)+VAR_VPM_PRE_VCHK_OFF_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Post-boot Car Power Check Generate Power Off Count
//void ATCMD_DIAG07()
//{
//  double result = (VAR_VPM_POST_VCHK_OFF_CNT_ACC<<16)+VAR_VPM_POST_VCHK_OFF_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Watchdog Reset System Count
//void ATCMD_DIAG08()
//{
//  double result = (VAR_VPM_WDOG_RST_CNT_ACC<<16)+VAR_VPM_WDOG_RST_CNT;
//  printf ("%.0f\n\r",result);
//}
//
//// Get Keep Alive Power On Count
//void ATCMD_DIAG09()
//{
//  double result = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC<<16)+VAR_VPM_PWR_ON_KEEP_ALIVE_CNT;
//  printf ("%.0f\n\r",result);
//}