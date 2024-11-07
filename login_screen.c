/*
 * File:   login_screen.c
 * Author: SRADHA N S
 * This code implements a login screen for a microcontroller with a 3-attempt limit for entering a 4-digit password
 * Created on 21 October, 2024, 3:34 PM
 */

#include <xc.h>
#include "main.h"
#include "stdio.h"
#pragma config WDTE=OFF

unsigned int sec_flag = 0;//flag to count 5 sec
/*function to display login screen*/
void login_screen()
{
    int attempt = 3;  // Initialize attempt count outside the loop
    while(1)
    {
        unsigned char key;
        /*if no attempts left block user for 120 sec*/
        if (attempt == 0)
        {
            clcd_print("YOU ARE BLOCKED ",LINE1(0));
            attempt = 3;  // Reset attempt 
            clcd_print("                ",LINE2(0));
            for (int i = 120 ; i >= 0 ; i--)
            {
                char str[16];
                sprintf(str,"Wait for..%dsec ",i);
                clcd_print(str,LINE2(0));
                __delay_ms(1000);  // 1 second delay for countdown
            }
            
        }
        else// If attempts remain
        {
            char usr_pass[5];//string for storing user password
            clcd_print("ENTER PASSWORD  ",LINE1(0));
            clcd_print("                ",LINE2(0));
            int pos = 0;
             // Loop to enter each digit
            while(pos < 4)
            {
                key = ALL_RELEASED;
                while (key == ALL_RELEASED)
                {  // Wait until a key is pressed
                    blink_cursor(pos);        // Blink cursor at current position
                    /*if screen idle for 5 sec return to default screen*/
                    TMR2ON = 1;
                    if (sec_flag == 1)
                    {
                        sec_flag = 0;
                        TMR2ON = 0;
                        operation_flag = 1;
                        return;
                    }
                    key = read_digital_keypad(STATE);  // Wait for input
                }
                TMR2ON = 0;
                if (key == SW1)// If key SW1 is pressed, enter '0'
                {
                    usr_pass[pos] = '0';
                    clcd_putch('*',LINE2(5+pos));
                    pos++;
                }
                else if (key == SW2)// If key SW2 is pressed, enter '1'
                {
                    usr_pass[pos] = '1';
                    clcd_putch('*',LINE2(5+pos));
                    pos++;
                }
            }
            usr_pass[4] = '\0';  // Null terminate user password
            if (password_check(usr_pass) == FAILURE)  // if password failed
            {
                attempt--;  // Reduce attempts
                char msg[16];
                sprintf(msg, "%d attempts left", attempt);  // Dynamic message
                clcd_print("WRONG PASSWORD",LINE1(0));
                clcd_print(msg, LINE2(0));
                __delay_ms(10);  // Display the message for a while
                clcd_print("                ",LINE2(0));  // Clear CLCD

            }
            else
            {
                operation_flag = 3;
                return;  // Successful login
            }
        }
    }
}
/*function to blink cursor*/
void blink_cursor(int pos)
{
    static unsigned int wait = 0;
    static int cursor = 1;
    
    // Blink the cursor
    if (wait++ == 30) 
    {
        cursor = !cursor;  // Toggle the cursor visibility
        wait = 0;          // Reset the counter
    }
    
    if (cursor) 
    {
        clcd_putch('_', LINE2(5+pos));  // Show cursor at position
    } 
    else 
    {
        clcd_putch(' ', LINE2(5+pos));  // Hide cursor (clear position)
    }
    
}
/*function to validate entered password with original password*/
int password_check(char *usr_pass)
{
    int flag = 0;
    /*check whether entered password correct*/
    for (int i=0;i<4;i++)
    {
        if (pass[i] != usr_pass[i])//if password failed set flag variable
        {
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        flag=0;
        return FAILURE;
    }
    else
    {
        return SUCCESS;
    }
    
}