/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

void shiftBuffer(char buffer[], char newChar) {
    int i;
    for(i = 0; i < strlen(buffer); i++) {
        if(i == strlen(buffer)-1) {
            buffer[i] = newChar;
        }
        else {
            buffer[i] = buffer[i+1];
        }
    }
}

int main (void) {



	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	// Print "Hello, world" on the LCD
	//20 spaces total 33 spaces total
	char* message = "Microcontrollers are lots of fun!";
	char buffer[21] = {' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', '\0'};

	//populate with white space to start
	int counter = 0;
	char newC = '\0';
	while(1) {
	    newC = message[counter];
	    shiftBuffer(buffer, newC);
	    counter++;
	    lcd_printf("%s", buffer);
	    if(counter >= strlen(message)) {
	        counter = 0;
	    }
	    timer_waitMicros(300000);
	}


	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}
