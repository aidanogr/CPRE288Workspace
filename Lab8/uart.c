/*
 * uart.c
 *
 *  Created on: Feb 12, 2025
 *      Author: andreyd
 */

#include "uart.h"



void cyBot_sendString(char* data, int length) {
    if(length < 0) { length = strlen(data); }

    int c;

    for(c = 0; c < length && data[c]; c++) {
        cyBot_sendByte(data[c]);
    }
}


