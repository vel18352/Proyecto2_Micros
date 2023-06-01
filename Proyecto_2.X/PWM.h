
#ifndef __PWM__

#include <xc.h>

void PWM_config(char canal, int periodo);

void PWM_duty(char canal, unsigned short duty);

void PWM_Enable_Mux(void);

void PWM_Disable_Mux(void);

void PWM_Mux_Channel_Select(int channel);

#endif /* __PWM__ */