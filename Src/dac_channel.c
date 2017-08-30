#include "stm32f30x_adc.h"
#include "stdio.h"
#include "main.h"

void DAC2_Configuration(void)
{
  DAC_InitTypeDef DAC_InitStructure;

  DAC_StructInit(&DAC_InitStructure); // Clear up structure

  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

  DAC_Init(DAC_Channel_2, &DAC_InitStructure); // Initialize the channel

  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA5 is
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /* Set DAC Channel2 DHR12R register */
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);
}

void TASK_UPDATE_FORCE_DAC_VALUE(void)
{
  DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);

  DAC_SetChannel2Data(DAC_Align_12b_R, VAR_REFERENCE_VOLTAGE);
}