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
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


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
// 目前設定每1mS進入SysTick之中斷一次
void SysTick_Handler(void)
{

    VAR_1mS_Event = 1;
    VAR_10mS_Counter++;                                     // 每10mSEvent

    VAR_TIMETICK_DELAY++;
    if (VAR_TIMETICK_DELAY == 300)
    {
        if (__MACRO_CHECK_IS_LED_ON)
        {
            __OUT_E2_GPIO_OUT_LED_PWR_G_SET_LO;
        }
        else
        {
            __OUT_E2_GPIO_OUT_LED_PWR_G_SET_HI;
        }
        VAR_TIMETICK_DELAY = 0;
    }
}


/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{

    uint8_t tmpvalue;

    if(I2C_GetFlagStatus(I2C2, I2C_FLAG_ADDR))
    {
        Tx_Idx2 = 0;
        Rx_Idx2 = 0;
        I2C_ClearFlag(I2C2, I2C_FLAG_ADDR);
    }

    if(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS))
    {
        tmpvalue = Buffer_Tx2[Tx_Idx2++];
        I2C2->TXDR = tmpvalue;
    }

    if(I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE))
    {
        tmpvalue = (uint8_t)I2C2->RXDR;
        Buffer_Rx2[Rx_Idx2++] = tmpvalue;
    }

    if(I2C_GetFlagStatus(I2C2, I2C_FLAG_NACKF))
    {
        I2C_ClearFlag(I2C2, I2C_FLAG_NACKF);
    }

    if(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
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
