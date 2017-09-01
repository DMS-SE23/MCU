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
  uint32_t idx = Tx_Idx, len = Tx_Len;  
  
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_ADDR))
  {
    Rx_Idx = 0;
    I2C_ClearFlag(I2C2, I2C_FLAG_ADDR);
  }
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS))
  {
    if(idx < len) {
      I2C2->TXDR = Buffer_Tx[Tx_Idx++];
    } else {
      I2C2->TXDR = 0xFF;
    }
  }
  if (I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE))
  {
    uint8_t temp;
    temp = (uint8_t)I2C2->RXDR;
    Buffer_Rx[Rx_Idx++] = temp;
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
* Function Name  : USART1_Handler
* Description    : This function handles USART1 exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
      /* Read one byte from the receive data register */
      DEBUG_RxBuffer[DEBUG_RxE++] = (USART_ReceiveData(USART1) & 0xFF);
      if(DEBUG_RxE==RxBufferSize)
          DEBUG_RxE = 0;

      if((DEBUG_RxS+RxBufferSize-1)%RxBufferSize==DEBUG_RxE)  // check buffer overflow
      {
          /* Disable the Receive interrupt */
          USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
      }
  }

  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
      if(DEBUG_TxS==DEBUG_TxE)
      {
          /* Disable the EVAL_COM1 Transmit interrupt */
          USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      }
      else
      {
          /* Write one byte to the transmit data register */
          USART_SendData(USART1, DEBUG_TxBuffer[DEBUG_TxS++]);
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
* Function Name  : EXTI_IRQHandler
* Description    : This function handles External lines interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// PA0 for Power Button
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
