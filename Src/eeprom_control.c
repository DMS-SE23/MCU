#include "project.h"
#include "stm32f30x.h"
#include "i2c_master.h"
#include "main.h"
#include <stdio.h>
#include "debug_port.h"
#include "eeprom_control.h"

unsigned char eeprom_default[384] = {
  /* 0000 */ 0x43,0x34,0x5,0x16,0x0,0x0,0x0,0x0,
  /* 0008 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0010 */ 0x0,0x2,0x0,0x5,0x0,0x1E,0x40,0x0,
  /* 0018 */ 0x14,0x0,0x0,0x0,0x2,0x0,0x0,0x0,
  /* 0020 */ 0x0,0x0,0x1E,0x0,0x3C,0xC0,0x2,0x64,
  /* 0028 */ 0x0,0x2,0x58,0x0,0x0,0x0,0x0,0x0,
  /* 0030 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0038 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0040 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0048 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0050 */ 0x0,0x0A,0x1,0x0,0x0,0x5,0x60,0x5,
  /* 0058 */ 0x60,0x0A,0x56,0x0A,0x56,0x0,0x0,0x0,
  /* 0060 */ 0x0,0x0,0x0,0x0,0x0,0x5,0x69,0x0A,
  /* 0068 */ 0x9B,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0070 */ 0x0,0x4,0xD9,0x0,0x0,0x0,0x0,0x0,
  /* 0078 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0080 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0088 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0090 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0098 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00A0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00A8 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00B0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00B8 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00C0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00C8 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00D0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00D8 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00E0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00E8 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00F0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 00F8 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0100 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0108 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0110 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0118 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0120 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0128 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0130 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0138 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0140 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0148 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0150 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0158 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0160 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0168 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0170 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  /* 0178 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
};

//unsigned char eeprom_default[128] = {
//  /* 0x00 */ 0x43,0x34,0x1,0x3,0x0,0x0,0x0,0x0,
//  //         --------- ---
//  //          |         |
//  //          |         +-- EEPROM FORMAT VERSION
//  //          +------------ MAGIC_WORD = 0x43 0x34
//  /* 0x08 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  /* 0x10 */ 0x0,0x2,0x0,0x5,0x0,0x0A,0x40,0x0,
//  //         ------- ------- -------- ---- ---
//  //          |       |       |        |    |
//  //          |       |       |        |    +-- POST_PWR_CHK_DLY (2 sec)
//  //          |       |       |        +------- PWR_MODE_CTRL (0x40)
//  //          |       |       |                   bit_7: AT_MODE
//  //          |       |       |                   bit_6: KEEP_ALIVE
//  //          |       |       +---------------- PWR_OFF_HARD_DLY (10 Sec)
//  //          |       +------------------------ PWR_OFF_EVT_DLY (5 Sec)
//  //          +-------------------------------- PWR_ON_DLY (2 Sec)
//  /* 0x18 */ 0x0,0x0,0x0,0x0,0x3,0x0,0x0A,0x0,
//  //         ---         ------- --------
//  //          |           |       |
//  //          |           |       +-- IGN_OFF_HIB_H_DLY (10 Sec)
//  //          |           +---------- WAKEUP_EVENT_MASK (Default 0x0003)
//  //          +---------------------- POST_PWR_CHK_DLY (2 sec)
//  /* 0x20 */ 0x0,0x0,0x1E,0x0,0x3C,0x0,0x0,0x0,
//  //             -------- -------- ---
//  //              |        |        |
//  //              |        |        +-- DISPLAY_MACHINE_SELECT (Default 0)
//  //              |        +----------- PWR_LOW_HARD_DLY (60 Sec)
//  //              +-------------------- PWR_LOW_EVT_DLY (30 Sec)
//  /* 0x28 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  //         ---
//  //          |
//  //          +-- PWR_OFF_MCU_SLP_MODE
//  /* 0x30 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  //         -------------------------------
//  //          |
//  //          +--> Serial Number (Total 25 Bytes)
//  /* 0x38 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  //         -------------------------------
//  //          |
//  //          +--> Serial Number (Total 25 Bytes)
//  /* 0x40 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  //         -------------------------------
//  //          |
//  //          +--> Serial Number (Total 25 Bytes)
//  /* 0x48 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  //         ---
//  //          |
//  //          +--> Serial Number (Total 25 Bytes)
//  /* 0x50 */ 0x0,0x0A,0x0,0x0,0x0,0x4,0xE4,0x4,
//  //         --------     --- --- -------- ---
//  //          |            |   |   |        |
//  //          |            |   |   |        +-- POSTBOOT_12V_CHECK (High)
//  //          |            |   |   +-- PREBOOT_12V_CHECK
//  //          |            |   +-- POSTBOOT VOLTAGE CHECk ENABLE (Default Off)
//  //          |            +-- PREBOOT VOLTAGE CHECK ENABLE (Default Off)
//  //          +-- WATCHDOG_VAL (10 Sec)
//  /* 0x58 */ 0xE4,0x9,0xBB,0x9,0xBB,0x26,0x45,0x26,
//  //         ---- -------- -------- --------- ----
//  //          |    |        |        |         |
//  //          |    |        |        |         +-- POSTBOOT_48V_CHECK (High)
//  //          |    |        |        +-- PREBOOT_48V_CHECK
//  //          |    |        +-- POSTBOOT_24V_CHECK
//  //          |    +-- PREBOOT_24V_CHECK
//  //          +-- POSTBOOT_12V_CHECK (Low)
//  /* 0x60 */ 0x45,0x5,0x74,0x5,0x74,0x0,0x0,0x0,
//  //         ---- -------- --------
//  //          |    |        |
//  //          |    |        +-- POSTBOOT_USER_CHECK
//  //          |    +-- PREBOOT_USER_CHECK
//  //          +-- POSTBOOT_48V_CHECK_LOW
//  /* 0x68 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  /* 0x70 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//  /* 0x78 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
//};

int vote_write_default = 0;
int vote_write_default_fail = 0;

void REFRESH_VPM_LOW_VOLTAGE_PROTECTION_VALUE()
{
//  unsigned char car_power_system;
//  unsigned char ReturnValueH, ReturnValueL;
//  // 判斷目前的Power System是12V or 24V
//  car_power_system = (VAR_SYSI2C_SYS_INFO>>6) & 0x03;
//  switch (car_power_system)
//  {
//    case 0: // 0 0: 24V Car Power System
//            // Pre Boot
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x59,&ReturnValueH); // Read Pre-Boot Value
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5A,&ReturnValueL); // Read Pre-Boot Value
//            VAR_VPM_PREBOOT_VOLTAGE_CHK = ReturnValueH * 256 + ReturnValueL; // Preboot
//            if (VAR_VPM_PREBOOT_VOLTAGE_CHK == 0xFFFF)
//              VAR_VPM_PREBOOT_VOLTAGE_CHK = (int)eeprom_default[0x59] * 256 + (int)eeprom_default[0x5A];
//            // Post Boot
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5B,&ReturnValueH); // Read Post-Boot Value
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5C,&ReturnValueL); // Read Post-Boot Value
//            VAR_VPM_POSTBOOT_VOLTAGE_CHK = ReturnValueH * 256 + ReturnValueL;
//            if (VAR_VPM_POSTBOOT_VOLTAGE_CHK == 0xFFFF)
//              VAR_VPM_POSTBOOT_VOLTAGE_CHK = (int)eeprom_default[0x5B] * 256 + (int)eeprom_default[0x5C];
//            break;
//    case 3: // 1 1: 12V Car Power System
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x55,&ReturnValueH); // Read Pre-Boot Value
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x56,&ReturnValueL); // Read Pre-Boot Value
//            VAR_VPM_PREBOOT_VOLTAGE_CHK = ReturnValueH * 256 + ReturnValueL; // Preboot
//            if (VAR_VPM_PREBOOT_VOLTAGE_CHK == 0xFFFF)
//              VAR_VPM_PREBOOT_VOLTAGE_CHK = (int)eeprom_default[0x55] * 256 + (int)eeprom_default[0x56];
//            //
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x57,&ReturnValueH); // Read Post-Boot Value
//            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x58,&ReturnValueL); // Read Post-Boot Value
//            VAR_VPM_POSTBOOT_VOLTAGE_CHK = ReturnValueH * 256 + ReturnValueL;
//            if (VAR_VPM_POSTBOOT_VOLTAGE_CHK == 0xFFFF)
//              VAR_VPM_POSTBOOT_VOLTAGE_CHK = (int)eeprom_default[0x57] * 256 + (int)eeprom_default[0x58];
//            break;
//    case 1: // 0 1: No Define
//    case 2: // 1 0: No Define
//    default:
//         break;
//  }
}

void REFRESH_UPS_START_CHARGE_VOLTAGE()
{
  unsigned char car_power_system;
  unsigned char ReturnValueH, ReturnValueL;
  // 判斷目前的Power System是12V or 24V
  car_power_system = (VAR_SYSI2C_SYS_INFO>>6) & 0x03;
  switch (car_power_system)
  {
    case 0: // 0 0: 24V Car Power System
            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x67,&ReturnValueH);
            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x68,&ReturnValueL);
            VAR_VPM_START_CHARGING_THRESHOLD = ReturnValueH * 256 + ReturnValueL;
            if (VAR_VPM_START_CHARGING_THRESHOLD == 0xFFFF)
              VAR_VPM_START_CHARGING_THRESHOLD = (int)eeprom_default[0x67] * 256 + (int)eeprom_default[0x68];
            break;
    case 3: // 1 1: 12V Car Power System
            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x65,&ReturnValueH);
            I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x66,&ReturnValueL);
            VAR_VPM_START_CHARGING_THRESHOLD = ReturnValueH * 256 + ReturnValueL;
            if (VAR_VPM_START_CHARGING_THRESHOLD == 0xFFFF)
              VAR_VPM_START_CHARGING_THRESHOLD = (int)eeprom_default[0x65] * 256 + (int)eeprom_default[0x66];
            break;
    case 1: // 0 1: No Define
    case 2: // 1 0: No Define
    default:
         break;
  }
}

// 回復到預設值
// 不會清掉機器序號與WWAN MAC Address
void EEPROM_LOAD_FACTORY_DEFAULT()
{
  // 24LC512的Write Cycle至少要10mS
  // 每一個Write中間要隔10mS以上
  // 因此在每一個Write Byte中間加上一個15mS Delay

  // 寫入Default Value到EEPROM
  for (int iCount = 4; iCount<0x30; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,iCount&0xFF,eeprom_default[iCount]);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
  // 跳掉器序號與WWAN MAC Address
  for (int iCount = 0x50; iCount<0xC0; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,iCount&0xFF,eeprom_default[iCount]);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
  // Fix data loss when writing EEPROM power loss
  for (int iCount = 0; iCount<4; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,iCount&0xFF,eeprom_default[iCount]);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
}

// Save User Default to EEPROM
void EEPROM_SAVE_USER_DEFAULT()
{
  unsigned char CheckSum = 0x00;
  unsigned char eeprom_user_default[39] = {
    /* 0080 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 0088 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 0090 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 0098 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 00A0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0
  };

  // 上次EEPROM設定未完成
  if (VAR_EEPROM_WRITE_EVENT != 0) {return; }

  // (1) Ignition On Delay
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x10,&eeprom_user_default[0]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x11,&eeprom_user_default[1]);
  // (2) Ignition Off Event Delay
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x12,&eeprom_user_default[2]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x13,&eeprom_user_default[3]);
  // (3) Ignition Off Run Power Off Flow Hard Delay
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x14,&eeprom_user_default[4]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x15,&eeprom_user_default[5]);
  // (4) 讀入AT_MODE & KEEP_ALIVE設定
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x16,&eeprom_user_default[6]);
  // (5) Post-boot Power Check Delay
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x17,&eeprom_user_default[7]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x18,&eeprom_user_default[8]);
  // (6) Wakeup Event Mask
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x1B,&eeprom_user_default[9]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x1C,&eeprom_user_default[10]);
  // (7) Power Low Event Delay
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x21,&eeprom_user_default[11]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x22,&eeprom_user_default[12]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x23,&eeprom_user_default[13]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x24,&eeprom_user_default[14]);
  // (8) Car Power System Information
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x25,&eeprom_user_default[15]);
  // (9) Power Off Event Mask
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x26,&eeprom_user_default[16]);
  // (10) Function Key Brightness
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x27,&eeprom_user_default[17]);
  // (11) Shut Down Delay
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x29,&eeprom_user_default[18]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x2A,&eeprom_user_default[19]);
  // (12) Shut Down Function Status
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x2B,&eeprom_user_default[20]);
  // (13) DI Input Type Control
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x2C,&eeprom_user_default[21]);
  // (14) 更新WATCHDOG TIMER VALUE
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x50,&eeprom_user_default[22]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x51,&eeprom_user_default[23]);
  // (15) DI Mode or Auto Detect Reserve Gear Mode
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x52,&eeprom_user_default[24]);
  // (16) Low Voltage Protection
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x55,&eeprom_user_default[25]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x56,&eeprom_user_default[26]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x57,&eeprom_user_default[27]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x58,&eeprom_user_default[28]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x59,&eeprom_user_default[29]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5A,&eeprom_user_default[30]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5B,&eeprom_user_default[31]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x5C,&eeprom_user_default[32]);
  // (17) UPS Start Charging Voltage
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x65,&eeprom_user_default[33]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x66,&eeprom_user_default[34]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x67,&eeprom_user_default[35]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x68,&eeprom_user_default[36]);
  // (18) Reference Voltage
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x71,&eeprom_user_default[37]);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x72,&eeprom_user_default[38]);

  for (int iCount = 0; iCount<39; iCount++)
  {
    CheckSum = (CheckSum + eeprom_user_default[iCount]) & 0xFF;
  }

  CheckSum = ~CheckSum;

  for (int iCount = 0; iCount<39; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,(0x80+iCount)&0xFF,eeprom_user_default[iCount]);
    __MACRO_DELAY_mS(15); // Delay 15mS
  }

  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xBF,CheckSum);
  __MACRO_DELAY_mS(15); // Delay 15mS

  VAR_EEPROM_USER_DEFAULT_STATUS = 1;
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x04,VAR_EEPROM_USER_DEFAULT_STATUS);
  __MACRO_DELAY_mS(15); // Delay 15mS
}

// Load User Default to EEPROM
int EEPROM_LOAD_USER_DEFAULT()
{
  unsigned char CheckSum = 0x00;
  unsigned char value = 0x00;
  unsigned char eeprom_user_default[39] = {
    /* 0080 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 0088 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 0090 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 0098 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    /* 00A0 */ 0x0,0x0,0x0,0x0,0x0,0x0,0x0
  };

  // 上次EEPROM設定未完成
  if (VAR_EEPROM_WRITE_EVENT != 0) {return 1; }

  for (int iCount = 0; iCount<39; iCount++)
  {
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,(0x80+iCount)&0xFF,&eeprom_user_default[iCount]);
    CheckSum = (CheckSum+eeprom_user_default[iCount])&0xFF;
  }

  CheckSum = ~CheckSum;

  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xBF,&value);

  if(value != CheckSum)
  {
    return 1;
  }

  // (1) Ignition On Delay
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x10,eeprom_user_default[0]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x11,eeprom_user_default[1]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (2) Ignition Off Event Delay
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x12,eeprom_user_default[2]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x13,eeprom_user_default[3]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (3) Ignition Off Run Power Off Flow Hard Delay
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x14,eeprom_user_default[4]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x15,eeprom_user_default[5]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (4) 讀入AT_MODE & KEEP_ALIVE設定
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x16,eeprom_user_default[6]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (5) Post-boot Power Check Delay
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x17,eeprom_user_default[7]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x18,eeprom_user_default[8]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (6) Wakeup Event Mask
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x1B,eeprom_user_default[9]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x1C,eeprom_user_default[10]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (7) Power Low Event Delay
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x21,eeprom_user_default[11]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x22,eeprom_user_default[12]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x23,eeprom_user_default[13]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x24,eeprom_user_default[14]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (8) Car Power System Information
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x25,eeprom_user_default[15]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (9) Power Off Event Mask
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x26,eeprom_user_default[16]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (10) Function Key Brightness
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x27,eeprom_user_default[17]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (11) Shut Down Delay
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x29,eeprom_user_default[18]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x2A,eeprom_user_default[19]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (12) Shut Down Function Status
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x2B,eeprom_user_default[20]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (13) DI Input Type Control
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x2C,eeprom_user_default[21]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (14) 更新WATCHDOG TIMER VALUE
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x50,eeprom_user_default[22]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x51,eeprom_user_default[23]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (15) DI Mode or Auto Detect Reserve Gear Mode
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x52,eeprom_user_default[24]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (16) Low Voltage Protection
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x55,eeprom_user_default[25]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x56,eeprom_user_default[26]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x57,eeprom_user_default[27]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x58,eeprom_user_default[28]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x59,eeprom_user_default[29]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x5A,eeprom_user_default[30]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x5B,eeprom_user_default[31]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x5C,eeprom_user_default[32]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (17) UPS Start Charging Voltage
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x65,eeprom_user_default[33]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x66,eeprom_user_default[34]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x67,eeprom_user_default[35]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x68,eeprom_user_default[36]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  // (18) Reference Voltage
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x71,eeprom_user_default[37]);
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x72,eeprom_user_default[38]);
  __MACRO_DELAY_mS(15); // Delay 15mS

  return 0;
}

// 回復到工廠預設值
void EEPROM_FILL_DEFAULT_VALUE()
{
  // 24LC512的Write Cycle至少要10mS
  // 每一個Write中間要隔10mS以上
  // 因此在每一個Write Byte中間加上一個15mS Delay

  // 寫入Default Value到EEPROM
  for (int iCount = 4; iCount<256; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,iCount&0xFF,eeprom_default[iCount]);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
  // 寫入Default Value到EEPROM
  for (int iCount = 0; iCount<128; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,iCount&0xFF,eeprom_default[256+iCount]);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
  // Fix data loss when writing EEPROM power loss
  for (int iCount = 0; iCount<4; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,iCount&0xFF,eeprom_default[iCount]);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
}

// Update Watchdog Timer
void WRITE_EEPROM_WATCHDOG_TIMER_VALUE(int value)
{
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x50,(value>>8)&0xFF);
  // 等待穩定
  __MACRO_DELAY_mS(15); // Delay 15mS
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x51,value&0xFF);
  // 等待穩定
  __MACRO_DELAY_mS(15); // Delay 15mS
}

// 更改存在EEPROM中的Serial Number
void EEPROM_UPDATE_SERIAL_NUMBER(int change_byte)
{
  switch (change_byte)
  {
    case 1: // Byte 1更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x30,VAR_SERIAL_NUMBER[0]&0xFF);
            break;
    case 2: // Byte 2更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x31,VAR_SERIAL_NUMBER[1]&0xFF);
            break;
    case 3: // Byte 3更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x32,VAR_SERIAL_NUMBER[2]&0xFF);
            break;
    case 4: // Byte 4更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x33,VAR_SERIAL_NUMBER[3]&0xFF);
            break;
    case 5: // Byte 5更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x34,VAR_SERIAL_NUMBER[4]&0xFF);
            break;
    case 6: // Byte 6更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x35,VAR_SERIAL_NUMBER[5]&0xFF);
            break;
    case 7: // Byte 7更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x36,VAR_SERIAL_NUMBER[6]&0xFF);
            break;
    case 8: // Byte 8更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x37,VAR_SERIAL_NUMBER[7]&0xFF);
            break;
    case 9: // Byte 9更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x38,VAR_SERIAL_NUMBER[8]&0xFF);
            break;
    case 10: // Byte 10更改
            I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0x39,VAR_SERIAL_NUMBER[9]&0xFF);
            break;
    default:
             break;
  }
}

// 更新EEPROM的Task序列
void TASK_EEPROM_UPDATE_BYTES()
{
  static int write_index;
  // 沒有Write EEPROM Event
  if (VAR_EEPROM_WRITE_EVENT == 0)
  {
    write_index = 0;
    return;
  }
  //
  I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,VAR_EEPROM_WRITE_QUEUE_ADDR_HI[write_index]&0xFF,VAR_EEPROM_WRITE_QUEUE_ADDR_LO[write_index]&0xFF,
                      VAR_EEPROM_WRITE_QUEUE_DATA[write_index]&0xFF);
  // write_index為要寫入EEPROM的此次index
  write_index++;
  if (write_index >= VAR_EEPROM_WRITE_EVENT)
  {
    VAR_EEPROM_WRITE_EVENT = 0; // 清掉Write EEPROM Event
    write_index = 0; // 結束Write
  }
}

// 由EEPROM更新LOG的參數設定
void UPDATE_LOG_VARIABLE()
{
  unsigned char ReturnValueH, ReturnValueL;

  unsigned int TempA[18];
  unsigned int TempB[18];
  unsigned int TempC[18];
  unsigned int TempFlag=0;
  unsigned int xCount=0;
  unsigned int yCount=0;
  unsigned int zCount=0;

  for (int iCount=0xC6; iCount<0xE9; iCount=iCount+2)
  {
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,iCount,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,iCount+1,&ReturnValueL);
    TempA[xCount++] = (int)ReturnValueH * 256 + (int)ReturnValueL;
  }

  for (int jCount=0x06; jCount<0x29; jCount=jCount+2)
  {
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x01,jCount,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x01,jCount+1,&ReturnValueL);
    TempB[yCount++] = (int)ReturnValueH * 256 + (int)ReturnValueL;
  }

  for (int kCount=0x46; kCount<0x69; kCount=kCount+2)
  {
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x01,kCount,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x01,kCount+1,&ReturnValueL);
    TempC[zCount++] = (int)ReturnValueH * 256 + (int)ReturnValueL;
  }

  for (int mCount=0; mCount<18; mCount++)
  {
    if(TempB[mCount]!=TempC[mCount])
    {
      TempFlag=1;
      break;
    }
  }

  if(TempFlag==1)
  {
    VAR_VPM_PWR_ON_IGN_CNT_ACC = TempA[0];         // EEPROM $00C6, $00C7
    VAR_VPM_PWR_ON_IGN_CNT = TempA[1];             // EEPROM $00C8, $00C9
    VAR_VPM_PWR_LOW_OFF_CNT_ACC = TempA[4];        // EEPROM $00CE, $00CF
    VAR_VPM_PWR_LOW_OFF_CNT = TempA[5];            // EEPROM $00D0, $00D1
    VAR_VPM_IGN_OFF_CNT_ACC = TempA[6];            // EEPROM $00D2, $00D3
    VAR_VPM_IGN_OFF_CNT = TempA[7];                // EEPROM $00D4, $00D5
    VAR_VPM_BAT_OVR_TMP_CNT_ACC = TempA[8];        // EEPROM $00D6, $00D7
    VAR_VPM_BAT_OVR_TMP_CNT = TempA[9];            // EEPROM $00D8, $00D9
    VAR_VPM_PRE_VCHK_OFF_CNT_ACC = TempA[10];       // EEPROM $00DA, $00DB
    VAR_VPM_PRE_VCHK_OFF_CNT = TempA[11];           // EEPROM $00DC, $00DD
    VAR_VPM_POST_VCHK_OFF_CNT_ACC = TempA[12];      // EEPROM $00DE, $00DF
    VAR_VPM_POST_VCHK_OFF_CNT = TempA[13];          // EEPROM $00E0, $00E1
    VAR_VPM_WDOG_RST_CNT_ACC = TempA[14];           // EEPROM $00E2, $00E3
    VAR_VPM_WDOG_RST_CNT = TempA[15];               // EEPROM $00E4, $00E5
    VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC = TempA[16];  // EEPROM $00E6, $00E7
    VAR_VPM_PWR_ON_KEEP_ALIVE_CNT = TempA[17];      // EEPROM $00E8, $00E9
  }
  else
  {
    VAR_VPM_PWR_ON_IGN_CNT_ACC = TempC[0];         // EEPROM $00C6, $00C7
    VAR_VPM_PWR_ON_IGN_CNT = TempC[1];             // EEPROM $00C8, $00C9
    VAR_VPM_PWR_LOW_OFF_CNT_ACC = TempC[4];        // EEPROM $00CE, $00CF
    VAR_VPM_PWR_LOW_OFF_CNT = TempC[5];            // EEPROM $00D0, $00D1
    VAR_VPM_IGN_OFF_CNT_ACC = TempC[6];            // EEPROM $00D2, $00D3
    VAR_VPM_IGN_OFF_CNT = TempC[7];                // EEPROM $00D4, $00D5
    VAR_VPM_BAT_OVR_TMP_CNT_ACC = TempC[8];        // EEPROM $00D6, $00D7
    VAR_VPM_BAT_OVR_TMP_CNT = TempC[9];            // EEPROM $00D8, $00D9
    VAR_VPM_PRE_VCHK_OFF_CNT_ACC = TempC[10];       // EEPROM $00DA, $00DB
    VAR_VPM_PRE_VCHK_OFF_CNT = TempC[11];           // EEPROM $00DC, $00DD
    VAR_VPM_POST_VCHK_OFF_CNT_ACC = TempC[12];      // EEPROM $00DE, $00DF
    VAR_VPM_POST_VCHK_OFF_CNT = TempC[13];          // EEPROM $00E0, $00E1
    VAR_VPM_WDOG_RST_CNT_ACC = TempC[14];           // EEPROM $00E2, $00E3
    VAR_VPM_WDOG_RST_CNT = TempC[15];               // EEPROM $00E4, $00E5
    VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC = TempC[16];  // EEPROM $00E6, $00E7
    VAR_VPM_PWR_ON_KEEP_ALIVE_CNT = TempC[17];      // EEPROM $00E8, $00E9
  }
}

// 由EEPROM更新VPM的參數設定
void UPDATE_VPM_VARIABLE_FROM_EEPROM()
{
  unsigned char ReturnValueH, ReturnValueL;
  vote_write_default = 0;
  vote_write_default_fail = 0;
  // 等待穩定
  __MACRO_DELAY_mS(15); // Delay 15mS
  // 檢查EEPROM Contents
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x00,&VAR_EEPROM_MAGIC_ID_HI);
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x01,&VAR_EEPROM_MAGIC_ID_LO);

  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x02,&ReturnValueL);
  if (ReturnValueL != EEPROM_VERSION)							// FORMAT_VER, EEPROM Format Version
  {
    vote_write_default = 2;
  }
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x00,&ReturnValueH);
  if (ReturnValueH != MAGIC_WD_HI)								// MAGIC_HI, Magic Word High Byte
  {
    vote_write_default = 1;
  }
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x01,&ReturnValueL);
  if (ReturnValueL != MAGIC_WD_LO)								// MAGIC_LO, Magic Word Low Byte
  {
    vote_write_default = 1;
  }
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x03,&ReturnValueL);
  if (ReturnValueL != PLATFORM_ID)								// PLATFORM_ID, 0x16: TREK-734 VPM
  {
    vote_write_default = 1;
  }
  // 判斷是否要初始化EEPROM的預設值
  if (vote_write_default == 1)
  {
    DEBUG_PRINT("@@ Init EEPROM to Factory Value\n\r");
    EEPROM_FILL_DEFAULT_VALUE(); // EEPROM全部清成預設值, 包含機器序號與WWAN MAC Address
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
  if (vote_write_default == 2)
  {
    DEBUG_PRINT("@@ Init EEPROM to Default Value\n\r");
    EEPROM_LOAD_FACTORY_DEFAULT(); // EEPROM全部清成預設值, 不含機器序號與WWAN MAC Address
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
  // 再讀一次資料
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x00,&ReturnValueH);
  if (ReturnValueH != MAGIC_WD_HI)
  {
    vote_write_default_fail = 1;
  }
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x01,&ReturnValueL);
  if (ReturnValueL != MAGIC_WD_LO)
  {
    vote_write_default_fail = 1;
  }
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x02,&ReturnValueL);
  if (ReturnValueL != EEPROM_VERSION) 						// FORMAT_VER, EEPROM Format Version
  {
    vote_write_default_fail = 1;
  }
  I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x03,&ReturnValueL);
  if (ReturnValueL != PLATFORM_ID) 							// PLATFORM_ID, 0x16: TREK-734 VPM
  {
    vote_write_default_fail = 1;
  }
  // 判斷是否寫入失敗
  if (vote_write_default_fail == 1)
  {
    DEBUG_PRINT("@@ Init EEPROM Fail\n\r");
    // 直接由預設表中設定，避免設為0xFF
    // (1) Ignition On Delay
    VAR_VPM_PWR_ON_DLY = (int)eeprom_default[0x10] * 256 + (int)eeprom_default[0x11]; // EEPROM $0010, $0011
    // (2) Ignition Off Event Delay
    VAR_VPM_PWR_OFF_EVT_DLY = (int)eeprom_default[0x12] * 256 + (int)eeprom_default[0x13];  // EEPROM $0012, $0013
    // (3) Ignition Off Run Power Off Flow Hard Delay
    VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY = (int)eeprom_default[0x14] * 256 + (int)eeprom_default[0x15];  // EEPROM $0014, $0015
    // (4) 讀入Power Mode Control設定
    VAR_VPM_PWR_MODE_CTRL = eeprom_default[0x16];    // EEPROM $0016
    // (6) Wakeup Event Mask
    VAR_WAKEUP_MASK_HI = eeprom_default[0x1B];  // EEPROM $001B
    VAR_WAKEUP_MASK_LO = eeprom_default[0x1C];  // EEPROM $001C
    // (7) Power Low Event Delay
    VAR_VPM_POWER_LOW_EVT_DLY = (int)eeprom_default[0x21] * 256 + (int)eeprom_default[0x22];  // EEPROM $0021, $0022
    VAR_VPM_POWER_LOW_HARD_DLY = (int)eeprom_default[0x23] * 256 + (int)eeprom_default[0x24]; // EEPROM $0023, $0024
    // (8) 更新WATCHDOG TIMER VALUE
    VAR_WATCHDOG_RESET_VALUE = (int)eeprom_default[0x50] * 256 + (int)eeprom_default[0x51]; // EEPROM $0050, $0051
    // (11) VPM Related Counter
//    VAR_VPM_COUNTER_UPDATE_FAIL = 1;  // =1, Fail Occurs
    VAR_VPM_PWR_ON_TIME_YEAR = 0;           // EEPROM $00C0, $00C1
    VAR_VPM_PWR_ON_MIN_HIGH_WORD = 0;       // EEPROM $00C2, $00C3
    VAR_VPM_PWR_ON_MIN_LOW_WORD = 0;        // EEPROM $00C4, $00C5
    // (12) LOG Related Counter
    VAR_VPM_PWR_ON_IGN_CNT_ACC = 0;         // EEPROM $00C6, $00C7
    VAR_VPM_PWR_ON_IGN_CNT = 0;             // EEPROM $00C8, $00C9
    VAR_VPM_PWR_LOW_OFF_CNT_ACC = 0;        // EEPROM $00CE, $00CF
    VAR_VPM_PWR_LOW_OFF_CNT = 0;            // EEPROM $00D0, $00D1
    VAR_VPM_IGN_OFF_CNT_ACC = 0;            // EEPROM $00D2, $00D3
    VAR_VPM_IGN_OFF_CNT = 0;                // EEPROM $00D4, $00D5
    VAR_VPM_BAT_OVR_TMP_CNT_ACC = 0;        // EEPROM $00D6, $00D7
    VAR_VPM_BAT_OVR_TMP_CNT = 0;            // EEPROM $00D8, $00D9
    VAR_VPM_PRE_VCHK_OFF_CNT_ACC = 0;       // EEPROM $00DA, $00DB
    VAR_VPM_PRE_VCHK_OFF_CNT = 0;           // EEPROM $00DC, $00DD
    VAR_VPM_POST_VCHK_OFF_CNT_ACC = 0;      // EEPROM $00DE, $00DF
    VAR_VPM_POST_VCHK_OFF_CNT = 0;          // EEPROM $00E0, $00E1
    VAR_VPM_WDOG_RST_CNT_ACC = 0;           // EEPROM $00E2, $00E3
    VAR_VPM_WDOG_RST_CNT = 0;               // EEPROM $00E4, $00E5
    VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC = 0;  // EEPROM $00E6, $00E7
    VAR_VPM_PWR_ON_KEEP_ALIVE_CNT = 0;      // EEPROM $00E8, $00E9
    // (13) 讀入Serial Number
    VAR_SERIAL_NUMBER[0] = eeprom_default[0x30];
    VAR_SERIAL_NUMBER[1] = eeprom_default[0x31];
    VAR_SERIAL_NUMBER[2] = eeprom_default[0x32];
    VAR_SERIAL_NUMBER[3] = eeprom_default[0x33];
    VAR_SERIAL_NUMBER[4] = eeprom_default[0x34];
    VAR_SERIAL_NUMBER[5] = eeprom_default[0x35];
    VAR_SERIAL_NUMBER[6] = eeprom_default[0x36];
    VAR_SERIAL_NUMBER[7] = eeprom_default[0x37];
    VAR_SERIAL_NUMBER[8] = eeprom_default[0x38];
    VAR_SERIAL_NUMBER[9] = eeprom_default[0x39];
    // (14) User Default Area Status
    VAR_EEPROM_USER_DEFAULT_STATUS = eeprom_default[0x04];
    // (18) Low Voltage Protection
    VAR_SYSI2C_SYS_INFO = eeprom_default[0x25];  // EEPROM $0025
    REFRESH_VPM_LOW_VOLTAGE_PROTECTION_VALUE();
    REFRESH_UPS_START_CHARGE_VOLTAGE();
  }
  else
  {
    if (vote_write_default == 1)
    {
      DEBUG_PRINT("@@ Init EEPROM Success\n\r");
    }
    // (1) Ignition On Delay
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x10,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x11,&ReturnValueL);
    VAR_VPM_PWR_ON_DLY = (int)ReturnValueH * 256 + (int)ReturnValueL; // EEPROM $0010, $0011
    // (2) Ignition Off Event Delay
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x12,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x13,&ReturnValueL);
    VAR_VPM_PWR_OFF_EVT_DLY = (int)ReturnValueH * 256 + (int)ReturnValueL;  // EEPROM $0012, $0013
    // (3) Ignition Off Run Power Off Flow Hard Delay
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x14,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x15,&ReturnValueL);
    VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY = (int)ReturnValueH * 256 + (int)ReturnValueL;  // EEPROM $0014, $0015
    // (4) 讀入AT_MODE & KEEP_ALIVE設定
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x16,&ReturnValueH);
    VAR_VPM_PWR_MODE_CTRL = ReturnValueH;    // EEPROM $0016
    // (6) Wakeup Event Mask
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x1B,&VAR_WAKEUP_MASK_HI); // EEPROM $001B
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x1C,&VAR_WAKEUP_MASK_LO); // EEPROM $001C
    // (7) Power Low Event Delay
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x21,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x22,&ReturnValueL);
    VAR_VPM_POWER_LOW_EVT_DLY = (int)ReturnValueH * 256 + (int)ReturnValueL;  // EEPROM $0021, $0022
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x23,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x24,&ReturnValueL);
    VAR_VPM_POWER_LOW_HARD_DLY = (int)ReturnValueH * 256 + (int)ReturnValueL; // EEPROM $0023, $0024
    // (8) 更新WATCHDOG TIMER VALUE
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x50,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x51,&ReturnValueL);
    VAR_WATCHDOG_RESET_VALUE = (int)ReturnValueH * 256 + (int)ReturnValueL; // EEPROM $0050, $0051
    // (11) VPM Related Counter
//    VAR_VPM_COUNTER_UPDATE_FAIL = 0;  // =1, Fail Occurs
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xC0,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xC1,&ReturnValueL);
    VAR_VPM_PWR_ON_TIME_YEAR = (int)ReturnValueH * 256 + (int)ReturnValueL;           // EEPROM $00C0, $00C1
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xC2,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xC3,&ReturnValueL);
    VAR_VPM_PWR_ON_MIN_HIGH_WORD = (int)ReturnValueH * 256 + (int)ReturnValueL;       // EEPROM $00C2, $00C3
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xC4,&ReturnValueH);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0xC5,&ReturnValueL);
    VAR_VPM_PWR_ON_MIN_LOW_WORD = (int)ReturnValueH * 256 + (int)ReturnValueL;        // EEPROM $00C4, $00C5
    // (12) LOG Related Counter
    UPDATE_LOG_VARIABLE();
    // (13) 讀入Serial Number
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x30,&VAR_SERIAL_NUMBER[0]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x31,&VAR_SERIAL_NUMBER[1]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x32,&VAR_SERIAL_NUMBER[2]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x33,&VAR_SERIAL_NUMBER[3]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x34,&VAR_SERIAL_NUMBER[4]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x35,&VAR_SERIAL_NUMBER[5]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x36,&VAR_SERIAL_NUMBER[6]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x37,&VAR_SERIAL_NUMBER[7]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x38,&VAR_SERIAL_NUMBER[8]);
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x39,&VAR_SERIAL_NUMBER[9]);
    // (14) User Default Area Status
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x04,&VAR_EEPROM_USER_DEFAULT_STATUS);
    // (18) Low Voltage Protection
    I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,0x25,&ReturnValueL);
    VAR_SYSI2C_SYS_INFO = ReturnValueL;  // EEPROM $0025
    REFRESH_VPM_LOW_VOLTAGE_PROTECTION_VALUE();
    REFRESH_UPS_START_CHARGE_VOLTAGE();
  }
}

// 清除所有EEPROM資料為0xFF
void EEPROM_ERASE_VALUE()
{
  // 24LC02的Write Cycle至少要10mS
  // 每一個Write中間要隔10mS以上
  // 因此在每一個Write Byte中間加上一個15mS Delay
  for (int iCount = 0; iCount<256; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,iCount&0xFF,0xFF);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }

  for (int iCount = 0; iCount<128; iCount++)
  {
    I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,iCount&0xFF,0xFF);
    // 等待穩定
    __MACRO_DELAY_mS(15); // Delay 15mS
  }
}

// Dump來自於EEPROM的參數資料
void DUMP_EEPROM_PARAMETER_VALUES()
{
  DEBUG_PRINT ("--- EEPROM PARAMETERS --------------------------------------\n\r");
  DEBUG_PRINT ("EEPROM Write Default (0:No, 1:Yes): %d\n\r",vote_write_default);
  DEBUG_PRINT ("EEPROM Write Default Fail (0:No, 1:Yes): %d\n\r",vote_write_default_fail);
  DEBUG_PRINT ("VAR_VPM_PWR_ON_DLY ($10,$11): %04X\n\r",VAR_VPM_PWR_ON_DLY);
  DEBUG_PRINT ("VAR_VPM_PWR_OFF_EVT_DLY ($12,$13): %04X\n\r",VAR_VPM_PWR_OFF_EVT_DLY);
  DEBUG_PRINT ("VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY ($14,$15) : %04X\n\r",VAR_VPM_IGN_OFF_PWR_OFF_HARD_DLY);
  DEBUG_PRINT ("VAR_VPM_PWR_MODE_CTRL ($16): %02X\n\r",VAR_VPM_PWR_MODE_CTRL);
  DEBUG_PRINT ("VAR_WAKEUP_MASK_HI,LO ($1B,$1C): %02X%02X\n\r",VAR_WAKEUP_MASK_HI,VAR_WAKEUP_MASK_LO);
  DEBUG_PRINT ("VAR_VPM_POWER_LOW_EVT_DLY ($21,$22): %04X\n\r",VAR_VPM_POWER_LOW_EVT_DLY);
  DEBUG_PRINT ("VAR_VPM_POWER_LOW_HARD_DLY ($23,$24): %04X\n\r",VAR_VPM_POWER_LOW_HARD_DLY);
  DEBUG_PRINT ("VAR_SYSI2C_SYS_INFO ($25): %02X\n\r",VAR_SYSI2C_SYS_INFO);
  DEBUG_PRINT ("VAR_WATCHDOG_RESET_VALUE ($50,$51): %04X\n\r",VAR_WATCHDOG_RESET_VALUE);
  DEBUG_PRINT ("VAR_VPM_START_CHARGING_THRESHOLD: %04X\n\r",VAR_VPM_START_CHARGING_THRESHOLD);
}

// Dump VPM Related Counter, 計數器資料
void DUMP_CURRENT_COUNTERS()
{
//  DEBUG_PRINT ("--- COUNTERS IN SYSTEM -------------------------------------\n\r");
//  DEBUG_PRINT ("VAR_COUNTER_ENABLE_POWER_ON_COUNT: %02X\n\r",VAR_COUNTER_ENABLE_POWER_ON_COUNT);
//  //printf ("VAR_VPM_COUNTER_UPDATE_FAIL: %02X\n\r", VAR_VPM_COUNTER_UPDATE_FAIL);    // =1, Fail Occurs
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_TIME_YEAR: %04X\n\r",VAR_VPM_PWR_ON_TIME_YEAR);                     // EEPROM $00C0, $00C1
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_MIN_HIGH_WORD: %04X\n\r",VAR_VPM_PWR_ON_MIN_HIGH_WORD);             // EEPROM $00C2, $00C3
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_MIN_LOW_WORD: %04X\n\r",VAR_VPM_PWR_ON_MIN_LOW_WORD);               // EEPROM $00C4, $00C5
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_IGN_CNT_ACC: %04X\n\r",VAR_VPM_PWR_ON_IGN_CNT_ACC);                 // EEPROM $00C6, $00C7
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_IGN_CNT: %04X\n\r",VAR_VPM_PWR_ON_IGN_CNT);                         // EEPROM $00C8, $00C9
//  DEBUG_PRINT ("VAR_VPM_PWR_LOW_OFF_CNT_ACC: %04X\n\r",VAR_VPM_PWR_LOW_OFF_CNT_ACC);               // EEPROM $00CE, $00CF
//  DEBUG_PRINT ("VAR_VPM_PWR_LOW_OFF_CNT: %04X\n\r",VAR_VPM_PWR_LOW_OFF_CNT);                       // EEPROM $00D0, $00D1
//  DEBUG_PRINT ("VAR_VPM_IGN_OFF_CNT_ACC: %04X\n\r",VAR_VPM_IGN_OFF_CNT_ACC);                       // EEPROM $00D2, $00D3
//  DEBUG_PRINT ("VAR_VPM_IGN_OFF_CNT: %04X\n\r",VAR_VPM_IGN_OFF_CNT);                               // EEPROM $00D4, $00D5
//  DEBUG_PRINT ("VAR_VPM_BAT_OVR_TMP_CNT_ACC: %04X\n\r",VAR_VPM_BAT_OVR_TMP_CNT_ACC);               // EEPROM $00D6, $00D7
//  DEBUG_PRINT ("VAR_VPM_BAT_OVR_TMP_CNT: %04X\n\r",VAR_VPM_BAT_OVR_TMP_CNT);                       // EEPROM $00D8, $00D9
//  DEBUG_PRINT ("VAR_VPM_PRE_VCHK_OFF_CNT_ACC: %04X\n\r",VAR_VPM_PRE_VCHK_OFF_CNT_ACC);             // EEPROM $00DA, $00DB
//  DEBUG_PRINT ("VAR_VPM_PRE_VCHK_OFF_CNT: %04X\n\r",VAR_VPM_PRE_VCHK_OFF_CNT);                     // EEPROM $00DC, $00DD
//  DEBUG_PRINT ("VAR_VPM_POST_VCHK_OFF_CNT_ACC: %04X\n\r",VAR_VPM_POST_VCHK_OFF_CNT_ACC);           // EEPROM $00DE, $00DF
//  DEBUG_PRINT ("VAR_VPM_POST_VCHK_OFF_CNT: %04X\n\r",VAR_VPM_POST_VCHK_OFF_CNT);                   // EEPROM $00E0, $00E1
//  DEBUG_PRINT ("VAR_VPM_WDOG_RST_CNT_ACC: %04X\n\r",VAR_VPM_WDOG_RST_CNT_ACC);                     // EEPROM $00E2, $00E3
//  DEBUG_PRINT ("VAR_VPM_WDOG_RST_CNT: %04X\n\r",VAR_VPM_WDOG_RST_CNT);                             // EEPROM $00E4, $00E5
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC: %04X\n\r",VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC);   // EEPROM $00E6, $00E7
//  DEBUG_PRINT ("VAR_VPM_PWR_ON_KEEP_ALIVE_CNT: %04X\n\r",VAR_VPM_PWR_ON_KEEP_ALIVE_CNT);           // EEPROM $00E8, $00E9
}

void DUMP_EEPROM_CONTENT()
{
  int iCount1, iCount2;
  unsigned char ReturnValue;
  for (iCount1 = 0; iCount1 < 256; iCount1 += 16)
  {
    DEBUG_PRINT ("00%02X:",iCount1);
    for (iCount2= 0; iCount2 < 16; iCount2++)
    {
      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x00,(iCount1+iCount2)&0xFF,&ReturnValue);
      DEBUG_PRINT (" %02X",ReturnValue);
    }
    DEBUG_PRINT ("\n\r");
  }
  for (iCount1 = 0; iCount1 < 128; iCount1 += 16)
  {
    DEBUG_PRINT ("01%02X:",iCount1);
    for (iCount2= 0; iCount2 < 16; iCount2++)
    {
      I2C_EEPROM_24LC512_READ_BYTE(0xA0,0x01,(iCount1+iCount2)&0xFF,&ReturnValue);
      DEBUG_PRINT (" %02X",ReturnValue);
    }
    DEBUG_PRINT ("\n\r");
  }
}

void WRITEBACK_VPM_EVENT_LOG_CNT(int type, int method)
{
  // 上次EEPROM設定未完成
  if (VAR_EEPROM_WRITE_EVENT != 0) {return; }

  switch (type)
  {
    // Ignition On Power On
    case 1:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xC6;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_ON_IGN_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xC7;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_ON_IGN_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xC8;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_ON_IGN_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xC9;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_ON_IGN_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x06;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_ON_IGN_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x07;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_ON_IGN_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x08;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_PWR_ON_IGN_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x09;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_PWR_ON_IGN_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x46;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_PWR_ON_IGN_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x47;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_PWR_ON_IGN_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x48;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_PWR_ON_IGN_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x49;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_PWR_ON_IGN_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xC8;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_ON_IGN_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xC9;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_ON_IGN_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x08;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_ON_IGN_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x09;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_ON_IGN_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x48;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_ON_IGN_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x49;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_ON_IGN_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
    // Car Power Low to OFF
    case 3:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xCE;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_LOW_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xCF;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_LOW_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xD0;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_LOW_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xD1;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_LOW_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x0E;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_LOW_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x0F;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_LOW_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x10;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_PWR_LOW_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x11;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_PWR_LOW_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x4E;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_PWR_LOW_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x4F;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_PWR_LOW_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x50;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_PWR_LOW_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x51;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_PWR_LOW_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xD0;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_LOW_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xD1;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_LOW_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x10;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_LOW_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x11;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_LOW_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x50;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_LOW_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x51;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_LOW_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
    // Ignition OFF to Off
    case 4:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xD2;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_IGN_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xD3;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_IGN_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xD4;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_IGN_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xD5;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_IGN_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x12;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_IGN_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x13;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_IGN_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x14;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_IGN_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x15;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_IGN_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x52;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_IGN_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x53;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_IGN_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x54;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_IGN_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x55;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_IGN_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xD4;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_IGN_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xD5;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_IGN_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x14;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_IGN_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x15;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_IGN_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x54;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_IGN_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x55;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_IGN_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
    // Battery Over Temperature
    case 5:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xD6;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_BAT_OVR_TMP_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xD7;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_BAT_OVR_TMP_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xD8;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_BAT_OVR_TMP_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xD9;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_BAT_OVR_TMP_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x16;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_BAT_OVR_TMP_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x17;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_BAT_OVR_TMP_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x18;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_BAT_OVR_TMP_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x19;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_BAT_OVR_TMP_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x56;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_BAT_OVR_TMP_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x57;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_BAT_OVR_TMP_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x58;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_BAT_OVR_TMP_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x59;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_BAT_OVR_TMP_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xD8;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_BAT_OVR_TMP_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xD9;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_BAT_OVR_TMP_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x18;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_BAT_OVR_TMP_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x19;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_BAT_OVR_TMP_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x58;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_BAT_OVR_TMP_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x59;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_BAT_OVR_TMP_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
    // Pre-boot Car Power Check Generate Power Off
    case 6:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xDA;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PRE_VCHK_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xDB;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PRE_VCHK_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xDC;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PRE_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xDD;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PRE_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x1A;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PRE_VCHK_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x1B;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PRE_VCHK_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x1C;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_PRE_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x1D;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_PRE_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x5A;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_PRE_VCHK_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x5B;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_PRE_VCHK_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x5C;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_PRE_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x5D;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_PRE_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xDC;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PRE_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xDD;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PRE_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x1C;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PRE_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x1D;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PRE_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x5C;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PRE_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x5D;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PRE_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
    // Post-boot Car Power Check Generate Power Off
    case 7:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xDE;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POST_VCHK_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xDF;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POST_VCHK_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xE0;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_POST_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xE1;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_POST_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x1E;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_POST_VCHK_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x1F;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_POST_VCHK_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x20;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_POST_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x21;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_POST_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x5E;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_POST_VCHK_OFF_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x5F;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_POST_VCHK_OFF_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x60;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_POST_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x61;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_POST_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xE0;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_POST_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xE1;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_POST_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x20;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_POST_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x21;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_POST_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x60;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_POST_VCHK_OFF_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x61;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_POST_VCHK_OFF_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
    // Watchdog Reset System
    case 8:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xE2,((VAR_VPM_WDOG_RST_CNT_ACC>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xE3,(VAR_VPM_WDOG_RST_CNT_ACC&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xE4,((VAR_VPM_WDOG_RST_CNT>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xE5,(VAR_VPM_WDOG_RST_CNT&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x22,((VAR_VPM_WDOG_RST_CNT_ACC>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x23,(VAR_VPM_WDOG_RST_CNT_ACC&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x24,((VAR_VPM_WDOG_RST_CNT>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x25,(VAR_VPM_WDOG_RST_CNT&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x62,((VAR_VPM_WDOG_RST_CNT_ACC>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x63,(VAR_VPM_WDOG_RST_CNT_ACC&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x64,((VAR_VPM_WDOG_RST_CNT>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x65,(VAR_VPM_WDOG_RST_CNT&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
      }
      else
      {
        // LOW WORD
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xE4,((VAR_VPM_WDOG_RST_CNT>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x00,0xE5,(VAR_VPM_WDOG_RST_CNT&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x24,((VAR_VPM_WDOG_RST_CNT>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x25,(VAR_VPM_WDOG_RST_CNT&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x64,((VAR_VPM_WDOG_RST_CNT>>8)&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
        I2C_EEPROM_24LC512_WRITE_BYTE(0xA0,0x01,0x65,(VAR_VPM_WDOG_RST_CNT&0xFF));
        __MACRO_DELAY_mS(15); // Delay 15mS
      }
      break;
    // Keep Alive On Power On
    case 9:
      if (method >= 1) // 更新到HIGH WORD
      {
        // HIGH WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xE6;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xE7;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xE8;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xE9;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x26;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x27;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[6] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[6] = 0x28;
        VAR_EEPROM_WRITE_QUEUE_DATA[6] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[7] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[7] = 0x29;
        VAR_EEPROM_WRITE_QUEUE_DATA[7] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[8] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[8] = 0x66;
        VAR_EEPROM_WRITE_QUEUE_DATA[8] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[9] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[9] = 0x67;
        VAR_EEPROM_WRITE_QUEUE_DATA[9] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[10] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[10] = 0x68;
        VAR_EEPROM_WRITE_QUEUE_DATA[10] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[11] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[11] = 0x69;
        VAR_EEPROM_WRITE_QUEUE_DATA[11] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 12; // Write 12 Byte
      }
      else
      {
        // LOW WORD
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xE8;
        VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xE9;
        VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0x28;
        VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0x29;
        VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0x68;
        VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT>>8)&0xFF;
        VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x01;
        VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0x69;
        VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_ON_KEEP_ALIVE_CNT&0xFF;
        VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
      }
      break;
  }
}

void INC_VPM_EVENT_LOG_CNT(int type)
{
  switch (type)
  {
    // Ignition On Power On
    case 1:
      VAR_VPM_PWR_ON_IGN_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_PWR_ON_IGN_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(1,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_PWR_ON_IGN_CNT = 0;
      VAR_VPM_PWR_ON_IGN_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(1,1); // 更新HIGH WORD & LOW WORD
      break;
    // Car Power Low to OFF
    case 3:
      VAR_VPM_PWR_LOW_OFF_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_PWR_LOW_OFF_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(3,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_PWR_LOW_OFF_CNT = 0;
      VAR_VPM_PWR_LOW_OFF_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(3,1); // 更新HIGH WORD & LOW WORD
      break;
    // Ignition OFF to Off
    case 4:
      VAR_VPM_IGN_OFF_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_IGN_OFF_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(4,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_IGN_OFF_CNT = 0;
      VAR_VPM_IGN_OFF_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(4,1); // 更新HIGH WORD & LOW WORD
      break;
    // Battery Over Temperature
    case 5:
      VAR_VPM_BAT_OVR_TMP_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_BAT_OVR_TMP_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(5,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_BAT_OVR_TMP_CNT = 0;
      VAR_VPM_BAT_OVR_TMP_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(5,1); // 更新HIGH WORD & LOW WORD
      break;
    // Pre-boot Car Power Check Generate Power Off
    case 6:
      VAR_VPM_PRE_VCHK_OFF_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_PRE_VCHK_OFF_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(6,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_PRE_VCHK_OFF_CNT = 0;
      VAR_VPM_PRE_VCHK_OFF_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(6,1); // 更新HIGH WORD & LOW WORD
      break;
    // Post-boot Car Power Check Generate Power Off
    case 7:
      VAR_VPM_POST_VCHK_OFF_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_POST_VCHK_OFF_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(7,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_POST_VCHK_OFF_CNT = 0;
      VAR_VPM_POST_VCHK_OFF_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(7,1); // 更新HIGH WORD & LOW WORD
      break;
    // Watchdog Reset System
    case 8:
      VAR_VPM_WDOG_RST_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_WDOG_RST_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(8,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_WDOG_RST_CNT = 0;
      VAR_VPM_WDOG_RST_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(8,1); // 更新HIGH WORD & LOW WORD
      break;
    // Keep Alive On Power On
    case 9:
      VAR_VPM_PWR_ON_KEEP_ALIVE_CNT++;
      // 是否超過65535 Times
      if (VAR_VPM_PWR_ON_KEEP_ALIVE_CNT <= 65535)
      {
        WRITEBACK_VPM_EVENT_LOG_CNT(9,0);
        return;
      }
      // 超過65535 Times
      VAR_VPM_PWR_ON_KEEP_ALIVE_CNT = 0;
      VAR_VPM_PWR_ON_KEEP_ALIVE_CNT_ACC++;
      WRITEBACK_VPM_EVENT_LOG_CNT(9,1); // 更新HIGH WORD & LOW WORD
      break;
  }
}

void WRITEBACK_POWER_ON_TIME(int method)
{
  // 上次EEPROM設定未完成
  if (VAR_EEPROM_WRITE_EVENT != 0) {return; }

  // LOW WORD
  VAR_EEPROM_WRITE_QUEUE_ADDR_HI[0] = 0x00;
  VAR_EEPROM_WRITE_QUEUE_ADDR_LO[0] = 0xC4;
  VAR_EEPROM_WRITE_QUEUE_DATA[0] = (VAR_VPM_PWR_ON_MIN_LOW_WORD>>8)&0xFF;
  VAR_EEPROM_WRITE_QUEUE_ADDR_HI[1] = 0x00;
  VAR_EEPROM_WRITE_QUEUE_ADDR_LO[1] = 0xC5;
  VAR_EEPROM_WRITE_QUEUE_DATA[1] = VAR_VPM_PWR_ON_MIN_LOW_WORD&0xFF;
  VAR_EEPROM_WRITE_EVENT = 2; // Write 2 Byte

  // 只更新LOW WORD
  if (method >= 1) // 更新到HIGH WORD
  {
    // HIGH WORD
    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[2] = 0x00;
    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[2] = 0xC2;
    VAR_EEPROM_WRITE_QUEUE_DATA[2] = (VAR_VPM_PWR_ON_MIN_HIGH_WORD>>8)&0xFF;
    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[3] = 0x00;
    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[3] = 0xC3;
    VAR_EEPROM_WRITE_QUEUE_DATA[3] = VAR_VPM_PWR_ON_MIN_HIGH_WORD&0xFF;
    VAR_EEPROM_WRITE_EVENT = 4; // Write 4 Byte
  }
  if (method >= 2) // 更新到YEAR
  {
    // YEAR
    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[4] = 0x00;
    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[4] = 0xC0;
    VAR_EEPROM_WRITE_QUEUE_DATA[4] = (VAR_VPM_PWR_ON_TIME_YEAR>>8)&0xFF;
    VAR_EEPROM_WRITE_QUEUE_ADDR_HI[5] = 0x00;
    VAR_EEPROM_WRITE_QUEUE_ADDR_LO[5] = 0xC1;
    VAR_EEPROM_WRITE_QUEUE_DATA[5] = VAR_VPM_PWR_ON_TIME_YEAR&0xFF;
    VAR_EEPROM_WRITE_EVENT = 6; // Write 6 Byte
  }
}

void INC_POWER_ON_TIME()
{
  VAR_VPM_PWR_ON_MIN_LOW_WORD++;
  // 判斷是否過完1年 (0X80520分鐘)
  // 0X80520 = 525600 = 1440 x 365 (天) = 24 x 60 x 365
  if ((VAR_VPM_PWR_ON_MIN_HIGH_WORD == 0x08) &&
      (VAR_VPM_PWR_ON_MIN_LOW_WORD >= 0x0520))
  {
    VAR_VPM_PWR_ON_TIME_YEAR++;
    VAR_VPM_PWR_ON_MIN_HIGH_WORD = 0;
    VAR_VPM_PWR_ON_MIN_LOW_WORD = 0;
    // 強迫更新回EEPROM
    WRITEBACK_POWER_ON_TIME(2); // 全部更新
    return;
  }
  // 是否超過65535 Minutes
  if (VAR_VPM_PWR_ON_MIN_LOW_WORD < 65535) return;
  // 超過65535 Minutes
  VAR_VPM_PWR_ON_MIN_LOW_WORD = 0;
  VAR_VPM_PWR_ON_MIN_HIGH_WORD++;
  WRITEBACK_POWER_ON_TIME(1); // 更新HIGH WORD & LOW WORD
}

void TASK_COUNTING_POWER_ON_MINUTES()
{
  static unsigned int LOCAL_VAR_CURRENT_SECOND = 0;
  static unsigned int LOCAL_VAR_UPDATE_TO_EEPROM = 0;
  // 若不允許計數時，立即返回
  if (VAR_COUNTER_ENABLE_POWER_ON_COUNT != 1) return;
  // 以下為計數開始 ---------------------------------
  if ((LOCAL_VAR_CURRENT_SECOND++) < 60) return;
  // 以下為秒數計算超過1分鐘時
  LOCAL_VAR_CURRENT_SECOND = 0;
  LOCAL_VAR_UPDATE_TO_EEPROM++;
  INC_POWER_ON_TIME(); // 新增1分鐘
  if (LOCAL_VAR_UPDATE_TO_EEPROM < 10) return;
  // 以下為每隔10分鐘要更新到EEPROM中一次
  LOCAL_VAR_UPDATE_TO_EEPROM = 0;
  DEBUG_PRINT ("..: update to eeprom power-on-time\n\r");
  WRITEBACK_POWER_ON_TIME(0); // 更新LOW WORD Only
}
//=============================================================================