/*
 * File:   isr.c
 * Author: SRADHA N S
 * To implement ISR to count 5sec 
 * Created on 22 October, 2024, 2:11 PM
 */


#include <xc.h>
#include "main.h"
#pragma config WDTE=OFF

void __interrupt() isr()
{
    static unsigned int count = 0;
    static unsigned int sec = 0;
    if (TMR2IF)
    {
        if (++count == 1250)
        {
            count = 0;
           if ( ++sec == 5)
           {
               sec_flag = 1;
               sec=0;
           }
          
        }
        TMR2IF = 0;
    }
}