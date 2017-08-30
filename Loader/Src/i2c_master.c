#include "stm32f30x.h"
#include "i2c_master.h"

#define I2C_TIMEOUT  0x8000

void I2C_Master_Init()
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
  I2C_InitStructure.I2C_Timing              = 0x10808DD3;

  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
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
    if((timeout--) == 0) return 0;
  }
  //--------------------------

  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  /*** Master transmitter ***/
  /*** In the case of a write transfer, the TXIS flag is set after each byte transmission, ***/
  /*** after the 9th SCL pulse when an ACK is received. ***/
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return 0;
  }
  //--------------------------

  /* Send Register address */
  I2C_SendData(I2C1, (uint8_t)CommandCode);
  /* Wait until TCR flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
  {
    if((timeout--) == 0) return 0;
  }
  //--------------------------

  /* Configure (i2c peripheral, slave address, Number_Bytes, ReloadEndMode, StartStopMode) */
  I2C_TransferHandling(I2C1, SlaveAddress, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return 0;
  }
  //--------------------------

  /* Write data to TXDR */
  I2C_SendData(I2C1, WriteData);
  /* Wait until TXIS flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((timeout--) == 0) return 0;
  }
  //--------------------------

  /* Wait until STOPF flag is set */
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((timeout--) == 0) return 0;
  }
  //--------------------------

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

  return (__RETURN_SUCCESS); // ¥¿½Tªð¦^
}
//=============================================================================