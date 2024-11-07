/*
 * File:   logs.c
 * Author: SRADHA N S
 * This file implements functionality for viewing and clearing logs stored in EEPROM, allowing user navigation and display on a CLCD
 * Created on 23 October, 2024, 3:41 AM
 */

#include <xc.h>
#include "main.h"
#include <stdio.h>
#pragma config WDTE = OFF

// Function to view the logs stored in the EEPROM
void view_logs()
{
    unsigned char key,prekey;// Variables for keypad input and previous key pressed
    int index = 0;// Index to track the currently displayed log
    int var = 0; // Counter for SW5 long press detection

    // Check if there are any logs to view
    if (total_logs == 0)
    {
        transition();
        clcd_print("No Logs ",LINE1(0));
        clcd_print("Available",LINE2(3));
        __delay_ms(1000);
    }
    else
    {
        
        while(1)// Infinite loop to continuously display logs
        {
            display_logs(index);// Call function to display the current log
            key = read_digital_keypad(LEVEL);//read digital keypad on level detection
            if (key != ALL_RELEASED)
            {
                // If SW5 is pressed, increment var to track long press
               if (key == SW5)
               {
                   var++;
                   if (var >= 10)// If long press detected
                    {// Reset var and exit
                        var = 0;
                        return;
                    }
                   prekey = SW5;// Store the current key pressed
               }
               else if (key == SW4)
               {
                   prekey = SW4;// Store the current key pressed
               }
            }
            else// Key released
            {
                 // If the previous key was SW4, navigate up the logs
                if (prekey == SW4) 
                {
                    prekey = 0;// Reset previous key
                    var = 0;// Reset long press counter
                    if (index > 0)
                    {
                        index--;// Decrement index to view previous log
                        display_logs(index);// Display the updated log
                    }
                }
                // If the previous key was SW5, navigate down the logs
                else if (prekey == SW5)  
                {
                    prekey = 0;// Reset previous key
                    var = 0;// Reset long press counter
                    if (index < total_logs-1 )// Prevent going out of bounds  
                    {
                        index++;// Increment index to view next log
                        display_logs(index); // Display the updated log
                    }
                }
            }
        }
    }
    
}
// Function to display the log at a given index
void display_logs(int index)
{
    char read_time[9];     // Array to hold the time read from EEPROM
    char read_events[3];   // Array to hold the events read from EEPROM
    char read_speed[3];    // Array to hold the speed read from EEPROM
    unsigned char address= 0x00; // Start address for reading from EEPROM
    char str[17]; // String to hold the formatted log message
    // Loop to read log data from EEPROM
    for (int pos = 0; pos <= total_logs ; pos++)
    {
        // Check if the current position matches the index
        if (pos == index)
        {
            for (int i=0; i<8 ;i++)
            {
               read_time[i] = read_ext_eeprom(address++);
            }
            read_time[8] = '\0';
            for (int i=0; i<2 ; i++)
            {
                read_events[i] = read_ext_eeprom (address++);
            }
            read_events[2] = '\0';
            for (int i=0; i<2 ; i++)
            {
                read_speed[i] = read_ext_eeprom(address++);
            }
            read_speed[2] = '\0';
            break;// Exit the loop after reading the required log
        }
        address = address + 12;// Move to the next log's address
    }
    // Display the log information on the CLCD
    clcd_print("# TIME     E  SP", LINE1(0));
    sprintf (str,"%d %s %s %s",index,read_time,read_events,read_speed);// Format the log information
    clcd_print(str,LINE2(0));// Display formatted log on the second line
}
// Function to clear all logs   
void clear_logs()
{
    transition();
    total_logs = 0;// Reset the total logs count to 0
    addr = 0x00;
    transition();
    clcd_print ("All logs cleared",LINE1(0));
    __delay_ms(1000);
    return;
}