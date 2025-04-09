/*
 * servo.h
 *
 *  Created on: Apr 8, 2025
 *      Author: ahogrady
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>


int convert_degrees_to_pulse_width(int degrees);
void initialize_servo();
void servo_move_to(int pulse_width);



#endif /* SERVO_H_ */
