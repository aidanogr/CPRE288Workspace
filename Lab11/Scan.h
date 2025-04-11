/*
 * Scan.h
 *
 *  Created on: Mar 11, 2025
 *      Author: andreyd
 */

#ifndef SCAN_H_
#define SCAN_H_


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "uart-interrupt.h"
#include "sleep.h"
#include "ping.h"
#include "servo.h"
#include "adc.h"


typedef struct obj_t {

    int start_angle;
    int end_angle;
    double distance;
    double width;

} obj_t;

//for the purposes of similarity we'll keep ir raw value even though it's stupid
typedef struct{
    float sound_dist;  // Distance in cm from PING sensor (cyBOT_Scan returns -1.0 if PING is not enabled)
    int IR_raw_val;    // Raw ADC value from IR sensor (cyBOT_Scan returns -1 if IR is not enabled)
} cyBOT_Scan_t;

void cyBOT_init_scan();
void cyBOT_Scan(int angle, cyBOT_Scan_t* scan_data);
void scan_range(int min_angle, int max_angle, obj_t arr[], int *arr_size);
void print_objects(obj_t arr[], int size);




#endif /* SCAN_H_ */
