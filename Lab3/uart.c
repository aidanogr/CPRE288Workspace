/*
 * uart.c
 *
 *  Created on: Feb 12, 2025
 *      Author: andreyd
 */

#include "uart.h"



void cyBot_sendString(char* data, int length) {
    int c = 0;
    while(c < length && data[c]) {
        cyBot_sendByte(data[c]);
        c++;
    }
}


