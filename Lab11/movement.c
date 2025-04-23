/*
 * movement.c
 *
 *  Created on: Feb 4, 2025
 *      Author: ahogrady
 */


#include "movement.h"



//const double MOVE_FORWARD_SPEED = 350.0;
//const double MAXIMUM_ROTATIONAL_VELOCITY = 350.0;
#define MOVE_FORWARD_SPEED 350
#define MAXIMUM_ROTATIONAL_VELOCITY 15
#define MINIMUM_ROTATIONAL_VELOCITY 10
#define ANGLE_OFFSET 5

char buffer[20];



void avoid_bump(oi_t *sensor, uint32_t right, uint32_t left);



double absoluteVal(double val) {
    if(val < 0) {
        return val*-1;
    }
    return val;
}


double move_to_obj(oi_t *sensor_data, double dist) { // dist in mm
    int sign = 1;
    if(dist < 0) {
        sign = -1;
        dist *= -1;
    }



    oi_update(sensor_data);

    double sum = 0;

    oi_setWheels(sign * MOVE_FORWARD_SPEED, sign * MOVE_FORWARD_SPEED);

    while(sum < dist) {
        oi_update(sensor_data);

        sum += sign * sensor_data->distance;

        if(sign > 0 && (sensor_data->bumpRight == 1 || sensor_data->bumpLeft == 1)) {
            oi_setWheels(0, 0);

            avoid_bump(sensor_data, sensor_data->bumpRight, sensor_data->bumpLeft);

            return sum;
        }
    }

    oi_setWheels(0, 0);

    return sum;
}





void avoid_bump(oi_t *sensor, uint32_t right, uint32_t left) {
    move_to_obj(sensor, -50);   // move back

    if(right == 1) {
        turn_left(sensor, 90);

    } else {
        turn_right(sensor, 90);
    }

    move_to_obj(sensor, 75);

    if(right == 1) {
        turn_right(sensor, 90);

    } else {
        turn_left(sensor, 90);
    }

    move_to_obj(sensor, 50);

    if(right == 1) {
        turn_right(sensor, 25);

    } else {
        turn_left(sensor, 25);
    }
}














double move_forward(oi_t *sensor_data, double dist) { // dist in mm

    oi_update(sensor_data);

    double sum = 0;

    oi_setWheels(MOVE_FORWARD_SPEED, MOVE_FORWARD_SPEED);

    while(sum < dist-2) {
        oi_update(sensor_data);

        sum += sensor_data->distance;

        if(sensor_data->bumpRight == 1 || sensor_data->bumpLeft == 1) {
            oi_setWheels(0, 0);

            move_backwards(sensor_data, 30);
            sprintf(buffer, "forwards, %d\n", (int) sum);
            uart_sendStr(buffer);
            return sum;
        }
    }

    oi_setWheels(0, 0);
    sprintf(buffer, "forwards,%d\n", (int) sum);
    uart_sendStr(buffer);
    return sum;
}


void move_backwards(oi_t* sensor_data, double dist) {
    oi_update(sensor_data);

    double sum = 0;

    oi_setWheels(-1*MOVE_FORWARD_SPEED, -1*MOVE_FORWARD_SPEED);

    while(sum < dist) {
        oi_update(sensor_data);
        sum += sensor_data->distance;

    }

    oi_setWheels(0, 0);
    sprintf(buffer, "backwards,%d\n", (int) sum);
    uart_sendStr(buffer);
    return sum;
}

void turn_left(oi_t* sensor_data, double degrees){
    lcd_printf("l1");

    oi_update(sensor_data);
    double sum = 0;

    lcd_printf("l2");

    oi_setWheels(MAXIMUM_ROTATIONAL_VELOCITY, -MAXIMUM_ROTATIONAL_VELOCITY);
    while(sum < degrees-ANGLE_OFFSET) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle);
    }
    oi_setWheels(0,0);


}

void turn_right(oi_t* sensor_data, double degrees){
    lcd_printf("r1");

    oi_update(sensor_data);
    double sum = 0;


    lcd_printf("r2");

    oi_setWheels(-MAXIMUM_ROTATIONAL_VELOCITY, MAXIMUM_ROTATIONAL_VELOCITY);
    while(sum < degrees-ANGLE_OFFSET) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle);
    }
    oi_setWheels(0,0);

}

