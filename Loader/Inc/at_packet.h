#ifndef _AT_PACKET_H
#define _AT_PACKET_H

/* Exported constants --------------------------------------------------------*/
/* Constants used by AT Command Usage */
#include "stm32f30x.h"


#define MAX_PACKET_SIZE_IN		1036
#define MAX_PACKET_SIZE_OUT		20

extern uint32_t at_packet_in_count;
extern uint8_t at_packet_in[MAX_PACKET_SIZE_IN];
extern uint32_t at_packet_out_count;
extern uint8_t at_packet_out[MAX_PACKET_SIZE_OUT];


extern uint8_t AT_CMPLETE_PACKET_IN;	// =1 when a complete packet is received.
extern uint8_t AT_PARTIAL_PACKET_IN;	// =1 when packet transmission in between.
extern uint8_t AT_DLE_PSNT_FLAG;		// = 1 when it is a DLE byte.


#define DLE		0x10
#define STX		0x02
#define ETX		0x03

void AT_Packet_Parsing(void);
void AT_MSG_Handler(void);

#endif

