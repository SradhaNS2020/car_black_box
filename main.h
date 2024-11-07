/* 
 * File:   main.h
 * Author: SRADHA N S
 * To Implement main header file
 * Created on 17 October, 2024, 7:29 PM
 */

#ifndef MAIN_H
#define	MAIN_H

/*header */
#include "i2c.h"              // I2C communication functions
#include "ds1307.h"           // DS1307 RTC functions
#include "adc.h"              // ADC functions for analog input
#include "clcd.h"             // Character LCD display functions
#include "digital_keypad.h"   // Digital keypad functions
#include "ext_eeprom.h"       // External EEPROM functions
#include "timers.h"           // Timer functions
#include "uart.h"             // UART communication functions


#define XTAL_FREQ 20000000

/* global variables */
extern unsigned int operation_flag ;//to choose between login screen ,default screen or menu screen
extern unsigned int sec_flag;//to check whether screen sit idle for 5 sec
extern unsigned char total_logs;//total no.of logs entered in eeprom
extern char time[9];  // "HH:MM:SS"
extern char pass[5];//set password
extern unsigned char clock_reg[3];//array to hold hour,min and sec
extern unsigned char addr;
/*functions*/
/*function to display default screen*/
void default_screen(unsigned char);
/*to clear screen between different operations*/
void transition();
/*function to store time events and speed to eeprom*/
void store_events(char* time, char* events ,char *speed);
/*function to display login screen*/
void login_screen();
/*function to validate password entered by user*/
int password_check(char *usr_pass);
/*function to blink cursor on a given position on clcd*/
void blink_cursor(int);
/*function to display menu screen*/
void menu_screen();
/*function to implement scrolling function*/
void display_menu(int);
/*function to select menu*/
void select_menu(int);
/*function to view logs*/
void view_logs();
/*function to clear all logs*/
void clear_logs();
/*function to display all logs*/
void download_logs();
/*function to reset time*/
void reset_time();
/*function to blink each time field*/
void blink_field(int pos);
/*function to update hour array*/
void update_hour();
/*function to update minute array*/
void update_min();
/*function to update sec array*/
void update_sec();
/*function to update new time to rtc*/
void write_to_rtc();
/*function to change password*/
void change_password();
/*function to display logs*/
void display_logs(int index);
/*function to read password from eeprom*/
void read_password();
/*function to validate password on resetting password*/
int password_validation(char *,char *);
/*function to write password to eeprom*/
void write_password_to_eeprom(char *);


#define SUCCESS     1
#define FAILURE     0

#endif	/* MAIN_H */