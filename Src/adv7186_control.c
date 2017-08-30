#include "stm32f30x.h"
#include "project.h"
#include "main.h"
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

  //For test 9/9 : from Justin.Wu/Sloane
  //Enable independent BT656 interface out memory pins
//  I2C_WRITE_BYTE (0xC0, 0x1E, 0x01);
}

void ADV7186_CVBS()
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
  //ac 40 00 ; Set Field Disable OLDI Transmiter PLL to 1, Powerdown LVDS Transmitter to 0
  I2C_WRITE_BYTE (0xAC, 0x40, 0x00);
  //c0 0c 40 ; Set Field power down to 0
  I2C_WRITE_BYTE (0xC0, 0x0C, 0x40);
  //44 ba 03 ; Set Field hdmi frun mode to 1
  I2C_WRITE_BYTE (0x44, 0xBA, 0x03);
  //4c 00 06 ; Set Field Powerdown ADC0 to 0
  I2C_WRITE_BYTE (0x4C, 0x00, 0x06);
  //b0 0c 00 ; Set Field vdp pwrdn to 0
  I2C_WRITE_BYTE (0xB0, 0x0C, 0x00);
  //b0 01 00 ; Set Field prim mode to 0
  I2C_WRITE_BYTE (0xB0, 0x01, 0x00);
  //b0 00 02 ; Set Field vid std to 2
  I2C_WRITE_BYTE (0xB0, 0x00, 0x02);
  //a4 00 81 ; Set Field output vid std to 140
  I2C_WRITE_BYTE (0xA4, 0x00, 0x81);
  //A4 E3 05 ; Scaler max line length adjustment
  I2C_WRITE_BYTE (0xA4, 0xE3, 0x05);
  //A4 E4 40 ; Scaler max line length adjustment
  I2C_WRITE_BYTE (0xA4, 0xE4, 0x40);
  //A4 E1 02 ; Scaler max Lcount adjustment
  I2C_WRITE_BYTE (0xA4, 0xE1, 0x02);
  //A4 E2 8A ; Scaler max Lcount adjustment
  I2C_WRITE_BYTE (0xA4, 0xE2, 0x8A);
  //A4 DB 27 ;
  I2C_WRITE_BYTE (0xA4, 0xDB, 0x27);
  //A4 DC B0 ;
  I2C_WRITE_BYTE (0xA4, 0xDC, 0xB0);
  //A4 DD 23 ;
  I2C_WRITE_BYTE (0xA4, 0xDD, 0x23);
  //A4 D0 08 ;
  I2C_WRITE_BYTE (0xA4, 0xD0, 0x08);
  //A4 D2 15 ;
  I2C_WRITE_BYTE (0xA4, 0xD2, 0x15);
  //A4 D3 04 ;
  I2C_WRITE_BYTE (0xA4, 0xD3, 0x04);
  //A4 D4 19 ;
  I2C_WRITE_BYTE (0xA4, 0xD4, 0x19);
  //A4 9A 06 ; DE adjustment(5/13)
  I2C_WRITE_BYTE (0xA4, 0x9A, 0x06);
  //A4 A2 02 ; shift 12 pixels (5/13)
  I2C_WRITE_BYTE (0xA4, 0xA2, 0x02);
  //A4 8B 40 ; Set output res 1024x600
  I2C_WRITE_BYTE (0xA4, 0x8B, 0x40);
  //A4 8C 02 ; Set output res 1024x600
  I2C_WRITE_BYTE (0xA4, 0x8C, 0x02);
  //A4 8D 58 ; Set output res 1024x600
  I2C_WRITE_BYTE (0xA4, 0x8D, 0x58);
  //A4 74 14 ; SAV Adjustment
  I2C_WRITE_BYTE (0xA4, 0x74, 0x14);
  //A4 75 04 ; EAV Adjustment
  I2C_WRITE_BYTE (0xA4, 0x75, 0x04);
  //A4 76 30 ; EAV Adjustment
  I2C_WRITE_BYTE (0xA4, 0x76, 0x30);
  //90 00 7F ; Autodetect settings (5/13)
  I2C_WRITE_BYTE (0x90, 0x00, 0x7F);
  //90 12 00 ; Disable 3D comb (5/13)
  I2C_WRITE_BYTE (0x90, 0x12, 0x00);
  //90 01 00 ; Pedestal Off
  I2C_WRITE_BYTE (0x90, 0x01, 0x00);
  //90 2B 11 ; ADI Recommended Write
  I2C_WRITE_BYTE (0x90, 0x2B, 0x11);
  //90 7B 6F ; SDP_EXTEND_MIN_MAX for VS
  I2C_WRITE_BYTE (0x90, 0x7B, 0x6F);
  //90 D4 60 ; ADI Recommended Write
  I2C_WRITE_BYTE (0x90, 0xD4, 0x60);
  //90 D7 44 ; ADI Recommended Write
  I2C_WRITE_BYTE (0x90, 0xD7, 0x44);
  //90 DD FC ; ADI Recommended Write
  I2C_WRITE_BYTE (0x90, 0xDD, 0xFC);
  //94 99 05 ; de h beg adjust
  I2C_WRITE_BYTE (0x94, 0x99, 0x05);
  //94 9B 05 ; de h end adjust
  I2C_WRITE_BYTE (0x94, 0x9B, 0x05);
  //94 A2 00 ; v end o adj
  I2C_WRITE_BYTE (0x94, 0xA2, 0x00);
  //94 A3 00 ; v end e adj
  I2C_WRITE_BYTE (0x94, 0xA3, 0x00);
  //94 A4 00 ; f tog o adj
  I2C_WRITE_BYTE (0x94, 0xA4, 0x00);
  //94 A5 00 ; f tog e adj
  I2C_WRITE_BYTE (0x94, 0xA5, 0x00);
  //94 AC 3F ; de v beg o adj
  I2C_WRITE_BYTE (0x94, 0xAC, 0x3F);
  //94 AD 3F ; de v beg e adj
  I2C_WRITE_BYTE (0x94, 0xAD, 0x3F);
  //94 B0 00 ;
  I2C_WRITE_BYTE (0x94, 0xB0, 0x00);
  //E0 53 10 ; ADI Recommended Write (5/13)
  I2C_WRITE_BYTE (0xE0, 0x53, 0x10);
  //E0 54 10 ; ADI Recommended Write (5/13)
  I2C_WRITE_BYTE (0xE0, 0x54, 0x10);
  //a4 b0 02 ; Set Field overlay_vid1_sel to 1
  I2C_WRITE_BYTE (0xA4, 0xB0, 0x02);
  //ac 0c 90 ; Set Field Rx color mode to 0
  I2C_WRITE_BYTE (0xAC, 0x0C, 0x90);
  //ac 00 2a ; Set Field TTL Rx Enable to 0, Powerdown LVDS Receivers to 0, Enable OLDI Receiver PLL to 1
  I2C_WRITE_BYTE (0xAC, 0x00, 0x2A);
  //ac 4c 70 ; Set Field TX color mode to 0
  I2C_WRITE_BYTE (0xAC, 0x4C, 0x70);
  //ac 40 08 ; Set Field Enable OLDI Transmiter PLL to 1, Powerdown LVDS Transmitter to 0
  I2C_WRITE_BYTE (0xAC, 0x40, 0x08);
  //AC 4C 79 ; LVDS 8bits-VESA Mapping
  I2C_WRITE_BYTE (0xAC, 0x4C, 0x79);

  //For test 9/9 : from Justin.Wu/Sloane
  //Enable independent BT656 interface out memory pins
//  I2C_WRITE_BYTE (0xC0, 0x1E, 0x01);
}

void ADV7186_RESET()
{
  //C0 FF 80 ; I2C self clearing reset
  I2C_WRITE_BYTE (0xC0,0xFF,0x80);
}