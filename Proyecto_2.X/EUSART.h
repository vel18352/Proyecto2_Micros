#ifndef __EUSART__
#define	__EUSART__

#include <xc.h> 
#include <stdint.h>


void EUSART_Init(void);

void EUSART_Enable_TX_Isr(void);

void EUSART_Enable_RX_Isr(void);

void EUSART_Disable_TX_Isr(void);

void EUSART_Disable_RX_Isr(void);

void Enviar_Texto(unsigned char*);

void Recibir_Texto(char* Output, unsigned int maxLength);

int string_to_int(char* str);

#endif