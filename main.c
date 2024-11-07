/*
 * File:   main.c
 * Author:SRADHA N S
 * This file sets up system modules and switches screens based on keypad input 
 * Created on 17 October, 2024, 7:31 PM
 */


#include <xc.h>
#include "main.h"
#pragma config WDTE=OFF// Disable Watchdog Timer

unsigned int operation_flag = 1; // Start with default screen active
char pass[5]; // Array to store the password
/* Function to initialize all required configurations*/
void init_config(void) 
{
    init_adc();              // Initialize ADC module
    init_clcd();             // Initialize CLCD display
    init_digital_keypad();   // Initialize digital keypad
    init_i2c(100000);        // Initialize I2C communication with 100kHz speed
    init_ds1307();           // Initialize DS1307 RTC (Real-Time Clock)
    GIE = 1;                 // Enable global interrupts
    PEIE = 1;                // Enable peripheral interrupts
    init_timer2();           // Initialize Timer2
    init_uart(9600);         // Initialize UART with 9600 baud rate
}

void main(void)
{
    init_config();
    /*to write default password from eeprom */
    unsigned char addr = 0x78;
    write_ext_eeprom(addr++,'1');
    write_ext_eeprom(addr++,'1');
    write_ext_eeprom(addr++,'1');
    write_ext_eeprom(addr,'1');
    unsigned char key;
    read_password();//retrieve stored password
    while (1)
    {
        /*read digital keypad on state detection*/
        key = read_digital_keypad(STATE);
        // Check keypress and set the appropriate operation flag
        if (key == SW1 || key == SW2 || key == SW3 )
        {
            operation_flag = 1;// Set flag for default screen
        }
        else if (key==SW4)
        {
            operation_flag = 2;// Set flag for login screen
        }
         // Screen handling based on the operation flag
        if (operation_flag == 1)
        {
            default_screen(key); // Show default screen
        }
        else if (operation_flag == 2)
        {
            //login screen
            login_screen();
            transition();
        }
        else if (operation_flag == 3)
        {
            //menu screen
            menu_screen();
            transition();
        }
    }
}
// Function to clear the display during screen transition
void transition()
{
    clcd_print("                ", LINE1(0)); // Clear previous message
    clcd_print("                ", LINE2(0)); // Clear previous message
}