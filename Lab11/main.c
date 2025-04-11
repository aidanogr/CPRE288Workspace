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

    cyBOT_init_scan(0b111);



    //callibrate_servo();
    servo_set_callibration(-104, 199);

    int num_objects = 0;
    int i;
    obj_t object_array[10];



    //scan_range(45, 135, object_array, &num_objects);


    //print_objects(object_array, num_objects);
    int round = 1;
    int min_angle = 0;
    int max_angle = 180;


    while(1) {
        if(round > 1) {
            min_angle = 45;
            max_angle = 135;
        }

        scan_range(min_angle, max_angle, object_array, &num_objects);

        if(!num_objects) {
            lcd_printf("We are NOT finding this mf\n");

            break;
        }

        int skinniest = 0;

        for(i = 1; i < num_objects; i++) {
            if(object_array[i].width < object_array[skinniest].width) { skinniest = i; }
        }

        // found skinniest, turn to it
        cyBOT_Scan((object_array[skinniest].start_angle + object_array[skinniest].end_angle) / 2, NULL);

        int angle = (object_array[skinniest].start_angle + object_array[skinniest].end_angle) / 2;

        lcd_printf("%d\n", angle);

        if(angle < 90) {
            turn_right(sensor_data, 90 - angle - 3);
        } else {
            turn_left(sensor_data, angle - 90 - 3);
        }

        int go_this_far = object_array[skinniest].distance * 10 - 190;
        int went_this_far = move_to_obj(sensor_data, go_this_far);


        if(went_this_far >= go_this_far) { break; }




        round++;
    }





    oi_free(sensor_data);



}



