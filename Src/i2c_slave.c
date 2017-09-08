#include "project.h"
#include "i2c_slave.h"
#include "main.h"
#include "vpm_control.h"
#include <stdio.h>
#include "eeprom_control.h"
#include "i2c_master.h"

/*I2C Tx index & Rx index*/
__IO uint32_t Rx_Idx=0, Tx_Idx=0, Tx_Len;

uint8_t Tx_chk;

uint8_t Buffer_Rx[MAX_I2C_RX_BUFFER] = {0};
uint8_t Buffer_Tx[MAX_I2C_TX_BUFFER] = {0};

#define INS_TX_BUFFER(x)        Buffer_Tx[Tx_Len++] = x; Tx_chk ^= x;
#define SET_TX_BUFFER(x)        Tx_Idx = Tx_Len = Tx_chk = 0; INS_TX_BUFFER(x);
#define CHK_TX_BUFFER()         INS_TX_BUFFER(Tx_chk)

typedef  void (*pFunction)(void);

void peripheral_irq_disable()
{
  //Disable UART1 interrupt
  NVIC_DisableIRQ(USART1_IRQn);
  //Disable I2C2 interrupt
  NVIC_DisableIRQ(I2C2_EV_IRQn);
  NVIC_DisableIRQ(I2C2_ER_IRQn);
  //Disable Power Button interrupt
  NVIC_DisableIRQ(EXTI0_IRQn);
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

  DEBUG_PRINT("..: I2C2 Slave Init Complete\r\n");
}
//-----------------------------------------------------------------------------
void I2C_Slave_STOP_UserCallback()
{
  /*This callback is called when there is a stop signal issued by host (write action).*/
  /*This callback wouldn't be called when the host completes issued a read action since the host just issued the NACK at last.*/

  /*Only address or address+cmd is sent from host*/
  if (Rx_Idx <= 1)
    return;
  
  uint8_t cmd = Buffer_Rx[0]; // i2c cmd
  
  switch(cmd) 
  {
    /* System and Power Control Class : 0x20 ~ 0x3F */
    case I2CCMD_GET_INTERRUPT_EVENT:                        //0x20
    case I2CCMD_GET_INTERRUPT_STATUS:                       //0x37
    case I2CCMD_GET_LAST_WAKEUP_SYSTEM_TRIGGER_SOURCE:      //0x3A
      
    /* Watchdog Class : 0x70 ~ 0x7F */
    case I2CCMD_GET_WATCHDOG_STATUS:                        //0x70
    case I2CCMD_GET_WATCHDOG_COUNT_DOWN_TIMER:              //0x72
    case I2CCMD_GET_WATCHDOG_CURRENT_COUNTER:               //0x74
      
    /* Battery Control and Information Class : 0x90 ~ 0x9F */
    case I2CCMD_GET_BATTERY_PACK_STATE_OF_CHARGE:           //0x90
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_EMPTY:             //0x91
    case I2CCMD_GET_BATTERY_PACK_FLAGS:                     //0x92
    case I2CCMD_GET_BATTERY_PACK_TEMPERATURE:               //0x93
    case I2CCMD_GET_BATTERY_PACK_VOLTAGE:                   //0x94
    case I2CCMD_GET_BATTERY_PACK_AVERAGE_CURRENT:           //0x95
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_FULL:              //0x96
    case I2CCMD_GET_BATTERY_PACK_STATE:                     //0x9F
      
    /* External EEPROM Access Class : 0xA0 ~ 0xAF */
    case I2CCMD_GET_SERIAL_NUMBER:                          //0xA4
      
    /* Diagnostic and Event Log Counters Class : 0xD0 ~ 0xDF */
    case I2CCMD_LOG_POWER_BUTTON_POWER_ON:                  //0xD0
    case I2CCMD_LOG_CAR_POWER_LOW_POWER_OFF:                //0xD1
    case I2CCMD_LOG_POWER_BUTTON_POWER_OFF:                 //0xD2
    case I2CCMD_LOG_PREBOOT_FAIL_POWER_OFF:                 //0xD4
    case I2CCMD_LOG_POSTBOOT_FAIL_POWER_OFF:                //0xD5
    case I2CCMD_LOG_WATCHDOG_RESET_SYSTEM:                  //0xD6
      
    /* VPM Firmware Management Class : 0xF0 ~ 0xFF */
    case I2CCMD_GET_VPM_FIRMWARE_VERSION:                   //0xF0
    case I2CCMD_GET_VPM_PLATFORM_NAME:                      //0xF1
    case I2CCMD_GET_FIRMWARE_CHECKSUM_VERIFY_METHOD:        //0xF2
    case I2CCMD_GET_VPM_BOOTLOADER_VERSION:                 //0xF3
    case I2CCMD_GET_VPM_FIRMWARE_MODE:                      //0xFE
      I2C_Slave_Command_Processing(cmd);
      return;
  }

  // write command will handle on main loop
  i2c_event = EVENT_NOTYET_READ;
}
//-----------------------------------------------------------------------------
// ------------------------------------------------------------
// 這是由I2C Master下Command下來時，做Write的動作之命令處理程序
// ------------------------------------------------------------
void I2C_Slave_Command_Processing(uint8_t cmd)
{
//  uint8_t ui8;
//  uint16_t ui16;
  
  switch (cmd)	 //from rxbuffer index "0" to tell what i2c command is.
  {
    /* System and Power Control Class : 0x20 ~ 0x3F */
    case I2CCMD_GET_INTERRUPT_EVENT:                        //0x20
      break;
    case I2CCMD_GET_INTERRUPT_STATUS:                       //0x37
      break;
    case I2CCMD_SET_INTERRUPT_STATUS:                       //0x38
      break;
    case I2CCMD_FORCE_VPM_ENTER_POWER_OFF_MODE:             //0x39
      break;
    case I2CCMD_GET_LAST_WAKEUP_SYSTEM_TRIGGER_SOURCE:      //0x3A
      break;
      
    /* Watchdog Class : 0x70 ~ 0x7F */
    case I2CCMD_GET_WATCHDOG_STATUS:                        //0x70
      break;
    case I2CCMD_SET_WATCHDOG_STATUS:                        //0x71
      break;
    case I2CCMD_GET_WATCHDOG_COUNT_DOWN_TIMER:              //0x72
      break;
    case I2CCMD_SET_WATCHDOG_COUNT_DOWN_TIMER:              //0x73
      break;
    case I2CCMD_GET_WATCHDOG_CURRENT_COUNTER:               //0x74
      break;
    case I2CCMD_RESET_WATCHDOG_CURRENT_COUNTER:             //0x75
      break;
      
    /* Battery Control and Information Class : 0x90 ~ 0x9F */
    case I2CCMD_GET_BATTERY_PACK_STATE_OF_CHARGE:           //0x90
      SET_TX_BUFFER((BAT_INFO_RelativeStateOfCharge >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_RelativeStateOfCharge);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_EMPTY:             //0x91
      SET_TX_BUFFER((BAT_INFO_AverageTimeToEmpty >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_AverageTimeToEmpty);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_FLAGS:                     //0x92
      SET_TX_BUFFER((BAT_INFO_BatteryStatus >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_BatteryStatus);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_TEMPERATURE:               //0x93
      SET_TX_BUFFER((BAT_INFO_Temperature >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_Temperature);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_VOLTAGE:                   //0x94
      SET_TX_BUFFER((BAT_INFO_Voltage >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_Voltage);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_AVERAGE_CURRENT:           //0x95
      SET_TX_BUFFER((BAT_INFO_AverageCurrent >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_AverageCurrent);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_FULL:              //0x96
      SET_TX_BUFFER((BAT_INFO_AverageTimeToFull >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_AverageTimeToFull);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_STATE:                     //0x9F
      SET_TX_BUFFER(VAR_BATTERY_STATE & 0xFF);
      CHK_TX_BUFFER();
      break;
      
    /* External EEPROM Access Class : 0xA0 ~ 0xAF */
    case I2CCMD_EEPROM_LOAD_DEFAULT:                        //0xA0
      break;
    case I2CCMD_EEPROM_SAVE_USER_DEFAULT:                   //0xA1
      break;
    case I2CCMD_EEPROM_LOAD_USER_DEFAULT:                   //0xA2
      break;
    case I2CCMD_EEPROM_RESET_TO_FACTORY_DEFAULT:            //0xA3
      break;
    case I2CCMD_GET_SERIAL_NUMBER:                          //0xA4
      break;
    case I2CCMD_SET_SERIAL_NUMBER:                          //0xA5
      break;
      
    /* Diagnostic and Event Log Counters Class : 0xD0 ~ 0xDF */
    case I2CCMD_LOG_POWER_BUTTON_POWER_ON:                  //0xD0
      break;
    case I2CCMD_LOG_CAR_POWER_LOW_POWER_OFF:                //0xD1
      break;
    case I2CCMD_LOG_POWER_BUTTON_POWER_OFF:                 //0xD2
      break;
    case I2CCMD_LOG_PREBOOT_FAIL_POWER_OFF:                 //0xD4
      break;
    case I2CCMD_LOG_POSTBOOT_FAIL_POWER_OFF:                //0xD5
      break;
    case I2CCMD_LOG_WATCHDOG_RESET_SYSTEM:                  //0xD6
      break;
      
    /* VPM Firmware Management Class : 0xF0 ~ 0xFF */
    case I2CCMD_GET_VPM_FIRMWARE_VERSION:                   //0xF0
      SET_TX_BUFFER(__DEF_PROJECT_MAIN_VERSION);
      INS_TX_BUFFER(__DEF_PROJECT_MINER_VERSION);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_VPM_PLATFORM_NAME:                      //0xF1
      SET_TX_BUFFER(__DEF_PROJECT_NAME_1);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_2);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_3);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_4);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_5);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_6);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_7);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_8);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_9);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_10);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_11);
      INS_TX_BUFFER(__DEF_PROJECT_NAME_12);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_FIRMWARE_CHECKSUM_VERIFY_METHOD:        //0xF2
      break;
    case I2CCMD_GET_VPM_BOOTLOADER_VERSION:                 //0xF3
      break;
    case I2CCMD_ENTER_FIRMWARE_DOWNLOAD_MODE:               //0xF4
      break;
    case I2CCMD_WARM_START:                                 //0xF5
      NVIC_SystemReset();
      break;
    case I2CCMD_GET_VPM_FIRMWARE_MODE:                      //0xFE
      SET_TX_BUFFER(0x55);
      CHK_TX_BUFFER();
      break;
      
    default:
      DEBUG_PRINT("I2C Slave unknown command %02X\r\n", cmd);
      break;
  }
}
//-----------------------------------------------------------------------------

uint8_t Read_I2C_Slave_Cmd(void)
{
  __IO uint8_t cmd;
  cmd = Buffer_Rx[0];
  return (cmd);
}
//-----------------------------------------------------------------------------

// 將要通知OS的Event塞到Event Queue中
int EVENTQUEUE_INSERT_TO_QUEUE(unsigned char evt_class, unsigned char evt_id)
{
  // =======================================
  // VPM必須在開機狀態，才能塞入Event Queue
//  if (VPM_STATE < 3000) return 1;
//  if (VPM_STATE > 4000) return 1;
  if (VAR_INTERRUPT_STATUS == 0) return 1;
  // =======================================
  // 要塞到Event Queue中，不可為0x00
  if (evt_class == 0x00) return 1;
  // if (evt_id == 0x00) return 1;
  // 先加大Event Queue目前的數量
  VAR_EVENT_SIZE++;
  if (VAR_EVENT_SIZE > __DEF_EVENT_QUEUE_SIZE) // 滿了
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

  // 有Event在Event Queue中，舉起來說有Interrupt
  if (VAR_EVENT_SIZE != 0)
  {
    __OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_HI;
  }

  return 0;
}
//-----------------------------------------------------------------------------
// 從 Event Queue 中取出要通知 OS 的 "Event"
int EVENTQUEUE_DELETE_FROM_QUEUE()
{
  int return_code;

  if (VAR_EVENT_SIZE == 0)
    return 0; // No More Event in Queue
  // 仍有Event
  VAR_EVENT_SIZE--;
  VAR_EVENT_OUTPUT++;
  if (VAR_EVENT_OUTPUT >= __DEF_EVENT_QUEUE_SIZE)
    VAR_EVENT_OUTPUT = 0;

  //(evt_class*256 + evt_id)
  return_code = VAR_EVENT_QUEUS[VAR_EVENT_OUTPUT][0] * 256 +
                VAR_EVENT_QUEUS[VAR_EVENT_OUTPUT][1];

  // 抓完後沒有Event了，取消Interrupt
  if (VAR_EVENT_SIZE == 0)
  {
    __OUT_A11_GPIO_OUT_UP_I2C_INT_BAR_SET_LO;
  }

  return (return_code);
}
//-----------------------------------------------------------------------------
// 傾印Event Queue的內容，這是Debug用
void EVENTQUEUE_DUMP_CONTENTS()
{
  printf ("Size: %d\r\n",VAR_EVENT_SIZE);
  printf ("Input (Front): %d\r\n",VAR_EVENT_INPUT);
  printf ("Output (End): %d\r\n",VAR_EVENT_OUTPUT);
  printf ("i  : ");
  for (int i=0; i < __DEF_EVENT_QUEUE_SIZE/2; i++)
  {
    printf ("%02d ",i);
  }
  printf ("\r\nCLS: ");
  for (int i=0; i < __DEF_EVENT_QUEUE_SIZE/2; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][0]);
  }
  printf ("\r\nIDX: ");
  for (int i=0; i < __DEF_EVENT_QUEUE_SIZE/2; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][1]);
  }
  printf ("\r\n---------\r\n");
  printf ("i  : ");
  for (int i=__DEF_EVENT_QUEUE_SIZE/2; i < __DEF_EVENT_QUEUE_SIZE; i++)
  {
    printf ("%02d ",i);
  }
  printf ("\r\nCLS: ");
  for (int i=__DEF_EVENT_QUEUE_SIZE/2; i < __DEF_EVENT_QUEUE_SIZE; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][0]);
  }
  printf ("\r\nIDX: ");
  for (int i=__DEF_EVENT_QUEUE_SIZE/2; i < __DEF_EVENT_QUEUE_SIZE; i++)
  {
    printf ("%02X ",VAR_EVENT_QUEUS[i][1]);
  }
  printf ("\r\n");
}