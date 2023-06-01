#include <xc.h>

void EUSART_Init(void)
{ 
    SPBRGH = 0;
    SPBRG = 25;                 //Se establece a 9600 baudrate a 4Mhz de velocidad de oscilador
    
    RCSTAbits.SPEN = 1;         //Se habilitan los pines para comunicacion UART
    
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;         //Habilitar transmisor y receptor
    
    TXSTAbits.SYNC = 0;         //Se establece el modo asíncrono de comunicación
    TXSTAbits.BRGH = 1;         //Se establece el modo de alta velocidad de transmisión
    TXSTAbits.SENDB = 1;        //Habilita la detección de bits de sincronización adicionales en la transmisión de datos
    
    TXSTAbits.TX9 = 0;
    RCSTAbits.RX9 = 0;          //8 bits de envio y recepcion de datos
}

void EUSART_Enable_TX_Isr(void)
{
    PIE1bits.TXIE = 1;          //Habilita interrupciones TX UART
}

void EUSART_Enable_RX_Isr(void)
{
    PIE1bits.RCIE = 1;         //Habilita interrupciones RX UART
}

void EUSART_Disable_TX_Isr(void)
{
    PIE1bits.TXIE = 0;        //Desabilitar interrupciones TX UART
}

void EUSART_Disable_RX_Isr(void)
{
    PIE1bits.RCIE = 0;          //Desabilitar interrupciones RX UART
}

void Enviar_Texto(unsigned char *text)          //Convertir una cadena de caracteres para enviar por UART
{
    while (*text != '\0')                       //Mientras el texto no se termine
    {       
    while (!TXIF);                              //Mientras la bandera no este activa
    TXREG = *text;                              //Envia caracter por TX
    *text++;                                    //Aumenta la posicion del caracter del string
    }    
}

void Recibir_Texto(char* Output, unsigned int maxLength)
{
    unsigned int i;
    for(i=0; i<maxLength; i++)
    {
        while(!RCIF);           //Espera a interrupcion de caracter de entrada
        Output[i] = RCREG;      //Lee data recibida

        if(Output[i] == '\r')   //Espera un return para terminar string de lectura
            break;
    }
    Output[i] = '\0';           //Busca caracter de finalizacion
}

int string_to_int(char* str) 
{
    return atoi(str);           //Funcion de C para convertir string numerico a integer
}