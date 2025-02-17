/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "uart.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
    cyBot_uart_init();

	
	while(1) {
	    uint8_t buttonPressed = button_getButton();
	    lcd_printf("%d", buttonPressed);

	    switch(buttonPressed) {
	        case(4):
	            cyBot_sendString("Button 4 pressed\r\n", -1);
	            while(button_getButton() == 4) {};
	            break;
	        case(3):
	            cyBot_sendString("Button 3 pressed\r\n", -1);
	            while(button_getButton() == 3) {};
	            break;
	        case(2):
               cyBot_sendString("Button 2 pressed\r\n", -1);
                while(button_getButton() == 2) {};
                break;
	        case(1):
                cyBot_sendString("Button 1 pressed\r\n", -1);
                while(button_getButton() == 1) {};
	    }
	}
}
