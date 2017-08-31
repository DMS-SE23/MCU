#include "project.h"
#include "i2c_slave.h"
#include "battery_info.h"
#include "main.h"
#include "vpm_control.h"
#include <stdio.h>
#include "eeprom_control.h"
#include "i2c_master.h"

/*I2C Tx index & Rx index*/
// __IO uint8_t Tx_Idx2=0, Rx_Idx2=0;
__IO uint8_t Rx_Idx2=0;
uint8_t Buffer_Rx2[2] = {0};
uint8_t Buffer_Tx2 = 0;

extern RTC_TIME VAR_RTC_TIME_S35390A;       // �o�w�q�bmain.c��          <-- �[�J
                                            // �ݭninclude "project.h" �����c�ŧi
extern ALARM_TIME VAR_ALARM_TIME;

/*initial fake VPM data*/
uint8_t wake_up_evnt_lo=0;
uint8_t wake_up_evnt_hi=0;
uint8_t wake_up_mask_lo=0;
uint8_t wake_up_mask_hi=0;
uint8_t module_ctrl_status=0;
uint8_t rtc_time_year=0;
uint8_t rtc_time_month=0;
uint8_t rtc_time_day=0;
uint8_t rtc_time_week=0;
uint8_t rtc_time_hour=0;
uint8_t rtc_time_min=0;
uint8_t rtc_time_sec=0;

typedef  void (*pFunction)(void);

void peripheral_irq_disable()
{
  //Disable UART3 interrupt
  NVIC_DisableIRQ(USART3_IRQn);
  //Disable I2C2 interrupt
  NVIC_DisableIRQ(I2C2_EV_IRQn);
  NVIC_DisableIRQ(I2C2_ER_IRQn);
  //Disable Ignition interrupt
  NVIC_DisableIRQ(EXTI1_IRQn);
  //Disable DI interrupt
  NVIC_DisableIRQ(EXTI9_5_IRQn);
  //Disable DMA interrupt
  NVIC_DisableIRQ(DMA1_Channel1_IRQn);
}

///**
//  * @brief Prepares the I2Cx slave for transmission.
//  * @retval : None.
//  */
//void I2C_Slave_BufferReadWrite()
//{
//  /* Enable Event IT needed for ADDR and STOPF events ITs */
//  I2C2->CR2 |= I2C_IT_EVT ;
//  /* Enable Error IT */
//  I2C2->CR2 |= I2C_IT_ERR;
//
//  /* I2Cx Slave Transmission using Interrupt with highest priority in the application */
//  /* Enable Buffer IT (TXE and RXNE ITs) */
//  I2C2->CR2 |= I2C_IT_BUF;
//}



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

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  I2C_DeInit(I2C2);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = DEF_VPM_I2C_SLAVE_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = 0x10B00000;

  I2C_Init(I2C2, &I2C_InitStructure);
  I2C_Cmd(I2C2, ENABLE);
  I2C_StretchClockCmd(I2C2, DISABLE);

  // Enable Transfer Adress Matched, transfer completed
  I2C_ITConfig(I2C2,I2C_IT_ADDR, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_TXIS, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_RXNE, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_NACKF, ENABLE);
  I2C_ITConfig(I2C2,I2C_IT_STOPF, ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure; // �w�q�@�Ӥ��_���c��
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ���}���_
  NVIC_Init(&NVIC_InitStructure); // ��l�Ƥ��_
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ���}���_
  NVIC_Init(&NVIC_InitStructure); // ��l�Ƥ��_

  I2C_Cmd(I2C2, ENABLE);

  DEBUG_PRINT("..: I2C2 Slave Init Complete\n\r");
}
//-----------------------------------------------------------------------------


void I2C_Slave_STOP_UserCallback()
{
  /*This callback is called when there is a stop signal issued by host (write action).*/
  /*This callback wouldn't be called when the host completes issued a read action since the host just issued the NACK at last.*/

  /*Only address or address+cmd is sent from host*/
  if (Rx_Idx2 <= 1)
    return;

  i2c_event = EVENT_NOTYET_READ;
}
//-----------------------------------------------------------------------------


/**
  * @brief  Manages the Rx transfer event.
  * @retval None
  */


void I2C_Slave_RX_UserCallback()
{
}

// �����^��User Command���D�nFunction
void I2C_Slave_TX_UserCallback()
{
}


// ------------------------------------------------------------
// �o�O��I2C Master�UCommand�U�ӮɡA��Write���ʧ@���R�O�B�z�{��
// ------------------------------------------------------------
void I2C_Slave_Command_Processing(uint8_t cmd)
{
}
//-----------------------------------------------------------------------------


uint8_t Read_I2C_Slave_Cmd(void)
{
  __IO uint8_t cmd;
  cmd = Buffer_Rx2[0];
  return (cmd);
}
//-----------------------------------------------------------------------------


// �N�n�q��OS��Event���Event Queue��
int EVENTQUEUE_INSERT_TO_QUEUE(unsigned char evt_class, unsigned char evt_id)
{
  // =======================================
  // VPM�����b�}�����A�A�~���JEvent Queue
//  if (VPM_STATE < 3000) return 1;
//  if (VPM_STATE > 4000) return 1;
  if (VAR_INTERRUPT_STATUS == 0) return 1;
  // =======================================
  // �n���Event Queue���A���i��0x00
  if (evt_class == 0x00) return 1;
  // if (evt_id == 0x00) return 1;
  // ���[�jEvent Queue�ثe���ƶq
  VAR_EVENT_SIZE++;
  if (VAR_EVENT_SIZE > __DEF_EVENT_QUEUE_SIZE) // ���F
  {
    VAR_EVENT_SIZE = __DEF_EVENT_QUEUE_SIZE;
    VAR_EVENT_OUTPUT++;
    if (VAR_EVENT_OUTPUT >= __DEF_EVENT_QUEUE_SIZE)
      VAR_EVENT_OUTPUT = 0;
  }
  VAR_EVENT_INPUT++;
  if (VAR_EVENT_INPUT >= __DEF_EVENT_QUEUE_SIZE)
    VAR_EVENT_INPUT = 0;

  VAR_EVENT_QUEUS[VAR_EVENT_INPUT][0] = evt_class;
  VAR_EVENT_QUEUS[VAR_EVENT_INPUT][1] = evt_id;

  // ��Event�bEvent Queue���A�|�_�ӻ���Interrupt
  if (VAR_EVENT_SIZE != 0)
  {
    __OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_LO;
  }

  return 0;
}
//-----------------------------------------------------------------------------


// �q Event Queue �����X�n�q�� OS �� "Event"
int EVENTQUEUE_DELETE_FROM_QUEUE()
{
  int return_code;

  if (VAR_EVENT_SIZE == 0)
    return 0; // No More Event in Queue
  // ����Event
  VAR_EVENT_SIZE--;
  VAR_EVENT_OUTPUT++;
  if (VAR_EVENT_OUTPUT >= __DEF_EVENT_QUEUE_SIZE)
    VAR_EVENT_OUTPUT = 0;

  //(evt_class*256 + evt_id)
  return_code = VAR_EVENT_QUEUS[VAR_EVENT_OUTPUT][0] * 256 +
                VAR_EVENT_QUEUS[VAR_EVENT_OUTPUT][1];

  // �짹��S��Event�F�A����Interrupt
  if (VAR_EVENT_SIZE == 0)
  {
    __OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_HI;
  }

  return (return_code);
}
//-----------------------------------------------------------------------------


// �ɦLEvent Queue�����e�A�o�ODebug��
void EVENTQUEUE_DUMP_CONTENTS()
{
  printf ("Size: %d\n\r",VAR_EVENT_SIZE);
  printf ("Input (Front): %d\n\r",VAR_EVENT_INPUT);
  printf ("Output (End): %d\n\r",VAR_EVENT_OUTPUT);
  printf ("i  : ");
  for (int i=0; i < __DEF_EVENT_QUEUE_SIZE/2; i++)
  {
    printf ("%02d ",i);
  }
  printf ("\n\rCLS: ");
  for (int i=0; i < __DEF_EVENT_QUEUE_SIZE/2; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][0]);
  }
  printf ("\n\rIDX: ");
  for (int i=0; i < __DEF_EVENT_QUEUE_SIZE/2; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][1]);
  }
  printf ("\n\r---------\n\r");
  printf ("i  : ");
  for (int i=__DEF_EVENT_QUEUE_SIZE/2; i < __DEF_EVENT_QUEUE_SIZE; i++)
  {
    printf ("%02d ",i);
  }
  printf ("\n\rCLS: ");
  for (int i=__DEF_EVENT_QUEUE_SIZE/2; i < __DEF_EVENT_QUEUE_SIZE; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][0]);
  }
  printf ("\n\rIDX: ");
  for (int i=__DEF_EVENT_QUEUE_SIZE/2; i < __DEF_EVENT_QUEUE_SIZE; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][1]);
  }
  printf ("\n\r");
}
//=============================================================================