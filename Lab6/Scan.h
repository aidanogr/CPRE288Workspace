/*
 * Scan.h
 *
 *  Created on: Feb 12, 2025
 *      Author: ahogrady
 */

#ifndef SCAN_H_
#define SCAN_H_

#include "uart-interrupt.h"
#include "cyBot_Scan.h"
#include "string.h"
#include "lcd.h"


struct obj_t;
typedef struct obj_t obj_t;

struct obj_t {
    obj_t *next;
    int index;
    int angle;
    double dist;
    int angular_width;
};


//data[0] => initial angle
//data[1] => number of data points
//data[2:] => data points
double *scan_range(int min_angle, int max_angle);


//obj_t *parse_scan_data(double *data);




#endif /* SCAN_H_ */
