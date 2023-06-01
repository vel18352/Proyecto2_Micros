#ifndef __ADC__

#include <xc.h>

void ADC_Int(uint8_t ADFM_Select);

void ADC_Select(int channel);

void ADC_Isr_Enable(void);

void ADC_Isr_Disable(void);
#endif 