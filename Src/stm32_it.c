/**
  ******************************************************************************
  * @file    stm32_it.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c_slave.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_ADDR))
  {
    Rx_Idx2 = 0;
    I2C_ClearFlag(I2C2, I2C_FLAG_ADDR);
  }
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS))
  {
    I2C_Slave_TX_UserCallback();
    I2C2->TXDR = Buffer_Tx2;
  }
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE))
  {
    uint8_t temp;
    temp = (uint8_t)I2C2->RXDR;
    Buffer_Rx2[Rx_Idx2++] = temp;
  }
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_NACKF))
  {
    I2C_ClearFlag(I2C2, I2C_FLAG_NACKF);
  }
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
  {
    /* In order to clear TXIS flag, it need to disable I2C2 peripheral.
       Otherwise it will cause data shift*/
    I2C2->CR1 &= ~I2C_CR1_PE;
    I2C2->CR1 |= I2C_CR1_PE;
    I2C_Slave_STOP_UserCallback();
    I2C_ClearFlag(I2C2, I2C_FLAG_STOPF);
  }
}


/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{

    /*** Not Acknowledge received flag ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_NACKF) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_NACKCF);

    /*** Bus error ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_BERR) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_BERRCF);

    /*** Arbitration lost ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_ARLO) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_ARLOCF);

    /*** Overrun/Underrun ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_OVR) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_OVRCF);

    /*** PEC Error in receiption ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_PECERR) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_PECCF);

    /*** Timeout or tlow detection flag ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_TIMEOUT) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_TIMOUTCF);

    /*** SMBus alert ***/
    if(I2C_GetFlagStatus(I2C2, I2C_ISR_ALERT) == SET)
        I2C_ClearFlag(I2C2, I2C_ICR_ALERTCF);

    I2C_DeInit(I2C2);
}

/*******************************************************************************
* Function Name  : USART3_Handler
* Description    : This function handles USART3 exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void) //USART3_IRQHandler()
{
//  // Handle OverRun
//  if(USART_GetFlagStatus(USART3, USART_FLAG_ORE)==SET)
//  {
//	  USART_ClearFlag(USART3, USART_FLAG_ORE);
//      USART_ReceiveData(USART3);
//  }
//
//  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//  {
//      /* Read one byte from the receive data register */
//      API_RxBuffer[API_RxE++] = (USART_ReceiveData(USART3) & 0xFF);
//      if(API_RxE==RxBufferSize)
//          API_RxE = 0;
//
//      if((API_RxS+RxBufferSize-1)%RxBufferSize==API_RxE)  // check buffer overflow
//      {
//          /* Disable the Receive interrupt */
//          USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//      }
//  }
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
      /* Read one byte from the receive data register */
      DEBUG_RxBuffer[DEBUG_RxE++] = (USART_ReceiveData(USART3) & 0xFF);
      if(DEBUG_RxE==RxBufferSize)
          DEBUG_RxE = 0;

      if((DEBUG_RxS+RxBufferSize-1)%RxBufferSize==DEBUG_RxE)  // check buffer overflow
      {
          /* Disable the Receive interrupt */
          USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
      }
  }

  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {
      if(DEBUG_TxS==DEBUG_TxE)
      {
          /* Disable the EVAL_COM1 Transmit interrupt */
          USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      }
      else
      {
          /* Write one byte to the transmit data register */
          USART_SendData(USART3, DEBUG_TxBuffer[DEBUG_TxS++]);
          if(DEBUG_TxS==TxBufferSize)
              DEBUG_TxS = 0;
      }
  }
}

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  DEBUG_PRINT("HardFault\r\n");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  DEBUG_PRINT("MemManage\r\n");
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  DEBUG_PRINT("BusFault\r\n");
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  DEBUG_PRINT("UsageFault\r\n");
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
  VAR_1mS_Event = 1;
  VAR_5mS_Counter++;
  VAR_10mS_Counter++;
  VAR_25mS_Counter++;
//  VAR_50mS_Counter++;
  VAR_100mS_Counter++;
  VAR_1S_Counter++;

//  TimingDelay--;

  if (VAR_TIMETICK_DELAY != 0)
  {
    VAR_TIMETICK_DELAY--;
  }
}

/******************************************************************************/
/*            STM32 Peripherals Interrupt Handlers                        */
/******************************************************************************/

/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//  USB_Istr();
//}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
  /* Test on DMA1 Channel1 Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA1_IT_TC1))
  {
    /* Clear DMA1 Channel1 Transfer Complete pending bit */
    DMA_ClearITPendingBit(DMA1_IT_TC1);
  }
}

/*******************************************************************************
* Function Name  : EXTI_IRQHandler
* Description    : This function handles External lines interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// PA1 for Ignition
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

// PA5 for Power Button
// PA7 for Alarm
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
  if(EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
}
#if !defined(STM32L1XX_MD) &&  !defined(STM32L1XX_HD) && !defined(STM32L1XX_MD_PLUS)&& ! defined (STM32F37X) && ! defined (STM32F30X)
/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
}

#endif /*STM32L1XX_HD*/

/*******************************************************************************
* Function Name  : USB_FS_WKUP_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}
/******************************************************************************/
/*                 STM32 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32xxx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
