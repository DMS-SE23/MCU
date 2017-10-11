#include "includes.h"

/*I2C Tx index & Rx index*/
__IO uint8_t Tx_Idx2=0, Rx_Idx2=0;
uint8_t Buffer_Rx2[32] = {0};
uint8_t Buffer_Tx2[32] = {0};

/**
* @brief  Initializes peripherals: I2C2 channels .
  * @param  None
  * @retval None
  */
void I2C_Slave_Init()
{

  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef I2C_InitStructure;

  RCC_I2CCLKConfig(RCC_I2C2CLK_SYSCLK);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  I2C_DeInit(I2C2);
  I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter        = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter       = 0x00;
  I2C_InitStructure.I2C_OwnAddress1         = DEF_VPM_I2C_SLAVE_ADDRESS;
  I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing              = 0x10B00000;

  I2C_Init(I2C2, &I2C_InitStructure);
  I2C_Cmd(I2C2, ENABLE);
  I2C_StretchClockCmd(I2C2, DISABLE);

  // Enable Transfer Adress Matched, transfer completed
  I2C_ITConfig(I2C2,I2C_IT_ADDR, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_TXIS, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_RXNE, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_NACKF, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_STOPF, ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure; // 定義一個中斷結構體
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 打開中斷
  NVIC_Init(&NVIC_InitStructure); // 初始化中斷
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 打開中斷
  NVIC_Init(&NVIC_InitStructure); // 初始化中斷

  I2C_Cmd(I2C2, ENABLE);
}
//-----------------------------------------------------------------------------


void I2C_Slave_STOP_UserCallback()
{
  /*This callback is called when there is a stop signal issued by host (write action).*/
  /*This callback wouldn't be called when the host completes issued a read action since the host just issued the NACK at last.*/

  /*Only address or address+cmd is sent from host*/
//  if (Rx_Idx2 <= 1)
//    return;

  i2c_event = EVENT_NOTYET_READ;
}
//-----------------------------------------------------------------------------


uint8_t Read_I2C_Slave_Cmd(void)
{
  __IO uint8_t cmd;
  cmd = Buffer_Rx2[0];
  return (cmd);
}

//=============================================================================