#include "stm32f30x.h"
#include "i2c_master.h"

void ADV7186_LVDS()
{
  //c0 fe 48 ; Set Field vdp slave addr to 36
  I2C_WRITE_BYTE (0xC0, 0xFE, 0x48);
  //c0 fd 44 ; Set Field cp slave addr to 34
  I2C_WRITE_BYTE (0xC0, 0xFD, 0x44);
  //c0 fb 84 ; Set Field panel intf slave addr to 66
  I2C_WRITE_BYTE (0xC0, 0xFB, 0x84);
  //c0 f8 4c ; Set Field afe slave addr to 38
  I2C_WRITE_BYTE (0xC0, 0xF8, 0x4C);
  //c0 f2 94 ; Set Field sdp_io slave addr to 74
  I2C_WRITE_BYTE (0xC0, 0xF2, 0x94);
  //c0 f1 90 ; Set Field sdp slave addr to 72
  I2C_WRITE_BYTE (0xC0, 0xF1, 0x90);
  //c0 ec b0 ; Set Field vfe_slave_addr to 80
  I2C_WRITE_BYTE (0xC0, 0xEC, 0xB0);
  //c0 eb a8 ; Set Field xmem_gamma slave addr to 84
  I2C_WRITE_BYTE (0xC0, 0xEB, 0xA8);
  //c0 ea a4 ; Set Field vsp io slave addr to 82
  I2C_WRITE_BYTE (0xC0, 0xEA, 0xA4);
  //c0 e9 e0 ; Set Field vsp slave addr to 112
  I2C_WRITE_BYTE (0xC0, 0xE9, 0xE0);
  //c0 e8 ac ; Set Field ldi_slave_addr to 86
  I2C_WRITE_BYTE (0xC0, 0xE8, 0xAC);
  //c0 0c 40 ; Set Field power down to 0
  I2C_WRITE_BYTE (0xC0, 0x0C, 0x40);
  //44 ba 00 ; Set Field hdmi frun en to 0
  I2C_WRITE_BYTE (0x44, 0xBA, 0x00);
  //ac 40 00 ; Set Field Disable OLDI Transmiter PLL to 1, Powerdown LVDS Transmitter to 0
  I2C_WRITE_BYTE (0xAC, 0x40, 0x00);
  //b0 01 05 ; Set Prim Mode = Digi-Comp mode
  I2C_WRITE_BYTE (0xB0, 0x01, 0x05);
  //b0 02 10 ; Set RGB input
  I2C_WRITE_BYTE (0xB0, 0x02, 0x10);
  //b0 0c 00 ; Set Field vdp pwrdn to 0
  I2C_WRITE_BYTE (0xB0, 0x0C, 0x00);
  //b0 05 0c ; Set Field free run en to 0
  I2C_WRITE_BYTE (0xB0, 0x05, 0x0C);
  //b0 00 0c ; Set Field vid std to 12
  I2C_WRITE_BYTE (0xB0, 0x00, 0x0C);
  //a4 b0 02 ; Set Field overlay_vid1_sel to 1
  I2C_WRITE_BYTE (0xA4, 0xB0, 0x02);
  //a4 00 FF ; Set Field out vid std to 140 ; 8C
  I2C_WRITE_BYTE (0xA4, 0x00, 0xFF);
  //A4 01 19 ; Set RGB output
  I2C_WRITE_BYTE (0xA4, 0x01, 0x19);
  //ac 0c 91 ; Set Field Rx color mode to 0 ; 07/01 : 0x90 -> 0x91 (24bits)
  I2C_WRITE_BYTE (0xAC, 0x0C, 0x91);
  //ac 00 2a ; Set Field TTL Rx Enable to 0 , Powerdown LVDS Receivers to 0 , Set Field Enable OLDI Receiver PLL to 1
  I2C_WRITE_BYTE (0xAC, 0x00, 0x2A);
  //ac 4c 71 ; Set Field TX color mode to 0 ; 7/06 : 0x70 -> 0x71 (full color)
  I2C_WRITE_BYTE (0xAC, 0x4C, 0x71);
  //ac 40 08 ; Set Field Enable OLDI Transmiter PLL to 1 , Powerdown LVDS Transmitter to 0
  I2C_WRITE_BYTE (0xAC, 0x40, 0x08);
}