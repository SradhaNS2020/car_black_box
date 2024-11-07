/* 
 * File:   ds1307.h
 */

#ifndef EXTERNAL_EEPROM_H
#define	EXTERNAL_EEPROM_H

#define SLAVE_WRITE_E                 0xA0
#define SLAVE_READ_E                  0xA1

#define EE                     0X00;

unsigned char read_ext_eeprom(unsigned char addr);
void write_ext_eeprom(unsigned char addr, unsigned char data);

#endif	/* DS1307_H */

