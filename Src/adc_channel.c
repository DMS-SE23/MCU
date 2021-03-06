#include "includes.h"

#define ADC1_SIZE 1
uint16_t ADC1_values[ADC1_SIZE];

uint16_t CalibrationValue_1;

#define ADC_BUFFER_SIZE     50
uint16_t ADC_History[ADC_BUFFER_SIZE];
uint16_t ADC_Sort[ADC_BUFFER_SIZE];
uint8_t ADC_first;


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

  ADC_StructInit(&ADC_InitStructure);

  /* Calibration procedure */
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
  
  /* Insert delay  */
  __MACRO_DELAY_mS(1);
  
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);

  while(ADC_GetCalibrationStatus(ADC1) != RESET );
  CalibrationValue_1 = ADC_GetCalibrationValue(ADC1);

  /* Configure the ADC1 in continuous mode */
  ADC_CommonInitStructure.ADC_Mode              = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock             = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode     = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_DMAMode           = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay  = 10;
  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ContinuousConvMode      = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution              = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent   = ADC_ExternalTrigConvEvent_0;
  ADC_InitStructure.ADC_ExternalTrigEventEdge   = ADC_ExternalTrigEventEdge_None;
  ADC_InitStructure.ADC_DataAlign               = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode             = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode             = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel         = ADC1_SIZE;
  ADC_Init(ADC1, &ADC_InitStructure);

  //configure each channel
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6,  1, ADC_SampleTime_61Cycles5);   //C0 : CURRENT_SENSE

  //  /* ADC1 DMA Enable */
  ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* Start ADC1 Software Conversion */
  ADC_StartConversion(ADC1);
  
  ADC_first= 1;  
}
//-----------------------------------------------------------------------------
int qsp(uint16_t n[], int left, int right) { 
    int i = left - 1; 
    int j, k;
    for(j = left; j < right; j++) { 
        if(n[j] <= n[right]) { 
            i++; 
            k = n[i];
            n[i] = n[j];
            n[j] = k;
        } 
    } 
    k = n[i+1];
    n[i+1] = n[right];
    n[right] = k;
    return i+1; 
} 

void qs(uint16_t n[], int left, int right) { 
    if(left < right) { 
        int q = qsp(n, left, right); 
        qs(n, left, q-1); 
        qs(n, q+1, right); 
    } 
} 

//#define USE_MIDDLE_MEAN

// 更新目前之ADC
void TASK_UPDATE_DETECTED_ADC_VALUE()
{
#ifdef USE_MIDDLE_MEAN    //  銝凋�����
    
    // check first time update
    if(ADC_first) 
    {
        ADC_first = 0;
        for(int i=0;i<ADC_BUFFER_SIZE;i++)
            ADC_History[i] = ADC1_values[0];
        
        VAR_CURRENT_SENSE_VALUE    = ADC1_values[0];
    } 
    else 
    {
        for(int i=1;i<ADC_BUFFER_SIZE;i++) {
            ADC_Sort[i-1] = ADC_History[i-1] = ADC_History[i];            
        }
        ADC_Sort[ADC_BUFFER_SIZE-1] = ADC_History[ADC_BUFFER_SIZE-1] = ADC1_values[0];
        
        qs(ADC_Sort, 0, ADC_BUFFER_SIZE-1);
        
        VAR_CURRENT_SENSE_VALUE = (ADC_Sort[ADC_BUFFER_SIZE/2-1]+ADC_Sort[ADC_BUFFER_SIZE/2])/2;
    }
    
#else // average
    
    // check first time update
    if(ADC_first) {
        ADC_first = 0;
        for(int i=0;i<ADC_BUFFER_SIZE;i++)
            ADC_History[i] = ADC1_values[0];
        
        VAR_CURRENT_SENSE_VALUE    = ADC1_values[0];
    } else {
        uint32_t total = ADC1_values[0];
        
        for(int i=1;i<ADC_BUFFER_SIZE;i++) {
            ADC_History[i-1] = ADC_History[i];
            total += ADC_History[i];
        }
        ADC_History[ADC_BUFFER_SIZE-1] = ADC1_values[0];
        
        VAR_CURRENT_SENSE_VALUE = total/ADC_BUFFER_SIZE;
    }    
#endif
}

//-----------------------------------------------------------------------------

void SHOW_ADC_INFO()
{
  static int count = 0;

  if (count++ > 10)
  {
    DEBUG_PRINT ("CURRENT_SENSE = %d ", VAR_CURRENT_SENSE_VALUE);
    DEBUG_PRINT ("\r\n");
    
    count = 0;
  }
}
//=============================================================================