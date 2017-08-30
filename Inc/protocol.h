#ifndef _PROTOCOL_H
#define _PROTOCOL_H

extern u8 TxBuffer[TxBufferSize];
extern u8 RxBuffer[RxBufferSize];
extern u16 TxS, TxE; // Tx Start, End Ptr
extern u16 RxS, RxE; // Rx Start, End Ptr

extern unsigned char Get_Platform_String(int type, int index);

// 檢查AT Command
extern void Check_AT_Command(void);
// ==================================================================
// COMMAND MODE
extern void AT_Command_Processing(); // 處理AT Command 程式進入點 (Command Mode)
// ==================================================================
// DATA MODE
extern void AT_Data_Processing(void); // 處理AT Command 程式進入點 (Data Mode)
//
//extern void MSG_TO_HOST(int i); // 傳送Message給MAIN CPU via UART1

#endif