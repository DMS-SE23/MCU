#include "project.h"
#include "stm32f30x_conf.h"
#include <stdio.h>        // for printf
#include "main.h"

void INIT_PWM_GPIO()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_2);
}

int INIT_PWM(int pwm_freq)
{
  // Calculates the timing. This is common for all channels
  int clk = 72e6; // 72MHz -> system core clock. This is default on the stm32f3 discovery
  int tim_freq = 2e6; // in Hz (2MHz) Base frequency of the pwm timer
  int prescaler = ((clk / tim_freq) - 1);

  // Calculate the period for a given pwm frequency
  //		int pwm_freq = 200; // in Hz
  int pwm_period = tim_freq/pwm_freq;		// 2MHz / 200Hz = 10000
						// For 50Hz we get: 2MHz / 50Hz = 40000

  // Calculate a number of pulses per millisecond.
  // Not used in this rutine but I put it here just as an example
  //int ms_pulses = (float)pwm_period / (1000.0/pwm_freq); // for 200Hz we get: 10000 / (1/200 * 1000) = 2000
  int ms_pulses = 2000; // for 200Hz we get: 10000 / (1/200 * 1000) = 2000

  //  Enable the TIM3 peripherie
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE );

  // Setup the timing and configure the TIM3 timer
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_Period = pwm_period - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  // Initialise the timer channels
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = ms_pulses*2; // preset pulse width 0..pwm_period
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // Pulse polarity
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

  // These settings must be applied on the timer 1.
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;

  // Setup one channel
  // Channel 1
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  // Starup the timer
  TIM_ARRPreloadConfig(TIM3, DISABLE);
  TIM_CtrlPWMOutputs(TIM3, ENABLE);
  TIM_Cmd(TIM3 , ENABLE);

  // The PWM is running now. The pulse width can be set by
  // TIM1->CCR1 = [0..pwm_period] -> 0..100% duty cycle
  //
  // For example:
  // int pulse_width = 3000;
  // TIM1->CCR1 = pulse_width;
  //
  // The firmware offers a API to do this:
  // TIM_SetCompare1(TIM1 , pulse_width); // This is a wrapper for TIM1->CCR1, the same as TIM1->CCR1=pulse_width;

  VAR_FUNCTION_KEY_PWM_PERIOD = pwm_period;
  return pwm_period;
}

void SET_PWM_WIDTH(int pwm_period, int duty_cycle)
{
  int pwm_pulses = (int)(pwm_period*(float)duty_cycle/100.0);
  TIM_SetCompare1(TIM3, pwm_pulses);    // Channel 1
}