#include "includes.h"

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
      
    /* ADC Class : 0x40 ~ 0x4F */
    case I2CCMD_GET_CURRENT_VALUE:                          //0x40
      
    /* DIO Class : 0x50 ~ 0x5F */
    case I2CCMD_GET_AMP_MUTE_STATUS:                        //0x50
      
    /* Watchdog Class : 0x70 ~ 0x7F */
    case I2CCMD_GET_WATCHDOG_STATUS:                        //0x70
    case I2CCMD_GET_WATCHDOG_COUNT_DOWN_TIMER:              //0x72
    case I2CCMD_GET_WATCHDOG_CURRENT_COUNTER:               //0x74
      
    /* Battery Control and Information Class : 0x80 ~ 0x9F */
    case I2CCMD_GET_BATTERY_PACK_ATRATE:                    //0x80
    case I2CCMD_GET_BATTERY_PACK_ATRATE_TIME_TO_EMPTY:      //0x82
    case I2CCMD_GET_BATTERY_PACK_STATE_OF_CHARGE:           //0x90
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_EMPTY:             //0x91
    case I2CCMD_GET_BATTERY_PACK_FLAGS:                     //0x92
    case I2CCMD_GET_BATTERY_PACK_TEMPERATURE:               //0x93
    case I2CCMD_GET_BATTERY_PACK_VOLTAGE:                   //0x94
    case I2CCMD_GET_BATTERY_PACK_AVERAGE_CURRENT:           //0x95
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_FULL:              //0x96
    case I2CCMD_GET_BATTERY_PACK_STATE_OF_HEALTH:           //0x97
    case I2CCMD_GET_BATTERY_PACK_REMAINING_CAPACITY:        //0x98
    case I2CCMD_GET_BATTERY_PACK_FULL_CHARGE_CAPACITY:      //0x99
    case I2CCMD_GET_BATTERY_PACK_DEVICE_NAME:               //0x9A
    case I2CCMD_GET_BATTERY_PACK_SERIAL_NUMBER:             //0x9B
    case I2CCMD_GET_BATTERY_PACK_MANUFACTURER_NAME:         //0x9C
    case I2CCMD_GET_BATTERY_PACK_CYCLE_COUNT:               //0x9D
    case I2CCMD_GET_BATTERY_PACK_FULL_SERIAL_NUMBER:        //0x9E
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
  uint8_t ui8;
  uint16_t ui16;
  uint32_t read_address;
  uint32_t checksum_tag;
  
  switch (cmd)	 //from rxbuffer index "0" to tell what i2c command is.
  {
    /* System and Power Control Class : 0x20 ~ 0x3F */
    case I2CCMD_GET_INTERRUPT_EVENT:                        //0x20
      ui16 = EVENTQUEUE_DELETE_FROM_QUEUE();
      SET_TX_BUFFER((ui16 >> 8) & 0xFF);
      INS_TX_BUFFER(ui16 & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_INTERRUPT_STATUS:                       //0x37
      SET_TX_BUFFER(VAR_INTERRUPT_STATUS & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_SET_INTERRUPT_STATUS:                       //0x38
      if (Buffer_Rx[1] == 0)
      {
        VAR_INTERRUPT_STATUS = 0;
      }
      else if (Buffer_Rx[1] == 1)
      {
        VAR_INTERRUPT_STATUS = 1;
      }
      break;
    case I2CCMD_FORCE_VPM_ENTER_POWER_OFF_MODE:             //0x39
      VAR_IMM_CHANGE_WORKING_MODE_EVENT = 1;
      break;
    case I2CCMD_GET_LAST_WAKEUP_SYSTEM_TRIGGER_SOURCE:      //0x3A
      SET_TX_BUFFER(VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF & 0xFF);
      CHK_TX_BUFFER();
      break;
      
    /* ADC Class : 0x40 ~ 0x4F */
    case I2CCMD_GET_CURRENT_VALUE:                          //0x40
      ui16 = VAR_CURRENT_SENSE_VALUE;
      SET_TX_BUFFER((ui16 >> 8) & 0xFF);
      INS_TX_BUFFER(ui16 & 0xFF);
      CHK_TX_BUFFER();
      break;
    
    /* ADC Class : 0x50 ~ 0x5F */
    case I2CCMD_GET_AMP_MUTE_STATUS:                        //0x50
      ui8 = VAR_AMP_MUTE_STATUS;
      SET_TX_BUFFER(ui8 & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_SET_AMP_MUTE_STATUS:                        //0x51
      if (Buffer_Rx[1] == 0)
      {
         __OUT_E9_GPIO_OUT_AMP_MUTE_SET_LO;
         VAR_AMP_MUTE_STATUS = 0;
      }
      else if (Buffer_Rx[1] == 1)
      {
         __OUT_E9_GPIO_OUT_AMP_MUTE_SET_HI;
         VAR_AMP_MUTE_STATUS = 1;
      }
      break;
    
    /* Watchdog Class : 0x70 ~ 0x7F */
    case I2CCMD_GET_WATCHDOG_STATUS:                        //0x70
      ui8 = VAR_WATCHDOG_STATUS;
      SET_TX_BUFFER(ui8 & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_SET_WATCHDOG_STATUS:                        //0x71
      if (Buffer_Rx[1] == 0)
      {
        VAR_WATCHDOG_STATUS = 0;
      }
      else if (Buffer_Rx[1] == 1)
      {
        VAR_WATCHDOG_COUNTER = VAR_WATCHDOG_RESET_VALUE;
        VAR_WATCHDOG_STATUS = 1;
      }
      break;
    case I2CCMD_GET_WATCHDOG_COUNT_DOWN_TIMER:              //0x72
      ui16 = VAR_WATCHDOG_RESET_VALUE;
      SET_TX_BUFFER((ui16 >> 8) & 0xFF);
      INS_TX_BUFFER(ui16 & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_SET_WATCHDOG_COUNT_DOWN_TIMER:              //0x73
      ui16 = (Buffer_Rx[1] << 8) | Buffer_Rx[2];
      ui8 = 0;
      if (ui16 != 0)
      {
        VAR_WATCHDOG_RESET_VALUE = ui16;
        ui8 = 0x50;
      }
      
      // 更新EEPROM的內容
      if (ui8 > 0)
      {
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = ui8;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx[1];
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = ui8+1;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = Buffer_Rx[2];
        VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Bytes
      }
      break;
    case I2CCMD_GET_WATCHDOG_CURRENT_COUNTER:               //0x74
      ui16 = VAR_WATCHDOG_COUNTER;
      SET_TX_BUFFER((ui16 >> 8) & 0xFF);
      INS_TX_BUFFER(ui16 & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_RESET_WATCHDOG_CURRENT_COUNTER:             //0x75
      VAR_WATCHDOG_COUNTER = VAR_WATCHDOG_RESET_VALUE;
      break;
      
    /* Battery Control and Information Class : 0x80 ~ 0x9F */
    case I2CCMD_GET_BATTERY_PACK_ATRATE:                    //0x80
      SET_TX_BUFFER((BAT_INFO_AtRate >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_AtRate);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_SET_BATTERY_PACK_ATRATE:                    //0x81
      ui16 = ((Buffer_Rx[1] << 8) | Buffer_Rx[2]);
      I2C_BatteryWrite2Byte(BAT_OFFSET_AtRate, ui16);
      break;
    case I2CCMD_GET_BATTERY_PACK_ATRATE_TIME_TO_EMPTY:      //0x82
      SET_TX_BUFFER((BAT_INFO_AtRateTimeToEmpty >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_AtRateTimeToEmpty);
      CHK_TX_BUFFER();
      break;
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
      SET_TX_BUFFER((BAT_INFO_Current >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_Current);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_TIME_TO_FULL:              //0x96
      SET_TX_BUFFER((BAT_INFO_AverageTimeToFull >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_AverageTimeToFull);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_STATE_OF_HEALTH:           //0x97
      SET_TX_BUFFER((BAT_INFO_StateOfHealth >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_StateOfHealth);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_REMAINING_CAPACITY:        //0x98
      SET_TX_BUFFER((BAT_INFO_RemainingCapacity >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_RemainingCapacity);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_FULL_CHARGE_CAPACITY:      //0x99
      SET_TX_BUFFER((BAT_INFO_FullChargeCapacity >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_FullChargeCapacity);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_DEVICE_NAME:               //0x9A
      SET_TX_BUFFER(BAT_INFO_DeviceName[0]);
      for (int i = 1 ; i < 21 ; i++)
      {
        INS_TX_BUFFER(BAT_INFO_DeviceName[i]);
      }
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_SERIAL_NUMBER:             //0x9B
      SET_TX_BUFFER((BAT_INFO_SerialNumber >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_SerialNumber);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_MANUFACTURER_NAME:         //0x9C
      SET_TX_BUFFER(BAT_INFO_ManufacturerName[0]);
      for (int i = 1 ; i < 21 ; i++)
      {
        INS_TX_BUFFER(BAT_INFO_ManufacturerName[i]);
      }
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_CYCLE_COUNT:               //0x9D
      SET_TX_BUFFER((BAT_INFO_CycleCount >> 8) & 0xFF);
      INS_TX_BUFFER(BAT_INFO_CycleCount);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_FULL_SERIAL_NUMBER:        //0x9E
      SET_TX_BUFFER(BAT_INFO_FullSerialNumber[0]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[1]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[2]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[3]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[4]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[5]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[6]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[7]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[8]);
      INS_TX_BUFFER(BAT_INFO_FullSerialNumber[9]);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_BATTERY_PACK_STATE:                     //0x9F
      SET_TX_BUFFER(VAR_BATTERY_STATE & 0xFF);
      CHK_TX_BUFFER();
      break;
      
    /* External EEPROM Access Class : 0xA0 ~ 0xAF */
    case I2CCMD_EEPROM_LOAD_DEFAULT:                        //0xA0
      EEPROM_LOAD_FACTORY_DEFAULT();
      UPDATE_VPM_VARIABLE_FROM_EEPROM();
      break;
    case I2CCMD_EEPROM_SAVE_USER_DEFAULT:                   //0xA1
      EEPROM_SAVE_USER_DEFAULT();
      break;
    case I2CCMD_EEPROM_LOAD_USER_DEFAULT:                   //0xA2
      EEPROM_LOAD_USER_DEFAULT();
      UPDATE_VPM_VARIABLE_FROM_EEPROM();
      break;
    case I2CCMD_EEPROM_RESET_TO_FACTORY_DEFAULT:            //0xA3
      EEPROM_FILL_DEFAULT_VALUE();
      UPDATE_VPM_VARIABLE_FROM_EEPROM();
      break;
    case I2CCMD_GET_SERIAL_NUMBER:                          //0xA4
      SET_TX_BUFFER(VAR_SERIAL_NUMBER[0]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[1]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[2]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[3]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[4]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[5]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[6]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[7]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[8]);
      INS_TX_BUFFER(VAR_SERIAL_NUMBER[9]);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_SET_SERIAL_NUMBER:                          //0xA5
      for(int i = 0 ; i < 10 ; i++)
      {
        VAR_SERIAL_NUMBER_BUFFER[i] = Buffer_Rx[i+1];
      }
      VAR_SERIAL_NUMBER_CHG_EVENT = 1;
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
      read_address = BootloaderAddress + FLASH_SIZE - (CHKSUM_TAG_OFFSET + CHKSUM_TAG_SIZE);
      checksum_tag= *((uint32_t*)read_address);
      ui8 = 0xFF;
      if (checksum_tag == CHK_ALL_TIME)
      {
        ui8 = 0x00;
      }
      else if (checksum_tag == CHK_ONCE)
      {
        ui8 = 0x01;
      }
      else if (checksum_tag == DO_NOT_CHK)
      {
        ui8 = 0x02;
      }
      SET_TX_BUFFER(ui8);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_GET_VPM_BOOTLOADER_VERSION:                 //0xF3
      read_address = BootloaderAddress + FLASH_SIZE - (BSL_VERSION_OFFSET + BSL_VERSION_SIZE);
      ui16 = *((uint16_t*)read_address);
      SET_TX_BUFFER((ui16 & 0xFF00) >> 8);
      INS_TX_BUFFER(ui16 & 0xFF);
      CHK_TX_BUFFER();
      break;
    case I2CCMD_ENTER_FIRMWARE_DOWNLOAD_MODE:               //0xF4
      #ifdef NDEBUG // Release Mode才存在支援
      {
        BOOT_SIG = 0xbabecafe;
        peripheral_irq_disable();
        u32 JumpAddress = *(__IO u32*) (BootloaderAddress + 4);
        pFunction Jump_To_Bootloader = (pFunction) JumpAddress;
        __set_MSP(*(__IO u32*) BootloaderAddress);
        Jump_To_Bootloader();
      }
      #endif // of NDEBUG
      break;
    case I2CCMD_WARM_START:                                 //0xF5
      peripheral_irq_disable();
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
  if (VPM_STATE < 3000) return 1;
  if (VPM_STATE > 5000) return 1;
  // VPM必須在CPU通知Driver Ready後，才能塞入Event Queue
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