/*
 * Scan.c
 *
 *  Created on: Mar 11, 2025
 *      Author: andreyd
 */

#include "Scan.h"

#define SIG_DIST 970
#define SIG_WIDTH 3
#define SIG_DELTA

//might as well initialize everything
void cyBOT_init_scan() {
    initialize_servo();
    ping_init();
    adc_init();
}

void cyBOT_Scan(int angle, cyBOT_Scan_t* scan_data) {
    //move servo to angle
    servo_move_to(convert_degrees_to_pulse_width(angle));
    timer_waitMillis(50);

    //passing null just turns the servo
    if(scan_data == NULL) {
        return;
    }

    //store raw ir distance (for compatibility with older code)
    scan_data->IR_raw_val = adc_read();
    timer_waitMillis(10);

    //store ping distance in meter
    scan_data->sound_dist = ping_get_cm();
    timer_waitMillis(50);
}




//uart debug is overrated we ball
void scan_range(int min_angle, int max_angle, obj_t object_array[], int *arr_size) {
    cyBOT_Scan_t *scan_data = (cyBOT_Scan_t *) calloc(1, sizeof(cyBOT_Scan_t));     // we need to fully completely transition to our own scan functions
    cyBOT_Scan(0, scan_data);
    sleep_millis(2000);


    int sum;
    int i;
    int num_objects = 0;
    char isDetecting = 0;


    uart_sendStr("start scan\n");
    while(min_angle < max_angle) {
            sum = 0;
            for(i = 0; i < 3; i++) {
                timer_waitMillis(20);
                cyBOT_Scan(min_angle, scan_data);
                sum += scan_data->IR_raw_val;
            }

            if(!isDetecting) {
                if(sum/3 > SIG_DIST) {
                    isDetecting = 1;
                    object_array[num_objects].start_angle = min_angle;
                }

            } else {
                if(sum/3 < SIG_DIST) {
                    isDetecting = 0;
                    object_array[num_objects].end_angle = min_angle;
                    num_objects++;
                }
            }


            char msg[100];
            sprintf(msg, "%d,%d\n", min_angle, sum/3);
            uart_sendStr(msg);
            min_angle+=2;
        }

        *arr_size = num_objects;



        for(i = 0; i < num_objects; i++) {
            int j;
            int avg = (object_array[i].start_angle + object_array[i].end_angle)/2;

            for(j = 0; j < 10; j++) {
                cyBOT_Scan(avg - j, NULL);
                sleep_millis(1000);
                cyBOT_Scan(avg - j, scan_data);

                if(scan_data->sound_dist < 310 || j == 9) {
                    object_array[i].distance = scan_data->sound_dist;
                    object_array[i].width = 2 * object_array[i].distance * tan( ((M_PI)/360) * (object_array[i].end_angle - object_array[i].start_angle));
                    break;
                }
            }
        }


        timer_waitMillis(1000);
        uart_sendStr("end scan\n");

        free(scan_data);
}

void print_objects(obj_t arr[], int size) {
    int i;
    for(i = 0; i < size; i++) {
        char msg[100];
        sprintf(msg, "%d,%d, %lf, %lf\n", arr[i].start_angle, arr[i].end_angle, arr[i].distance, arr[i].width);
        uart_sendStr(msg);
    }
}

