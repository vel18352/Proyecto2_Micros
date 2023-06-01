/*
Archivo:        Proyecto 2
Dispositivo:    PIC16F887
Autor:		    Emilio Velasquez 18352
Compilador:	    XC8, MPLABX 5.40
Programa:       Controlador de brazo robotico de servomotores
Hardware:	    1 kit de brazo robotico, 4 servomotores, PIC16F887, 6 leds, 3 push buttons, 4 series de headers, 4 potenciometros de 1K ohm
 */
//******************************************************************************
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = OFF      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        
// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
// Includes
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>    
#include <stdlib.h>
#include <string.h>
#include <pic16f887.h>    //Se incluyen las librerias necesarias

#include "ADC.h"
#include "Map.h"
#include "PWM.h"        
#include "EUSART.h"     
#include "EEPROM.h"      //Librerias propias
//******************************************************************************
// Definiciones
//******************************************************************************
#define _XTAL_FREQ 4000000  //Se establece el oscilador a 4.0Mhz

//******************************************************************************
// Variables
//******************************************************************************
uint8_t  TMR0_Pre = 185;           //Precarga TMR0
uint8_t  Bandera_ADC = 0;          //Bandera para rotacion de ADC
uint8_t  Selector[1];              //Variable para selector de menu
uint8_t  Flag_1 = 0;
uint8_t  Flag_2 = 1;
uint8_t  Flag_3 = 1;                //Variables para banderas de menu

unsigned short Servo1_Val = 0;
unsigned short Servo2_Val = 0;     
unsigned short Servo3_Val = 0;
unsigned short Servo4_Val = 0;     //Variables para almacenar Duty Cicle

uint8_t Servo_Pos[4];              //Variable de array para almacenar posicion de servos
//******************************************************************************
// Interrupciones
//******************************************************************************
void __interrupt() isr(void)
{      
    if (RBIF)                                               //Interrupcion PORTB    
    {
        if (!RB0)                                           //En caso de presionar boton 1
        { 
            Flag_1 = (Flag_1 + 1) % 4;
            PORTB = (PORTB & 0x0F) | (1 << (Flag_1 + 4));   //Se realiza un desplazamiento de 4 bits y se realiza un and con 0x0F para mostrar del bit 4 al 7 de indicador de menu
        }
        RBIF = 0;                                           //Limpiar interrupcion PORTB
    }
     
    if (PORTBbits.RB4 == 1)                                 //Se chequea bit del menu
    {
    if (ADIF)                                               //Chequea bandera de interrupcion de ADC
    {       
        if (Bandera_ADC == 0)                               //En caso Bandera_ADC 
        {
            Servo_Pos[0] = ADRESH;                          //Lee ADC y almacena en ADC0 la lectura del primer potenciometro            
            Servo1_Val = map(Servo_Pos[0],0,255,160,650);   //Mapeo para los rangos a los cuales trabaja el servo de PWM
            PWM_duty(1,Servo1_Val);                         //Funcion para seleccionar canal y enviar valor PWM           
            PWM_Mux_Channel_Select(0);                      //Rotar el canal del PWM
            __delay_ms(4);                                  //Terminar de completar el duty cicle
            Bandera_ADC = 1;                                //Rotar bandera ADC
        }
        else if (Bandera_ADC == 1)
        {
            Servo_Pos[1] = ADRESH;                          //Lee ADC y almacena en ADC1 la lectura del primer potenciometro
            Servo2_Val = map(Servo_Pos[1],0,255,160,650);   //Mapeo para los rangos a los cuales trabaja el servo de PWM           
            PWM_duty(1,Servo2_Val);                         //Funcion para seleccionar canal y enviar valor PWM           
            PWM_Mux_Channel_Select(1);                      //Rotar el canal del PWM
            __delay_ms(4);                                  //Terminar de completar el duty cicle
            Bandera_ADC = 2;                                //Rotar bandera ADC
        }    
        else if (Bandera_ADC == 2)
        {
            Servo_Pos[2] = ADRESH;                          //Lee ADC y almacena en ADC2 la lectura del primer potenciometro
            Servo3_Val = map(Servo_Pos[2],0,255,160,650);   //Mapeo para los rangos a los cuales trabaja el servo de PWM
            PWM_duty(1,Servo3_Val);                         //Funcion para seleccionar canal y enviar valor PWM
            PWM_Mux_Channel_Select(2);                      //Rotar el canal del PWM
            __delay_ms(4);                                  //Completar de completar el duty cicle
            Bandera_ADC = 3;                                //Rotar bandera ADC
        }
        else if (Bandera_ADC == 3)
        {
            Servo_Pos[3] = ADRESH;                          //Lee ADC y almacena en ADC3 la lectura del primer potenciometro
            Servo4_Val = map(Servo_Pos[3],0,255,160,650);   //Mapeo para los rangos a los cuales trabaja el servo de PWM
            PWM_duty(1,Servo4_Val);                         //Funcion para seleccionar canal y enviar valor PWM
            PWM_Mux_Channel_Select(3);                      //Rotar el canal del PWM
            __delay_ms(4);                                  //Terminar de completar el duty cicle
            Bandera_ADC = 0;                                //Rotar bandera ADC 
        }
        ADIF = 0;                                           //Reinicia bandera ADC
        Flag_2 = 1;
        Flag_3 = 1;                                         //Se reinician banderas para mostrar los menus
    }
    }
    
    else if (PORTBbits.RB5 == 1)                            //Se chequea bit del menu  
    {
        if (Flag_2 == 1)                                    //En caso de que flag 2 sea verdadero muestra menu en la consola
        {
            Enviar_Texto(" Menu EEPROM \n");
            Enviar_Texto("Presione Boton 2 para guardar posicion y boton 3 para llamar a posicion \n");    //Se muestra instrucciones del menu
            Flag_2 = 0;                                     //Se limpia bandera de menu
        }
        if (RBIF)                                           //Interrupcion PORTB
        {
            if (!RB1)                                       //En caso de presionar el boton 2
            { 
                Enviar_Texto("Escribiendo EEPROM \n");      //Mensaje de escribiendo en la eeprom
                PORTDbits.RD3 = 0;
                PORTDbits.RD2 = 1;                          //Enciende led de espera de escritura
                EEPROM_Write(0x00, Servo_Pos[0]);    
                EEPROM_Write(0x01, Servo_Pos[1]);
                EEPROM_Write(0x02, Servo_Pos[2]);
                EEPROM_Write(0x03, Servo_Pos[3]);           //Guarda la ultima posicion de los potencimetros en la eeprom
                PORTDbits.RD3 = 1;
                PORTDbits.RD2 = 0;                          //Cambia led para confirmar que se realizo guardado
                Enviar_Texto("Datos Guardados \n");         //Muestra mensaje de guardado 
                Flag_2 = 1;                                 //Activa bandera de menu
                RBIF = 0;                                   //Limpia interrupcion PORTB
            }
            if (!RB2)                                       //En caso de presionar boton 3
            {
                Enviar_Texto("leyendo EEPROM \n");          //Muestra texto de lectura
                PWM_duty(1,map(EEPROM_Read(0x00),0,255,160,650));   //Carga dato de la eeprom del potenciometro, realiza un mapeo para enviar los valores al CCP y genera el PWM
                PWM_Mux_Channel_Select(0);                          //Rota el canal del PWM
                __delay_ms(350);                                    //Realiza un delay para completar el duty cicle
                PWM_duty(1,map(EEPROM_Read(0x01),0,255,160,650));   //Realiza el mismo proceso para los demas servomotores
                PWM_Mux_Channel_Select(1);
                __delay_ms(350);
                PWM_duty(1,map(EEPROM_Read(0x02),0,255,160,650));
                PWM_Mux_Channel_Select(2);
                __delay_ms(350);
                PWM_duty(1,map(EEPROM_Read(0x03),0,255,160,650));   
                PWM_Mux_Channel_Select(3);
                __delay_ms(350);        
                Flag_2 = 1;                                         //Activa bandera de menu
                RBIF = 0;                                           //Limpia interrupcion PORTB
            }
        }         
    }
     
    else if (PORTBbits.RB6 == 1)                                    //Chequea bit del menu
    {
    if (Flag_3 == 1)                                                //En caso de ser flag 3 verdadero muestra texto del menu
    {   
        Enviar_Texto("Menu UART \na. Recibir comandos, b. Salir modo Uart\n");  //Muestra instrucciones del menu
        Flag_3 = 0;                                                 //Limpia bandera del menu
    }    
    Recibir_Texto(Selector,1);                                      //Recibe el texto para seleccionar opcion del menu
            if (Selector[0] == 0x61)                                //En caso de ser "a" 
            {       
                int numbers[4];                                     //Crea un array temporal para almacenar angulos        
                Enviar_Texto("Ingrese Angulos: \n");                //Muestra texto
                for(int i = 0; i < 4; i++)                          //Loop for para pedir angulos de servomotores
                {
                    char buffer[50];                                //Buffer para recibir caracteres de UART
                    Recibir_Texto(buffer, 3);                       //Recibe y almacena caracteres
                    numbers[i] = string_to_int(buffer);             //Funcion para convertir caracteres numericos a un entero
                    char numStr[12];                                //Array temporal para almacenar valor numerico de hasta 32 bits
                    itoa(numStr, numbers[i], 10);                   //Convertir int to string para mostrar valores ingresados anteriormente
                    Enviar_Texto(numStr);                           //Muestra valor numerico 
                    Enviar_Texto("\n");                             //Salto de linea
                }               
                PWM_duty(1,map(numbers[0],0,180,160,650));          //Envia valor del angulo ingresado anteriormente y realiza un mapeo al CCP
                PWM_Mux_Channel_Select(0);                          //Rota canal del PWM
                __delay_ms(500);                                    //Espera a que se complete el duty cicle
                PWM_duty(1,map(numbers[1],0,180,160,650));          //Repite el mismo procedimiento para los demas servomotores
                PWM_Mux_Channel_Select(1);
                __delay_ms(500);
                PWM_duty(1,map(numbers[2],0,180,160,650));
                PWM_Mux_Channel_Select(2);
                __delay_ms(500);
                PWM_duty(1,map(numbers[3],0,180,160,650));
                PWM_Mux_Channel_Select(3);
                __delay_ms(500);

                Enviar_Texto("Listo\n");                            //Muestra mensaje de confirmacion 
                RCIF = 0;                                           //Limpia bandera de interrupcion
                Flag_3 = 1;                                         //Activa menu
            }                   
            else if (Selector[0] == 0x62)                           //En caso de ser "b"
            {
            Flag_1 = (Flag_1 + 1) % 4;
            PORTB = (PORTB & 0x0F) | (1 << (Flag_1 + 4));           //Sale del menu de UART y rota el bit del menu
            }            
    }    
}

//******************************************************************************
// Configuraciones
//******************************************************************************
void Setup(void)
{
    ANSEL  = 0x0F;      //Enciende primeros 4 canales del ADC
    ANSELH = 0x00;   
    TRISA = 0x0F;       //Establece primeros dos bits de PORTA como entrada
    PORTA = 0;          //Limpia PORTA
    TRISB = 0X07;       //Establece 4 bits como salida y 4 como entrada de PORTB
    PORTB = 0X10;       //Enciende bit4 de PORTB como selector de menu
    IOCB = 0X07;        //Pull UP
    TRISC = 0;          //Establece PORTC como salida
    PORTC = 0;          //Limpia PORTC
    TRISD = 0;          //Establece PORTD como salida
    PORTD = 0x08;       //Enciende bit3 de PORTD para led de EEPROM
    TRISE = 0;          //Establece PORTE como salida
    PORTE = 0;          //Limpia PORTE
    
    OPTION_REGbits.nRBPU = 0;   //Habilita Pull Up
    
    OSCCONbits.SCS = 1;         //Oscilador interno
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;       //Oscilador a 4Mhz
}

void Int_Enable(void)
{
    T0IF = 0;   //Limpia interrupcion TMR0
    T0IE = 0;   //Deshabilita interrupcion TMR0
    
    RBIF = 0;   //Limpiar interrupcion PORTB
    RBIE = 1;   //Habilitar interrupcion PORTB
    
    PEIE = 1;   //Habilitar interrupciones Perifericas
    GIE = 1;    //Habilitar interrupciones Globales
}

//******************************************************************************
// Subrutinas
//******************************************************************************
void ADC_Change(void)                           //Conmutar canal el cual se le hara lectura de ADC para ambos potenciometros
{
    if (ADCON0bits.GO)                          //Se chequea que no este ocurriendo una lectura de ADC
    {           
        return;
    }
    else if (Bandera_ADC == 0)                  //En caso Bandera_ADC sea verdadero 
    {
        ADC_Select(0);                          //Se selecciona el Canal 0 para la lectura de ADC         
    }
    else if (Bandera_ADC == 1)
    {
        ADC_Select(1);                          //Se selecciona el Canal 1 para la lectura de ADC       
    }        
    else if (Bandera_ADC == 2)
    {
        ADC_Select(2);                          //Se selecciona el Canal 2 para la lectura de ADC         
    }
        else if (Bandera_ADC == 3)
    {
        ADC_Select(3);                          //Se selecciona el Canal 3 para la lectura de ADC    
    }
    while(ADCON0bits.GO);                       //Espera a terminar la conversion ADC
    ADCON0bits.GO = 1;                          //Inicia la conversion ADC
}


//******************************************************************************
// Main Loop
//******************************************************************************
void main(void) 
{
    Setup();                            //Carga configuracion de puertos
    Int_Enable();                       //Carga configuracion de interrupciones
  
    PWM_config(1,4);                    //Carga configuracion de CCP, enciende CCP1 a 4ms
    PWM_Enable_Mux();                   //Habilita el multiplexado interno de PWM
    
    ADC_Int(0);                         //Habilita ADC justificado a la izquierda
    ADC_Isr_Enable();                   //Habilta interrupciones ADC
     
    EUSART_Init();                      //Inicia y configura UART
    EUSART_Disable_TX_Isr();            
    EUSART_Disable_RX_Isr();            //Deshabilita interrupciones UART
   
    while(1)                            //Loop principal
    {
        ADC_Change();                   //Llama a la funcion de conmutar el canal de ADC
    }
    return;
}