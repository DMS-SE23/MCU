/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CMD_HANDLE_H
#define _CMD_HANDLE_H

#include "stm32f30x.h"

typedef enum eErrorCode
{
    ErrCode_OK		       	= 0x00,
    ErrCode_FAIL		= 0x01,
    ErrCode_UnsuportedCommand	= 0x02,
    ErrCode_OutOfRange		= 0x03,
    ErrCode_ChecksumFail        = 0x04
} ErrCode;


#define CMD_GET_CHIP_ID				0x00
#define CMD_GET_PRODUCT_ID			0x01
#define CMD_GET_USER_ID				0x02
#define CMD_GET_BSL_VER				0x03
#define CMD_ERASE_FLASH_AREA			0x04
#define CMD_WRT_BSL_VER				0x05
#define CMD_CHK_USR_AREA_EMPTY			0x06
#define CMD_WRT_BLOCK				0x07
#define CMD_RD_BLOCK				0x08
#define CMD_WRT_USR_AREA_CHKSUM			0x09
#define CMD_WRT_USR_AP_VER			0x0A
#define CMD_GET_USR_AP_VER			0x0B
#define CMD_WRT_CHKSUM_TAG			0x0C
#define CMD_RD_CHKSUM_TAG			0x0D
#define CMD_WRT_COMPLETE_TAG			0x0E
#define CMD_WRT_BSL_INFO			0x0F
#define CMD_RD_BSL_INFO				0x10
#define CMD_ENTER_BSL_MODE			0x11
#define CMD_RESET_SYSTEM			0x12
#define CMD_GET_LAST_ENTER_BSL_REASON	        0x13
#define CMD_RD_BLOCK_ON_RAM			0x16
#define	CMD_JUMP_TO_RAM_BOOTLOADER		0x17
#define CMD_WRT_BLOCK_ON_RAM			0x18
#define CMD_WRT_START_END_ADDRESS		0x19
#define CMD_CHK_RAM_BOOTLOADER_MODE		0x1A

/* Exported functions ------------------------------------------------------- */
uint8_t Program_Info_Data(int index, uint8_t* buf);
int I2C_CMD_Check(uint8_t command);
int I2C_CMD_Check_NByte(uint16_t number);
void I2C_CMD_Handler(uint8_t command);
void I2C_CMD_Fail(uint8_t command);
void In_Bootloader_Mode_Response(void);
unsigned char Get_Platform_String(int type, int index);

#endif