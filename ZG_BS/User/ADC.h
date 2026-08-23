#ifndef __ADC_H  // ??:__AD_H ? __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void Analog_Init(void);
uint16_t ADC_GetValue(uint8_t ADC_Channel);
uint16_t Light_GetValue(uint8_t ADC_CHANNEL);

#endif
