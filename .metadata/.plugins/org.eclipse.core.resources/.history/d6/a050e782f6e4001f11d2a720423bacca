/*
 * movement.c
 *
 *  Created on: Feb 4, 2025
 *      Author: ahogrady
 */


#include "open_interface.h"
#include <stdint.h>
#include "Timer.h"
#include "movement.h"
//const double MOVE_FORWARD_SPEED = 350.0;
//const double MAXIMUM_ROTATIONAL_VELOCITY = 350.0;
#define MOVE_FORWARD_SPEED 150
#define MAXIMUM_ROTATIONAL_VELOCITY 15
#define MINIMUM_ROTATIONAL_VELOCITY 10



double absoluteVal(double val) {
    if(val < 0) {
        return val*-1;
    }
    return val;
}

double move_forward(oi_t* sensor_data, double distance_mm) {

    oi_update(sensor_data);
    double sum = 0; // distance member in oi_t struct is type double
    double initial_distance = sensor_data->distance;
    oi_setWheels(MOVE_FORWARD_SPEED, MOVE_FORWARD_SPEED); //move forward at full speed

    while (sum < distance_mm-20) {
        oi_update(sensor_data);
        double tempDistance = sensor_data->distance;
        if(sensor_data->bumpRight == 1) {

            move_backwards(sensor_data, 150);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 150);
            oi_setWheels(MOVE_FORWARD_SPEED, MOVE_FORWARD_SPEED);

        }
        else if(sensor_data->bumpLeft == 1) {

            move_backwards(sensor_data, 150);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 150);
            oi_setWheels(MOVE_FORWARD_SPEED, MOVE_FORWARD_SPEED);

        }
        sensor_data->distance = tempDistance;

        sum += absoluteVal(sensor_data -> distance - initial_distance); // use -> notation since pointer

    }
    oi_setWheels(0,0); //stop
    return sum;
}


void move_backwards(oi_t* sensor_data, double distance_mm) {
     oi_update(sensor_data);
     double sum = 0; // distance member in oi_t struct is type double
     double initial_distance = sensor_data->distance;
     oi_setWheels(-MOVE_FORWARD_SPEED, -MOVE_FORWARD_SPEED); //move forward at full speed

    while (sum < distance_mm-20) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> distance - initial_distance); // use -> notation since pointer

    }
    oi_setWheels(0,0); //stop
   // return sum;
}

void turn_left(oi_t* sensor_data, double degrees){
    //TODO
    /*
    oi_update(sensor_data);
    double sum = 0;
    double initial_angle = absoluteVal(sensor_data->angle);




    double speed = 0;

    while(sum < degrees) {
        oi_setWheels(speed, -speed);
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle - initial_angle);

        if(sum/2 > degrees) {
            if(speed < MAXIMUM_ROTATIONAL_VELOCITY) {
                if(speed > MINIMUM_ROTATIONAL_VELOCITY) {
                    speed -= 1;
                }
            }
            else if (absoluteVal(degrees - sum) < 30 && speed > MINIMUM_ROTATIONAL_VELOCITY) {
                speed -= 1;
            }
        }
        else {
            if(speed < MAXIMUM_ROTATIONAL_VELOCITY) {
                speed += 1;
            }
        }
    }
    oi_setWheels(0,0);

    */

    oi_update(sensor_data);
    double sum = 0;
    double initial_angle = absoluteVal(sensor_data->angle);



    oi_setWheels(MAXIMUM_ROTATIONAL_VELOCITY, -MAXIMUM_ROTATIONAL_VELOCITY);
    while(sum < degrees-5) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle);
        lcd_printf("%lf, ", sensor_data->angle);
    }
    oi_setWheels(0,0);


}

void turn_right(oi_t* sensor_data, double degrees){
    oi_update(sensor_data);
    double sum = 0;
    double initial_angle = absoluteVal(sensor_data->angle);



    oi_setWheels(-MAXIMUM_ROTATIONAL_VELOCITY, MAXIMUM_ROTATIONAL_VELOCITY);
    while(sum < degrees-5) {
        oi_update(sensor_data);
        sum += absoluteVal(sensor_data -> angle);
        lcd_printf("%lf, ", sensor_data->angle);
    }
    oi_setWheels(0,0);

}

