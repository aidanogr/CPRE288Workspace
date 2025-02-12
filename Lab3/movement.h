/*
 * movement.h
 *
 *  Created on: Feb 4, 2025
 *      Author: ahogrady
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void move_backwards(oi_t* sensor_data, double distance_mm);
double move_forward(oi_t* sensor_data, double distance_mm);
double move_forward1(oi_t* sensor_data, double distance_mm);
void turn_left(oi_t* sensor_data, double degrees);
void turn_right(oi_t* sensor_data, double degrees);

#endif /* MOVEMENT_H_ */
