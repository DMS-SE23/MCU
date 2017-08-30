#ifndef __I2C_MASTER_H
#define __I2C_MASTER_H

void I2C_Master_Init();

// I2C Standard Write Function
int I2C_WRITE_BYTE(unsigned char SlaveAddress,
                   unsigned char CommandCode,
                   unsigned char WriteData);

#define __RETURN_SUCCESS    0
#define __RETURN_FAIL       -1

#endif
//=============================================================================