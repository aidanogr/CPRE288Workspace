/*
 * uart.h
 *
 *  Created on: Feb 25, 2025
 *      Author: andreyd
 */

#ifndef UART_H_
#define UART_H_

#include "cyBot_uart.h"
#include "lcd.h"
#include <string.h>


void uart_init(void);

void uart_sendChar(char data);

char uart_receive(void);

void uart_sendStr(char *data);



#endif /* UART_H_ */
