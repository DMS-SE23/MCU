#ifndef __ADC_CHANNEL_H
#define __ADC_CHANNEL_H

void ADC_Configuration(void);

// 掛在100mS下，偵測目前之ADC出來之值
void TASK_UPDATE_DETECTED_ADC_VALUE(void);
void SHOW_ADC_INFO(void);

#endif