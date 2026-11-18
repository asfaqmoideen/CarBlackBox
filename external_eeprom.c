/*
 * File:   external_eeprom.c
 * Author: Asfaq
 *
 * Created on 25 May, 2023, 10:46 AM
 */


#include <xc.h>
#include "external_eeprom.h"
#include "i2c.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
    for (unsigned int wait = 3000;wait--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_rep_start();
    i2c_write(SLAVE_READ_E);
    data = i2c_read(1);
    i2c_stop();

    return data;
}

void eeprom_write_string(unsigned char memory_loc, char *data)
{
    while(*data != '\0')
    {
        write_external_eeprom(memory_loc, *data);
        data++;
        memory_loc++;
        
    }
}

