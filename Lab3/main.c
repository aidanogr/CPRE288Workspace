/*
 * main.c
 *
 *  Created on: Jan 30, 2025
 *      Author: ahogrady
 *
 *      LAST COMMIT: FIXED INITIAL DISTANCE IN "move_forward(...)"
 *                   ADDED "move_forward1(...)" FOR EASE-IN-OUT ALGORITHM
 */

#include <stdio.h>
#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"

void cyBot_sendString(char* data, int length) {
    int c = 0;
    while(c < length && data[c]) {
        cyBot_sendByte(data[c]);
        c++;
    }
}

int main() {

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                      // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    cyBot_uart_init();

    while(1) {
        char b = cyBot_getByte();
        lcd_printf("%c", b);
        char message[20];
        sprintf(message, "%c recieved by cybot", b);

        cyBot_sendString(message, strlen(message));
    }



}



