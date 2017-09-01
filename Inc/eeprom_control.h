#ifndef __EEPROM_CONTROL_H
#define __EEPROM_CONTROL_H

//EEPROM FACTORY DEFAULT VALUE
#define MAGIC_WD_HI			0x43		//offset : 0x00
#define MAGIC_WD_LO			0x34		//offset : 0x01
#define EEPROM_VERSION		        0x00		//offset : 0x02
#define PLATFORM_ID			0x1F		//offset : 0x03 //DMS-SE23

// �^�_��w�]��
// ���|�M�������Ǹ��PWWAN MAC Address
void EEPROM_LOAD_FACTORY_DEFAULT(void);
// Save User Default to EEPROM
void EEPROM_SAVE_USER_DEFAULT(void);
// Load User Default to EEPROM
int EEPROM_LOAD_USER_DEFAULT(void);
// �^�_��u�t�w�]��
void EEPROM_FILL_DEFAULT_VALUE(void);
// Update Watchdog Timer
void WRITE_EEPROM_WATCHDOG_TIMER_VALUE(int value);
// ���s�bEEPROM����Serial Number
void EEPROM_UPDATE_SERIAL_NUMBER(int change_byte);
// ��sEEPROM��Task�ǦC
void TASK_EEPROM_UPDATE_BYTES(void);
// ��EEPROM��sVPM���ѼƳ]�w
void UPDATE_VPM_VARIABLE_FROM_EEPROM(void);
// ��sLOG���ѼƳ]�w
void UPDATE_LOG_VARIABLE(void);
// �M���Ҧ�EEPROM��Ƭ�0xFF
void EEPROM_ERASE_VALUE(void);
// Dump�Ӧ۩�EEPROM���ѼƸ��
void DUMP_EEPROM_PARAMETER_VALUES(void);
// Dump VPM Related Counter, �p�ƾ����
void DUMP_CURRENT_COUNTERS(void);

void DUMP_EEPROM_CONTENT(void);

// �p��VPM EVENT LOG����
void WRITEBACK_VPM_EVENT_LOG_CNT(int type, int method);
void INC_VPM_EVENT_LOG_CNT(int type);

void WRITEBACK_POWER_ON_TIME(int method);
void INC_POWER_ON_TIME(void);
void TASK_COUNTING_POWER_ON_MINUTES(void);

#endif