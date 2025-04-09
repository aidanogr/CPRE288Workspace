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
#include <math.h>
#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "Scan.h"
#include "sleep.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"




int main() {
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    //oi_init(sensor_data); // do this only once at start of main()

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                      // and enables time functions (e.g. timer_waitMillis)
    button_init();
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    uart_interrupt_init();
    initialize_servo();
    cyBOT_init_Scan(0b111);

    int pulse_width = 0.0015 * 16000000;
    uint8_t button_pressed = 0;
    int dir = 1;

    while(1) {
        button_pressed = button_getButton();
        lcd_printf("button %d\ndir: %d", button_pressed, dir);
    
        switch(button_pressed) {

            case 0:
                break;
            case 1:
//                while(button_pressed == )
                pulse_width += convert_degrees_to_pulse_width(1)*dir;
                servo_move_to(pulse_width);
                break;
            case 2:
                pulse_width += convert_degrees_to_pulse_width(5)*dir;
                servo_move_to(pulse_width);
                break;
            case 3:
                dir *= -1;
                break;
            case 4:
                if(dir == -1){ 
                    pulse_width = convert_degrees_to_pulse_width(5);
                    servo_move_to(pulse_width);
                }
                else {
                    pulse_width = convert_degrees_to_pulse_width(175);
                    servo_move_to(pulse_width);
                }
                break;

        }
        timer_waitMillis(500);
    }

    servo_move_to(0.0015 * 16000000);
    timer_waitMillis(2000);

    servo_move_to(0.0011666 * 16000000);
    timer_waitMillis(2000);

    servo_move_to(0.0018333 * 16000000);
    timer_waitMillis(2000);

    servo_move_to(0.0015 * 16000000);
    timer_waitMillis(2000);

    return 0;
}



