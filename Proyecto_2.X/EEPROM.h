#ifndef __EEPROM__
#define	__EEPROM__

#include <xc.h> 
#include <stdint.h>


uint8_t EEPROM_Read(uint8_t adress);

void EEPROM_Write(uint8_t adress, uint8_t data);


#endif