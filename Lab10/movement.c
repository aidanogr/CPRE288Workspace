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




void avoid_bump(oi_t *sensor, uint32_t right, uint32_t left);



double absoluteVal(double val) {
    if(val < 0) {
        return val*-1;
    }
    return val;
}


double move_to_obj(oi_t *sensor, double dist) { // dist in mm
    int sign = 1;
    if(dist < 0) {
        sign = -1;
        dist *= -1;
    }



    oi_update(sensor);

    double sum = 0;

    oi_setWheels(sign * MOVE_FORWARD_SPEED, sign * MOVE_FORWARD_SPEED);

    while(sum < dist) {
        oi_update(sensor);

        sum += sign * sensor->distance;

        if(sign > 0 && (sensor->bumpRight == 1 || sensor->bumpLeft == 1)) {
            oi_setWheels(0, 0);

            avoid_bump(sensor, sensor->bumpRight, sensor->bumpLeft);

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
}














//Dont ask why but it works...
double move_forward1(oi_t* sensor_data, double distance_mm) {
      oi_update(sensor_data);
      uint16_t initial_time = timer_getMillis();
      uint16_t total_time = 0;
      double velocity = 0;
      double distance_traveled = 0;

      double t1 = (-16.0/9.0)*((MOVE_FORWARD_SPEED*MOVE_FORWARD_SPEED*MOVE_FORWARD_SPEED)/(distance_mm*distance_mm));
      double three_fourths_dist_over_moveSpeed = (0.75)*(distance_mm/MOVE_FORWARD_SPEED);
      double t2 = 0;
      while(distance_mm > distance_traveled) {

          oi_update(sensor_data);
          total_time = timer_getMillis() - initial_time;

          t2 = (total_time/1000.0-three_fourths_dist_over_moveSpeed);
          t2 *= t2;

          velocity = MOVE_FORWARD_SPEED + t1*t2;

          distance_traveled += sensor_data->distance;
          oi_setWheels(velocity, velocity);
      }
      oi_setWheels(0, 0);
      return distance_traveled;
}

double move_forward(oi_t* sensor_data, double distance_mm) {

    oi_update(sensor_data);
    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(MOVE_FORWARD_SPEED, MOVE_FORWARD_SPEED); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);


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

