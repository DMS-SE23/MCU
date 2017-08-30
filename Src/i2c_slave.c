#include "project.h"
#include "i2c_slave.h"
#include "battery_info.h"
#include "main.h"
#include "rtc_control.h"
#include "vpm_control.h"
#include <stdio.h>
#include "eeprom_control.h"
#include "i2c_master.h"
#include "odometer_control.h"
#include "pwm_control.h"

/*I2C Tx index & Rx index*/
// __IO uint8_t Tx_Idx2=0, Rx_Idx2=0;
__IO uint8_t Rx_Idx2=0;
uint8_t Buffer_Rx2[2] = {0};
uint8_t Buffer_Tx2 = 0;

extern RTC_TIME VAR_RTC_TIME_S35390A;       // 這定義在main.c中          <-- 加入
                                            // 需要include "project.h" 的結構宣告
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

// 此為回傳User Command的主要Function
void I2C_Slave_TX_UserCallback()
{
//temp0823  uint32_t CounterValue; // for RTC Hour-Minute-Second Used
  static int var_int_event = 0;
  static unsigned int var_int_read_out_complete = 0;
  uint32_t read_address;
  uint32_t bl_version;
  uint32_t checksum_tag;

  switch (Buffer_Rx2[0])
  {
    // 獲得 Notify OS 的 Event Clase, INT_EVENT_CLASS (0x00)
    case I2CCMD_INTERRUPT_EVENT_CLASS:
          if (var_int_read_out_complete == 0)
          {
//            DEBUG_PRINT("OS asking : event class \r\n");
            var_int_event = EVENTQUEUE_DELETE_FROM_QUEUE();
            var_int_read_out_complete = 1;
          }
          Buffer_Tx2 = (var_int_event>>8) & 0xFF;
          break;
    // 獲得 Notify OS 的 Event Index, INT_EVENT_INDEX (0x01)
    case I2CCMD_INTERRUPT_EVENT_INDEX:
//          DEBUG_PRINT("OS asking : event index \r\n");
          Buffer_Tx2 = var_int_event & 0xFF;
          var_int_read_out_complete = 0;
          break;
    // Get System Information (0x02)
    case I2CCMD_GET_SYSTEM_INFORMATION_HI:
          if (__IN_A1_GPIO_IN_IGNITION_UP_BAR_TEST_LO) // On
          {
            Buffer_Tx2 = 0x01;
          }
          else
          {
            Buffer_Tx2 = 0x00;
          }
          break;
    // Get System Information (0x03)
    case I2CCMD_GET_SYSTEM_INFORMATION_LO:
          Buffer_Tx2 = VAR_SYSI2C_SYS_INFO & 0xFF;
          break;
    // Get Wake-up Event Source Mask (0x04)
    case I2CCMD_WAKE_UP_MASK_HI:
          Buffer_Tx2 = VAR_WAKEUP_MASK_HI;
          break;
    // Get Wake-up Event Source Mask (0x05)
    case I2CCMD_WAKE_UP_MASK_LO:
          Buffer_Tx2 = VAR_WAKEUP_MASK_LO;
          break;
    // Get VPM Control Status (0x08)
    case I2CCMD_MODULE_CTRL_STATUS:
          Buffer_Tx2 = VAR_VPM_PWR_MODE_CTRL;
          break;
    // Get System Power Status (0x09)
    case I2CCMD_SYSTEM_POWER_STATUS:
          Buffer_Tx2 = VAR_SYSTEM_POWER_STATUS;
          break;
    // Get Interrupt Status (0x0A)
    case I2CCMD_INTERRUPT_STATUS:
          Buffer_Tx2 = VAR_INTERRUPT_STATUS;
          break;
    // Get Last Wake-up Event Source (0x0C)
    case I2CCMD_GET_LAST_WAKE_UP_EVENT_SRC:
          Buffer_Tx2 = VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF;
          break;
    // RTC and Alarm(0x10~0x19)
    case I2CCMD_RTC_TIME_YEAR:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          Buffer_Tx2 = RTC_DateStructure.RTC_Year;
          break;
        }
    case I2CCMD_RTC_TIME_MONTH:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          Buffer_Tx2 = RTC_DateStructure.RTC_Month;
          break;
        }
    case I2CCMD_RTC_TIME_DAY:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          Buffer_Tx2 = RTC_DateStructure.RTC_Date;
          break;
        }
    case I2CCMD_RTC_TIME_WEEK:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          (RTC_DateStructure.RTC_WeekDay == 7) ? (Buffer_Tx2 = 0) : (Buffer_Tx2 = RTC_DateStructure.RTC_WeekDay);
          break;
        }
    case I2CCMD_RTC_TIME_HOUR:
        {
//        CounterValue = RTC_GetCounter();
//        Buffer_Tx2 = ((CounterValue / 3600) % 24) & 0xff;
          RTC_TimeTypeDef RTC_TimeStructure;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          Buffer_Tx2 = RTC_TimeStructure.RTC_Hours;
          break;
        }
    case I2CCMD_RTC_TIME_MIN:
        {
//        CounterValue = RTC_GetCounter();
//        Buffer_Tx2 = ((CounterValue % 3600) / 60) & 0xff;
          RTC_TimeTypeDef RTC_TimeStructure;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          Buffer_Tx2 = RTC_TimeStructure.RTC_Minutes;
          break;
        }
    case I2CCMD_RTC_TIME_SEC:
        {
//        CounterValue = RTC_GetCounter();
//        Buffer_Tx2 = ((CounterValue % 3600) % 60) & 0xFF;
          RTC_TimeTypeDef RTC_TimeStructure;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          Buffer_Tx2 = RTC_TimeStructure.RTC_Seconds;
          break;
        }
    case I2CCMD_ALARM_TIME_WEEK:
          Buffer_Tx2 = VAR_ALARM_TIME.DayOfWeek;
          break;
    case I2CCMD_ALARM_TIME_HOUR:
          Buffer_Tx2 = VAR_ALARM_TIME.Hour;
          break;
    case I2CCMD_ALARM_TIME_MIN:
          Buffer_Tx2 = VAR_ALARM_TIME.Minute;
          break;
    // Watchdog (0x20~0x25)
    case I2CCMD_WDG_COUNTDOWN_TIMER_HI:
          Buffer_Tx2 = (VAR_WATCHDOG_RESET_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_WDG_COUNTDOWN_TIMER_LO:
          Buffer_Tx2 = VAR_WATCHDOG_RESET_VALUE & 0xFF;
          break;
    case I2CCMD_WDG_CURRENT_COUNTER_HI:
          Buffer_Tx2 = (VAR_WATCHDOG_COUNTER>>8) & 0xFF;
          break;
    case I2CCMD_WDG_CURRENT_COUNTER_LO:
          Buffer_Tx2 = VAR_WATCHDOG_COUNTER & 0xFF;
          break;
    case I2CCMD_WDG_STATUS:
          Buffer_Tx2 = VAR_WATCHDOG_STATUS & 0xFF;
          break;
    // EEPROM (0x30, 0x3E~0x3F)
    case I2CCMD_EEPROM_USER_DEFAULT_STATUS:
          Buffer_Tx2 = VAR_EEPROM_USER_DEFAULT_STATUS & 0xFF;
          break;
    case I2CCMD_EEPROM_MAGIC_ID_HI:
          Buffer_Tx2 = VAR_EEPROM_MAGIC_ID_HI & 0xFF;
          break;
    case I2CCMD_EEPROM_MAGIC_ID_LO:
          Buffer_Tx2 = VAR_EEPROM_MAGIC_ID_LO & 0xFF;
          break;
    // Odometer counter(0x40~0x43)
    case I2CCMD_ODOMETER_GET_CLEAR_HI:
          VAR_ODOMETER_VALUE = Odometer_GetCounter();
          Buffer_Tx2 = (VAR_ODOMETER_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_ODOMETER_GET_CLEAR_LO:
          Buffer_Tx2 = VAR_ODOMETER_VALUE & 0xFF;
          Odometer_ClearCounter();
          break;
    case I2CCMD_ODOMETER_GET_HI:
          VAR_ODOMETER_VALUE = Odometer_GetCounter();
          Buffer_Tx2 = (VAR_ODOMETER_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_ODOMETER_GET_LO:
          Buffer_Tx2 = VAR_ODOMETER_VALUE & 0xFF;
          break;
    // Rear View Status (0x46)
    case I2CCMD_REAR_VIEW_STATUS:
          Buffer_Tx2 = VAR_REAR_VIEW_STATUS & 0xFF;
          break;
    // Function Key Brightness (0x47)
    case I2CCMD_FUNCTION_KEY_BRIGHTNESS:
          Buffer_Tx2 = VAR_FUNCTION_KEY_BRIGHTNESS & 0xFF;
          break;
    // Auto Detect Reserve Gear (0x48) 
    case I2CCMD_AUTO_DET_RESERVE_GEAR_STATUS:
          Buffer_Tx2 = VAR_AUTO_DETECT_RESERVE_GEAR_STATUS;
          break;
    // GPS Antenna Status (0x50)
    case I2CCMD_GPS_ANTENNA_STATUS:
          Buffer_Tx2 = VAR_GPS_ANTENNA_STATUS;
          break;
    // Multi-protocol Transceiver Status (0x51)
    case I2CCMD_TRANSCEIVER_STATUS:
          Buffer_Tx2 = VAR_TRANSCEIVER_STATUS & 0xFF;
          break;
    // Digital Inputs Status (0x52)
    case I2CCMD_DI_STATUS:
          Buffer_Tx2 = 0xFF;
          // DIO Channel 1
          if (__IN_A4_GPIO_IN_DI_1_TEST_LO) Buffer_Tx2 &= 0xFE;         // 1111 1110
          // DIO Channel 2
          if (__IN_D2_GPIO_IN_DI_2_TEST_LO) Buffer_Tx2 &= 0xFD;         // 1111 1101
          // DIO Channel 3
          if (__IN_A6_GPIO_IN_DI_3_TEST_LO) Buffer_Tx2 &= 0xFB;         // 1111 1011
          // DIO Channel 4
          if (__IN_A8_GPIO_IN_DI_4_TEST_LO) Buffer_Tx2 &= 0xF7;         // 1111 0111
          // DIO Channel 5
          if (__IN_A11_GPIO_IN_DI_5_TEST_LO) Buffer_Tx2 &= 0xEF;        // 1110 1111
          // DIO Channel 6
          if (__IN_A12_GPIO_IN_DI_6_TEST_LO) Buffer_Tx2 &= 0xDF;        // 1101 1111
          break;
    // VPM Delay Time (0x54~0x5F)
    case I2CCMD_VPM_POWER_LOW_EVT_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_POWER_LOW_EVT_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_POWER_LOW_EVT_DLY_LO:
          Buffer_Tx2 = VAR_VPM_POWER_LOW_EVT_DLY & 0xFF;
          break;
    case I2CCMD_VPM_POWER_LOW_HARD_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_POWER_LOW_HARD_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_POWER_LOW_HARD_DLY_LO:
          Buffer_Tx2 = VAR_VPM_POWER_LOW_HARD_DLY & 0xFF;
          break;
    case I2CCMD_VPM_PWR_ON_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_PWR_ON_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_PWR_ON_DLY_LO:
          Buffer_Tx2 = VAR_VPM_PWR_ON_DLY & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_EVT_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_PWR_OFF_EVT_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_EVT_DLY_LO:
          Buffer_Tx2 = VAR_VPM_PWR_OFF_EVT_DLY & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_HARD_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_HARD_DLY_LO:
          Buffer_Tx2 = VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY & 0xFF;
          break;
    case I2CCMD_VPM_POST_PWR_CHK_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_POST_PWR_CHK_DLY_TIME>>8) & 0xFF;
          break;
    case I2CCMD_VPM_POST_PWR_CHK_DLY_LO:
          Buffer_Tx2 = VAR_VPM_POST_PWR_CHK_DLY_TIME & 0xFF;
          break;
    // Get Car Power Voltage and Unit (0x60~0x63)
    case I2CCMD_CAR_POWER_VOLT_HI:
          Buffer_Tx2 = (VAR_CURRENT_V_CB_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_CAR_POWER_VOLT_LO:
          Buffer_Tx2 = VAR_CURRENT_V_CB_VALUE & 0xFF;
          break;
    case I2CCMD_CAR_POWER_UNIT_HI:
          if (((VAR_SYSI2C_SYS_INFO>>6) & 0x03) == 0)
          {
            // 24V Car Power System
            Buffer_Tx2 = (((int)(__DEF_24V_CAR_PWR_VOLTAGE_UNIT*1000000))>>8) & 0xFF;
          }
          else
          {
            // 12V Car Power System
            Buffer_Tx2 = (((int)(__DEF_12V_CAR_PWR_VOLTAGE_UNIT*1000000))>>8) & 0xFF;
          }
          break;
    case I2CCMD_CAR_POWER_UNIT_LO:
          if (((VAR_SYSI2C_SYS_INFO>>6) & 0x03) == 0)
          {
            // 24V Car Power System
            Buffer_Tx2 = ((int)(__DEF_24V_CAR_PWR_VOLTAGE_UNIT*1000000)) & 0xFF;
          }
          else
          {
            // 12V Car Power System
            Buffer_Tx2 = ((int)(__DEF_12V_CAR_PWR_VOLTAGE_UNIT*1000000)) & 0xFF;
          }
          break;
    // Pre-boot or Post-boot Power Check Voltage (0x64~0x67)
    case I2CCMD_PRE_PWR_CHK_VOLT_HI:
          Buffer_Tx2 = (VAR_VPM_PREBOOT_VOLTAGE_CHK>>8) & 0xFF;
          break;
    case I2CCMD_PRE_PWR_CHK_VOLT_LO:
          Buffer_Tx2 = VAR_VPM_PREBOOT_VOLTAGE_CHK & 0xFF;
          break;
    case I2CCMD_POST_PWR_CHK_VOLT_HI:
          Buffer_Tx2 = (VAR_VPM_POSTBOOT_VOLTAGE_CHK>>8) & 0xFF;
          break;
    case I2CCMD_POST_PWR_CHK_VOLT_LO:
          Buffer_Tx2 = VAR_VPM_POSTBOOT_VOLTAGE_CHK & 0xFF;
          break;
    // UPS Start Charge Voltage (0x68~0x69)
    case I2CCMD_UPS_CHARGE_VOLT_HI:
          Buffer_Tx2 = (VAR_VPM_START_CHARGING_THRESHOLD>>8) & 0xFF;
          break;
    case I2CCMD_UPS_CHARGE_VOLT_LO:
          Buffer_Tx2 = VAR_VPM_START_CHARGING_THRESHOLD & 0xFF;
          break;
    // VPM Shut-down Event Source Mask (0x6A)
    case I2CCMD_VPM_SHUTDOWN_EVENT_SRC:
          Buffer_Tx2 = VAR_SHUTDOWN_MASK & 0xFF;
          break;
    // VPM Shut-down Delay (0x6B~0x6C)
    case I2CCMD_VPM_SHUTDOWN_DLY_HI:
          Buffer_Tx2 = (VAR_VPM_SHUT_DOWN_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_SHUTDOWN_DLY_LO:
          Buffer_Tx2 = VAR_VPM_SHUT_DOWN_DLY & 0xFF;
          break;
    // Shut Down Function Status (0x6D)
    case I2CCMD_VPM_SHUTDOWN_EN:
          Buffer_Tx2 = VAR_SHUTDOWN_EN;
          break;
    // Reference Voltage (0x6E~0x6F)
    case I2CCMD_REFERENCE_VOLT_HI:
          Buffer_Tx2 = (VAR_REFERENCE_VOLTAGE>>8) & 0xFF;
          break;
    case I2CCMD_REFERENCE_VOLT_LO:
          Buffer_Tx2 = VAR_REFERENCE_VOLTAGE & 0xFF;
          break;
    // Car Power Low Protection Range of 12V Minimum (0x70~0x71)
    case I2CCMD_LVP_RANGE_12V_MIN_HI:
          Buffer_Tx2 = __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_H;
          break;
    case I2CCMD_LVP_RANGE_12V_MIN_LO:
          Buffer_Tx2 = __DEF_12V_CAR_PWR_PROTECT_RANGE_MIN_L;
          break;
    // Car Power Low Protection Range of 12V Default (0x72~0x73)
    case I2CCMD_LVP_RANGE_12V_DEF_HI:
          Buffer_Tx2 = __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H;
          break;
    case I2CCMD_LVP_RANGE_12V_DEF_LO:
          Buffer_Tx2 = __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
          break;
    // Car Power Low Protection Range of 12V Maximum (0x74~0x75)
    case I2CCMD_LVP_RANGE_12V_MAX_HI:
          Buffer_Tx2 = __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_H;
          break;
    case I2CCMD_LVP_RANGE_12V_MAX_LO:
          Buffer_Tx2 = __DEF_12V_CAR_PWR_PROTECT_RANGE_MAX_L;
          break;
    // Car Power Low Protection Range of 24V Minimum (0x76~0x77)
    case I2CCMD_LVP_RANGE_24V_MIN_HI:
          Buffer_Tx2 = __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_H;
          break;
    case I2CCMD_LVP_RANGE_24V_MIN_LO:
          Buffer_Tx2 = __DEF_24V_CAR_PWR_PROTECT_RANGE_MIN_L;
          break;
    // Car Power Low Protection Range of 24V Default (0x78~0x79)
    case I2CCMD_LVP_RANGE_24V_DEF_HI:
          Buffer_Tx2 = __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H;
          break;
    case I2CCMD_LVP_RANGE_24V_DEF_LO:
          Buffer_Tx2 = __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
          break;
    // Car Power Low Protection Range of 24V Maximum (0x7A~0x7B)
    case I2CCMD_LVP_RANGE_24V_MAX_HI:
          Buffer_Tx2 = __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_H;
          break;
    case I2CCMD_LVP_RANGE_24V_MAX_LO:
          Buffer_Tx2 = __DEF_24V_CAR_PWR_PROTECT_RANGE_MAX_L;
          break;
    //------------------------------------------------------
    // Battery Information (0x81~0x8F)
    case I2CCMD_BAT_STATE_OF_CHARGE:                        //0x81, 充電%
          Buffer_Tx2 = BAT_INFO_StateOfCharge & 0xFF;
          break;
    case I2CCMD_BAT_TIM_TO_EMPTY_HI:                        //0x82
          Buffer_Tx2 = (BAT_INFO_TimeToEmpty>>8) & 0xFF;
          break;
    case I2CCMD_BAT_TIM_TO_EMPTY_LO:                        //0x83
          Buffer_Tx2 = BAT_INFO_TimeToEmpty & 0xFF;
          break;
    case I2CCMD_BAT_FLAGS_HI:                               //0x86
          Buffer_Tx2 = (BAT_INFO_Flags>>8) & 0xFF;
          break;
    case I2CCMD_BAT_FLAGS_LO:                               //0x87
          Buffer_Tx2 = BAT_INFO_Flags & 0xFF;
          break;
    case I2CCMD_BAT_TEMPERATURE_HI:                         //0x88
          Buffer_Tx2 = (BAT_INFO_Temperature>>8) & 0xFF;
          break;
    case I2CCMD_BAT_TEMPERATURE_LO:                         //0x89
          Buffer_Tx2 = BAT_INFO_Temperature & 0xFF;
          break;
    case I2CCMD_BAT_VOLTAGE_HI:                             //0x8A
          Buffer_Tx2 = (BAT_INFO_Voltage>>8) & 0xFF;
          break;
    case I2CCMD_BAT_VOLTAGE_LO:                             //0x8B
          Buffer_Tx2 = BAT_INFO_Voltage & 0xFF;
          break;
    case I2CCMD_BAT_CURRENT_HI:                             //0x8C
          Buffer_Tx2 = (BAT_INFO_Current>>8) & 0xFF;
          break;
    case I2CCMD_BAT_CURRENT_LO:                             //0x8D
          Buffer_Tx2 = BAT_INFO_Current & 0xFF;
          break;
    case I2CCMD_BAT_AVERAGE_CURRENT_HI:                     //0x8E
          Buffer_Tx2 = (BAT_INFO_AverageCurrent>>8) & 0xFF;
          break;
    case I2CCMD_BAT_AVERAGE_CURRENT_LO:                     //0x8F
          Buffer_Tx2 = BAT_INFO_AverageCurrent & 0xFF;
          break;
    //------------------------------------------------------
    // Serial Number (0x90~0x9A)
    case I2CCMD_SERIAL_NUMBER_SIZE:
          Buffer_Tx2 = __DEF_SERIAL_NUMBER_SIZE & 0xFF;
          break;
    case I2CCMD_SERIAL_NUMBER_1:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[0];
          break;
    case I2CCMD_SERIAL_NUMBER_2:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[1];
          break;
    case I2CCMD_SERIAL_NUMBER_3:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[2];
          break;
    case I2CCMD_SERIAL_NUMBER_4:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[3];
          break;
    case I2CCMD_SERIAL_NUMBER_5:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[4];
          break;
    case I2CCMD_SERIAL_NUMBER_6:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[5];
          break;
    case I2CCMD_SERIAL_NUMBER_7:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[6];
          break;
    case I2CCMD_SERIAL_NUMBER_8:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[7];
          break;
    case I2CCMD_SERIAL_NUMBER_9:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[8];
          break;
    case I2CCMD_SERIAL_NUMBER_10:
          Buffer_Tx2 = VAR_SERIAL_NUMBER[9];
          break;
    // Diagnostic and Event Log Counters (0xB0~0xCF)
    case I2CCMD_LOG_PWR_ON_HH:
          Buffer_Tx2 = (VAR_VPM_PWR_ON_IGN_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_ON_HL:
          Buffer_Tx2 = VAR_VPM_PWR_ON_IGN_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_PWR_ON_LH:
          Buffer_Tx2 = (VAR_VPM_PWR_ON_IGN_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_ON_LL:
          Buffer_Tx2 = VAR_VPM_PWR_ON_IGN_CNT & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_HH:
          Buffer_Tx2 = (VAR_VPM_PWR_LOW_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_HL:
          Buffer_Tx2 = VAR_VPM_PWR_LOW_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_LH:
          Buffer_Tx2 = (VAR_VPM_PWR_LOW_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_LL:
          Buffer_Tx2 = VAR_VPM_PWR_LOW_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_HH:
          Buffer_Tx2 = (VAR_VPM_IGN_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_HL:
          Buffer_Tx2 = VAR_VPM_IGN_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_LH:
          Buffer_Tx2 = (VAR_VPM_IGN_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_LL:
          Buffer_Tx2 = VAR_VPM_IGN_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_HH:
          Buffer_Tx2 = (VAR_VPM_BAT_OVR_TMP_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_HL:
          Buffer_Tx2 = VAR_VPM_BAT_OVR_TMP_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_LH:
          Buffer_Tx2 = (VAR_VPM_BAT_OVR_TMP_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_LL:
          Buffer_Tx2 = VAR_VPM_BAT_OVR_TMP_CNT & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_HH:
          Buffer_Tx2 = (VAR_VPM_PRE_VCHK_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_HL:
          Buffer_Tx2 = VAR_VPM_PRE_VCHK_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_LH:
          Buffer_Tx2 = (VAR_VPM_PRE_VCHK_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_LL:
          Buffer_Tx2 = VAR_VPM_PRE_VCHK_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_HH:
          Buffer_Tx2 = (VAR_VPM_POST_VCHK_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_HL:
          Buffer_Tx2 = VAR_VPM_POST_VCHK_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_LH:
          Buffer_Tx2 = (VAR_VPM_POST_VCHK_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_LL:
          Buffer_Tx2 = VAR_VPM_POST_VCHK_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_HH:
          Buffer_Tx2 = (VAR_VPM_WDOG_RST_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_HL:
          Buffer_Tx2 = VAR_VPM_WDOG_RST_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_LH:
          Buffer_Tx2 = (VAR_VPM_WDOG_RST_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_LL:
          Buffer_Tx2 = VAR_VPM_WDOG_RST_CNT & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_HH:
          Buffer_Tx2 = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_HL:
          Buffer_Tx2 = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_LH:
          Buffer_Tx2 = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_LL:
          Buffer_Tx2 = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT & 0xFF;
          break;
    // VPM Firmware Management (0xE0~0xFE)
    case I2CCMD_GET_PLATFORM_NAME_SIZE:
          Buffer_Tx2 = __DEF_PROJECT_NAME_STRING_SIZE & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID1:
          Buffer_Tx2 = __DEF_PROJECT_NAME_1 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID2:
          Buffer_Tx2 = __DEF_PROJECT_NAME_2 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID3:
          Buffer_Tx2 = __DEF_PROJECT_NAME_3 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID4:
          Buffer_Tx2 = __DEF_PROJECT_NAME_4 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID5:
          Buffer_Tx2 = __DEF_PROJECT_NAME_5 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID6:
          Buffer_Tx2 = __DEF_PROJECT_NAME_6 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID7:
          Buffer_Tx2 = __DEF_PROJECT_NAME_7 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID8:
          Buffer_Tx2 = __DEF_PROJECT_NAME_8 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID9:
          Buffer_Tx2 = __DEF_PROJECT_NAME_9 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID10:
          Buffer_Tx2 = __DEF_PROJECT_NAME_10 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID11:
          Buffer_Tx2 = __DEF_PROJECT_NAME_11 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID12:
          Buffer_Tx2 = __DEF_PROJECT_NAME_12 & 0xFF;
          break;
    case I2CCMD_GET_BOOTLOADER_VERSION_HI:
          read_address = BootloaderAddress + FLASH_SIZE - (BSL_VERSION_OFFSET + BSL_VERSION_SIZE);
          bl_version = *((uint16_t*)read_address);
          Buffer_Tx2 = (bl_version & 0xFF00) >> 8;
          break;
    case I2CCMD_GET_BOOTLOADER_VERSION_LO:
          read_address = BootloaderAddress + FLASH_SIZE - (BSL_VERSION_OFFSET + BSL_VERSION_SIZE);
          bl_version = *((uint16_t*)read_address);
          Buffer_Tx2 = bl_version & 0xFF;
          break;
    case I2CCMD_GET_FIRMWARE_VERSION_HI:
          Buffer_Tx2 = __DEF_PROJECT_MAIN_VERSION & 0xFF;
          break;
    case I2CCMD_GET_FIRMWARE_VERSION_LO:
          Buffer_Tx2 = __DEF_PROJECT_MINER_VERSION & 0xFF;
          break;
    case I2CCMD_GET_FIRMWARE_CKS_METHOD:
          read_address = BootloaderAddress + FLASH_SIZE - (CHKSUM_TAG_OFFSET + CHKSUM_TAG_SIZE);
          checksum_tag= *((uint32_t*)read_address);
          Buffer_Tx2 = 0xFF;
          if (checksum_tag == CHK_ALL_TIME)
          {
            Buffer_Tx2 = 0x00;
          }
          else if (checksum_tag == CHK_ONCE)
          {
            Buffer_Tx2 = 0x01;
          }
          else if (checksum_tag == DO_NOT_CHK)
          {
            Buffer_Tx2 = 0x02;
          }
          break;
    case I2CCMD_GET_MODE:
          Buffer_Tx2 = 0x55;
          break;
    // Get VPM State (Debug)
    case I2CCMD_GET_CURRENT_VPM_STATE_HI:
          Buffer_Tx2 = (VPM_STATE>>8) & 0xFF;
          break;
    case I2CCMD_GET_CURRENT_VPM_STATE_LO:
          Buffer_Tx2 = VPM_STATE & 0xFF;
          break;
    default:
          Buffer_Tx2 = 0xFF;
          break;
  }
}


// ------------------------------------------------------------
// 這是由I2C Master下Command下來時，做Write的動作之命令處理程序
// ------------------------------------------------------------
void I2C_Slave_Command_Processing(uint8_t cmd)
{
  int var_brightness_value = 0;
  
  switch (cmd)	 //from rxbuffer index "0" to tell what i2c command is.
  {
    // Get System Information (0x03)
    case I2CCMD_GET_SYSTEM_INFORMATION_LO:
          VAR_SYSI2C_SYS_INFO = Buffer_Rx2[1];
          VAR_SYSI2C_SYS_INFO_CHANGE = 1;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x25;
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx2[1];
          VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
          break;
    // Wake-up Event Source Mask High Byte (0x04)
    case I2CCMD_WAKE_UP_MASK_HI:
          VAR_WAKEUP_MASK_HI = Buffer_Rx2[1];
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x1B; // Mask High Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx2[1];
          VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
          break;
    // Wake-up Event Source Mask Low Byte (0x05)
    case I2CCMD_WAKE_UP_MASK_LO:
          VAR_WAKEUP_MASK_LO = Buffer_Rx2[1];
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x1C; // Mask Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx2[1];
          VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
          break;
    // VPM Control Status (0x08)
    case I2CCMD_MODULE_CTRL_STATUS:
          VAR_VPM_PWR_MODE_CTRL = Buffer_Rx2[1];
          VAR_VPM_AT_MODE = (VAR_VPM_PWR_MODE_CTRL>>7) & 0x01;
          VAR_VPM_KEEP_ALIVE = (VAR_VPM_PWR_MODE_CTRL>>6) & 0x01;
          VAR_VPM_POSTBOOT_VOLTAGE_CHK_ENABLE = (VAR_VPM_PWR_MODE_CTRL>>1) & 0x01;
          VAR_VPM_PREBOOT_VOLTAGE_CHK_ENABLE = VAR_VPM_PWR_MODE_CTRL & 0x01;
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x16;
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx2[1];
          VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
          break;
    // Set Interrupt Status
    case I2CCMD_INTERRUPT_STATUS:
          VAR_INTERRUPT_STATUS = Buffer_Rx2[1];
          if (Buffer_Rx2[1] == 0x00)
          {
            VAR_EVENT_SIZE = 0;
            VAR_EVENT_INPUT = 0;
            VAR_EVENT_OUTPUT = 0;
            __OUT_B12_GPIO_OUT_VPM_INT_SET_HI;
          }
          break;
    // VPM Work Mode Change (0x0B)
    case I2CCMD_FORCE_VPM_WORK_MODE_CHG:
          // 在此先不管是否系統目前為在S3或是S5或是S0
          // 先設定有此Event發生
          if (Buffer_Rx2[1] == 0x01) // Power Off
          {
            VAR_IMM_CHANGE_WORKING_MODE_EVENT = 1; // Goto Power Off
          }
          else
          if (Buffer_Rx2[1] == 0x04) // Reboot
          {
            // System Warm Reset
            NVIC_SystemReset();
          }
          else
          if (Buffer_Rx2[1] == 0x05) // Jump to Bootloader
          {
            #ifdef NDEBUG // Release Mode才存在支援
              BOOT_SIG = 0xbabecafe;
              peripheral_irq_disable();
              u32 JumpAddress = *(__IO u32*) (BootloaderAddress + 4);
              pFunction Jump_To_Bootloader = (pFunction) JumpAddress;
              __set_MSP(*(__IO u32*) BootloaderAddress);
              Jump_To_Bootloader();
            #endif // of NDEBUG
          }
          else
          {
            // Do nothing
          }
          break;
    // RTC and Alarm Setting (0x10~0x19)
    case I2CCMD_RTC_TIME_YEAR:
          // Valid Range is 00 ~ 99
//          if (Buffer_Rx2[1] > 99) Buffer_Rx2[1] = 99;
//          VAR_RTC_TIME_S35390A.Year = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_DateTypeDef RTC_DateStructure;

          if (Buffer_Rx2[1] > 99) Buffer_Rx2[1] = 99;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          RTC_DateStructure.RTC_Year = Buffer_Rx2[1];
          while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 1; // 有User改變RTC時間, YEAR
          break;
        }
    case I2CCMD_RTC_TIME_MONTH:
//          VAR_RTC_TIME_S35390A.Month = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_DateTypeDef RTC_DateStructure;

          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          RTC_DateStructure.RTC_Month = Buffer_Rx2[1];
          while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 2; // 有User改變RTC時間, MONTH
          break;
        }
    case I2CCMD_RTC_TIME_DAY:
//          VAR_RTC_TIME_S35390A.DayOfMonth = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_DateTypeDef RTC_DateStructure;

          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          RTC_DateStructure.RTC_Date = Buffer_Rx2[1];
          while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 3; // 有User改變RTC時間, DAY
          break;
        }
    case I2CCMD_RTC_TIME_WEEK:
//          VAR_RTC_TIME_S35390A.DayOfWeek = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_DateTypeDef RTC_DateStructure;

          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          if(Buffer_Rx2[1] == 0)
            RTC_DateStructure.RTC_WeekDay = 7;
          else
            RTC_DateStructure.RTC_WeekDay = Buffer_Rx2[1];
          while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 4; // 有User改變RTC時間, WEEK
          break;
        }
    case I2CCMD_RTC_TIME_HOUR:
//          VAR_USER_CHANGE_RTC_VALUE = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_TimeTypeDef RTC_TimeStructure;

          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          RTC_TimeStructure.RTC_Hours = Buffer_Rx2[1];
          while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 5; // 有User改變RTC時間，改變時
          break;
        }
    case I2CCMD_RTC_TIME_MIN:
//          VAR_USER_CHANGE_RTC_VALUE = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_TimeTypeDef RTC_TimeStructure;

          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          RTC_TimeStructure.RTC_Minutes = Buffer_Rx2[1];
          while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 6; // 有User改變RTC時間，改變分
          break;
        }
    case I2CCMD_RTC_TIME_SEC:
//          VAR_USER_CHANGE_RTC_VALUE = TRANSLATE_2_RTC_FORMAT(Buffer_Rx2[1]);
        {
          RTC_TimeTypeDef RTC_TimeStructure;

          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          RTC_TimeStructure.RTC_Seconds = Buffer_Rx2[1];
          while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR);

          I2C_USER_SET_RTC_TIME_EVENT = 7; // 有User改變RTC時間，改變秒
          break;
        }
    case I2CCMD_ALARM_TIME_WEEK:
          VAR_USER_CHANGE_ALARM_VALUE = Buffer_Rx2[1];
          I2C_USER_SET_ALARM_TIME_EVENT = 1;
          break;
    case I2CCMD_ALARM_TIME_HOUR:
          VAR_USER_CHANGE_ALARM_VALUE = Buffer_Rx2[1];
          I2C_USER_SET_ALARM_TIME_EVENT = 2;
          break;
    case I2CCMD_ALARM_TIME_MIN:
          VAR_USER_CHANGE_ALARM_VALUE = Buffer_Rx2[1];
          I2C_USER_SET_ALARM_TIME_EVENT = 3;
          break;
    // Watchdog (0x20~0x25)
    case I2CCMD_WDG_COUNTDOWN_TIMER_HI:
          if (I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT != 0)
          {
            I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 1;
          break;
    case I2CCMD_WDG_COUNTDOWN_TIMER_LO:
          if (I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT != 1)
          {
            I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE = VAR_USER_CHANGE_WDG_COUNTDOWN_TIMER_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_WDG_COUNTDOWN_TIMER_EVENT = 2;
          break;
    case I2CCMD_WDG_RST_CURRENT_COUNTER:
          if (Buffer_Rx2[1] == 0x00)
          {
            VAR_WATCHDOG_COUNTER = VAR_WATCHDOG_RESET_VALUE;
          }
          else
          {
            // Do nothing
          }
          break;
    case I2CCMD_WDG_STATUS:
          if (Buffer_Rx2[1] == 0x00)
          {
            VAR_WATCHDOG_STATUS = 0;
          }
          if (Buffer_Rx2[1] == 0x01)
          {
            VAR_WATCHDOG_COUNTER = VAR_WATCHDOG_RESET_VALUE;
            VAR_WATCHDOG_STATUS = 1;
          }
          else
          {
            // Do nothing
          }
          break;
    // EEPROM Default(0x30)
    case I2CCMD_EEPROM_DEFAULT:
          if (Buffer_Rx2[1] == 0x00)
          {
            I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 1;
          }
          else
          if (Buffer_Rx2[1] == 0x01)
          {
            I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 2;
          }
          else
          if (Buffer_Rx2[1] == 0x02)
          {
            I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 3;
          }
          else
          if (Buffer_Rx2[1] == 0x03)
          {
            I2C_USER_LOAD_EEPROM_DEFAULT_EVENT = 4;
          }
          else
          {
            // Do nothing
          }
          break;
    // Odometer counter (0x44)
    case I2CCMD_ODOMETER_CLEAR:
          if (Buffer_Rx2[1] == 0x00)
          {
            Odometer_ClearCounter();
          }
          break;
    // Rear View Status (0x46)
    case I2CCMD_REAR_VIEW_STATUS:
          if (Buffer_Rx2[1] == 0x00)
          {
            I2C_USER_SET_REAR_VIEW_EVENT = 1;
          }
          else
          if (Buffer_Rx2[1] == 0x01)
          {
            I2C_USER_SET_REAR_VIEW_EVENT = 2;
          }
          else
          {
            // Do nothing
          }
          break;
    // Function Key Brightness (0x47)
    case I2CCMD_FUNCTION_KEY_BRIGHTNESS:
          var_brightness_value = (int)Buffer_Rx2[1];
          if ((var_brightness_value >= 0) && (var_brightness_value <= 100))
          {
            VAR_FUNCTION_KEY_BRIGHTNESS = (int)Buffer_Rx2[1];
            SET_PWM_WIDTH(VAR_FUNCTION_KEY_PWM_PERIOD, VAR_FUNCTION_KEY_BRIGHTNESS);
          }
          break;
    // Auto Detect Reserve Gear (0x48) 
    case I2CCMD_AUTO_DET_RESERVE_GEAR_STATUS:
          VAR_AUTO_DETECT_RESERVE_GEAR_STATUS = Buffer_Rx2[1];
          // DI Mode
          if (VAR_AUTO_DETECT_RESERVE_GEAR_STATUS == 0x00)
          {
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x52;
            VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x00;
            VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Bytes 
          }
          // Auto Detect Reserve Gear Mode
          if (VAR_AUTO_DETECT_RESERVE_GEAR_STATUS == 0x01)
          {
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x52;
            VAR_EEPROM_WRITE_QUEUE_DATA[0] = 0x01;
            VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Bytes 
          }
          break;
    // Multi-protocol Transceiver Status (0x51)
    case I2CCMD_TRANSCEIVER_STATUS:
          VAR_TRANSCEIVER_STATUS = Buffer_Rx2[1];
          // RS-232 Mode
          if (VAR_TRANSCEIVER_STATUS == 0x01)
          {
            __OUT_D11_GPIO_OUT_339_MODE1_SET_LO;
            __OUT_D12_GPIO_OUT_339_MODE0_SET_HI;
            __OUT_D10_GPIO_OUT_TERM_339_SET_LO;
          }
          // RS-485 Mode without Termination
          if (VAR_TRANSCEIVER_STATUS == 0x02)
          {
            __OUT_D11_GPIO_OUT_339_MODE1_SET_HI;
            __OUT_D12_GPIO_OUT_339_MODE0_SET_LO;
            __OUT_D10_GPIO_OUT_TERM_339_SET_LO;
          }
          // RS-485 Mode with Termination
          if (VAR_TRANSCEIVER_STATUS == 0x03)
          {
            __OUT_D11_GPIO_OUT_339_MODE1_SET_HI;
            __OUT_D12_GPIO_OUT_339_MODE0_SET_LO;
            __OUT_D10_GPIO_OUT_TERM_339_SET_HI;
          }
          break;
    // VPM Delay Time (0x54~0x5F)
    case I2CCMD_VPM_POWER_LOW_EVT_DLY_HI:
          if (I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_POWER_LOW_EVT_DLY_LO:
          if (I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE = VAR_USER_CHANGE_VPM_POWER_LOW_EVT_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_POWER_LOW_EVT_DLY_EVENT = 2;
          break;
    case I2CCMD_VPM_POWER_LOW_HARD_DLY_HI:
          if (I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_POWER_LOW_HARD_DLY_LO:
          if (I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE = VAR_USER_CHANGE_VPM_POWER_LOW_HARD_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_POWER_LOW_HARD_DLY_EVENT = 2;
          break;
    case I2CCMD_VPM_PWR_ON_DLY_HI:
          if (I2C_USER_SET_VPM_PWR_ON_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_PWR_ON_DLY_LO:
          if (I2C_USER_SET_VPM_PWR_ON_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE = VAR_USER_CHANGE_VPM_PWR_ON_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_PWR_ON_DLY_EVENT = 2;
          break;
    case I2CCMD_VPM_PWR_OFF_EVT_DLY_HI:
          if (I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_PWR_OFF_EVT_DLY_LO:
          if (I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE = VAR_USER_CHANGE_VPM_PWR_OFF_EVT_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_PWR_OFF_EVT_DLY_EVENT = 2;
          break;
    case I2CCMD_VPM_PWR_OFF_HARD_DLY_HI:
          if (I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_PWR_OFF_HARD_DLY_LO:
          if (I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE = VAR_USER_CHANGE_VPM_PWR_OFF_HARD_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_PWR_OFF_HARD_DLY_EVENT = 2;
          break;
    case I2CCMD_VPM_POST_PWR_CHK_DLY_HI:
          if (I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_POST_PWR_CHK_DLY_LO:
          if (I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE = VAR_USER_CHANGE_VPM_POST_PWR_CHK_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_POST_PWR_CHK_DLY_EVENT = 2;
          break;
    case I2CCMD_PRE_PWR_CHK_VOLT_HI:
          if (I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT != 0)
          {
            I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 1;
          break;
    case I2CCMD_PRE_PWR_CHK_VOLT_LO:
          if (I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT != 1)
          {
            I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE = VAR_USER_CHANGE_PRE_PWR_CHK_VOLT_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_PRE_PWR_CHK_VOLT_EVENT = 2;
          break;
    case I2CCMD_POST_PWR_CHK_VOLT_HI:
          if (I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT != 0)
          {
            I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 1;
          break;
    case I2CCMD_POST_PWR_CHK_VOLT_LO:
          if (I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT != 1)
          {
            I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE = VAR_USER_CHANGE_POST_PWR_CHK_VOLT_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_POST_PWR_CHK_VOLT_EVENT = 2;
          break;
    case I2CCMD_UPS_CHARGE_VOLT_HI:
          if (I2C_USER_SET_UPS_CHARGE_VOLT_EVENT != 0)
          {
            I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 1;
          break;
    case I2CCMD_UPS_CHARGE_VOLT_LO:
          if (I2C_USER_SET_UPS_CHARGE_VOLT_EVENT != 1)
          {
            I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE = VAR_USER_CHANGE_UPS_CHARGE_VOLT_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_UPS_CHARGE_VOLT_EVENT = 2;
          break;
    // VPM Shut-down Event Source Mask (0x6A)
    case I2CCMD_VPM_SHUTDOWN_EVENT_SRC:
          VAR_SHUTDOWN_MASK = Buffer_Rx2[1];
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x26; // Mask Low Byte
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx2[1];
          VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
          break;
    // VPM Shut-down Delay (0x6B~0x6C)
    case I2CCMD_VPM_SHUTDOWN_DLY_HI:
          if (I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT != 0)
          {
            I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT = 1;
          break;
    case I2CCMD_VPM_SHUTDOWN_DLY_LO:
          if (I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT != 1)
          {
            I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE = VAR_USER_CHANGE_VPM_SHUT_DOWN_DLY_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_VPM_SHUT_DOWN_DLY_EVENT = 2;
          break;
    // Shut Down Function Status (0x6D)
    case I2CCMD_VPM_SHUTDOWN_EN:
          VAR_SHUTDOWN_EN = Buffer_Rx2[1];
          // 更新EEPROM的內容
          VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
          VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x2B;
          VAR_EEPROM_WRITE_QUEUE_DATA[0] = Buffer_Rx2[1];
          VAR_EEPROM_WRITE_EVENT = 1; // Write 1 Byte
          break;
    // Reference Voltage (0x6E~0x6F)
    case I2CCMD_REFERENCE_VOLT_HI:
          if (I2C_USER_SET_REFERENCE_VOLT_EVENT != 0)
          {
            I2C_USER_SET_REFERENCE_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_REFERENCE_VOLT_VALUE = Buffer_Rx2[1]<<8;
          I2C_USER_SET_REFERENCE_VOLT_EVENT = 1;
          break;
    case I2CCMD_REFERENCE_VOLT_LO:
          if (I2C_USER_SET_REFERENCE_VOLT_EVENT != 1)
          {
            I2C_USER_SET_REFERENCE_VOLT_EVENT = 0;
            return;
          }
          VAR_USER_CHANGE_REFERENCE_VOLT_VALUE = VAR_USER_CHANGE_REFERENCE_VOLT_VALUE | Buffer_Rx2[1];
          I2C_USER_SET_REFERENCE_VOLT_EVENT = 2;
          break;
    // Reset Car Power Protection Value as Default (0x7C)
    case I2CCMD_LVP_VALUE_DEF:
          if (Buffer_Rx2[1] == 0x02) // 24V system range
          {
            VAR_VPM_PREBOOT_VOLTAGE_CHK = (__DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H<<8) | __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_VPM_POSTBOOT_VOLTAGE_CHK = (__DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H<<8) | __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x59;
            VAR_EEPROM_WRITE_QUEUE_DATA[0] = __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x5A;
            VAR_EEPROM_WRITE_QUEUE_DATA[1] = __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x5B;
            VAR_EEPROM_WRITE_QUEUE_DATA[2] = __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_H;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x5C;
            VAR_EEPROM_WRITE_QUEUE_DATA[3] = __DEF_24V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_EEPROM_WRITE_EVENT = 4;
          }
          else
          if (Buffer_Rx2[1] == 0x03) // 12V system range
          {
            VAR_VPM_PREBOOT_VOLTAGE_CHK = (__DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H<<8) | __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_VPM_POSTBOOT_VOLTAGE_CHK = (__DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H<<8) | __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0x55;
            VAR_EEPROM_WRITE_QUEUE_DATA[0] = __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0x56;
            VAR_EEPROM_WRITE_QUEUE_DATA[1] = __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x57;
            VAR_EEPROM_WRITE_QUEUE_DATA[2] = __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_H;
            VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
            VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x58;
            VAR_EEPROM_WRITE_QUEUE_DATA[3] = __DEF_12V_CAR_PWR_PROTECT_RANGE_DEFAULT_L;
            VAR_EEPROM_WRITE_EVENT = 4;
          }
          break;
    // Serial Number (0x91~0x9A)
    case I2CCMD_SERIAL_NUMBER_1:
          VAR_SERIAL_NUMBER[0] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 1;
          break;
    case I2CCMD_SERIAL_NUMBER_2:
          VAR_SERIAL_NUMBER[1] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 2;
          break;
    case I2CCMD_SERIAL_NUMBER_3:
          VAR_SERIAL_NUMBER[2] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 3;
          break;
    case I2CCMD_SERIAL_NUMBER_4:
          VAR_SERIAL_NUMBER[3] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 4;
          break;
    case I2CCMD_SERIAL_NUMBER_5:
          VAR_SERIAL_NUMBER[4] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 5;
          break;
    case I2CCMD_SERIAL_NUMBER_6:
          VAR_SERIAL_NUMBER[5] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 6;
          break;
    case I2CCMD_SERIAL_NUMBER_7:
          VAR_SERIAL_NUMBER[6] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 7;
          break;
    case I2CCMD_SERIAL_NUMBER_8:
          VAR_SERIAL_NUMBER[7] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 8;
          break;
    case I2CCMD_SERIAL_NUMBER_9:
          VAR_SERIAL_NUMBER[8] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 9;
          break;
    case I2CCMD_SERIAL_NUMBER_10:
          VAR_SERIAL_NUMBER[9] = Buffer_Rx2[1];
          VAR_SERIAL_NUMBER_CHG_EVENT = 10;
          break;
    default:
          break;
  }
}
//-----------------------------------------------------------------------------


uint8_t Read_I2C_Slave_Cmd(void)
{
  __IO uint8_t cmd;
  cmd = Buffer_Rx2[0];
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
    __OUT_B12_GPIO_OUT_VPM_INT_SET_LO;
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
    __OUT_B12_GPIO_OUT_VPM_INT_SET_HI;
  }

  return (return_code);
}
//-----------------------------------------------------------------------------


// 傾印Event Queue的內容，這是Debug用
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
//-----------------------------------------------------------------------------


/*** for DEBUG only ***/
void I2C_Slave_TX_UserDEBUG()
{
  static int var_int_event = 0;
  static unsigned int var_int_read_out_complete = 0;
  uint32_t read_address;
  uint32_t bl_version;
  uint32_t checksum_tag;

  static unsigned char tmpindex = 0;
  static unsigned char delaytime = 5;                      // 5 seconds
  uint8_t DBG_Buffer_Tx2 = 0;

  if (VPM_STATE != 4000) return;

  switch (tmpindex)
  {
    // 獲得 Notify OS 的 Event Clase, INT_EVENT_CLASS (0x00)
    case I2CCMD_INTERRUPT_EVENT_CLASS:
          if (var_int_read_out_complete == 0)
          {
//            DEBUG_PRINT("OS asking : event class \r\n");
            var_int_event = EVENTQUEUE_DELETE_FROM_QUEUE();
            var_int_read_out_complete = 1;
          }
          DBG_Buffer_Tx2 = (var_int_event>>8) & 0xFF;

          break;
    // 獲得 Notify OS 的 Event Index, INT_EVENT_INDEX (0x01)
    case I2CCMD_INTERRUPT_EVENT_INDEX:
//          DEBUG_PRINT("OS asking : event index \r\n");
          DBG_Buffer_Tx2 = var_int_event & 0xFF;

          var_int_read_out_complete = 0;
          break;
    // Get System Information (0x02)
    case I2CCMD_GET_SYSTEM_INFORMATION_HI:
          DBG_Buffer_Tx2 = (VAR_SYSI2C_SYS_INFO>>8) & 0xFF;
          if (__IN_A1_GPIO_IN_IGNITION_UP_BAR_TEST_LO) // On
          {
            DBG_Buffer_Tx2 |= 0x01;
          }
          else
          {
            DBG_Buffer_Tx2 &= 0xFE;
          }
          break;
    // Get System Information (0x03)
    case I2CCMD_GET_SYSTEM_INFORMATION_LO:
          DBG_Buffer_Tx2 = VAR_SYSI2C_SYS_INFO & 0xFF;
          break;
    // Get Wake-up Event Source Mask (0x04)
    case I2CCMD_WAKE_UP_MASK_HI:
          DBG_Buffer_Tx2 = VAR_WAKEUP_MASK_HI;
          break;
    // Get Wake-up Event Source Mask (0x05)
    case I2CCMD_WAKE_UP_MASK_LO:
          DBG_Buffer_Tx2 = VAR_WAKEUP_MASK_LO;
          break;
    // Get VPM Control Status (0x08)
    case I2CCMD_MODULE_CTRL_STATUS:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_MODE_CTRL;
          break;
    // Get System Power Status (0x09)
    case I2CCMD_SYSTEM_POWER_STATUS:
          DBG_Buffer_Tx2 = VAR_SYSTEM_POWER_STATUS;
          break;
    // Get Last Wake-up Event Source (0x0C)
    case I2CCMD_GET_LAST_WAKE_UP_EVENT_SRC:
          DBG_Buffer_Tx2 = VAR_LAST_WAKEUP_EVENT_SOURCE_FROM_POWER_OFF;
          break;
    // RTC and Alarm(0x10~0x19)
    case I2CCMD_RTC_TIME_YEAR:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          DBG_Buffer_Tx2 = RTC_DateStructure.RTC_Year;
          break;
        }
    case I2CCMD_RTC_TIME_MONTH:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          DBG_Buffer_Tx2 = RTC_DateStructure.RTC_Month;
          break;
        }
    case I2CCMD_RTC_TIME_DAY:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          DBG_Buffer_Tx2 = RTC_DateStructure.RTC_Date;
          break;
        }
    case I2CCMD_RTC_TIME_WEEK:
        {
          RTC_DateTypeDef RTC_DateStructure;
          RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
          (RTC_DateStructure.RTC_WeekDay == 7) ? (DBG_Buffer_Tx2 = 0) : (DBG_Buffer_Tx2 = RTC_DateStructure.RTC_WeekDay);
          break;
        }
    case I2CCMD_RTC_TIME_HOUR:
        {
//        CounterValue = RTC_GetCounter();
//        Buffer_Tx2 = ((CounterValue / 3600) % 24) & 0xff;
          RTC_TimeTypeDef RTC_TimeStructure;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          DBG_Buffer_Tx2 = RTC_TimeStructure.RTC_Hours;
          break;
        }
    case I2CCMD_RTC_TIME_MIN:
        {
//        CounterValue = RTC_GetCounter();
//        Buffer_Tx2 = ((CounterValue % 3600) / 60) & 0xff;
          RTC_TimeTypeDef RTC_TimeStructure;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          DBG_Buffer_Tx2 = RTC_TimeStructure.RTC_Minutes;
          break;
        }
    case I2CCMD_RTC_TIME_SEC:
        {
//        CounterValue = RTC_GetCounter();
//        Buffer_Tx2 = ((CounterValue % 3600) % 60) & 0xFF;
          RTC_TimeTypeDef RTC_TimeStructure;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          DBG_Buffer_Tx2 = RTC_TimeStructure.RTC_Seconds;
          break;
        }
    case I2CCMD_ALARM_TIME_WEEK:
          DBG_Buffer_Tx2 = VAR_ALARM_TIME.DayOfWeek;
          break;
    case I2CCMD_ALARM_TIME_HOUR:
          DBG_Buffer_Tx2 = VAR_ALARM_TIME.Hour;
          break;
    case I2CCMD_ALARM_TIME_MIN:
          DBG_Buffer_Tx2 = VAR_ALARM_TIME.Minute;
          break;
    // Watchdog (0x20~0x25)
    case I2CCMD_WDG_COUNTDOWN_TIMER_HI:
          DBG_Buffer_Tx2 = (VAR_WATCHDOG_RESET_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_WDG_COUNTDOWN_TIMER_LO:
          DBG_Buffer_Tx2 = VAR_WATCHDOG_RESET_VALUE & 0xFF;
          break;
    case I2CCMD_WDG_CURRENT_COUNTER_HI:
          DBG_Buffer_Tx2 = (VAR_WATCHDOG_COUNTER>>8) & 0xFF;
          break;
    case I2CCMD_WDG_CURRENT_COUNTER_LO:
          DBG_Buffer_Tx2 = VAR_WATCHDOG_COUNTER & 0xFF;
          break;
    case I2CCMD_WDG_STATUS:
          DBG_Buffer_Tx2 = VAR_WATCHDOG_STATUS & 0xFF;
          break;
    // EEPROM (0x30, 0x3E~0x3F)
    case I2CCMD_EEPROM_USER_DEFAULT_STATUS:
          DBG_Buffer_Tx2 = VAR_EEPROM_USER_DEFAULT_STATUS & 0xFF;
          break;
    case I2CCMD_EEPROM_MAGIC_ID_HI:
          DBG_Buffer_Tx2 = VAR_EEPROM_MAGIC_ID_HI & 0xFF;
          break;
    case I2CCMD_EEPROM_MAGIC_ID_LO:
          DBG_Buffer_Tx2 = VAR_EEPROM_MAGIC_ID_LO & 0xFF;
          break;
    // Odometer counter(0x40~0x43)
    case I2CCMD_ODOMETER_GET_CLEAR_HI:
          VAR_ODOMETER_VALUE = Odometer_GetCounter();
          DBG_Buffer_Tx2 = (VAR_ODOMETER_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_ODOMETER_GET_CLEAR_LO:
          DBG_Buffer_Tx2 = VAR_ODOMETER_VALUE & 0xFF;
          Odometer_ClearCounter();
          break;
    case I2CCMD_ODOMETER_GET_HI:
          VAR_ODOMETER_VALUE = Odometer_GetCounter();
          DBG_Buffer_Tx2 = (VAR_ODOMETER_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_ODOMETER_GET_LO:
          DBG_Buffer_Tx2 = VAR_ODOMETER_VALUE & 0xFF;
          break;
    // Rear View Status (0x46)
    case I2CCMD_REAR_VIEW_STATUS:
          DBG_Buffer_Tx2 = VAR_REAR_VIEW_STATUS & 0xFF;
          break;
    // Digital Inputs Status (0x52)
    case I2CCMD_DI_STATUS:
          DBG_Buffer_Tx2 = 0xFF;
          // DIO Channel 1
          if (__IN_A4_GPIO_IN_DI_1_TEST_LO) DBG_Buffer_Tx2 &= 0xFE;         // 1111 1110
          // DIO Channel 2
          if (__IN_D2_GPIO_IN_DI_2_TEST_LO) DBG_Buffer_Tx2 &= 0xFD;         // 1111 1101
          // DIO Channel 3
          if (__IN_A6_GPIO_IN_DI_3_TEST_LO) DBG_Buffer_Tx2 &= 0xFB;         // 1111 1011
          // DIO Channel 4
          if (__IN_A8_GPIO_IN_DI_4_TEST_LO) DBG_Buffer_Tx2 &= 0xF7;         // 1111 0111
          // DIO Channel 5
          if (__IN_A11_GPIO_IN_DI_5_TEST_LO) DBG_Buffer_Tx2 &= 0xEF;        // 1110 1111
          // DIO Channel 6
          if (__IN_A12_GPIO_IN_DI_6_TEST_LO) DBG_Buffer_Tx2 &= 0xDF;        // 1101 1111
          break;
    // VPM Delay Time (0x54~0x5F)
    case I2CCMD_VPM_POWER_LOW_EVT_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_POWER_LOW_EVT_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_POWER_LOW_EVT_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_POWER_LOW_EVT_DLY & 0xFF;
          break;
    case I2CCMD_VPM_POWER_LOW_HARD_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_POWER_LOW_HARD_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_POWER_LOW_HARD_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_POWER_LOW_HARD_DLY & 0xFF;
          break;
    case I2CCMD_VPM_PWR_ON_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_ON_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_PWR_ON_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_ON_DLY & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_EVT_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_OFF_EVT_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_EVT_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_OFF_EVT_DLY & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_HARD_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_PWR_OFF_HARD_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY & 0xFF;
          break;
    case I2CCMD_VPM_POST_PWR_CHK_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_POST_PWR_CHK_DLY_TIME>>8) & 0xFF;
          break;
    case I2CCMD_VPM_POST_PWR_CHK_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_POST_PWR_CHK_DLY_TIME & 0xFF;
          break;
    // Get Car Power Voltage and Unit (0x60~0x63)
    case I2CCMD_CAR_POWER_VOLT_HI:
          DBG_Buffer_Tx2 = (VAR_CURRENT_V_CB_VALUE>>8) & 0xFF;
          break;
    case I2CCMD_CAR_POWER_VOLT_LO:
          DBG_Buffer_Tx2 = VAR_CURRENT_V_CB_VALUE & 0xFF;
          break;
    case I2CCMD_CAR_POWER_UNIT_HI:
          if (((VAR_SYSI2C_SYS_INFO>>6) & 0x0003) == 0)
          {
            // 24V Car Power System
            DBG_Buffer_Tx2 = (((int)(__DEF_24V_CAR_PWR_VOLTAGE_UNIT*1000000))>>8) & 0xFF;
          }
          else
          {
            // 12V Car Power System
            DBG_Buffer_Tx2 = (((int)(__DEF_12V_CAR_PWR_VOLTAGE_UNIT*1000000))>>8) & 0xFF;
          }
          break;
    case I2CCMD_CAR_POWER_UNIT_LO:
          if (((VAR_SYSI2C_SYS_INFO>>6) & 0x0003) == 0)
          {
            // 24V Car Power System
            DBG_Buffer_Tx2 = ((int)(__DEF_24V_CAR_PWR_VOLTAGE_UNIT*1000000)) & 0xFF;
          }
          else
          {
            // 12V Car Power System
            DBG_Buffer_Tx2 = ((int)(__DEF_12V_CAR_PWR_VOLTAGE_UNIT*1000000)) & 0xFF;
          }
          break;
    // Pre-boot or Post-boot Power Check Voltage (0x64~0x67)
    case I2CCMD_PRE_PWR_CHK_VOLT_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_PREBOOT_VOLTAGE_CHK>>8) & 0xFF;
          break;
    case I2CCMD_PRE_PWR_CHK_VOLT_LO:
          DBG_Buffer_Tx2 = VAR_VPM_PREBOOT_VOLTAGE_CHK & 0xFF;
          break;
    case I2CCMD_POST_PWR_CHK_VOLT_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_POSTBOOT_VOLTAGE_CHK>>8) & 0xFF;
          break;
    case I2CCMD_POST_PWR_CHK_VOLT_LO:
          DBG_Buffer_Tx2 = VAR_VPM_POSTBOOT_VOLTAGE_CHK & 0xFF;
          break;
    // UPS Start Charge Voltage (0x68~0x69)
    case I2CCMD_UPS_CHARGE_VOLT_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_START_CHARGING_THRESHOLD>>8) & 0xFF;
          break;
    case I2CCMD_UPS_CHARGE_VOLT_LO:
          DBG_Buffer_Tx2 = VAR_VPM_START_CHARGING_THRESHOLD & 0xFF;
          break;
    // VPM Shut-down Event Source Mask (0x6A)
    case I2CCMD_VPM_SHUTDOWN_EVENT_SRC:
          DBG_Buffer_Tx2 = VAR_SHUTDOWN_MASK & 0xFF;
          break;
    // VPM Shut-down Delay (0x6B~0x6C)
    case I2CCMD_VPM_SHUTDOWN_DLY_HI:
          DBG_Buffer_Tx2 = (VAR_VPM_SHUT_DOWN_DLY>>8) & 0xFF;
          break;
    case I2CCMD_VPM_SHUTDOWN_DLY_LO:
          DBG_Buffer_Tx2 = VAR_VPM_SHUT_DOWN_DLY & 0xFF;
          break;
    // Shut Down Function Status (0x6D)
    case I2CCMD_VPM_SHUTDOWN_EN:
          DBG_Buffer_Tx2 = VAR_SHUTDOWN_EN;
          break;
    // Reference Voltage (0x6E~0x6F)
    case I2CCMD_REFERENCE_VOLT_HI:
          DBG_Buffer_Tx2 = (VAR_REFERENCE_VOLTAGE>>8) & 0xFF;
          break;
    case I2CCMD_REFERENCE_VOLT_LO:
          DBG_Buffer_Tx2 = VAR_REFERENCE_VOLTAGE & 0xFF;
          break;
    //------------------------------------------------------
    // Battery Information (0x81~0x8F)
    case I2CCMD_BAT_STATE_OF_CHARGE:                        //0x81, 充電%
          DBG_Buffer_Tx2 = BAT_INFO_StateOfCharge & 0xFF;
          break;
    case I2CCMD_BAT_TIM_TO_EMPTY_HI:                        //0x82
          DBG_Buffer_Tx2 = (BAT_INFO_TimeToEmpty>>8) & 0xFF;
          break;
    case I2CCMD_BAT_TIM_TO_EMPTY_LO:                        //0x83
          DBG_Buffer_Tx2 = BAT_INFO_TimeToEmpty & 0xFF;
          break;
    case I2CCMD_BAT_FLAGS_HI:                               //0x86
          DBG_Buffer_Tx2 = (BAT_INFO_Flags>>8) & 0xFF;
          break;
    case I2CCMD_BAT_FLAGS_LO:                               //0x87
          DBG_Buffer_Tx2 = BAT_INFO_Flags & 0xFF;
          break;
    case I2CCMD_BAT_TEMPERATURE_HI:                         //0x88
          DBG_Buffer_Tx2 = (BAT_INFO_Temperature>>8) & 0xFF;
          break;
    case I2CCMD_BAT_TEMPERATURE_LO:                         //0x89
          DBG_Buffer_Tx2 = BAT_INFO_Temperature & 0xFF;
          break;
    case I2CCMD_BAT_VOLTAGE_HI:                             //0x8A
          DBG_Buffer_Tx2 = (BAT_INFO_Voltage>>8) & 0xFF;
          break;
    case I2CCMD_BAT_VOLTAGE_LO:                             //0x8B
          DBG_Buffer_Tx2 = BAT_INFO_Voltage & 0xFF;
          break;
    case I2CCMD_BAT_CURRENT_HI:                             //0x8C
          DBG_Buffer_Tx2 = (BAT_INFO_Current>>8) & 0xFF;
          break;
    case I2CCMD_BAT_CURRENT_LO:                             //0x8D
          DBG_Buffer_Tx2 = BAT_INFO_Current & 0xFF;
          break;
    case I2CCMD_BAT_AVERAGE_CURRENT_HI:                     //0x8E
          DBG_Buffer_Tx2 = (BAT_INFO_AverageCurrent>>8) & 0xFF;
          break;
    case I2CCMD_BAT_AVERAGE_CURRENT_LO:                     //0x8F
          DBG_Buffer_Tx2 = BAT_INFO_AverageCurrent & 0xFF;
          break;
    //------------------------------------------------------
    // Serial Number (0x90~0x9A)
    case I2CCMD_SERIAL_NUMBER_SIZE:
          DBG_Buffer_Tx2 = __DEF_SERIAL_NUMBER_SIZE & 0xFF;
          break;
    case I2CCMD_SERIAL_NUMBER_1:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[0];
          break;
    case I2CCMD_SERIAL_NUMBER_2:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[1];
          break;
    case I2CCMD_SERIAL_NUMBER_3:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[2];
          break;
    case I2CCMD_SERIAL_NUMBER_4:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[3];
          break;
    case I2CCMD_SERIAL_NUMBER_5:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[4];
          break;
    case I2CCMD_SERIAL_NUMBER_6:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[5];
          break;
    case I2CCMD_SERIAL_NUMBER_7:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[6];
          break;
    case I2CCMD_SERIAL_NUMBER_8:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[7];
          break;
    case I2CCMD_SERIAL_NUMBER_9:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[8];
          break;
    case I2CCMD_SERIAL_NUMBER_10:
          DBG_Buffer_Tx2 = VAR_SERIAL_NUMBER[9];
          break;
    // Diagnostic and Event Log Counters (0xB0~0xCF)
    case I2CCMD_LOG_PWR_ON_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_ON_IGN_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_ON_HL:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_ON_IGN_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_PWR_ON_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_ON_IGN_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_ON_LL:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_ON_IGN_CNT & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_LOW_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_HL:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_LOW_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_LOW_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POWER_LOW_LL:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_LOW_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_IGN_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_HL:
          DBG_Buffer_Tx2 = VAR_VPM_IGN_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_IGN_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PWR_OFF_LL:
          DBG_Buffer_Tx2 = VAR_VPM_IGN_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_BAT_OVR_TMP_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_HL:
          DBG_Buffer_Tx2 = VAR_VPM_BAT_OVR_TMP_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_BAT_OVR_TMP_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_BAT_OVT_LL:
          DBG_Buffer_Tx2 = VAR_VPM_BAT_OVR_TMP_CNT & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_PRE_VCHK_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_HL:
          DBG_Buffer_Tx2 = VAR_VPM_PRE_VCHK_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_PRE_VCHK_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_PRE_PWR_CHK_LL:
          DBG_Buffer_Tx2 = VAR_VPM_PRE_VCHK_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_POST_VCHK_OFF_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_HL:
          DBG_Buffer_Tx2 = VAR_VPM_POST_VCHK_OFF_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_POST_VCHK_OFF_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_POST_PWR_CHK_LL:
          DBG_Buffer_Tx2 = VAR_VPM_POST_VCHK_OFF_CNT & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_WDOG_RST_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_HL:
          DBG_Buffer_Tx2 = VAR_VPM_WDOG_RST_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_WDOG_RST_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_WDG_RST_LL:
          DBG_Buffer_Tx2 = VAR_VPM_WDOG_RST_CNT & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_HH:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC>>8) & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_HL:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_LH:
          DBG_Buffer_Tx2 = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8) & 0xFF;
          break;
    case I2CCMD_LOG_KEEP_ALIVE_LL:
          DBG_Buffer_Tx2 = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT & 0xFF;
          break;
    // VPM Firmware Management (0xE0~0xFE)
    case I2CCMD_GET_PLATFORM_NAME_SIZE:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_STRING_SIZE & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID1:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_1 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID2:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_2 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID3:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_3 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID4:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_4 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID5:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_5 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID6:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_6 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID7:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_7 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID8:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_8 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID9:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_9 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID10:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_10 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID11:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_11 & 0xFF;
          break;
    case I2CCMD_GET_PLATFORM_ID12:
          DBG_Buffer_Tx2 = __DEF_PROJECT_NAME_12 & 0xFF;
          break;
    case I2CCMD_GET_BOOTLOADER_VERSION_HI:
          read_address = BootloaderAddress + FLASH_SIZE - (BSL_VERSION_OFFSET + BSL_VERSION_SIZE);
          bl_version = *((uint16_t*)read_address);
          DBG_Buffer_Tx2 = (bl_version & 0xFF00) >> 8;
          break;
    case I2CCMD_GET_BOOTLOADER_VERSION_LO:
          read_address = BootloaderAddress + FLASH_SIZE - (BSL_VERSION_OFFSET + BSL_VERSION_SIZE);
          bl_version = *((uint16_t*)read_address);
          DBG_Buffer_Tx2 = bl_version & 0xFF;
          break;
    case I2CCMD_GET_FIRMWARE_VERSION_HI:
          DBG_Buffer_Tx2 = __DEF_PROJECT_MAIN_VERSION & 0xFF;
          break;
    case I2CCMD_GET_FIRMWARE_VERSION_LO:
          DBG_Buffer_Tx2 = __DEF_PROJECT_MINER_VERSION & 0xFF;
          break;
    case I2CCMD_GET_FIRMWARE_CKS_METHOD:
          read_address = BootloaderAddress + FLASH_SIZE - (CHKSUM_TAG_OFFSET + CHKSUM_TAG_SIZE);
          checksum_tag= *((uint32_t*)read_address);
          DBG_Buffer_Tx2 = 0xFF;
          if (checksum_tag == CHK_ALL_TIME)
          {
            DBG_Buffer_Tx2 = 0x00;
          }
          else if (checksum_tag == CHK_ONCE)
          {
            DBG_Buffer_Tx2 = 0x01;
          }
          else if (checksum_tag == DO_NOT_CHK)
          {
            DBG_Buffer_Tx2 = 0x02;
          }
          break;
    case I2CCMD_GET_MODE:
          DBG_Buffer_Tx2 = 0x55;
          break;
    // Get VPM State (Debug)
    case I2CCMD_GET_CURRENT_VPM_STATE_HI:
          DBG_Buffer_Tx2 = (VPM_STATE>>8) & 0xFF;
          break;
    case I2CCMD_GET_CURRENT_VPM_STATE_LO:
          DBG_Buffer_Tx2 = VPM_STATE & 0xFF;
          break;
    default:
          DBG_Buffer_Tx2 = 0xFF;
          break;
  }
//  if((tmpindex == 0x06)                     || \
//     (tmpindex == 0x07)                     || \
//     (tmpindex == 0x0A)                     || \
//     (tmpindex == 0x0D)                     || \
//     (tmpindex >= 0x1A && tmpindex <= 0x1F) || \
//     (tmpindex >= 0x26 && tmpindex <= 0x2F) || \
//     (tmpindex >= 0x32 && tmpindex <= 0x3D) || \
//     (tmpindex == 0x45)                     || \
//     (tmpindex >= 0x47 && tmpindex <= 0x4F) || \
//     (tmpindex == 0x50)                     || \
//     (tmpindex == 0x51)                     || \
//     (tmpindex >= 0x70 && tmpindex <= 0x7F) || \
//     (tmpindex == 0x80)                     || \
//     (tmpindex == 0x84)                     || \
//     (tmpindex == 0x85)                     || \
//     (tmpindex >= 0x9B && tmpindex <= 0x9F) || \
//     (tmpindex >= 0xA0 && tmpindex <= 0xAF) || \
//     (tmpindex >= 0xD0 && tmpindex <= 0xDF) || \
//     (tmpindex == 0xEE)                     || \
//     (tmpindex == 0xEF)                     || \
//     (tmpindex >= 0xF4 && tmpindex <= 0xF7) || \
//     (tmpindex >= 0xF9 && tmpindex <= 0xFD) || \
//     (tmpindex == 0xFF))
  if(tmpindex < 0x10 || tmpindex > 0x1A)            //only check 0x10 ~ 0x1A
  {
     tmpindex = 0x10-1;
  }
  else
  {
    DEBUG_PRINT("tmpindex = 0x%02X   ,Hex = 0x%02X   ,Dec = %03d\n\r", tmpindex, DBG_Buffer_Tx2, DBG_Buffer_Tx2);
  }
  if(delaytime-- == 0)
  {
    delaytime = 5;
    tmpindex++;
  }
}

//=============================================================================