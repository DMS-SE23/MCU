#include "stm32f30x_conf.h"
#include "stm32_it.h"
#include <system_stm32f30x.h>
//#include <core_cm3.h>
#include <stm32f30x.h>
#include "gpio_control.h"
#include <stdio.h>


#include "project.h"
#include "main.h"
#include "at_command.h"

u8 TxBuffer[TxBufferSize];
u8 RxBuffer[RxBufferSize];
u16 TxS=0, TxE=0; // Tx Start, End Ptr
u16 RxS=0, RxE=0; // Rx Start, End Ptr

//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(USART1, (uint8_t) ch);
//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//    {}
//  return ch;
//}

unsigned char Get_Platform_String(int type, int index)
{
  // 版本宣告 ============================================================
  // *VER:000.000*                                                      //
  unsigned char _PLATFORM_VERSION[13] = {'*','V','E','R',':',           //
           __MACRO_GET_DIGITAL_IN_HUNDREDS(__DEF_PROJECT_MAIN_VERSION), //
           __MACRO_GET_DIGITAL_IN_TENS(__DEF_PROJECT_MAIN_VERSION),     //
           __MACRO_GET_DIGITAL_IN_UNITS(__DEF_PROJECT_MAIN_VERSION),    //
                                    '.',                                //
           __MACRO_GET_DIGITAL_IN_HUNDREDS(__DEF_PROJECT_MINER_VERSION),//
           __MACRO_GET_DIGITAL_IN_TENS(__DEF_PROJECT_MINER_VERSION),    //
           __MACRO_GET_DIGITAL_IN_UNITS(__DEF_PROJECT_MINER_VERSION),   //
                                    '*'};                               //
  // 版本宣告 ============================================================

  // 平台宣告 ======================================================
  // *PLF:xxxxxxxx*                                               //
  unsigned char _PLATFORM_NAME[__DEF_PROJECT_NAME_STRING_SIZE+6]  //
                                   = {'*','P','L','F',':',        //
                                    __DEF_PROJECT_NAME_1,         //
                                    __DEF_PROJECT_NAME_2,         //
                                    __DEF_PROJECT_NAME_3,         //
                                    __DEF_PROJECT_NAME_4,         //
                                    __DEF_PROJECT_NAME_5,         //
                                    __DEF_PROJECT_NAME_6,         //
                                    __DEF_PROJECT_NAME_7,         //
                                    __DEF_PROJECT_NAME_8,         //
                                    __DEF_PROJECT_NAME_9,         //
                                    __DEF_PROJECT_NAME_10,        //
                                    __DEF_PROJECT_NAME_11,        //
                                    __DEF_PROJECT_NAME_12,        //
                                    '*'};                         //
  // 平台宣告 ======================================================

  if (type == 1) // 傳回版本資訊，以Byte格式傳回
  {
    switch (index)
    {
      case 0:  return ((unsigned char)__DEF_PROJECT_MAIN_VERSION); break;
      case 1:  return ((unsigned char)__DEF_PROJECT_MINER_VERSION); break;
      default: return (0); break;
    }
  }
  else
  if (type == 2) // 傳回版本資訊，以ASC格式傳回
  {
    if (index >=13) return(0);
    return (_PLATFORM_VERSION[index]);
  }
  else
  if (type == 3) // 傳回平台資訊，以ASCC格式傳回
  {
    if (index >= (__DEF_PROJECT_NAME_STRING_SIZE + 6)) return(0);
    return (_PLATFORM_NAME[index]);
  }
  return (0x0);
}

//// ==================================================================
//// 檢查AT Command
//u8 AT_hasbyte()
//{
//    if(RxS==RxE)
//      return 0;
//    return 1;
//}
//
//u8 AT_getchar(u8 *data)
//{
//    if ( AT_hasbyte() )
//    {
//        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // 禁止中斷
//        *data = RxBuffer[RxS++];
//        if(RxS==RxBufferSize)
//            RxS = 0;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 恢復中斷
//        return 1;
//    }
//    return 0;
//}
//
//void AT_putchar(u8 data)
//{
//    if((TxS+TxBufferSize-1)%TxBufferSize!=TxE)  // check buffer overflow
//    {
//        USART_ITConfig(USART1, USART_IT_TXE, DISABLE); // 禁止中斷
//
//        TxBuffer[TxE++] = data;
//        if(TxE==TxBufferSize)
//          TxE = 0;
//
//        USART_ITConfig(USART1, USART_IT_TXE, ENABLE); // 恢復中斷
//    }
//}
//
//void Check_AT_Command()
//{
//  u8 data;
//  if (!ATCommandRdy && AT_getchar(&data))
//  {
//    if (data == 0x0d)
//    {
//      ATCmd[ATCmdLength++] = 0x0d;
//      ATCommandRdy = 1;
//    }
//    else
//    {
//      ATCmd[ATCmdLength++] = data&0xff;
//    }
//  }
//}
//
//// 建立自己的printf，但是沒有Format
//void AT_PRINTF(const char *string)
//{
//  int var_index = 0;
//  while (string[var_index] != 0)
//  {
//    AT_putchar(string[var_index++]);
//  }
//}

//// 處理AT Command 程式進入點 (Command Mode)
//void AT_Command_Processing()
//{
//  if (ATCmd[0] == 0x0d) // 直接按Enter
//  {
//    return;
//  }
//  // ------------------------------------------------------
//  if (ATCmd[0] == 'A')
//  {
//    if (ATCmd[1] == 'T')
//    {
//      if (ATCmd[2] == 'F')
//      {
//        switch (ATCmd[3])
//        {
//          case '0':
//                    ATCMD_ATF0();
//                    return;
//                    break;
//          case '1':
//                    ATCMD_ATF1();
//                    return;
//                    break;
//		  case '2':
//		  			ATCMD_ATF2();
//                    return;
//                    break;
//          case 'D':
//		  			ATCMD_ATFD();
//                    return;
//                    break;
//        }
//      }
//      if (ATCmd[2] == 'W')
//      {
//        if (ATCmd[3] == 'S')
//        {
//          ATCMD_ATWS();
//          return;
//        }
//      }
//      if (ATCmd[2] == 'S')
//      {
//        if (ATCmd[3] == 'N')
//        {
//          ATCMD_ATSN();
//          return;
//        }
//      }
//      if (ATCmd[2] == 'P')
//      {
//        if (ATCmd[3] == 'C')
//        {
//          ATCMD_ATPC();
//          return;
//        }
//      }
//      if ((ATCmd[2] == 'M') & (ATCmd[3] == 'E')) { ATCMD_ATME(); return; }
//    }
//  }
//
//  if (ATCmd[0] == 'B')
//  {
//    if (ATCmd[1] == '0')
//    {
//      switch (ATCmd[2])
//      {
//        case '1': ATCMD_B01(); return; break;
//        case '2': ATCMD_B02(); return; break;
//        case '3': ATCMD_B03(); return; break;
//        case '4': ATCMD_B04(); return; break;
//        case '5': ATCMD_B05(); return; break;
//        case '8': ATCMD_B08(); return; break;
//        case '9': ATCMD_B09(); return; break;
//      }
//    }
//    if (ATCmd[1] == '1')
//    {
//      switch (ATCmd[2])
//      {
//        case '0': ATCMD_B10(); return; break;
//        case '1': ATCMD_B11(); return; break;
//        case '2': ATCMD_B12(); return; break;
//      }
//    }
//    if (ATCmd[1] == '2')
//    {
//      switch (ATCmd[2])
//      {
//        case '2': ATCMD_B22(); return; break;
//        case '9': ATCMD_B29(); return; break;
//      }
//    }
//  }
//
//  if (ATCmd[0] == 'D')
//  {
//    if ((ATCmd[1] == 'I') && (ATCmd[2] == 'G')) { ATCMD_DIG(); return; }
//    if ((ATCmd[1] == 'B') && (ATCmd[2] == 'G'))
//    {
//      if (ATCmd[3] == '0')
//      {
//        switch (ATCmd[4])
//        {
//          case '1': ATCMD_DBG01(); return; break;
//          case '2': ATCMD_DBG02(); return; break;
//          case '3': ATCMD_DBG03(); return; break;
//        }
//      }
//    }
//    if ((ATCmd[1] == 'I') && (ATCmd[2] == 'A') && (ATCmd[3] == 'G'))
//    {
//      if (ATCmd[4] == '0')
//      {
//        switch (ATCmd[5])
//        {
//          case '1': ATCMD_DIAG01(); return; break;
//          case '2': ATCMD_DIAG02(); return; break;
//          case '3': ATCMD_DIAG03(); return; break;
//          case '4': ATCMD_DIAG04(); return; break;
//          case '5': ATCMD_DIAG05(); return; break;
//          case '6': ATCMD_DIAG06(); return; break;
//          case '7': ATCMD_DIAG07(); return; break;
//          case '8': ATCMD_DIAG08(); return; break;
//          case '9': ATCMD_DIAG09(); return; break;
//        }
//      }
//    }
//  }
//  // =======================
//  // EEPROM Related Commands
//  // =======================
//  if (ATCmd[0] == 'E')
//  {
//    if (ATCmd[1] == 'E')
//    {
//      switch (ATCmd[2])
//      {
//        case 'T': ATCMD_EET(); return; break;
//        case 'G': ATCMD_EEG(ATCmd[3],ATCmd[4],
//                            ATCmd[5],ATCmd[6]); return; break;
//        case 'S': ATCMD_EES(ATCmd[3],ATCmd[4],
//                            ATCmd[5],ATCmd[6],
//                            ATCmd[7],ATCmd[8]); return; break;
//      }
//      if ((ATCmd[2] == 'D') && (ATCmd[3] == 'F')) {ATCMD_EEDF(); return;}
//      if ((ATCmd[2] == 'U') && (ATCmd[3] == 'S')) {ATCMD_EEUS(); return;}
//      if ((ATCmd[2] == 'U') && (ATCmd[3] == 'L')) {ATCMD_EEUL(); return;}
//      if ((ATCmd[2] == 'F') && (ATCmd[3] == 'A')) {ATCMD_EEFA(); return;}
//      if ((ATCmd[2] == 'R') && (ATCmd[3] == 'S')) {ATCMD_EERS(); return;}
//      if ((ATCmd[2] == 'P') && (ATCmd[3] == 'P')) {ATCMD_EEPP(); return;}
//      if ((ATCmd[2] == 'P') && (ATCmd[3] == 'C')) {ATCMD_EEPC(); return;}
//    }
//    if (ATCmd[1] == 'V')
//    {
//      if ((ATCmd[2] == 'Q') && (ATCmd[3] == 'A')) {
//                                          ATCMD_EVQA(ATCmd[4],ATCmd[5],
//                                                     ATCmd[6],ATCmd[7]); return;}
//      if ((ATCmd[2] == 'Q') && (ATCmd[3] == 'G')) {ATCMD_EVQG(); return;}
//      if ((ATCmd[2] == 'Q') && (ATCmd[3] == 'F')) {ATCMD_EVQF(); return;}
//    }
//    printf ("ERR-E01\n\r"); // 不認識的Exx Command
//    return;
//  }
//
//  if (ATCmd[0] == 'M')
//  {
//    if (ATCmd[1] == 'C')
//    {
//      if (ATCmd[2] == 'G') { ATCMD_MCG(); return;}
//      if (ATCmd[2] == 'S') { ATCMD_MCS(ATCmd[3],
//                                       ATCmd[4]); return; }
//      if ((ATCmd[2] == 'B') && (ATCmd[3] == 'G')) { ATCMD_MCBG(); return; }
//      if ((ATCmd[2] == 'V') && (ATCmd[3] == 'G')) { ATCMD_MCVG(); return; }
//      if ((ATCmd[2] == 'P') && (ATCmd[3] == 'R')) { ATCMD_MCPR(ATCmd[4]); return; }
//      if ((ATCmd[2] == 'P') && (ATCmd[3] == 'G')) { ATCMD_MCPG(ATCmd[4],
//                                                               ATCmd[5]); return; }
//      if ((ATCmd[2] == 'P') && (ATCmd[3] == 'D')) { ATCMD_MCPD(ATCmd[4]); return; }
//      if ((ATCmd[2] == 'P') && (ATCmd[3] == 'S')) { ATCMD_MCPS(ATCmd[4],
//                                                               ATCmd[5],
//                                                               ATCmd[6],
//                                                               ATCmd[7],
//                                                               ATCmd[8],
//                                                               ATCmd[9]); return; }
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'G')) { ATCMD_MCAG(); return; }
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'S')) { ATCMD_MCAS(ATCmd[4],
//                                                               ATCmd[5]); return; }
//      if ((ATCmd[2] == 'C') && (ATCmd[3] == 'G')) { ATCMD_MCCG(); return; }
//      if ((ATCmd[2] == 'C') && (ATCmd[3] == 'S')) { ATCMD_MCCS(ATCmd[4],
//                                                               ATCmd[5]); return; }
//      if ((ATCmd[2] == 'L') && (ATCmd[3] == 'G')) { ATCMD_MCLG(); return; }
//      if ((ATCmd[2] == 'L') && (ATCmd[3] == 'S')) { ATCMD_MCLS(ATCmd[4]); return; }
//      if ((ATCmd[2] == 'U') && (ATCmd[3] == 'G')) { ATCMD_MCUG(ATCmd[4]); return; }
//      if ((ATCmd[2] == 'U') && (ATCmd[3] == 'S')) { ATCMD_MCUS(ATCmd[4],
//                                                               ATCmd[5],
//                                                               ATCmd[6],
//                                                               ATCmd[7],
//                                                               ATCmd[8]); return; }
//    }
//  }
//
//  if (ATCmd[0] == 'R')
//  {
//    if (ATCmd[1] == 'T')
//    {
//      if ((ATCmd[2] == 'C') && (ATCmd[3] == 'G')) { ATCMD_RTCG(); return; }
//      if ((ATCmd[2] == 'C') && (ATCmd[3] == 'S')) { ATCMD_RTCS(); return; }
//      // Alarm Time Setting
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'G')) { ATCMD_RTAG(); return; }
//      // Set Alarm Time Setting
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'S')) { ATCMD_RTAS(ATCmd[4],
//                                                               ATCmd[5],
//                                                               ATCmd[6],
//                                                               ATCmd[7],
//                                                               ATCmd[8],
//                                                               ATCmd[9]); return; }
//      // Clear Alarm Status
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'C')) { ATCMD_RTAC(); return; }
//      // Reset RTC
//      if ((ATCmd[2] == 'R') && (ATCmd[3] == 'S') && (ATCmd[4] == 'T'))
//                                                  { ATCMD_RTRST(); return; }
//      // 設定Alarm INT Enable Bit
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'E')) { ATCMD_RTAE(); return; }
//      // Get RTC INT Pin Level
//      if ((ATCmd[2] == 'A') && (ATCmd[3] == 'I')) { ATCMD_RTAI(); return; }
//    }
//  }
//
//  if (ATCmd[0] == 'W')
//  {
//    if (ATCmd[1] == 'D')
//    {
//      if ((ATCmd[2] == 'T') && (ATCmd[3] == 'G')) { ATCMD_WDTG(); return; }
//      if ((ATCmd[2] == 'T') && (ATCmd[3] == 'S')) { ATCMD_WDTS(
//                                                       ATCmd[4]
//                                                      ,ATCmd[5]
//                                                      ,ATCmd[6]
//                                                      ,ATCmd[7]); return; }
//      if ((ATCmd[2] == 'C') && (ATCmd[3] == 'G')) { ATCMD_WDCG(); return; }
//      if ((ATCmd[2] == 'T') && (ATCmd[3] == 'R')) { ATCMD_WDTR(); return; }
//      if ((ATCmd[2] == 'T') && (ATCmd[3] == 'E')) { ATCMD_WDTE(); return; }
//      if ((ATCmd[2] == 'T') && (ATCmd[3] == 'D')) { ATCMD_WDTD(); return; }
//    }
//    if (ATCmd[1] == 'K')
//    {
//      if ((ATCmd[2] == 'S') && (ATCmd[3] == 'G')) { ATCMD_WKSG(); return; }
//      if ((ATCmd[2] == 'M') && (ATCmd[3] == 'G')) { ATCMD_WKMG(); return; }
//      if ((ATCmd[2] == 'M') && (ATCmd[3] == 'S')) { ATCMD_WKMS(
//                                                       ATCmd[4]
//                                                      ,ATCmd[5]); return; }
//    }
//  }
//
//  if (ATCmd[0] == 'I')
//  {
//    if (ATCmd[1] == 'G')
//    {
//      if ((ATCmd[2] == 'N') && (ATCmd[3] == 'G')) { ATCMD_IGNG(); return; }
//      if ((ATCmd[2] == 'M') && (ATCmd[3] == 'G')) { ATCMD_IGMG(); return; }
//      if ((ATCmd[2] == 'M') && (ATCmd[3] == 'S')) { ATCMD_IGMS(ATCmd[4]);
//                                                    return; }
//    }
//  }
//
//  /* 都不認識了的Commands (Command Mode)-------------------*/
//  printf ("ERR-C01\r\n"); // Unknown Command in Command Mode
//}

//// 處理AT Command 程式進入點 (Data Mode)
//void AT_Data_Processing()
//{
//  /* 都不認識了的Commands (Data Mode)----------------------*/
//  AT_PRINTF("ERR-C02\r\n");
//}
//
