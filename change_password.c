/*
 * File:   change_password.c
 * Author: SRADHA N S
 * This file implements functionality for reading, changing, and validating a user password stored in external EEPROM, ensuring secure password updates through user input on a keypad
 * Created on 25 October, 2024, 2:06 PM
 */

#include <xc.h>
#include <stdio.h>
#include "main.h"
#pragma config WDTE=OFF
// Function to read the existing password from EEPROM
void read_password()
{
    unsigned char addr = 0x78; // Starting address for password in EEPROM
    // Read each character of the password
    pass[0] = read_ext_eeprom(addr++);
    pass[1] = read_ext_eeprom(addr++);
    pass[2] = read_ext_eeprom(addr++);
    pass[3] = read_ext_eeprom(addr);
    pass[4] = '\0';
}
// Function to change the user password
void change_password ()
{
    transition();
    clcd_print ("Type NewPassword",LINE1(0));// Prompt user to enter new password
    char usr_pass1[5],usr_pass2[5];// Arrays to store user passwords
    int pos = 0;// Position index for password entry
    unsigned char key ;
    while(pos<4)// Loop to get first password input
    {
        key = ALL_RELEASED;
        while (key == ALL_RELEASED)
        {
            blink_cursor(pos);// Blink cursor
            key = read_digital_keypad(STATE);// Read keypad state
        }
        if (key == SW1)//if SW1 pressed store 0 in given pos
        {
            usr_pass1[pos] = '0';
            clcd_putch('*',LINE2(5+pos));
            pos++;
        }
        else if (key == SW2)//if SW2 pressed store 1 on given pos
        {
            usr_pass1[pos] = '1';
            clcd_putch('*',LINE2(5+pos));
            pos++;
        }
    }
    usr_pass1[4]='\0';// Null-terminate the first password
    pos = 0;// Reset position for second password entry
    transition();
    clcd_print ("Retype Password",LINE1(0));// Prompt user to re-enter password
    while(pos<4)// Loop to get second password input
    {
        key = ALL_RELEASED;
        while (key == ALL_RELEASED)
        {
            blink_cursor(pos);// Blink cursor 
            key = read_digital_keypad(STATE);// Read keypad on state detection
        }
        if (key == SW1)//if SW1 pressed store 0 in given pos
        {
            usr_pass2[pos] = '0';
            clcd_putch('*',LINE2(5+pos));
            pos++;
        }
        else if (key == SW2)//if SW2 pressed store 1 on given pos
        {
            usr_pass2[pos] = '1';
            clcd_putch('*',LINE2(5+pos));
            pos++;
        }
    }
    usr_pass2[pos]='\0';// Null-terminate the second password
    int status = password_validation(usr_pass1,usr_pass2);//check for successful password validation
    if (status)//if successful
    {
        transition();
        write_password_to_eeprom(usr_pass1);// Write the new password to EEPROM
        read_password();// Read the new password back from EEPROM
        clcd_print("PASSWORD UPDATED",LINE1(0));
        clcd_print("SUCCESSFULLY",LINE2(2));
        __delay_ms(1000);
        transition();
        return;// Exit function
    }
    else//if password validation failed
    {
        transition();
        /*print message and return*/
        clcd_print("FAILED TO UPDATE",LINE1(0));
        clcd_print("PASSWORD",LINE2(2));
        __delay_ms(1000);
        transition();
        return;
    }
    
}
// Function to validate if the two passwords match
int password_validation(char *pass1 , char *pass2)
{
    for (int i=0; i<4 ; i++)
    {
        if (pass1[i] != pass2[i])// Compare each character
        {
            return FAILURE; // Return failure if any character does not match
        }
    }
    return SUCCESS;// Return success if all characters match
}
// Function to write the new password to EEPROM
void write_password_to_eeprom(char *pass)
{
    unsigned char addr = 0x78; // Starting address for password in EEPROM
    int pos = 0; // Position index for writing password
    // Write each character of the password to EEPROM
    write_ext_eeprom(addr++, pass[pos++]);
    write_ext_eeprom(addr++, pass[pos++]);
    write_ext_eeprom(addr++, pass[pos++]);
    write_ext_eeprom(addr, pass[pos]);
   
}