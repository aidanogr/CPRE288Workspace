// Simple 'Hello, world' program
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
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
void shiftBuffer(char buffer[], char newChar) {
    uint16_t i;
    for(i = 0; i < strlen(buffer); i++) {
        if(i == strlen(buffer)-1) {
            buffer[i] = newChar;
        }
        else {
            buffer[i] = buffer[i+1];
        }
    }
}

void delay_ms(int delay) {
	volatile int count;
	while (delay > 0) {
		count = 16000;              // Roughly 1 ms at 16 MHz
		while (count > 0) {
			count--;
		}
		delay--;
	}
}

int main (void) {

	SYSCTL_RCGCGPIO_R |= 0x20;      // Enable clock for Port F
	while ((SYSCTL_PRGPIO_R & 0x20) == 0);  // Wait for Port F to be ready

	// Configure PF1 (Red LED) as output
	GPIO_PORTF_DIR_R |= 0x02;       // Set PF1 as output
	GPIO_PORTF_DEN_R |= 0x02;       // Enable digital function for PF1

	while (1) {
		GPIO_PORTF_DATA_R ^= 0x02;  // Toggle Red LED
		delay_ms(500);              // Delay 500ms
	}
/*
	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	// Print "Hello, world" on the LCD
	//20 spaces total 33 spaces total
	char* message = "Microcontrollers are lots of fun!";
	char buffer[21] = {' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', '\0'};

	//populate with white space to start
	uint16_t counter = 0;
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

	return 0; */
}
