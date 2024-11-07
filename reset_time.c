/*
 * File:   reset_time.c
 * Author: SRADHA N S
 * This file manages the functionality for resetting the RTC time through user input, updating the display, and writing the new time to the RTC registers
 * Created on 25 October, 2024, 1:48 PM
 */

#include <xc.h>
#include <stdio.h>
#include "main.h"
#pragma config WDTE=OFF

// Arrays to hold time values in string format
char hour_array[3], min_array[3], sec_array[3];
int hour, min, sec;

// Function to reset the time of the RTC
void reset_time()
{
    unsigned int key = ALL_RELEASED; // Initialize key state
    transition(); // Transition to time setting screen
    clcd_print("TIME <HH:MM:SS>", LINE1(0)); // Prompt for time input
    char n_time[9]; // Buffer for displaying the new time
    
    // Get the current time from the clock register
    get_time(clock_reg);
    // Extract hour, minute, and second from clock_reg
    hour = ((clock_reg[0] >> 4) & 0x03) * 10 + (clock_reg[0] & 0x0F);
    min = ((clock_reg[1] >> 4) & 0x07) * 10 + (clock_reg[1] & 0x0F);
    sec = ((clock_reg[2] >> 4) & 0x07) * 10 + (clock_reg[2] & 0x0F);
    
    // Update string representations of time
    update_hour();
    update_min();
    update_sec();
    sprintf(n_time, "%s:%s:%s", hour_array, min_array, sec_array); // Format new time string
    clcd_print(n_time, LINE2(0)); // Display the current time on the CLCD

    int pos = 6; // Position of the currently editable field (seconds)
    int var1 = 0, var2 = 0; // Variables to track key presses
    unsigned int wait = 0; // Wait counter

    while (1) // Loop until time is set
    {
        if (wait++ == 10) // Every 10 counts
        {
            wait = 0; // Reset wait counter
            key = read_digital_keypad(LEVEL); // Read keypad input
            if (key == ALL_RELEASED) // If no key is pressed
            {
                blink_field(pos); // Blink the currently selected field
                if (var2 >= 4000) // If var2 indicates time has been set
                {
                   var2 = 0; // Reset var2
                   write_to_rtc(); // Write the new time to RTC
                   clcd_print(" TIME CHANGED  ", LINE1(0)); // Display success message
                   clcd_print("  SUCCESSFULLY ", LINE2(0));
                   __delay_ms(1000);
                   return; // Exit the function
                }
                else if (var1 > 0) // If var1 indicates a key press
                {
                    var1 = 0; // Reset var1
                    if (pos == 0) // If editing hours
                    {
                        clcd_print(hour_array, LINE2(pos)); // Display hour
                    }
                    else if (pos == 3) // If editing minutes
                    {
                        clcd_print(min_array, LINE2(pos)); // Display minutes
                    }
                    else if (pos == 6) // If editing seconds
                    {
                        clcd_print(sec_array, LINE2(pos)); // Display seconds
                    }
                    pos = pos + 3; // Move to the next editable field
                    if (pos > 7) // Wrap around if past last field
                    {
                        pos = 0;
                    }
                }
                else if (var2 > 0) // If var2 indicates time increment
                {
                    var2 = 0; // Reset var2
                    if (pos == 0) // Increment hours
                    {
                        if (++hour == 24) // Wrap around after 23
                        {
                            hour = 0;
                        }
                        update_hour(); // Update hour string
                        clcd_print(hour_array, LINE2(pos)); // Display updated hour
                    }
                    else if (pos == 3) // Increment minutes
                    {
                        if (++min == 60) // Wrap around after 59
                        {
                            min = 0;
                        }
                        update_min(); // Update minute string
                        clcd_print(min_array, LINE2(pos)); // Display updated minutes
                    }
                    else if (pos == 6) // Increment seconds
                    {
                        if (++sec == 60) // Wrap around after 59
                        {
                            sec = 0;
                        }
                        update_sec(); // Update second string
                        clcd_print(sec_array, LINE2(pos)); // Display updated seconds
                    }
                }
            }
            else if (key ==  SW5) // If SW5 is pressed, increment var1
            {
                var1++;
            }
            else if (key == SW4) // If SW4 is pressed, increment var2
            {
                var2++;
            }
        }
    }
}

// Function to write the updated time to the RTC
void write_to_rtc() 
{
    unsigned char hour_bcd, min_bcd, sec_bcd;

    // Convert decimal to BCD format for RTC
    hour_bcd = ((hour / 10) << 4) | (hour % 10); 
    min_bcd = ((min / 10) << 4) | (min % 10);     
    sec_bcd = ((sec / 10) << 4) | (sec % 10);   

    // Write the BCD values to the RTC registers
    write_ds1307(HOUR_ADDR, hour_bcd);
    write_ds1307(MIN_ADDR, min_bcd);
    write_ds1307(SEC_ADDR, sec_bcd);
}

// Function to blink the currently selected time field
void blink_field(int pos)
{
    static unsigned int wait = 0; // Static variable to track wait time
    static int cursorVisible = 1; // Cursor visibility state

    // Blink the cursor
    if (wait++ == 10) // Every 10 counts
    {
        cursorVisible = !cursorVisible; // Toggle cursor visibility
        wait = 0; // Reset wait counter
    }
    
    // Clear the field if cursor is not visible
    if (cursorVisible) 
    {
        clcd_print("  ", LINE2(pos)); // Clear the field
    }
    else 
    {
        // Redisplay the selected field based on position
        if (pos == 6)
        {
            clcd_print(sec_array, LINE2(pos));  
        } 
        else if (pos == 0)
        {
            clcd_print(hour_array, LINE2(pos)); // Display hour
        }
        else if (pos == 3)
        {
            clcd_print(min_array, LINE2(pos)); // Display minutes
        }
    }
}

// Function to update the hour array with the current hour
void update_hour() 
{
    hour_array[0] = (hour / 10) + '0';  // Convert to character
    hour_array[1] = (hour % 10) + '0';  // Convert to character
    hour_array[2] = '\0'; // Null-terminate the string
}

// Function to update the minute array with the current minute
void update_min()
{
    min_array[0] = (min / 10) + '0';    // Convert to character
    min_array[1] = (min % 10) + '0';    // Convert to character
    min_array[2] = '\0'; // Null-terminate the string
}

// Function to update the second array with the current second
void update_sec()
{
    sec_array[0] = (sec / 10) + '0';    // Convert to character
    sec_array[1] = (sec % 10) + '0';    // Convert to character
    sec_array[2] = '\0'; // Null-terminate the string
}
