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
    ping_init();










    //cyBOT_init_Scan(0b1);


    //i have no idea what the hell im doing
    //callibrate_servo();
    servo_set_callibration(-82, 237);
//    timer_waitMillis(2000);
//    servo_move_to(convert_degrees_to_pulse_width(0));
//    timer_waitMillis(2000);
//    servo_move_to(convert_degrees_to_pulse_width(360));
//    timer_waitMillis(2000);

    servo_move_to(convert_degrees_to_pulse_width(0));
    timer_waitMillis(2000);

    servo_move_to(convert_degrees_to_pulse_width(45));

    timer_waitMillis(2000);

    servo_move_to(convert_degrees_to_pulse_width(90));
    timer_waitMillis(2000);

    servo_move_to(convert_degrees_to_pulse_width(180));
    timer_waitMillis(2000);

    servo_move_to(convert_degrees_to_pulse_width(145));
    timer_waitMillis(2000);



    int pulse_width = 90;       // not really pulse width, its the amount of "degrees" but its not fr degrees so I didn't wanna call it degrees //valid
    uint8_t button_pressed = 0;
    int dir = 1;
/*
    while(1) {
        button_pressed = button_getButton();
        lcd_printf("Button: %d\nDir: %d\nPulse (deg): %d", button_pressed, dir, pulse_width);
    
        switch(button_pressed) {
            case 0:
                break;
            case 1:
                pulse_width += 1 * dir;
                servo_move_to(convert_degrees_to_pulse_width(pulse_width));
                timer_waitMillis(100);
                break;
            case 2:
                pulse_width += 5 * dir;
                servo_move_to(convert_degrees_to_pulse_width(pulse_width));
                timer_waitMillis(100);
                break;
            case 3:
                dir *= -1;
                timer_waitMillis(500);
                break;
            case 4:
                if(dir == -1){ 
                    pulse_width = 5;
                } else {
                    pulse_width = 175;
                }

                servo_move_to(convert_degrees_to_pulse_width(pulse_width));
                break;
        }
    }

//    servo_move_to(0.0015 * 16000000);
//    timer_waitMillis(2000);
//
//    servo_move_to(0.0011666 * 16000000);
//    timer_waitMillis(2000);
//
//    servo_move_to(0.0018333 * 16000000);
//    timer_waitMillis(2000);
//
//    servo_move_to(0.0015 * 16000000);
//    timer_waitMillis(2000);
*/
    return 0;
}



