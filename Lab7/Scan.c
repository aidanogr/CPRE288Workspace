/*
 * Scan.c
 *
 *  Created on: Mar 11, 2025
 *      Author: andreyd
 */

#include "Scan.h"

#define SIG_DIST 835





void scan_range(int min_angle, int max_angle, obj_t object_array[], int *arr_size) {
    cyBOT_Scan_t *scan_data = (cyBOT_Scan_t *) calloc(1, sizeof(cyBOT_Scan_t));
    cyBOT_Scan(0, scan_data);
    sleep_millis(2000);


    int sum;
    int i;
    int num_objects = 0;
    char isDetecting = 0;


    while(min_angle < max_angle) {
            sum = 0;
            for(i = 0; i < 3; i++) {
                cyBOT_Scan(min_angle, scan_data);
                sum += scan_data->IR_raw_val;
            }

            if(!isDetecting) {
                if(sum/3 > SIG_DIST) {
                    isDetecting = 1;
                    object_array[num_objects].start_angle = min_angle;
                }

            }
            else {
                if(sum/3 < SIG_DIST) {
                    isDetecting = 0;
                    object_array[num_objects].end_angle = min_angle;
                    num_objects++;
                }
            }


            char msg[100];
            sprintf(msg, "%d,%d\r\n", min_angle, sum/3);
            uart_sendStr(msg);
            min_angle+=2;
        }

        if(isDetecting) {
            object_array[num_objects].end_angle = min_angle;
            num_objects++;
        }

        *arr_size = num_objects;



        for(i = 0; i < num_objects; i++) {
            int avg = (object_array[i].start_angle + object_array[i].end_angle)/2;
            cyBOT_Scan(avg, scan_data);
            sleep_millis(600);
            cyBOT_Scan(avg, scan_data);
            object_array[i].distance = scan_data->sound_dist;
            object_array[i].width = 2 * object_array[i].distance * tan( ((M_PI)/360) * (object_array[i].end_angle - object_array[i].start_angle));
        }
}

void print_objects(obj_t arr[], int size) {

    int i;
    for(i = 0; i < size; i++) {
        char msg[100];
        sprintf(msg, "%d,%d, %lf, %lf\r\n", arr[i].start_angle, arr[i].end_angle, arr[i].distance, arr[i].width);
        uart_sendStr(msg);
    }

}

