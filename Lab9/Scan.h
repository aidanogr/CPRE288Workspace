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
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "sleep.h"


typedef struct obj_t {

    int start_angle;
    int end_angle;
    double distance;
    double width;

} obj_t;

void scan_range(int min_angle, int max_angle, obj_t arr[], int *arr_size);

void print_objects(obj_t arr[], int size);




#endif /* SCAN_H_ */
