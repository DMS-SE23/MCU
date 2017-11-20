#ifndef _CONFIG_H
#define _CONFIG_H

/* Exported types ------------------------------------------------------------*/
/*share data with user program*/
#pragma location=0x20007FF8 
__no_init volatile int BOOT_SIG;


/*Board UART*/
#define COM_PORT					GPIOA
#define COM_PORT_CLK				RCC_APB2Periph_GPIOA
#define COM_TX_PIN					GPIO_Pin_9
#define COM_RX_PIN					GPIO_Pin_10
#define COM_USART_CLK				RCC_APB2Periph_USART1
#define COM_USART					USART1


/*Board Configuration Summary*/
/*
Bootloader	10K
AP Program	236k
Usr Data	8K
Info Data	2k
*/
#define BootloaderAddress		0x8000000
#define	ApplicationAddress		(BootloaderAddress + 0x2800)
#define UserDataAddress			(BootloaderAddress + 0x3D800)
#define InfoDataAddress			(BootloaderAddress + 0x3F800)



#define PAGE_SIZE				(0x800)
#define FLASH_SIZE				(0x40000)

#define BSL_FW_VERSION_MAJOR				0
#define BSL_FW_VERSION_MINOR				2
#define PRODUCT_ID				"DMS-SE23-MCU"

#define __DEF_PROJECT_NAME_STRING_SIZE 12
// キ嘿, 12じ舱 (12 Bytes)
#define __DEF_PROJECT_NAME_1  'D'
#define __DEF_PROJECT_NAME_2  'M'
#define __DEF_PROJECT_NAME_3  'S'
#define __DEF_PROJECT_NAME_4  '-'
#define __DEF_PROJECT_NAME_5  'S'
#define __DEF_PROJECT_NAME_6  'E'
#define __DEF_PROJECT_NAME_7  '2'
#define __DEF_PROJECT_NAME_8  '3'
// 皌PCB腹
#define __DEF_PROJECT_NAME_9  '-'
#define __DEF_PROJECT_NAME_10 'A'
#define __DEF_PROJECT_NAME_11 '0'
#define __DEF_PROJECT_NAME_12 '1'

// ﹚竡眔琘计锣ΘASC II Code
// 眔κ计
#define __MACRO_GET_DIGITAL_IN_HUNDREDS(x) x/100+0x30   
// 眔计
#define __MACRO_GET_DIGITAL_IN_TENS(x)     (x-(x/100))/10+0x30  
// 眔计
#define __MACRO_GET_DIGITAL_IN_UNITS(x)    (x%10)+0x30 

#define   DevID_FlashSize  *((u16 *)0x1FFFF7E0);  
#define   DevID_SNo0       *((u32 *)0x1FFFF7AC);  
#define   DevID_SNo1       *((u32 *)0x1FFFF7B0);  
#define   DevID_SNo2       *((u32 *)0x1FFFF7B4); 

//Information data offset in flash, count from backwards
#define CMP_TAG_OFFSET			        0x00
#define CHKSUM_TAG_OFFSET		        0x02
#define USR_AREA_CHKSUM_OFFSET	                0x06
#define AP_VERSION_OFFSET		        0x0A
#define USR_AP_END_ADD_OFFSET	                0x0C
#define BSL_INFO_OFFSET			        0x10
#define BSL_VERSION_OFFSET			0x20

//Information data size
#define COMPLETE_TAG_SIZE			0x02
#define CHKSUM_TAG_SIZE				0x04
#define USR_AREA_CHKSUM_SIZE		        0x04
#define AP_VERSION_SIZE				0x02
#define AP_END_ADD_SIZE				0x04
#define BSL_INFO_SIZE				0x10
#define BSL_VERSION_SIZE			0x02
#define INFO_DATA_SIZE				(COMPLETE_TAG_SIZE + CHKSUM_TAG_SIZE + USR_AREA_CHKSUM_SIZE + AP_VERSION_SIZE +AP_END_ADD_SIZE + BSL_INFO_SIZE + BSL_VERSION_SIZE)


//Information data Index
#define I_COMPLETE_TAG				0x01
#define I_CHKSUM_TAG				0x02
#define I_USR_AREA_CHKSUM			0x03
#define I_AP_VERSION				0x04
#define I_USR_AP_END_ADD			0x05
#define I_BSL_INFO			        0x06
#define I_BSL_VERSION				0x07


#endif
