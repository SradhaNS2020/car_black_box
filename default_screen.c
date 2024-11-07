/*
 * File:   default_screen.c
 * Author: SRADHA N S
 * Implements a default screen that shows the time, a gear shift event, and speed and store it on external eeprom
 * Created on 18 October, 2024, 6:56 PM
 */


#include <xc.h>
#pragma config WDTE=OFF
#include "main.h"

// Array of gear shift events
unsigned char *events[] = {"ON", "GN", "G1", "G2", "G3", "G4", "GR", "C "};
// Arrays for storing clock, speed, and ADC value
unsigned char clock_reg[3], speed[3] = {0};
unsigned short adc_val;
unsigned char i = 0;           // Index for events
unsigned char addr = 0x00;     // EEPROM address for storing logs
unsigned char total_logs = 0;  // Counter for total logged events
// Function to display the default screen
void default_screen(unsigned char key) 
{
        
    clcd_print ("  TIME     E  SP",LINE1(0));
    //get time from RTC
    get_time (clock_reg);
    //display_time
    display_time(clock_reg);
    //gear shifting events
    if (key == SW1)
    {
        i = 7;//collision
        //store event
        store_events(time, events[i], speed);
    }
    else if (key == SW2)
    {
        if (i!= 6)//gear change
        {
            i++;
        }
        //store event
       store_events(time, events[i], speed);

    }
    else if (key == SW3)
    {
        /*gear change*/
        if (i!=1)
        {
            i--;
        }
        //store event
        store_events(time, events[i], speed);

    }
    clcd_print (events[i],LINE2(11));
    //display the speed
        adc_val = read_adc()/10.23;
        speed[0] = (adc_val/10) + '0';
        speed[1] = (adc_val%10) + '0';
        clcd_print (speed,LINE2(14));
        
}
/*function to store events on eeprom on each gear change*/
void store_events(char *time,char* events ,char *speed)
{
    /*store upto 10 events*/
    if (total_logs < 10)
    {
        total_logs++;
    }
    /*write each event byte by byte to eeprom*/
    for (int i=0; i<8 ;i++)
    {
        write_ext_eeprom (addr++,time[i]);
    }
    for (int i=0; i<2 ; i++)
    {
        write_ext_eeprom (addr++,events[i]);
    }
    for (int i=0; i<2 ; i++)
    {
        write_ext_eeprom(addr++,speed[i]);
    }
    /*wrap around to first address on reaching 120th address*/
    if (addr == 120)
    {
        addr = 0x00;
    }
    
}