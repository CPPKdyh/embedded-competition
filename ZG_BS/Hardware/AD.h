#ifndef __AD_H
#define __AD_H

void Analog_Init(void);
uint16_t ADC_GetValue(uint8_t ADC_Channel);
uint16_t Light_GetValue(uint8_t ADC_CHANNEL);
uint16_t Power_GetValue(uint8_t ADC_CHANNEL);

#endif
