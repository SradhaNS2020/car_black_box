/* 
 * File:   uart.h
 */

#ifndef UART_H
#define	UART_H

#define FOSC                20000000

void init_uart(unsigned long baud);
unsigned char getchar_uart(void);
void putchar_uart(unsigned char data);
void puts_uart(const char *s);

#endif	/* UART_H */