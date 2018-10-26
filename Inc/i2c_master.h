#ifndef __I2C_MASTER_H
#define __I2C_MASTER_H

#define I2C_RTC_READ_NBYTE_DBG              0x10    //0x10~0x1F
#define I2C_RTC_WRITE_NBYTE_DBG             0x20    //0x20~0x2F
#define I2C_EEPROM_24LC512_READ_BYTE_DBG    0x30    //0x30~0x3F
#define I2C_EEPROM_24LC512_WRITE_BYTE_DBG   0x40    //0x40~0x4F
#define I2C_READ_BYTE_DBG                   0x50    //0x50~0x5F
#define I2C_WRITE_BYTE_DBG                  0x60    //0x60~0x6F
#define I2C_READ_WORD_DBG                   0x80    //0x80~0x8F
#define I2C_READ_NBYTE_DBG                  0x90    //0x90~0x9F
#define I2C_WRITE_NBYTE_DBG                 0xA0    //0xA0~0xAF

void I2C1_Master_Init();

int I2C_BUSBUSY_UserCallback(unsigned char DBGValue);
int I2C_TIMEOUT_UserCallback(unsigned char DBGValue);

// S-35390 I2C Read Bytes
int I2C_RTC_READ_NBYTE(unsigned char SAddr_Cmd,
                       unsigned char ByteCount,
                       unsigned char* ReturndValue);

// S-35390 I2C Write Bytes
int I2C_RTC_WRITE_NBYTE(unsigned char SAddr_Cmd,
                        unsigned char ByteCount,
                        unsigned char* WriteData);

// EEPROM 24LC512 Read Function
int I2C_EEPROM_24LC512_READ_BYTE(unsigned char ChipID,
                                 unsigned char ReadAddr1,
                                 unsigned char ReadAddr2,
                                 unsigned char *ReturndValue);
// EEPROM 24LC512 Write Function
int I2C_EEPROM_24LC512_WRITE_BYTE(unsigned char ChipID,
                                  unsigned char ReadAddr1,
                                  unsigned char ReadAddr2,
                                  unsigned char Data);

int I2C_READ_BYTE(unsigned char SlaveAddress,
                  unsigned char CommandCode,
                  unsigned char *ReturndValue);

int I2C_WRITE_BYTE(unsigned char SlaveAddress,
                   unsigned char CommandCode,
                   unsigned char WriteData);

int I2C_READ_WORD(unsigned char SlaveAddress,
                  unsigned char CommandCode,
                  unsigned int *ReturndValue);

int I2C_WRITE_WORD(unsigned char SlaveAddress,
                   unsigned char CommandCode,
                   unsigned int WriteData);

int I2C_READ_NBYTE(unsigned char SlaveAddress,
                   unsigned char CommandCode,
                   unsigned char ByteCount,
                   unsigned char *ReturndValue);

int I2C_WRITE_NBYTE(unsigned char SlaveAddress,
                    unsigned char CommandCode,
                    unsigned char ByteCount,
                    unsigned char *WriteData);
#endif
//=============================================================================