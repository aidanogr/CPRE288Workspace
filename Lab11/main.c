
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
    volatile oi_t sensor;
    volatile oi_t *sensor_data = &sensor;
    oi_init(sensor_data);

    timer_init();
    button_init();
    lcd_init();
    uart_interrupt_init(sensor_data);
    cyBOT_init_scan();
  //  cyBOT_init_Scan(0b100);
   // callibrate_servo();
    servo_set_callibration(-84, 232);




    int num_objects = 0;
    int i;  //object iterator for object_array
    obj_t object_array[10];

    int round = 1;
    int min_angle = 0;
    int max_angle = 180;

  //  move_forward(sensor_data, 100.0);
  //  move_forward(sensor_data, 50.0);
  //  move_forward(sensor_data, 10.0);
    //lcd_printf("%d", sensor_data);
    while(1) {
        //wait for command from uart
    }
    move_forward(sensor_data, 80);
    scan_range(0,180, object_array, &num_objects);

/*
    while(1) {
        if(round > 1) {
            min_angle = 45;
            max_angle = 135;
        }

        scan_range(min_angle, max_angle, object_array, &num_objects);

        if(num_objects == 0) {
            lcd_printf("We are NOT finding this mf\n");
            break;
        }

        int skinniest_index = 0;
        for(i = 1; i < num_objects; i++) {
            if(object_array[i].width < object_array[skinniest_index].width) { skinniest_index = i; }
        }

        // found skinniest_index, turn to it with servo (find angle)
        int angle_of_skinniest_object = (object_array[skinniest_index].start_angle + object_array[skinniest_index].end_angle) / 2;
        cyBOT_Scan(angle_of_skinniest_object, NULL);
        lcd_printf("deg of skinniest: %d\n", angle_of_skinniest_object);

        //turn to angle with cybot      //TODO What does this do?
        if(angle_of_skinniest_object < 90) {
            turn_right(sensor_data, 90 - angle_of_skinniest_object - 3);
        } else {
            turn_left(sensor_data, angle_of_skinniest_object - 90 - 3);
        }

        int go_this_far = object_array[skinniest_index].distance * 10 - 190;    //TODO where does this value come from
        int went_this_far = move_to_obj(sensor_data, go_this_far);
        if(went_this_far >= go_this_far) { break; }                             //TODO i dont know how this termination works


        round++;
    }




*/




}



