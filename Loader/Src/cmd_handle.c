#include "includes.h"

FLASH_Status FLASHStatus = FLASH_COMPLETE;
pFunction Jump_To_Bootloader;
pFunction Jump_To_RAM_Bootloader;
uint8_t* RamBSLStartAdd;
uint8_t* RamBSLEndAddr;


#pragma section = "RAMBSL"
static uint8_t *GetRAMBSLStartAddress()
{
/* Return start address of section TEMPSTORAGE. */
return __section_begin("RAMBSL");
}

static uint8_t *GetRAMBSLEndAddress()
{
/* Return start address of section TEMPSTORAGE. */
return __section_end("RAMBSL");
}


/*return 0x00 success
             0x01 erase failure
             0x02 program failure*/
uint8_t Program_Info_Data(int index, uint8_t* buf)
{
	uint8_t info_data[INFO_DATA_SIZE] = {0};
	uint8_t i = 0;
	uint32_t info_start_address = BootloaderAddress + FLASH_SIZE - INFO_DATA_SIZE;
	uint16_t written_data = 0;

	//Read out the current information data
	for (i = 0; i < INFO_DATA_SIZE; i++)
	{
		info_data[i] = *(uint8_t*)info_start_address;
		info_start_address++;
	}

	//Erase the information data page - last page in flash
	FLASHStatus = FLASH_ErasePage(InfoDataAddress);
	if (FLASHStatus != FLASH_COMPLETE)
		return 0x01;

	//Program back the data
	switch (index)
	{
		case I_COMPLETE_TAG:
			for (i = 0; i < COMPLETE_TAG_SIZE; i++)
			{
				info_data[INFO_DATA_SIZE - CMP_TAG_OFFSET - i - 1] = *(buf + i);
			}
			break;
		case I_CHKSUM_TAG:
			for (i = 0; i < CHKSUM_TAG_SIZE; i++)
			{
				info_data[INFO_DATA_SIZE - CHKSUM_TAG_OFFSET - i - 1] = *(buf + i);
			}
			break;
		case I_USR_AREA_CHKSUM:
			for (i = 0; i < USR_AREA_CHKSUM_SIZE; i++)
			{
				info_data[INFO_DATA_SIZE - USR_AREA_CHKSUM_OFFSET - i - 1] = *(buf + i);
			}
			break;
		case I_AP_VERSION:
			for (i = 0; i < AP_VERSION_SIZE; i++)
			{
				info_data[INFO_DATA_SIZE - AP_VERSION_OFFSET - i - 1] = *(buf + i);
			}
			break;
		case I_USR_AP_END_ADD:
			for (i = 0; i < AP_END_ADD_SIZE; i++)
			{
				info_data[INFO_DATA_SIZE - USR_AP_END_ADD_OFFSET - i - 1] = *(buf + i);
			}
			break;
		case I_BSL_INFO:
                    {
			uint8_t length = *buf;
			buf++;
			for (i = 0; i < length; i++)
			{
				info_data[INFO_DATA_SIZE - BSL_INFO_OFFSET - i - 1] = *(buf + i);
			}
			info_data[INFO_DATA_SIZE - BSL_INFO_OFFSET - BSL_INFO_SIZE] = length;
			break;
                    }
                case I_BSL_VERSION:
			for (i = 0; i < BSL_VERSION_SIZE; i++)
			{
				info_data[INFO_DATA_SIZE - BSL_VERSION_OFFSET - i - 1] = *(buf + i);
			}
			break;
		default:
			break;
	}

	info_start_address = BootloaderAddress + FLASH_SIZE - INFO_DATA_SIZE;

	for (i = 0; i < INFO_DATA_SIZE; i+=2)
	{
		written_data = (info_data[i+1] << 8 | info_data[i]);

		FLASH_ProgramHalfWord(info_start_address, written_data);

		if (*(uint16_t*)info_start_address != written_data)
		{
			return 0x02;
		}

		info_start_address+=2;
	}

	return 0x00;

}

int I2C_CMD_Check(uint8_t command)
{
  uint8_t* data;
  data = Buffer_Rx2+1;

  if(command == CMD_GET_CHIP_ID)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_GET_PRODUCT_ID)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_GET_BSL_VER)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_WRT_BSL_VER)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_ERASE_FLASH_AREA)
  {
    return I2C_CMD_Check_NByte(2);
  }
  else if (command == CMD_CHK_USR_AREA_EMPTY)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_WRT_BLOCK)
  {
    uint8_t write_length = (*(data+5) << 8 | *(data+6));
    return I2C_CMD_Check_NByte(write_length+8);
  }
  else if (command == CMD_RD_BLOCK)
  {
    return I2C_CMD_Check_NByte(8);
  }
  else if (command == CMD_WRT_USR_AREA_CHKSUM)
  {
    return I2C_CMD_Check_NByte(5);
  }
  else if (command == CMD_WRT_USR_AP_VER)
  {
    return I2C_CMD_Check_NByte(3);
  }
  else if (command == CMD_GET_USR_AP_VER)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_WRT_CHKSUM_TAG)
  {
    return I2C_CMD_Check_NByte(2);
  }
  else if (command == CMD_RD_CHKSUM_TAG)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_WRT_COMPLETE_TAG)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_WRT_BSL_INFO)
  {
    uint8_t write_length = *data;
    return I2C_CMD_Check_NByte(write_length+2);
  }
  else if (command == CMD_RD_BSL_INFO)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_ENTER_BSL_MODE)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_RESET_SYSTEM)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_GET_LAST_ENTER_BSL_REASON)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_JUMP_TO_RAM_BOOTLOADER)
  {
    return I2C_CMD_Check_NByte(1);
  }
  else if (command == CMD_WRT_BLOCK_ON_RAM)
  {
    uint8_t load_length = (*(data+4) << 8 | *(data+5));
    return I2C_CMD_Check_NByte(load_length+7);
  }
  else if (command == CMD_WRT_START_END_ADDRESS)
  {
    return I2C_CMD_Check_NByte(6);
  }
  else if (command == CMD_CHK_RAM_BOOTLOADER_MODE)
  {
    return I2C_CMD_Check_NByte(1);
  }
  return 0;
}

int I2C_CMD_Check_NByte(uint16_t number)
{
  unsigned char cks = 0;
  unsigned int i = 0;

  for (i = 0 ; i <= number ; i++)
  {
    cks ^= Buffer_Rx2[i];
  }

  if(cks == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void I2C_CMD_Handler(uint8_t command)
{
  uint8_t* data;
  data = Buffer_Rx2+1;
  uint16_t TxDataLen = 0;
  uint8_t checksum = 0x00;
  uint16_t i = 0;

  // Command
  Buffer_Tx2[0] = command;

  // Data Length
  Buffer_Tx2[1] = 0x00;
  Buffer_Tx2[2] = 0x01;

  // Status
  Buffer_Tx2[3] = ErrCode_UnsuportedCommand;
  TxDataLen = 4;

  //===========================================
  if(command == CMD_GET_CHIP_ID)
  {
    uint32_t tmp32 = 0x00;

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x0A + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    tmp32 = DevID_SNo0;
    Buffer_Tx2[4] = (tmp32 & 0xff00) >> 8;
    Buffer_Tx2[5] = (tmp32 & 0xff);
    tmp32 = DevID_SNo1;
    Buffer_Tx2[6] = (tmp32 & 0xff000000) >> 24;
    Buffer_Tx2[7] = (tmp32 & 0xff0000) >> 16;
    Buffer_Tx2[8] = (tmp32 & 0xff00) >> 8;
    Buffer_Tx2[9] = (tmp32 & 0xff);
    tmp32 = DevID_SNo2;
    Buffer_Tx2[10] = (tmp32 & 0xff000000) >> 24;
    Buffer_Tx2[11] = (tmp32 & 0xff0000) >> 16;
    Buffer_Tx2[12] = (tmp32 & 0xff00) >> 8;
    Buffer_Tx2[13] = (tmp32 & 0xff);
    TxDataLen += 10;
  }
  else if (command == CMD_GET_PRODUCT_ID)
  {
    uint8_t data_length = 0x00;
    uint32_t i = 0x00;

    // Data Length
    data_length = strlen(PRODUCT_ID) + 0x01;
    Buffer_Tx2[1] = (data_length & 0xff00) >> 8;
    Buffer_Tx2[2] = (data_length & 0xff);

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    data_length -= 1;
    for (i = 0; i < data_length; i++)
    {
      Buffer_Tx2[4 + i] = PRODUCT_ID[i];
    }
    TxDataLen += data_length;
  }
  else if(command == CMD_GET_BSL_VER)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x02 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    Buffer_Tx2[4] = BSL_FW_VERSION_MAJOR;
    Buffer_Tx2[5] = BSL_FW_VERSION_MINOR;
    TxDataLen += 2;
  }
  else if (command == CMD_WRT_BSL_VER)
  {
    uint8_t bsl_version[BSL_VERSION_SIZE] = {BSL_FW_VERSION_MAJOR, BSL_FW_VERSION_MINOR};

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    uint8_t val = Program_Info_Data(I_BSL_VERSION, bsl_version);
    // Status
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_ERASE_FLASH_AREA)
  {
    uint8_t area = *data;
    uint32_t erase_start_address = 0x00;
    uint32_t erase_end_address = 0x00;
    uint16_t start_page = 0x00;
    uint16_t end_page = 0x00;
    uint32_t i = 0x00;

    // Erase Starting Address
    if (area == 0x00)             // Erase User AP
    {
      erase_start_address = ApplicationAddress;
      erase_end_address = UserDataAddress - 1;
    }
    else if (area == 0x01)        // Erase User Data
    {
      erase_start_address = UserDataAddress;
      erase_end_address = InfoDataAddress - 1;
    }
    else if (area == 0x02)        // Erase Information Data - last page in flash
    {
      erase_start_address = InfoDataAddress;
      erase_end_address = BootloaderAddress + FLASH_SIZE - 1;
    }
    else if (area == 0x03)        // Erase User AP + Information Data
    {
      erase_start_address = ApplicationAddress;
      erase_end_address = UserDataAddress - 1;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_UnsuportedCommand;
      return;
    }

    start_page = (erase_start_address - BootloaderAddress) / PAGE_SIZE;
    end_page = (erase_end_address - BootloaderAddress) / PAGE_SIZE;

    // Start Erase
    for (i = start_page; i <= end_page; i++)
    {
      FLASHStatus = FLASH_ErasePage(BootloaderAddress + (PAGE_SIZE * i));
      if (FLASHStatus != FLASH_COMPLETE)
        break;
    }

    if (area == 0x03)       // Erase User AP + Information Data
    {
      erase_start_address = InfoDataAddress;
      erase_end_address = BootloaderAddress + FLASH_SIZE - 1;
      start_page = (erase_start_address - BootloaderAddress) / PAGE_SIZE;
      end_page = (erase_end_address - BootloaderAddress) / PAGE_SIZE;

      // Start Erase
      for (i = start_page; i <= end_page; i++)
      {
        FLASHStatus = FLASH_ErasePage(BootloaderAddress + (PAGE_SIZE * i));
        if (FLASHStatus != FLASH_COMPLETE)
          break;
      }
    }

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    if (FLASHStatus != FLASH_COMPLETE)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_CHK_USR_AREA_EMPTY)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x01 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    if (*((uint16_t*)(BootloaderAddress + FLASH_SIZE - (CMP_TAG_OFFSET + COMPLETE_TAG_SIZE))) == COMPLETED)
    {
      Buffer_Tx2[4] = 0x00;       // User Area is not Empty
    }
    else
    {
      Buffer_Tx2[4] = 0x01;       // User Area Empty
    }
    TxDataLen += 1;
  }
  else if (command == CMD_WRT_BLOCK)
  {
    uint8_t area = *data;
    uint32_t offset = (*(data+1) << 24 | *(data+2) << 16 | *(data+3) << 8 | *(data+4));
    uint8_t write_length = (*(data+5) << 8 | *(data+6));
    uint8_t* buf = data + 7;        // Exclude Area (1) + Offset (4) + Data Length (2)
    uint32_t write_address = 0x00;
    uint16_t ramsource = 0x00;
    uint32_t i = 0x00;

    // Write Start Address
    if (area == 0x00)         // Write User AP
    {
      write_address = ApplicationAddress + offset;

      if ((write_address + write_length - 1) > (UserDataAddress - 1))
      {
        Buffer_Tx2[3] = ErrCode_OutOfRange;
        return;
      }
    }
    else if (area == 0x01)    // Write User Data
    {
      write_address = UserDataAddress + offset;

      if ((write_address + write_length - 1) > (InfoDataAddress - 1))
      {
        Buffer_Tx2[3] = ErrCode_OutOfRange;
        return;
      }
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_UnsuportedCommand;
      return;
    }

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    //Status
    Buffer_Tx2[3] = ErrCode_OK;

    if (write_length / 2)             // Odd Length
    {
      buf[write_length] = 0xFF;       // 補 0xFF 到最後
    }
    for (i = 0; i < write_length; i+=2)
    {
      ramsource = *(buf+1) << 8 | *buf;
      FLASH_ProgramHalfWord(write_address, ramsource);

      if (*(uint16_t*)write_address != ramsource)
      {
        Buffer_Tx2[3] = ErrCode_FAIL;
      }

      buf += 2;
      write_address += 2;
    }
  }
  else if (command == CMD_RD_BLOCK)
  {
    uint8_t area = *data;
    uint32_t offset = (*(data+1) << 24 | *(data+2) << 16 | *(data+3) << 8 | *(data+4));
    uint8_t read_length = (*(data+5) << 8 | *(data+6));
    uint16_t return_length = read_length + 0x01;
    uint32_t read_address = 0x00;
    uint32_t i = 0;

    if (area == 0x00)         // Read User AP
    {
      read_address = ApplicationAddress + offset;

      if ((read_address + read_length - 1) > (UserDataAddress - 1))
      {
        Buffer_Tx2[3] = ErrCode_OutOfRange;
        return;
      }
    }
    else if (area == 0x01)    // Read User Data
    {
      read_address = UserDataAddress + offset;

      if ((read_address + read_length - 1) > (InfoDataAddress - 1))
      {
        Buffer_Tx2[3] = ErrCode_OutOfRange;
        return;
      }
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_UnsuportedCommand;
      return;
    }

    // Data Length
    Buffer_Tx2[1] = (return_length & 0xff00) >> 8;
    Buffer_Tx2[2] = return_length & 0xff;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    for (i = 0; i < read_length; i++)
    {
      Buffer_Tx2[4 + i] =  *((uint8_t*)read_address);
      read_address++;
    }
    TxDataLen += read_length;
  }
  else if (command == CMD_WRT_USR_AREA_CHKSUM)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    uint8_t val = Program_Info_Data(I_USR_AREA_CHKSUM, data);
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_WRT_USR_AP_VER)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    uint8_t val = Program_Info_Data(I_AP_VERSION, data);
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_GET_USR_AP_VER)
  {
    uint32_t read_address = BootloaderAddress + FLASH_SIZE - (AP_VERSION_OFFSET + AP_VERSION_SIZE);

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x02 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    uint32_t ap_version = *((uint16_t*)read_address);
    Buffer_Tx2[4] = (ap_version & 0xff00) >> 8;
    Buffer_Tx2[5] = ap_version & 0xff;
    TxDataLen += 2;
  }
  else if (command == CMD_WRT_CHKSUM_TAG)
  {
    uint8_t check_type = *data;
    uint8_t checksum_tag[CHKSUM_TAG_SIZE] = {0};

    if (check_type == 0x00)           // Check All Time
    {
      checksum_tag[3] = CHK_ALL_TIME & 0xff;
      checksum_tag[2] = (CHK_ALL_TIME & 0xff00) >> 8;
      checksum_tag[1] = (CHK_ALL_TIME & 0xff0000) >> 16;
      checksum_tag[0] = (CHK_ALL_TIME & 0xff000000) >> 24;
    }
    else if (check_type == 0x01)      // Check Once
    {
      checksum_tag[3] = CHK_ONCE & 0xff;
      checksum_tag[2] = (CHK_ONCE & 0xff00) >> 8;
      checksum_tag[1] = (CHK_ONCE & 0xff0000) >> 16;
      checksum_tag[0] = (CHK_ONCE & 0xff000000) >> 24;
    }
    else if (check_type == 0x02)      // Do not Check
    {
      checksum_tag[3] = DO_NOT_CHK & 0xff;
      checksum_tag[2] = (DO_NOT_CHK & 0xff00) >> 8;
      checksum_tag[1] = (DO_NOT_CHK & 0xff0000) >> 16;
      checksum_tag[0] = (DO_NOT_CHK & 0xff000000) >> 24;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_UnsuportedCommand;
      return;
    }

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    uint8_t val = Program_Info_Data(I_CHKSUM_TAG, checksum_tag);
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_RD_CHKSUM_TAG)
  {
    uint32_t read_address = BootloaderAddress + FLASH_SIZE - (CHKSUM_TAG_OFFSET + CHKSUM_TAG_SIZE);

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x01 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    uint32_t checksum_tag= *((uint32_t*)read_address);
    if (checksum_tag == CHK_ALL_TIME)
    {
      Buffer_Tx2[4] = 0x00;
    }
    else if (checksum_tag == CHK_ONCE)
    {
      Buffer_Tx2[4] = 0x01;
    }
    else if (checksum_tag == DO_NOT_CHK)
    {
      Buffer_Tx2[4] = 0x02;
    }
    TxDataLen += 1;
  }
  else if (command == CMD_WRT_COMPLETE_TAG)
  {
    uint8_t tag[COMPLETE_TAG_SIZE] = {(COMPLETED & 0xFF), (COMPLETED & 0xFF)};

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    uint8_t val = Program_Info_Data(I_COMPLETE_TAG, tag);
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_WRT_BSL_INFO)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    uint8_t val = Program_Info_Data(I_BSL_INFO, data);
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_RD_BSL_INFO)
  {
    uint8_t bsl_info[16] = {0};
    uint32_t read_address = BootloaderAddress + FLASH_SIZE - (BSL_INFO_OFFSET + BSL_INFO_SIZE);
    uint32_t i = 0;

    for (i = 0; i < 16; i++)
    {
      bsl_info[i] = *(uint8_t*)read_address;
      read_address++;
    }

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = bsl_info[0] + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    for (i=0; i < bsl_info[0]; i++)
    {
      Buffer_Tx2[4+i] = bsl_info[BSL_INFO_SIZE - i - 1];
    }
    TxDataLen += bsl_info[0];
  }
  else if (command == CMD_ENTER_BSL_MODE)
  {
    BOOT_SIG = 0xbabecafe;
    JumpAddress = *(__IO uint32_t*) (BootloaderAddress + 4);
    Jump_To_Bootloader = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) BootloaderAddress);
    Jump_To_Bootloader();
  }
  else if (command == CMD_RESET_SYSTEM)
  {
    Bootloader_Update_OK = 1;

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;
  }
  else if (command == CMD_GET_LAST_ENTER_BSL_REASON)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x01 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    Buffer_Tx2[4] = Last_Enter_Bootloader_Reason;
    TxDataLen += 1;
  }
  else if (command == CMD_JUMP_TO_RAM_BOOTLOADER)
  {
    RamBSLStartAdd = GetRAMBSLStartAddress();
    JumpAddress = *(__IO uint32_t*) (RamBSLStartAdd + 4);
    Jump_To_RAM_Bootloader = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*)RamBSLStartAdd);
    Jump_To_RAM_Bootloader();
  }
  else if (command == CMD_WRT_BLOCK_ON_RAM)
  {
    RamBSLStartAdd = GetRAMBSLStartAddress();
    RamBSLEndAddr = GetRAMBSLEndAddress();

    uint32_t offset = (*(data) << 24 | *(data+1) << 16 | *(data+2) << 8 | *(data+3));
    uint8_t load_length = (*(data+4) << 8 | *(data+5));
    uint8_t* buf = data+6;    // Exclude Offset (4) + Data Length (2)
    uint8_t* pt = RamBSLStartAdd + offset;

    if( (pt + load_length - 1) > (RamBSLEndAddr - 1) )
    {
      Buffer_Tx2[3] = ErrCode_OutOfRange;
      return;
    }

    // Place Data in RAM
    memcpy(pt,buf,load_length);

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;
  }
  else if (command == CMD_WRT_START_END_ADDRESS)
  {
    uint8_t flag = *data;
    data++;

    // Write Data
    if (flag != 0x02) // End User AP
    {
      Buffer_Tx2[3] = ErrCode_UnsuportedCommand;
      return;
    }

    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x00 + 0x01;

    // Status
    uint8_t val = Program_Info_Data(I_USR_AP_END_ADD, data);
    if (val != 0)
    {
      Buffer_Tx2[3] = ErrCode_FAIL;
    }
    else
    {
      Buffer_Tx2[3] = ErrCode_OK;
    }
  }
  else if (command == CMD_CHK_RAM_BOOTLOADER_MODE)
  {
    // Data Length
    Buffer_Tx2[1] = 0x00;
    Buffer_Tx2[2] = 0x01 + 0x01;

    // Status
    Buffer_Tx2[3] = ErrCode_OK;

    // Data
    Buffer_Tx2[4] = 0x01;
    TxDataLen += 1;
  }

  // Checksum
  for (i = 0 ; i < TxDataLen ; i++)
  {
    checksum ^= Buffer_Tx2[i];
  }
  Buffer_Tx2[TxDataLen] = checksum;
  for (i = TxDataLen + 1 ; i < 32 ; i++)
  {
    Buffer_Tx2[i] = 0x00;
  }
}

void I2C_CMD_Fail(uint8_t command)
{
  uint16_t i = 0;
  uint8_t checksum = 0x00;

  // Command
  Buffer_Tx2[0] = command;

  // Data Length
  Buffer_Tx2[1] = 0x00;
  Buffer_Tx2[2] = 0x01;

  // Status
  Buffer_Tx2[3] = ErrCode_ChecksumFail;

  // Checksum
  for (i = 0 ; i < 4 ; i++)
  {
    checksum ^= Buffer_Tx2[i];
  }
  Buffer_Tx2[4] = checksum;
  for (i = 5 ; i < 32 ; i++)
  {
    Buffer_Tx2[i] = 0x00;
  }
}

void In_Bootloader_Mode_Response()
{
  uint16_t i = 0;

  Buffer_Tx2[0] = 0xAA;

  for (i = 1 ; i < 32 ; i++)
  {
    Buffer_Tx2[i] = 0x00;
  }
}

unsigned char Get_Platform_String(int type, int index)
{
  // 版本宣告 ============================================================
  // *VER:000.000*                                                      //
  unsigned char _PLATFORM_VERSION[13] = {'*','V','E','R',':',           //
           __MACRO_GET_DIGITAL_IN_HUNDREDS(BSL_FW_VERSION_MAJOR), //
           __MACRO_GET_DIGITAL_IN_TENS(BSL_FW_VERSION_MAJOR),     //
           __MACRO_GET_DIGITAL_IN_UNITS(BSL_FW_VERSION_MAJOR),    //
                                    '.',                                //
           __MACRO_GET_DIGITAL_IN_HUNDREDS(BSL_FW_VERSION_MINOR),//
           __MACRO_GET_DIGITAL_IN_TENS(BSL_FW_VERSION_MINOR),    //
           __MACRO_GET_DIGITAL_IN_UNITS(BSL_FW_VERSION_MINOR),   //
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
      case 0:  return ((unsigned char)BSL_FW_VERSION_MAJOR); break;
      case 1:  return ((unsigned char)BSL_FW_VERSION_MINOR); break;
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