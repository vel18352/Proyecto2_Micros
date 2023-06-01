
#include "PWM.h"

void PWM_config(char canal, int periodo)
{
    CCP1CON = 0;  
    CCP2CON = 0;                        //Apagar ambios CCP
    
    switch (periodo)
    {
        case 2:
            PR2 = 125;
            break;
        case 3:
            PR2 = 188; 
            break;
        case 4:
            PR2 = 250; 
            break;
        default:
            PR2 = 63;                  
    }                                   //Se selecciona entre 3 casos de 2, 3 y 4 ms para periodo y un default de 1 ms
    
    if (canal == 1)                     //Configuracion CCP1
    {
        CCP1CONbits.P1M = 0;            // Modo sigle output
        CCP1CONbits.CCP1M = 0b1100;     // PWM        
    }
    else if (canal == 2)                //Configuracion CCP2
    {
        CCP2CONbits.CCP2M = 0b1100;     // PWM
    }
    else if (canal == 3)                //Configuracion para encender ambos
    {
        CCP1CONbits.P1M = 0;            // Modo sigle output
        CCP1CONbits.CCP1M = 0b1100;     // PWM
        CCP2CONbits.CCP2M = 0b1100;     // PWM
    }
        PIR1bits.TMR2IF = 0;            // Bandera TMR2    
        T2CONbits.T2CKPS = 0b11;        // Prescaler 1:16
        T2CONbits.TMR2ON = 1;           // Iniciar TMR2
        while (!PIR1bits.TMR2IF);       // Esperar un ciclo del TMR2
        PIR1bits.TMR2IF = 0;
}


void PWM_duty(char canal, unsigned short duty)
{
        if (canal == 1)
        {
            CCPR1L = duty>>2;               //Shift 2 a la derecha duty cicle
            CCP1CONbits.DC1B = duty & 3;    //Realiza un and con los dos bits menos significativos y envia 
        }
        if (canal == 2)
        {
            CCPR2L = (duty>>2);              //Shift 2 a la derecha duty cicle
            CCP2CONbits.DC2B0 = duty & 0b01; 
            CCP2CONbits.DC2B1 = duty & 0b10; //Realiza un and con los dos bits menos significativos y envia 
        }
        return;
}

void PWM_Enable_Mux()
{
    PSTRCONbits.STRSYNC = 1;                //Habilita bit de sincronizacion para multiplexado
}

void PWM_Disable_Mux()
{
    PSTRCONbits.STRSYNC = 0;                //deshabilita bit de sincronizacion para multiplexado
}

void PWM_Mux_Channel_Select(int channel)    //Seleccionar canal el cual funcionara como salida del PWM
{
    if (channel == 0)
    {
        PSTRCONbits.STRA = 1;               //Enciende P1A, Apaga P1B, P1C y P1D
        PSTRCONbits.STRB = 0;
        PSTRCONbits.STRC = 0;
        PSTRCONbits.STRD = 0;
    }
    else if (channel == 1)
    {
        PSTRCONbits.STRA = 0;               //Enciende P1B, Apaga P1A, P1C y P1D
        PSTRCONbits.STRB = 1;
        PSTRCONbits.STRC = 0;
        PSTRCONbits.STRD = 0; 
    }
    else if (channel == 2)
    {
        PSTRCONbits.STRA = 0;               //Enciende P1C, Apaga P1A, P1B y P1D
        PSTRCONbits.STRB = 0;
        PSTRCONbits.STRC = 1;
        PSTRCONbits.STRD = 0;
    }
    else if (channel == 3)
    {
        PSTRCONbits.STRA = 0;               //Enciende P1D, Apaga P1A, P1B y P1C
        PSTRCONbits.STRB = 0;
        PSTRCONbits.STRC = 0;
        PSTRCONbits.STRD = 1;
    }
}