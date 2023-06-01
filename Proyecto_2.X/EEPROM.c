
#include <xc.h>


uint8_t EEPROM_Read(uint8_t adress)                 //Leer eeprom
{
    while(EECON1bits.RD || EECON1bits.WR);          //Espera a que termine la lectura anterior
    EECON1bits.EEPGD = 0;                           //Selecciona para leer en data memory
    EEADR = adress;                                 //Selecciona cual banco de memoria se leera
    EECON1bits.RD = 1;                              //Inicia la lectura
    return EEDAT;                                   //Retorna valor del banco el cual se efectuo la lectura  
}


void EEPROM_Write(uint8_t adress, uint8_t data)     //Escribir en eeprom
{
    while(EECON1bits.WR);                           //Espera a que termine la escritura anterior
    EECON1bits.EEPGD = 0;                           //Selecciona escribir en data memory
    EECON1bits.WREN = 1;                            //Habilitar escritura de la eeprom
    EEADR = adress;                                 //Lee el adress en el cual se escribira
    EEDAT = data;                                   //Lee la data que se escribira
    
    INTCONbits.GIE = 0;                             //Deshabilitar interrupciones globales
    EECON2 = 0x55;
    EECON2 = 0xAA;                                  //Secuencia de flasheo de memoria

    EECON1bits.WR = 1;                              //Inicia escritura

    INTCONbits.GIE = 1;                             //Habilita interrupciones globales
    EECON1bits.WREN = 0;                            //Deshabilita la escritura de eeprom
    INTCONbits.RBIF = 0;                            //Limpia interrupcion PORTB
}