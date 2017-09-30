/**
  ******************************************************************************
  * @file    debug_port.c
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************

  ******************************************************************************
  */

#include "includes.h"

// 定義Debug Port
// ==========================================
// Using PC4 & PC5 for Debug Port
//#define USING_USART1_DEBUG_PORT
// ==========================================

void DEBUG_PORT_INITIALIZATION()
{
  USART_InitTypeDef USART_InitStructure;        // this is for the GPIO pins used as TX and RX
  GPIO_InitTypeDef GPIO_InitStructure;          // this is for the USART1 initilization
  // USART1 Interrupt設定用之定義
  NVIC_InitTypeDef NVIC_InitStructure;          // this is used to configure the NVIC (nested vector interrupt controller)

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  // UART1 Initial
  // Configure USART1 Tx (PC.4) as alternate function push-pull
  // Configure USART1 Rx (PC.5) as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_7);

  /* Configure USART1 pins:  --------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_DeInit(USART1);

  // DEBUG USARTx 配置
  // 115200, N/8/1
  USART_InitStructure.USART_BaudRate            = 115200;
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;                  //Word Length = 8 Bits
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;                     //One Stop Bit
  USART_InitStructure.USART_Parity              = USART_Parity_No;                      //No parity
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;       //Hardware flow control disabled (RTS and CTS signals)
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;        //Receive and transmit enabled
  USART_Init(USART1, &USART_InitStructure);                                             // Configure UART1

  // 啟動 DEBUG USARTx
  USART_Cmd(USART1, ENABLE);                                                            // Enable USART1

  /* Here the USART1 receive interrupt is enabled
   * and the interrupt controller is configured
   * to jump to the USART1_IRQHandler() function
   * if the USART1 receive interrupt occurs
   */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                        // enable the USART1 receive interrupt

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                                     // we want to configure the USART1 interrupts
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                             // this sets the priority group of the USART1 interrupts
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                                    // this sets the subpriority inside the group
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       // the USART1 interrupts are globally enabled
  NVIC_Init(&NVIC_InitStructure);                                                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

  // finally this enables the complete USART1 peripheral
  USART_Cmd(USART1, ENABLE);
}

int dbg_fputc(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
  return ch;
}

void DEBUG_PRINT(const char *fmt, ...)
{
  #define MAX_DBG_MSG_LEN (128)
  char buf[MAX_DBG_MSG_LEN];
  int length;                                   // 要印出之字串長度
  int idx;                                      // index用
  va_list ap;
  va_start(ap, fmt);
  length = vsnprintf(buf, sizeof buf, fmt, ap);
  va_end(ap);
  // Output Debug String
  for (idx = 0; idx < length; idx++)
  {
    dbg_fputc(buf[idx]);
  }
}


// 這是Heart Beat LED，要掛在每一秒的Task中
void HEART_BEAT_LED_CONTROL_TASK()
{
  static int current_state = 0;
  if (current_state)
  {
    __OUT_C3_GPIO_OUT_DBG_GPIO_SET_HI;
    current_state = 0;
  }
  else
  {
    __OUT_C3_GPIO_OUT_DBG_GPIO_SET_LO;
    current_state = 1;
  }
}

u8 DEBUG_hasbyte()
{
    if(DEBUG_RxS==DEBUG_RxE)
      return 0;
    return 1;
}

u8 DEBUG_getchar(u8 *data)
{
    if ( DEBUG_hasbyte() )
    {
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // 禁止中斷
        *data = DEBUG_RxBuffer[DEBUG_RxS++];
        if(DEBUG_RxS==RxBufferSize)
            DEBUG_RxS = 0;
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 恢復中斷
        return 1;
    }
    return 0;
}

void Check_DEBUG_Command()
{
  u8 data;
  if (!VAR_DEBUG_PACKET_READY  && DEBUG_getchar(&data))
  {
    if (data == 0x0d)
    {
      DEBUG_Cmd[VAR_DEBUG_CMD_LENGTH++] = 0x0d;
      VAR_DEBUG_PACKET_READY  = 1;
    }
    else
    {
      DEBUG_Cmd[VAR_DEBUG_CMD_LENGTH++] = data&0xff;
    }

    //Error Protection, avoid memory overflow
    if (VAR_DEBUG_CMD_LENGTH == DEBUG_CmdSize)
    {
      VAR_DEBUG_CMD_LENGTH = 0;
    }
  }
}

// 處理DEBUG Command 程式進入點 (Command Mode)
void DEBUG_Command_Processing()
{
  if (DEBUG_Cmd[0] == 0x0d) // 直接按Enter
  {
    return;
  }
  // ------------------------------------------------------
  /* 都不認識了的Commands (Command Mode)-------------------*/
  DEBUG_PRINT ("DBG_ERR-C01\r\n"); // Unknown Command in Command Mode
}