#include <xc.h>
#include "main.h"

unsigned char read_ext_eeprom(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(SLAVE_READ_E);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ext_eeprom(unsigned char addr, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}
