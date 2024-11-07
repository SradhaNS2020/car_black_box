/*
 * File:   timers.c
 * Author: SRADHA N S
 * to implement timer2
 * Created on 22 October, 2024, 2:09 PM
 */


#include <xc.h>
#include "main.h"


void init_timer2(void)
{
    /* Selecting the scale as 1:16 */
    T2CKPS0 = 1;
    T2CKPS1 = 1;

    /* Loading the Pre Load register with 250 */
    PR2 = 250;
    
    /* The timer interrupt is enabled */
    TMR2IE = 1;
       
}