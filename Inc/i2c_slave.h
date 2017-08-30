/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_SLAVE_H
#define __I2C_SLAVE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {NOEVENT = 0, EVENT_NOTYET_READ = 1,EVENT_READ =2} EventStatus;


#define ClockSpeed			100000
//#define SlaveAddr			0x30
#define DEF_VPM_I2C_SLAVE_ADDRESS       0xF0

/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/*Indicate a complete i2c event is received or not*/
extern EventStatus i2c_event;
/*i2c slave received command*/
extern uint8_t i2c_cmd;

extern uint8_t Buffer_Rx2[2];
extern uint8_t Buffer_Tx2;
extern __IO uint8_t Rx_Idx2;


/*Buffer data Index RX/TX*/
extern __IO uint8_t Tx_Idx2 , Rx_Idx2;


/*fake I2C Data variables*/
extern uint8_t wake_up_evnt_lo;
extern uint8_t wake_up_evnt_hi;
extern uint8_t wake_up_mask_lo;
extern uint8_t wake_up_mask_hi;
extern uint8_t module_ctrl_status;
extern uint8_t rtc_time_year;
extern uint8_t rtc_time_month;
extern uint8_t rtc_time_day;
extern uint8_t rtc_time_week;
extern uint8_t rtc_time_hour;
extern uint8_t rtc_time_min;
extern uint8_t rtc_time_sec;


/*I2C Command*/
/* System Information Class : 0x00 ~ 0x0F */
#define I2CCMD_INTERRUPT_EVENT_CLASS        0x00
#define I2CCMD_INTERRUPT_EVENT_INDEX        0x01
#define I2CCMD_GET_SYSTEM_INFORMATION_HI    0x02
#define I2CCMD_GET_SYSTEM_INFORMATION_LO    0x03
#define I2CCMD_WAKE_UP_MASK_HI              0x04
#define I2CCMD_WAKE_UP_MASK_LO	            0x05
#define I2CCMD_MODULE_CTRL_STATUS           0x08
#define I2CCMD_SYSTEM_POWER_STATUS          0x09
#define I2CCMD_INTERRUPT_STATUS             0x0A
#define I2CCMD_FORCE_VPM_WORK_MODE_CHG      0x0B
#define I2CCMD_GET_LAST_WAKE_UP_EVENT_SRC   0x0C
#define I2CCMD_GET_CURRENT_VPM_STATE_HI     0x0E
#define I2CCMD_GET_CURRENT_VPM_STATE_LO     0x0F

/* RTC and Alarm Class : 0x10 ~ 0x1F */
#define I2CCMD_RTC_TIME_YEAR                0x10
#define I2CCMD_RTC_TIME_MONTH               0x11
#define I2CCMD_RTC_TIME_DAY		    0x12
#define I2CCMD_RTC_TIME_WEEK		    0x13
#define I2CCMD_RTC_TIME_HOUR		    0x14
#define I2CCMD_RTC_TIME_MIN		    0x15
#define I2CCMD_RTC_TIME_SEC		    0x16
#define I2CCMD_ALARM_TIME_WEEK		    0x17
#define I2CCMD_ALARM_TIME_HOUR		    0x18
#define I2CCMD_ALARM_TIME_MIN		    0x19

/* Watchdog Class : 0x20 ~ 0x2F */
#define I2CCMD_WDG_COUNTDOWN_TIMER_HI       0x20
#define I2CCMD_WDG_COUNTDOWN_TIMER_LO       0x21
#define I2CCMD_WDG_CURRENT_COUNTER_HI       0x22
#define I2CCMD_WDG_CURRENT_COUNTER_LO       0x23
#define I2CCMD_WDG_RST_CURRENT_COUNTER      0x24
#define I2CCMD_WDG_STATUS                   0x25

/* External EEPROM Access Class : 0x30 ~ 0x3F */
#define I2CCMD_EEPROM_DEFAULT               0x30
#define I2CCMD_EEPROM_USER_DEFAULT_STATUS   0x31
#define I2CCMD_EEPROM_MAGIC_ID_HI           0x3E
#define I2CCMD_EEPROM_MAGIC_ID_LO           0x3F

/* Odometer Class : 0x40 ~ 0x45 */
#define I2CCMD_ODOMETER_GET_CLEAR_HI        0x40
#define I2CCMD_ODOMETER_GET_CLEAR_LO        0x41
#define I2CCMD_ODOMETER_GET_HI              0x42
#define I2CCMD_ODOMETER_GET_LO              0x43
#define I2CCMD_ODOMETER_CLEAR               0x44

/* LCD Control & Brightness Control Class : 0x46 ~ 0x4F */
#define I2CCMD_REAR_VIEW_STATUS             0x46
#define I2CCMD_FUNCTION_KEY_BRIGHTNESS      0x47
#define I2CCMD_AUTO_DET_RESERVE_GEAR_STATUS 0x48

/* System and Power Control Class : 0x50 ~ 0x6F */
#define I2CCMD_GPS_ANTENNA_STATUS           0x50
#define I2CCMD_TRANSCEIVER_STATUS           0x51
#define I2CCMD_DI_STATUS                    0x52
#define I2CCMD_VPM_POWER_LOW_EVT_DLY_HI     0x54
#define I2CCMD_VPM_POWER_LOW_EVT_DLY_LO     0x55
#define I2CCMD_VPM_POWER_LOW_HARD_DLY_HI    0x56
#define I2CCMD_VPM_POWER_LOW_HARD_DLY_LO    0x57
#define I2CCMD_VPM_PWR_ON_DLY_HI            0x58
#define I2CCMD_VPM_PWR_ON_DLY_LO            0x59
#define I2CCMD_VPM_PWR_OFF_EVT_DLY_HI       0x5A
#define I2CCMD_VPM_PWR_OFF_EVT_DLY_LO       0x5B
#define I2CCMD_VPM_PWR_OFF_HARD_DLY_HI      0x5C
#define I2CCMD_VPM_PWR_OFF_HARD_DLY_LO      0x5D
#define I2CCMD_VPM_POST_PWR_CHK_DLY_HI      0x5E
#define I2CCMD_VPM_POST_PWR_CHK_DLY_LO      0x5F

#define I2CCMD_CAR_POWER_VOLT_HI            0x60
#define I2CCMD_CAR_POWER_VOLT_LO            0x61
#define I2CCMD_CAR_POWER_UNIT_HI            0x62
#define I2CCMD_CAR_POWER_UNIT_LO            0x63
#define I2CCMD_PRE_PWR_CHK_VOLT_HI          0x64
#define I2CCMD_PRE_PWR_CHK_VOLT_LO          0x65
#define I2CCMD_POST_PWR_CHK_VOLT_HI         0x66
#define I2CCMD_POST_PWR_CHK_VOLT_LO         0x67
#define I2CCMD_UPS_CHARGE_VOLT_HI           0x68
#define I2CCMD_UPS_CHARGE_VOLT_LO           0x69
#define I2CCMD_VPM_SHUTDOWN_EVENT_SRC       0x6A
#define I2CCMD_VPM_SHUTDOWN_DLY_HI          0x6B
#define I2CCMD_VPM_SHUTDOWN_DLY_LO          0x6C
#define I2CCMD_VPM_SHUTDOWN_EN              0x6D
#define I2CCMD_REFERENCE_VOLT_HI            0x6E
#define I2CCMD_REFERENCE_VOLT_LO            0x6F

#define I2CCMD_LVP_RANGE_12V_MIN_HI         0x70
#define I2CCMD_LVP_RANGE_12V_MIN_LO         0x71
#define I2CCMD_LVP_RANGE_12V_DEF_HI         0x72
#define I2CCMD_LVP_RANGE_12V_DEF_LO         0x73
#define I2CCMD_LVP_RANGE_12V_MAX_HI         0x74
#define I2CCMD_LVP_RANGE_12V_MAX_LO         0x75
#define I2CCMD_LVP_RANGE_24V_MIN_HI         0x76
#define I2CCMD_LVP_RANGE_24V_MIN_LO         0x77
#define I2CCMD_LVP_RANGE_24V_DEF_HI         0x78
#define I2CCMD_LVP_RANGE_24V_DEF_LO         0x79
#define I2CCMD_LVP_RANGE_24V_MAX_HI         0x7A
#define I2CCMD_LVP_RANGE_24V_MAX_LO         0x7B
#define I2CCMD_LVP_VALUE_DEF                0x7C

/* Battery Control and Information Class : 0x80 ~ 0x9F */
#define I2CCMD_BAT_STATE_OF_CHARGE          0x81
#define I2CCMD_BAT_TIM_TO_EMPTY_HI          0x82
#define I2CCMD_BAT_TIM_TO_EMPTY_LO          0x83
#define I2CCMD_BAT_FLAGS_HI                 0x86
#define I2CCMD_BAT_FLAGS_LO                 0x87
#define I2CCMD_BAT_TEMPERATURE_HI           0x88
#define I2CCMD_BAT_TEMPERATURE_LO           0x89
#define I2CCMD_BAT_VOLTAGE_HI               0x8A
#define I2CCMD_BAT_VOLTAGE_LO               0x8B
#define I2CCMD_BAT_CURRENT_HI               0x8C
#define I2CCMD_BAT_CURRENT_LO               0x8D
#define I2CCMD_BAT_AVERAGE_CURRENT_HI       0x8E
#define I2CCMD_BAT_AVERAGE_CURRENT_LO       0x8F

#define I2CCMD_SERIAL_NUMBER_SIZE           0x90
#define I2CCMD_SERIAL_NUMBER_1              0x91
#define I2CCMD_SERIAL_NUMBER_2              0x92
#define I2CCMD_SERIAL_NUMBER_3              0x93
#define I2CCMD_SERIAL_NUMBER_4              0x94
#define I2CCMD_SERIAL_NUMBER_5              0x95
#define I2CCMD_SERIAL_NUMBER_6              0x96
#define I2CCMD_SERIAL_NUMBER_7              0x97
#define I2CCMD_SERIAL_NUMBER_8              0x98
#define I2CCMD_SERIAL_NUMBER_9              0x99
#define I2CCMD_SERIAL_NUMBER_10             0x9A

/* Diagnostic and Event Log Counters Class : 0xB0 ~ 0xCF */
#define I2CCMD_LOG_PWR_ON_HH                0xB0
#define I2CCMD_LOG_PWR_ON_HL                0xB1
#define I2CCMD_LOG_PWR_ON_LH                0xB2
#define I2CCMD_LOG_PWR_ON_LL                0xB3
#define I2CCMD_LOG_POWER_LOW_HH             0xB4
#define I2CCMD_LOG_POWER_LOW_HL             0xB5
#define I2CCMD_LOG_POWER_LOW_LH             0xB6
#define I2CCMD_LOG_POWER_LOW_LL             0xB7
#define I2CCMD_LOG_PWR_OFF_HH               0xB8
#define I2CCMD_LOG_PWR_OFF_HL               0xB9
#define I2CCMD_LOG_PWR_OFF_LH               0xBA
#define I2CCMD_LOG_PWR_OFF_LL               0xBB
#define I2CCMD_LOG_BAT_OVT_HH               0xBC
#define I2CCMD_LOG_BAT_OVT_HL               0xBD
#define I2CCMD_LOG_BAT_OVT_LH               0xBE
#define I2CCMD_LOG_BAT_OVT_LL               0xBF
#define I2CCMD_LOG_PRE_PWR_CHK_HH           0xC0
#define I2CCMD_LOG_PRE_PWR_CHK_HL           0xC1
#define I2CCMD_LOG_PRE_PWR_CHK_LH           0xC2
#define I2CCMD_LOG_PRE_PWR_CHK_LL           0xC3
#define I2CCMD_LOG_POST_PWR_CHK_HH          0xC4
#define I2CCMD_LOG_POST_PWR_CHK_HL          0xC5
#define I2CCMD_LOG_POST_PWR_CHK_LH          0xC6
#define I2CCMD_LOG_POST_PWR_CHK_LL          0xC7
#define I2CCMD_LOG_WDG_RST_HH               0xC8
#define I2CCMD_LOG_WDG_RST_HL               0xC9
#define I2CCMD_LOG_WDG_RST_LH               0xCA
#define I2CCMD_LOG_WDG_RST_LL               0xCB
#define I2CCMD_LOG_KEEP_ALIVE_HH            0xCC
#define I2CCMD_LOG_KEEP_ALIVE_HL            0xCD
#define I2CCMD_LOG_KEEP_ALIVE_LH            0xCE
#define I2CCMD_LOG_KEEP_ALIVE_LL            0xCF

/* VPM Firmware Management Class : 0xE0 ~ 0xFF */
#define I2CCMD_GET_PLATFORM_NAME_SIZE       0xE0
#define I2CCMD_GET_PLATFORM_ID1             0xE1
#define I2CCMD_GET_PLATFORM_ID2             0xE2
#define I2CCMD_GET_PLATFORM_ID3             0xE3
#define I2CCMD_GET_PLATFORM_ID4             0xE4
#define I2CCMD_GET_PLATFORM_ID5             0xE5
#define I2CCMD_GET_PLATFORM_ID6             0xE6
#define I2CCMD_GET_PLATFORM_ID7             0xE7
#define I2CCMD_GET_PLATFORM_ID8             0xE8
#define I2CCMD_GET_PLATFORM_ID9             0xE9
#define I2CCMD_GET_PLATFORM_ID10            0xEA
#define I2CCMD_GET_PLATFORM_ID11            0xEB
#define I2CCMD_GET_PLATFORM_ID12            0xEC
#define I2CCMD_GET_PLATFORM_ID13            0xED
#define I2CCMD_GET_BOOTLOADER_VERSION_HI    0xF0
#define I2CCMD_GET_BOOTLOADER_VERSION_LO    0xF1
#define I2CCMD_GET_FIRMWARE_VERSION_HI      0xF2
#define I2CCMD_GET_FIRMWARE_VERSION_LO      0xF3
#define I2CCMD_GET_FIRMWARE_CKS_METHOD      0xF8
#define I2CCMD_GET_MODE                     0xFE

/* Exported functions ------------------------------------------------------- */
void I2C_Slave_Init(void);
//void I2C_Slave_BufferReadWrite(void);
void I2C_Slave_TX_UserCallback();				    /*The function is called when the TX transfer is taken place.*/
void I2C_Slave_RX_UserCallback();  					/*The function is called when the RX transfer is taken place.*/
void I2C_Slave_STOP_UserCallback();					/*The function is called when the RX transfer completed with STOP signal generated by master.*/
void I2C_Slave_Command_Processing(uint8_t cmd);     /*The function is used when a completed i2c packet is received.*/
uint8_t Read_I2C_Slave_Cmd();
void EVENTQUEUE_DUMP_CONTENTS();                    // Dump Event Queue for Debug Used
void I2C_Slave_TX_UserDEBUG();                      // Check TX data for Debug used
int EVENTQUEUE_DELETE_FROM_QUEUE();
int EVENTQUEUE_INSERT_TO_QUEUE(unsigned char evt_class, unsigned char evt_id);
#endif
