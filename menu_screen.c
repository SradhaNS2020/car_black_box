/*
 * File:   menu_screen.c
 * Author: SRADHA N S
 * This file implements a menu screen  allowing users to navigate through various options using digitalkeypad.
 * Created on 22 October, 2024, 10:21 PM
 */

#include <xc.h>
#include "main.h"
#pragma config WDTE=OFF

unsigned char* menu[] = {" view log       "," clear log      "," download log   "," reset time     "," change password"};


void menu_screen()
{
    int current_option = 0;// Tracks the currently selected menu option
    unsigned char key;
    int var1 = 0, var2 = 0;  // Counters for long press detection
    unsigned int wait = 0;
    while (1)
    {
        
        if (wait++ == 10)
        {
            wait = 0;
            display_menu(current_option);  // Display the menu with the current option
            key = read_digital_keypad(LEVEL_DETECTION);  // Read which switch is pressed

            if (key != ALL_RELEASED)
            {
                TMR2ON = 0;
                if (key == SW4)
                {
                    var1++;  // Increment the counter for SW4 long press
                    if (var1 >= 5)  // Long press detected
                    {
                        var1 = 0;
                        select_menu(current_option);  // Enter the selected menu option
                    }
                }
                else if (key == SW5)
                {
                    var2++;  // Increment the counter for SW5 long press
                    if (var2 >= 5)  // Long press detected
                    {
                        var2 = 0;
                        operation_flag = 1;  // Exit the menu
                        return;
                    }
                }
            }
            else  // Key released
            {
                // to check whether system idle for 5sec,then return to default screen
                if (!TMR2ON)
                {
                    TMR2ON = 1;
                }
                if (sec_flag == 1)
                {
                    TMR2ON = 0;
                    sec_flag = 0;
                    operation_flag = 1;
                    transition();
                    return;
                }
                // Handle short press functionality
                if (var1 > 0 && var1 < 5)  // Short press SW4
                {
                    var1 = 0;
                    if (current_option > 0)
                    {
                        current_option--;  // Scroll up in the menu
                        display_menu(current_option);
                    }
                }
                else if (var2 > 0 && var2 <5)  // Short press SW5
                {
                    var2 = 0;
                    if (current_option < 4)
                    {
                        current_option++;  // Scroll down in the menu
                        display_menu(current_option);
                    }
                }
            }
        }
    }
}

/*function to implement scroll*/
void display_menu(int current_option)
{
    if (current_option < 4)
    {
        clcd_putch('*',LINE1(0));
        clcd_print(menu[current_option],LINE1(1));
        clcd_putch(' ',LINE2(0));
        clcd_print(menu[current_option+1],LINE2(1));
    }
    else
    {
        clcd_putch(' ',LINE1(0));
        clcd_print(menu[current_option-1],LINE1(1));
        clcd_putch('*',LINE2(0));
        clcd_print(menu[current_option],LINE2(1));
    }  
}
/*function to select menu options*/
void select_menu(int current_option)
{
    if (current_option == 0)
    {
       view_logs();//view logs
    }
    else if (current_option == 1)
    {
       clear_logs();//clear logs
    }
    else if (current_option == 2)
    {
        download_logs();//download logs
    }
    else if (current_option == 3)
    {
       reset_time();//reset time
    }
    else if (current_option == 4)
    {
       change_password(); //change password
    }
    return;        
}