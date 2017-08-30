#include "stm32f30x_adc.h"
#include "stdio.h"
#include "main.h"
#include "gpio_control.h"
#include "i2c_slave.h"
#include "debug_port.h"

#define ADC1_SIZE 2
uint16_t ADC1_values[ADC1_SIZE];

#define ADC2_SIZE 2
uint16_t ADC2_values[ADC2_SIZE];

uint16_t CalibrationValue_1;
uint16_t CalibrationValue_2;


void ADC_Configuration(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;

  /* Configure the ADC clock */
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);

  /* ADC12 Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
  
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* Enable DMA2 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  
  /* DMA1 channel1 configuration ---------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)&ADC1_values[0];
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize          = ADC1_SIZE;
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* DMA2 channel1 configuration ---------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&ADC2->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)&ADC2_values[0];
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize          = ADC2_SIZE;
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel1, &DMA_InitStructure);

  ADC_StructInit(&ADC_InitStructure);

  /* Calibration procedure */
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
  ADC_VoltageRegulatorCmd(ADC2, ENABLE);
  
  /* Insert delay  */
  __MACRO_DELAY_mS(1);
  
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC2);

  while(ADC_GetCalibrationStatus(ADC1) != RESET );
  CalibrationValue_1 = ADC_GetCalibrationValue(ADC1);
  while(ADC_GetCalibrationStatus(ADC2) != RESET );
  CalibrationValue_2 = ADC_GetCalibrationValue(ADC2);

  /* Configure the ADC1 in continuous mode */
  ADC_CommonInitStructure.ADC_Mode              = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock             = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode     = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_DMAMode           = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay  = 10;
  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  /* Configure the ADC2 in continuous mode */
  ADC_CommonInitStructure.ADC_Mode              = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock             = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode     = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_DMAMode           = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay  = 10;
  ADC_CommonInit(ADC2, &ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ContinuousConvMode      = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution              = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent   = ADC_ExternalTrigConvEvent_0;
  ADC_InitStructure.ADC_ExternalTrigEventEdge   = ADC_ExternalTrigEventEdge_None;
  ADC_InitStructure.ADC_DataAlign               = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode             = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode             = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel         = ADC1_SIZE;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_InitStructure.ADC_ContinuousConvMode      = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution              = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent   = ADC_ExternalTrigConvEvent_0;
  ADC_InitStructure.ADC_ExternalTrigEventEdge   = ADC_ExternalTrigEventEdge_None;
  ADC_InitStructure.ADC_DataAlign               = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode             = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode             = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel         = ADC2_SIZE;
  ADC_Init(ADC2, &ADC_InitStructure);

  //configure each channel
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6,  1, ADC_SampleTime_7Cycles5);   //C0 : +V_CAR_BATT_DET
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7,  2, ADC_SampleTime_7Cycles5);   //C1 : PCB_TEMP
  
  ADC_RegularChannelConfig(ADC2, ADC_Channel_5,  1, ADC_SampleTime_7Cycles5);   //C4 : GPS_VCC_REF
  ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 2, ADC_SampleTime_7Cycles5);   //C5 : +GPS_VCC_REF

  //  /* ADC1 DMA Enable */
  ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC1, ENABLE);
  //  /* ADC2 DMA Enable */
  ADC_DMAConfig(ADC2, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC2, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);

  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  /* Enable DMA2 channel1 */
  DMA_Cmd(DMA2_Channel1, ENABLE);
  
  /* Start ADC1 Software Conversion */
  ADC_StartConversion(ADC1);
  /* Start ADC2 Software Conversion */
  ADC_StartConversion(ADC2);
}
//-----------------------------------------------------------------------------


// 更新目前之ADC
void TASK_UPDATE_DETECTED_ADC_VALUE()
{

  VAR_CURRENT_V_CB_VALUE    = ADC1_values[0];
  VAR_PCB_TEMP_VALUE        = ADC1_values[1];
  VAR_GPS_REF_VALUE         = ADC2_values[0];
  VAR_GPS_RF_VALUE          = ADC2_values[1];

  // 判斷車電是否存在
  if (VAR_CURRENT_V_CB_VALUE < 0x01ff)
  {
    if (CAR_POWER_EXIST == 1)
    {
      // 車電不存在
      DEBUG_PRINT ("@@: Turn off charger, lose car power\r\n");
      CAR_POWER_EXIST = 0;
      VAR_SYSTEM_POWER_STATUS &= 0xFE;                          // bit 0 = 0 for I2C CMD 0x09
      __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_LO;                  // 關掉Battery Charger
      // =========
      // Notify OS
      // =========
      EVENTQUEUE_INSERT_TO_QUEUE(0x02, 0x01);                   // CLASS = 0x02
                                                                // ID = 0000 0001
                                                                //              - <-- Power Lost
    }
  }
  else
  {
    if (CAR_POWER_EXIST == 0)
    {
      // 車電存在
      CAR_POWER_EXIST = 1;
      VAR_SYSTEM_POWER_STATUS |= 0x01;                          // bit 0 = 1 for I2C CMD 0x09
      if (VAR_BATTERY_EXIST == 1)                               // 電池存在
      {
        DEBUG_PRINT ("@@: Turn on charger\r\n");
        __OUT_C8_GPIO_OUT_BAT_CHARGER_EN_SET_HI;                // 開啟Battery Charger
      }
    }
  }
  
  int REF = VAR_GPS_REF_VALUE;
  int RF  = VAR_GPS_RF_VALUE;
    
  if(abs(REF-RF) < 50) VAR_GPS_ANTENNA_STATUS = 0; // GPS removed
  else if(REF<900)  VAR_GPS_ANTENNA_STATUS = 1; // GPS SHORTEN
  else VAR_GPS_ANTENNA_STATUS = 2; // Normal
}
//=============================================================================