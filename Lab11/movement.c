/*
 * movement.c
 *
 *  Created on: Feb 4, 2025
 *      Author: ahogrady
 */


#include "movement.h"
#include "Timer.h"


//const double MOVE_FORWARD_SPEED = 350.0;
//const double MAXIMUM_ROTATIONAL_VELOCITY = 350.0;
#define MOVE_FORWARD_SPEED 350
#define MAXIMUM_ROTATIONAL_VELOCITY 15
#define MINIMUM_ROTATIONAL_VELOCITY 10
#define ANGLE_OFFSET 5

char buffer[20];
oi_t *sensor_data;

void movement_init() {

    sensor_data = oi_alloc();
    oi_init(sensor_data);

}

void movement_free() {
    oi_free(sensor_data);
}


double absoluteVal(double n) {
    return n < 0 ? n * -1 : n;
}



double move_forward(double distance_mm) { // dist in mm
    int dir = 1;

    if(distance_mm < 0) {
        dir = -1;
        distance_mm *= -1;
    }

    oi_update(sensor_data);
    double sum = 0;
    char buffer[100];
    oi_setWheels(MOVE_FORWARD_SPEED * dir, MOVE_FORWARD_SPEED * dir);


    while (sum < distance_mm) {
        oi_update(sensor_data);
        lcd_printf("%lf", sum);

        if(dir > 0 && (sensor_data->bumpRight == 1 || sensor_data->bumpLeft == 1)) {
            oi_setWheels(0, 0);

            if(sensor_data->bumpLeft == 1) {
                uart_sendStr("error,bump,left\n");
            }
            else {
                uart_sendStr("error,bump,right\n");
            }
            sprintf(buffer, "moved,%d\n", (int) sum * dir);
            uart_sendStr(buffer);
            move_forward(-50);
            return sum;

        } else if(dir > 0 && (sensor_data->cliffFrontLeftSignal > 2000)) {
            oi_setWheels(0, 0);

            uart_sendStr("error,boundary\n");
            sprintf(buffer, "moved,%d\n", (int) sum * dir);
            uart_sendStr(buffer);
            move_forward(-50);
            return sum;
        }

        sum += dir * sensor_data->distance;
        timer_waitMillis(10);
    }

    oi_setWheels(0,0); //stop
    oi_update(sensor_data);
    sprintf(buffer, "moved,%d\n", (int) sum * dir);
    uart_sendStr(buffer);


    return sum;
}


void turn_left(double degrees){
    oi_update(sensor_data);
    double sum = 0;
    char buffer[100];

    oi_setWheels(MAXIMUM_ROTATIONAL_VELOCITY, -MAXIMUM_ROTATIONAL_VELOCITY);
    while(sum < degrees-ANGLE_OFFSET) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle);
    }

    oi_setWheels(0,0);
    oi_update(sensor_data);

    sprintf(buffer, "turned,%d\n", (int) (sum * -1));
    uart_sendStr(buffer);
}

void turn_right(double degrees){
    oi_update(sensor_data);
    double sum = 0;
    char buffer[100];

    oi_setWheels(-MAXIMUM_ROTATIONAL_VELOCITY, MAXIMUM_ROTATIONAL_VELOCITY);
    while(sum < degrees-ANGLE_OFFSET) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle);
    }

    oi_setWheels(0,0);
    oi_update(sensor_data);

    sprintf(buffer, "turned,%d\n", (int) sum);
    uart_sendStr(buffer);
}
