/*
 * File:   download.c
 * Author: SRADHA N S
 * This file provides functionality to download logs from EEPROM and send them via UART
 * Created on 25 October, 2024, 1:04 PM
 */

#include <xc.h>
#include <stdio.h>
#include "main.h"
#pragma config WDTE=OFF

// Function to download logs from EEPROM and send them via UART
void download_logs()
{
    // Check if there are no logs to download
    if (total_logs == 0)
    {
        transition(); 
        clcd_print("No logs ", LINE1(0)); // Display message indicating no logs
        clcd_print("Available",LINE2(2)); 
        __delay_ms(1000); 
        return; // Exit the function if no logs
    }
    else
    {
        char read_time[9]; // Buffer to read time from EEPROM
        char read_events[3]; // Buffer to read events from EEPROM
        char read_speed[3]; // Buffer to read speed from EEPROM
        unsigned char address = 0x00; // Starting address in EEPROM
        char str[17]; // Buffer for formatted log string
        int index = 0; // Index to track the current log entry
        
        // Print header for log data over UART
        puts_uart("# TIME     E  SP\n");
        // Loop through all logs to read and send them
        while (index < total_logs)
        {
            // Read time from EEPROM
            for (int i = 0; i < 8; i++)
            {
                read_time[i] = read_ext_eeprom(address++); // Read each byte of time
            }
            read_time[8] = '\0'; // Null-terminate the string

            // Read events from EEPROM
            for (int i = 0; i < 2; i++)
            {
                read_events[i] = read_ext_eeprom(address++); // Read each byte of events
            }
            read_events[2] = '\0'; // Null-terminate the string

            // Read speed from EEPROM
            for (int i = 0; i < 2; i++)
            {
                read_speed[i] = read_ext_eeprom(address++); // Read each byte of speed
            }
            read_speed[2] = '\0'; // Null-terminate the string

            // Format the log entry and send it over UART
            sprintf(str, "%d %s %s %s\n", index, read_time, read_events, read_speed);
            puts_uart(str); // Send formatted log entry via UART
            index++; // Move to the next log entry
        }
        transition(); // Transition to a different screen or state after downloading
        clcd_print("DOWNLOADED",LINE1(0)); // Indicate successful download on CLCD
        clcd_print("SUCCESSFULLY",LINE2(0)); // Display success message
        __delay_ms(1000); // Delay to allow user to read the message
        return;
    }
}