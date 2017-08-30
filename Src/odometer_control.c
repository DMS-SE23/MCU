#include "stdio.h"
#include "main.h"

void Odometer_Configuration()
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_DeInit(TIM1);

  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

  TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
    
  TIM_SetCounter(TIM1, 0);
  TIM_Cmd(TIM1, ENABLE);
}

uint16_t Odometer_GetCounter(void)
{
  return TIM_GetCounter(TIM1);
}

void Odometer_ClearCounter(void)
{
  TIM_SetCounter(TIM1, 0);
}
