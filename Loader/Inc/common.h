/**
  ******************************************************************************
  * @file    IAP/inc/common.h
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file provides all the headers of the common functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _COMMON_H
#define _COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"


/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);
extern pFunction Jump_To_Application;
extern pFunction Jump_To_Bootloader;
extern pFunction Jump_To_RAM_Bootloader;
extern uint32_t JumpAddress;
extern uint8_t Last_Enter_Bootloader_Reason;
extern volatile int Bootloader_Update_OK;

/* Exported constants --------------------------------------------------------*/
//Enter bootloader reason
#define PIN_PUSH		        0x01
#define USER_PROGRAM_EMPTY		0x02
#define CHKSUM_ERROR			0x03
#define FORCED_BY_COMMAND		0x04

//Checksum Tag Definition
#define CHK_ALL_TIME			0xFFFFFFFF
#define CHK_ONCE			0x0000FFFF
#define DO_NOT_CHK			0x00000000
//Complete Tag Definition
#define COMPLETED			0xAAAA
#define NOT_COMPLETED			0xFFFF


/* Exported macro ------------------------------------------------------------*/

/* Common routines */


/* Exported functions ------------------------------------------------------- */


#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2009 STMicroelectronics *****END OF FILE******/
