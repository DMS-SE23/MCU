/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_SLAVE_H
#define __I2C_SLAVE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {NOEVENT = 0, EVENT_NOTYET_READ = 1,EVENT_READ =2} EventStatus;


#define ClockSpeed			100000
//#define SlaveAddr			0x30
#define DEF_VPM_I2C_SLAVE_ADDRESS       0xD0

/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/*Indicate a complete i2c event is received or not*/
extern EventStatus i2c_event;
/*i2c slave received command*/
extern uint8_t i2c_cmd;

#define MAX_I2C_RX_BUFFER           256
#define MAX_I2C_TX_BUFFER           256

/*Buffer data Index RX/TX*/
extern __IO uint32_t Rx_Idx, Tx_Idx, Tx_Len;

extern uint8_t Buffer_Rx[MAX_I2C_RX_BUFFER];
extern uint8_t Buffer_Tx[MAX_I2C_TX_BUFFER];

/*I2C Command*/
/* System and Power Control Class : 0x20 ~ 0x3F */
#define I2CCMD_GET_INTERRUPT_EVENT                          0x20
#define I2CCMD_GET_INTERRUPT_STATUS                         0x37
#define I2CCMD_SET_INTERRUPT_STATUS                         0x38
#define I2CCMD_FORCE_VPM_ENTER_POWER_OFF_MODE               0x39
#define I2CCMD_GET_LAST_WAKEUP_SYSTEM_TRIGGER_SOURCE        0x3A

/* ADC Class : 0x40 ~ 0x4F */
#define I2CCMD_GET_CURRENT_VALUE                            0x40

/* DIO Class : 0x50 ~ 0x5F */
#define I2CCMD_GET_AMP_MUTE_STATUS                          0x50
#define I2CCMD_SET_AMP_MUTE_STATUS                          0x51
#define I2CCMD_GET_AMP_12V_EN_STATUS                        0x52
#define I2CCMD_SET_AMP_12V_EN_STATUS                        0x53
#define I2CCMD_GET_AMP_DISABLE_STATUS                       0x54
#define I2CCMD_SET_AMP_DISABLE_STATUS                       0x55

/* Watchdog Class : 0x70 ~ 0x7F */
#define I2CCMD_GET_WATCHDOG_STATUS                          0x70
#define I2CCMD_SET_WATCHDOG_STATUS                          0x71
#define I2CCMD_GET_WATCHDOG_COUNT_DOWN_TIMER                0x72
#define I2CCMD_SET_WATCHDOG_COUNT_DOWN_TIMER                0x73
#define I2CCMD_GET_WATCHDOG_CURRENT_COUNTER                 0x74
#define I2CCMD_RESET_WATCHDOG_CURRENT_COUNTER               0x75

/* Battery Control and Information Class : 0x80 ~ 0x9F */
#define I2CCMD_GET_BATTERY_PACK_ATRATE                      0x80
#define I2CCMD_SET_BATTERY_PACK_ATRATE                      0x81
#define I2CCMD_GET_BATTERY_PACK_ATRATE_TIME_TO_EMPTY        0x82
#define I2CCMD_GET_BATTERY_PACK_STATE_OF_CHARGE             0x90
#define I2CCMD_GET_BATTERY_PACK_TIME_TO_EMPTY               0x91
#define I2CCMD_GET_BATTERY_PACK_FLAGS                       0x92
#define I2CCMD_GET_BATTERY_PACK_TEMPERATURE                 0x93
#define I2CCMD_GET_BATTERY_PACK_VOLTAGE                     0x94
#define I2CCMD_GET_BATTERY_PACK_AVERAGE_CURRENT             0x95
#define I2CCMD_GET_BATTERY_PACK_TIME_TO_FULL                0x96
#define I2CCMD_GET_BATTERY_PACK_STATE_OF_HEALTH             0x97
#define I2CCMD_GET_BATTERY_PACK_REMAINING_CAPACITY          0x98
#define I2CCMD_GET_BATTERY_PACK_FULL_CHARGE_CAPACITY        0x99
#define I2CCMD_GET_BATTERY_PACK_DEVICE_NAME                 0x9A
#define I2CCMD_GET_BATTERY_PACK_SERIAL_NUMBER               0x9B
#define I2CCMD_GET_BATTERY_PACK_MANUFACTURER_NAME           0x9C
#define I2CCMD_GET_BATTERY_PACK_CYCLE_COUNT                 0x9D
#define I2CCMD_GET_BATTERY_PACK_FULL_SERIAL_NUMBER          0x9E
#define I2CCMD_GET_BATTERY_PACK_STATE                       0x9F

/* External EEPROM Access Class : 0xA0 ~ 0xAF */
#define I2CCMD_EEPROM_LOAD_DEFAULT                          0xA0
#define I2CCMD_EEPROM_SAVE_USER_DEFAULT                     0xA1
#define I2CCMD_EEPROM_LOAD_USER_DEFAULT                     0xA2
#define I2CCMD_EEPROM_RESET_TO_FACTORY_DEFAULT              0xA3
#define I2CCMD_GET_SERIAL_NUMBER                            0xA4
#define I2CCMD_SET_SERIAL_NUMBER                            0xA5

/* Diagnostic and Event Log Counters Class : 0xD0 ~ 0xDF */
#define I2CCMD_LOG_POWER_BUTTON_POWER_ON                    0xD0
#define I2CCMD_LOG_CAR_POWER_LOW_POWER_OFF                  0xD1
#define I2CCMD_LOG_POWER_BUTTON_POWER_OFF                   0xD2
#define I2CCMD_LOG_PREBOOT_FAIL_POWER_OFF                   0xD4
#define I2CCMD_LOG_POSTBOOT_FAIL_POWER_OFF                  0xD5
#define I2CCMD_LOG_WATCHDOG_RESET_SYSTEM                    0xD6

/* VPM Firmware Management Class : 0xF0 ~ 0xFF */
#define I2CCMD_GET_VPM_FIRMWARE_VERSION                     0xF0
#define I2CCMD_GET_VPM_PLATFORM_NAME                        0xF1
#define I2CCMD_GET_FIRMWARE_CHECKSUM_VERIFY_METHOD          0xF2
#define I2CCMD_GET_VPM_BOOTLOADER_VERSION                   0xF3
#define I2CCMD_ENTER_FIRMWARE_DOWNLOAD_MODE                 0xF4
#define I2CCMD_WARM_START                                   0xF5
#define I2CCMD_GET_VPM_FIRMWARE_MODE                        0xFE

/* Exported functions ------------------------------------------------------- */
void I2C_Slave_Init(void);
//void I2C_Slave_BufferReadWrite(void);
void I2C_Slave_STOP_UserCallback();					/*The function is called when the RX transfer completed with STOP signal generated by master.*/
void I2C_Slave_Command_Processing(uint8_t cmd);     /*The function is used when a completed i2c packet is received.*/
uint8_t Read_I2C_Slave_Cmd();
void EVENTQUEUE_DUMP_CONTENTS();                    // Dump Event Queue for Debug Used
void I2C_Slave_TX_UserDEBUG();                      // Check TX data for Debug used
int EVENTQUEUE_DELETE_FROM_QUEUE();
int EVENTQUEUE_INSERT_TO_QUEUE(unsigned char evt_class, unsigned char evt_id);
#endif
