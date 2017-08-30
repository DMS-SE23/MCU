/******************************************************************************
  * @file    IAP/src/at_packet.c 
  * @brief   This file provides all the at command functions.
  ******************************************************************************
 */

#include "stdio.h"
#include "string.h"
#include "at_packet.h"
#include "cmd_handle.h"
#include "config.h"

uint32_t at_packet_in_count = 0;
uint8_t at_packet_in[MAX_PACKET_SIZE_IN];
uint32_t at_packet_out_count = 0;
uint8_t at_packet_out[MAX_PACKET_SIZE_OUT];



uint8_t AT_CMPLETE_PACKET_IN;
uint8_t AT_PARTIAL_PACKET_IN;
uint8_t AT_DLE_PSNT_FLAG;


//This funtion doesn't check if receive buffer contains data or not - be careful!
static  uint8_t AT_Receive_Byte (void)
{
//  return USART_ReceiveData(COM_USART);
}

//Have to wait the status (make sure the byte is sent already and return back then)
static void AT_Send_Byte (uint8_t c)
{
//  USART_SendData(COM_USART, c);
//  while (USART_GetFlagStatus(COM_USART, USART_FLAG_TXE) == RESET);
}

static void AT_SendResponse(uint8_t *buf, uint32_t length)
{
//	uint32_t i, cks=0;
//
//	AT_Send_Byte(DLE);
//	AT_Send_Byte(STX);
//
//	for(i = 0; i < length; i++)
//	{
//		if(buf[i] == DLE)			// escape DLE
//		{
//			AT_Send_Byte(DLE);
//		}
//		AT_Send_Byte(buf[i]);
//		cks ^= buf[i];
//	}
//
//	if(cks == DLE)
//	{
//		AT_Send_Byte(DLE);
//	}
//	AT_Send_Byte(cks);
//
//	AT_Send_Byte(DLE);
//	AT_Send_Byte(ETX);
}



void AT_ResetPacket(void)
{
//	AT_CMPLETE_PACKET_IN = 0;
//	AT_DLE_PSNT_FLAG = 0;
//	AT_PARTIAL_PACKET_IN = 0;
//	at_packet_in_count = 0;
}



void AT_Packet_Parsing(void)
{
//	uint8_t ch;
//	unsigned char cks = 0;
//	uint16_t i = 0;
//	
//	//When there is data stored in received buffer and it hasn't formed a complete packet.
//	while ((USART_GetFlagStatus(COM_USART, USART_FLAG_RXNE) != RESET) && !AT_CMPLETE_PACKET_IN)
//	{
//		ch = AT_Receive_Byte();
//		//		AT_Send_Byte(ch);
//		
//		if(AT_DLE_PSNT_FLAG)
//		{
//			AT_DLE_PSNT_FLAG = 0;
//					
//			if(ch == STX)									// start of packet
//			{
//				at_packet_in_count = 0;
//				AT_PARTIAL_PACKET_IN = 1;
//			}
//			else if(ch == ETX)								// end of packet
//			{
//				AT_PARTIAL_PACKET_IN = 0;
//
//				// check Checksum 
//				for(i = 0; i < at_packet_in_count; i++)
//					cks ^= at_packet_in[i];
//		
//				if( cks == 0 )
//				{
//					AT_CMPLETE_PACKET_IN = 1;
//				}
//				else
//				{
//					// checksum error
//					goto reset_packet;
//				}
//			}
//			else if(ch == DLE)								// escape DLE
//			{
//
//				AT_DLE_PSNT_FLAG = 0;
//
//				if(AT_PARTIAL_PACKET_IN)
//					goto insert_into_packet;
//			}
//			else
//			{
//				goto reset_packet;
//			}
//		}
//		else if(ch == DLE)
//		{	
//			AT_DLE_PSNT_FLAG = 1;
//		}
//		else if(AT_PARTIAL_PACKET_IN)
//		{
//			goto insert_into_packet;
//		}
//		else
//		{
//		}
//		
//		return;
//		
//	insert_into_packet:
//		at_packet_in[at_packet_in_count++] = ch;
//		if(at_packet_in_count >= MAX_PACKET_SIZE_IN)
//			goto reset_packet;	
//
//		return;
//
//	reset_packet:
//		AT_ResetPacket();
//				
//	}
			
}

void AT_MSG_Handler(void)
{
//	AT_CMD_Handler(at_packet_in, at_packet_in_count, at_packet_out, &at_packet_out_count);
//	AT_SendResponse(at_packet_out, at_packet_out_count);
//	AT_ResetPacket();
}

