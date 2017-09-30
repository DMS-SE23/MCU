#include "includes.h"

#define I2C_TIMEOUT  0x8000

void I2C1_Master_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_DeInit(I2C1);
  I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter        = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter       = 0x00;
  I2C_InitStructure.I2C_OwnAddress1         = 0x00;
  I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//  I2C_InitStructure.I2C_Timing              = 0x0031040a;
  I2C_InitStructure.I2C_Timing              = 0x10808DD3;

  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);

  DEBUG_PRINT("..: I2C1 Master Init Complete\r\n");
}
//-----------------------------------------------------------------------------


int I2C_BUSBUSY_UserCallback(unsigned char DBGValue)
{

	I2C_SoftwareResetCmd(I2C1);
	I2C1_Master_Init();

	DEBUG_PRINT("??: I2C Bus Busy => %x\r\n", DBGValue);

	return (__RETURN_FAIL);
}
//-----------------------------------------------------------------------------


int I2C_TIMEOUT_UserCallback(unsigned char DBGValue)
{

	I2C_SoftwareResetCmd(I2C1);

	DEBUG_PRINT("??: I2C Bus Timeout => %x\r\n", DBGValue);

	return (__RETURN_FAIL);
}
//-----------------------------------------------------------------------------


// S-35390 I2C Read Bytes
int I2C_RTC_READ_NBYTE(unsigned char SAddr_Cmd,
                       unsigned char ByteCount,
                       unsigned char* ReturndValue)
{
  uint8_t tmpindex = 0;

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_RTC_READ_NBYTE_DBG);
  }
  //--------------------------

  //Step1 : Read data
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C1, SAddr_Cmd, ByteCount, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

  /* Wait until RXNE flag is set */
  tmpindex = 0;
  while(tmpindex != ByteCount)
  {
    /* Wait until RXNE flag is set */
    timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
    {
      if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_RTC_READ_NBYTE_DBG+1));
    }

    /* Read data from RXDR */
    *(ReturndValue+tmpindex) = I2C_ReceiveData(I2C1);
    tmpindex++;
  }

  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_RTC_READ_NBYTE_DBG+2));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  /* If all operations OK */
  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


// S-35390 I2C Write Bytes
int I2C_RTC_WRITE_NBYTE(unsigned char SAddr_Cmd,
                        unsigned char ByteCount,
                        unsigned char* WriteData)
{

  uint8_t tmpindex = 0;

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_RTC_WRITE_NBYTE_DBG);
  }
  //--------------------------

  //Step1 : Write data
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SAddr_Cmd, ByteCount, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_RTC_WRITE_NBYTE_DBG+1));
  }
  //--------------------------

  tmpindex = 0;
  while(tmpindex < ByteCount)
  {

    /* Write data to TXDR */
    I2C_SendData(I2C1, *(WriteData+tmpindex));
    tmpindex++;

    if(tmpindex == ByteCount)
    {
      /* Wait until TC flag is set */
      timeout = I2C_TIMEOUT;
      while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
      {
        if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_RTC_WRITE_NBYTE_DBG+3));
      }
    }
    else
    {
      /* Wait until TXIS flag is set */
      timeout = I2C_TIMEOUT;
      while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
      {
        if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_RTC_WRITE_NBYTE_DBG+2));
      }
    }
  }


  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_RTC_WRITE_NBYTE_DBG+2));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


/**
  * @brief              : Read Byte from I2Cx communication
  * @param  ChipID      : Control Byte (Slave Address)
  * @param  ReadAddr1   : Command (Address High Byte)
  * @param  ReadAddr2   : Command (Address Low Byte)
  * @retval             : *ReturndValue
  * @retval             : Status, 0 - failed, 1 - passed
  */
int I2C_EEPROM_24LC512_READ_BYTE (unsigned char ChipID,
                                  unsigned char ReadAddr1,
                                  unsigned char ReadAddr2,
                                  unsigned char *ReturndValue)
{
  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_EEPROM_24LC512_READ_BYTE_DBG);
  }
  //--------------------------

  //Step1 : Send slave address
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, ChipID, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_READ_BYTE_DBG+1));
  }
  //--------------------------

  //Step2 :
  /* Send Register address1 */
  I2C_SendData(I2C1, (uint8_t)ReadAddr1);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_READ_BYTE_DBG+2));
  }
  //--------------------------

  //Step3 : Send command
  /* Send Register address2 */
  I2C_SendData(I2C1, (uint8_t)ReadAddr2);
  /* Wait until TC flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_READ_BYTE_DBG+3));
  }
  //--------------------------

  //Step4 : Read data
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C1, ChipID, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  /* Wait until RXNE flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_READ_BYTE_DBG+4));
  }
  //--------------------------

  /* Read data from RXDR */
  *ReturndValue = I2C_ReceiveData(I2C1);

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_READ_BYTE_DBG+5));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  /* If all operations OK */
  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


/**
  * @brief              : Write Byte via I2Cx communication
  * @param  ChipID      : Control Byte (Slave Address)
  * @param  ReadAddr1   : Command (Address High Byte)
  * @param  ReadAddr2   : Command (Address Low Byte)
  * @retval             : Status, 0 - failed, 1 - passed
  */
int I2C_EEPROM_24LC512_WRITE_BYTE (unsigned char ChipID,
                                   unsigned char ReadAddr1,
                                   unsigned char ReadAddr2,
                                   unsigned char Data)
{

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_EEPROM_24LC512_WRITE_BYTE_DBG);
  }
  //--------------------------

  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, ChipID, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);
  /*** Master transmitter ***/
  /*** In the case of a write transfer, the TXIS flag is set after each byte transmission, ***/
  /*** after the 9th SCL pulse when an ACK is received. ***/
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_WRITE_BYTE_DBG+1));
  }
  //--------------------------

  /* Send Register address1 */
  I2C_SendData(I2C1, (uint8_t) ReadAddr1);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_WRITE_BYTE_DBG+2));
  }
  //--------------------------

  /* Send Register address2 */
  I2C_SendData(I2C1, (uint8_t) ReadAddr2);
  /* Wait until TCR flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_WRITE_BYTE_DBG+3));
  }
  //--------------------------

  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, ChipID, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_WRITE_BYTE_DBG+4));
  }
  //--------------------------

  /* Write data to TXDR */
  I2C_SendData(I2C1, Data);

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_EEPROM_24LC512_WRITE_BYTE_DBG+6));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


int I2C_READ_BYTE(unsigned char SlaveAddress,
                  unsigned char CommandCode,
                  unsigned char *ReturndValue)
{

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_READ_BYTE_DBG);
  }
  //--------------------------

  //Step1 : Send slave address
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_BYTE_DBG+1));
  }
  //--------------------------

  //Step2 : Send command
  /* Send Register address */
  I2C_SendData(I2C1, (uint8_t)CommandCode);
  /* Wait until TC flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_BYTE_DBG+2));
  }
  //--------------------------

  //Step3 : Read data
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  /* Wait until RXNE flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_BYTE_DBG+3));
  }
  //--------------------------

  /* Read data from RXDR */
  *ReturndValue = I2C_ReceiveData(I2C1);
  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_BYTE_DBG+4));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  /* If all operations OK */
  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


int I2C_WRITE_BYTE(unsigned char SlaveAddress,
                   unsigned char CommandCode,
                   unsigned char WriteData)
{
  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_WRITE_BYTE_DBG);
  }
  //--------------------------

  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_BYTE_DBG+1));
  }
  //--------------------------

  /* Send Register address */
  I2C_SendData(I2C1, (uint8_t)CommandCode);
  /* Wait until TCR flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_BYTE_DBG+2));
  }
  //--------------------------

  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */

  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_BYTE_DBG+3));
  }
  //--------------------------

  /* Write data to TXDR */
  I2C_SendData(I2C1, WriteData);

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_BYTE_DBG+4));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


int I2C_READ_WORD(unsigned char SlaveAddress,
                  unsigned char CommandCode,
                  unsigned int *ReturndValue)
{
  uint8_t tmpvalue = 0;

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_READ_WORD_DBG);
  }
  //--------------------------

  //Step1 : Send slave address
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_WORD_DBG+1));
  }
  //--------------------------

  //Step2 : Send command
  /* Send Register address */
  I2C_SendData(I2C1, (uint8_t)CommandCode);
  /* Wait until TC flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_WORD_DBG+2));
  }
  //--------------------------

  //Step3 : Read data
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C1, SlaveAddress, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  /* Wait until RXNE flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_WORD_DBG+3));
  }
  //--------------------------

  /* Read data from RXDR */
  tmpvalue = I2C_ReceiveData(I2C1);
  /* Wait until RXNE flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_WORD_DBG+4));
  }
  //--------------------------

  /* Read data from RXDR */
  *ReturndValue = (((unsigned int)I2C_ReceiveData(I2C1))*256) + (unsigned int)tmpvalue;

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_WORD_DBG+5));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  /* If all operations OK */
  return (__RETURN_SUCCESS);
}
//=============================================================================


int I2C_READ_NBYTE(unsigned char SlaveAddress,
                   unsigned char CommandCode,
                   unsigned char ByteCount,
                   unsigned char *ReturndValue)
{

  uint8_t tmpindex = 0;

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_READ_NBYTE_DBG);
  }
  //--------------------------

  //Step1 : Send slave address
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_NBYTE_DBG+1));
  }
  //--------------------------

  //Step2 : Send command
  /* Send Register CommandCode */
  I2C_SendData(I2C1, (uint8_t)CommandCode);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_NBYTE_DBG+2));
  }
  //--------------------------

  //Step3 : Read data
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C1, SlaveAddress, ByteCount, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

  /* Wait until RXNE flag is set */
  tmpindex = 0;
  while(tmpindex != ByteCount)
  {
    timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
    {
      if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_NBYTE_DBG+3));
    }

    /* Read data from RXDR */
    *(ReturndValue+tmpindex) = I2C_ReceiveData(I2C1);
    tmpindex++;
  }

  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_READ_NBYTE_DBG+4));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  /* If all operations OK */
  return (__RETURN_SUCCESS);
}
//-----------------------------------------------------------------------------


int I2C_WRITE_NBYTE(unsigned char SlaveAddress,
                    unsigned char CommandCode,
                    unsigned char ByteCount,
                    unsigned char *WriteData)
{

  uint8_t tmpindex = 0;

  /* Test on BUSY Flag */
  uint32_t timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((timeout--) == 0) return I2C_BUSBUSY_UserCallback((unsigned char)I2C_WRITE_NBYTE_DBG);
  }
  //--------------------------

  //Step1 : Send slave address
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)I2C_WRITE_NBYTE_DBG);
  }
  //--------------------------

  //Step2 : Send command
  /* Send Register CommandCode */
  I2C_SendData(I2C1, (uint8_t)CommandCode);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_NBYTE_DBG+1));
  }
  //--------------------------

  //Step3 : Write data
  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, ByteCount, I2C_AutoEnd_Mode, I2C_No_StartStop);

  /* Wait until RXNE flag is set */
  tmpindex = 0;
  while(tmpindex != ByteCount)
  {
    timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
    {
      if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_NBYTE_DBG+2));
    }

    /* Write data to TXDR */
    I2C_SendData(I2C1, *(WriteData+tmpindex));
    tmpindex++;
  }

  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return I2C_TIMEOUT_UserCallback((unsigned char)(I2C_WRITE_NBYTE_DBG+3));
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  return (__RETURN_SUCCESS);
}
//=============================================================================