/*
 * servo.c
 *
 *  Created on: Apr 8, 2025
 *      Author: ahogrady
 */
#include "servo.h"


int convert_degrees_to_pulse_width(int degrees) {
    return ((degrees/180.0 + 1)/1000)*16000000;
}

void initialize_servo() {
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x2) ==0) {}
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_DEN_R |= 0x20;

    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x0000700000;
    SYSCTL_RCGCTIMER_R |= 0x02;
    
    TIMER1_CTL_R &= ~0x4000;
    while ((SYSCTL_PRTIMER_R & 0x2) == 0) {}

    TIMER1_CTL_R &= ~0x0100;
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R |= 0x8;
    TIMER1_TBMR_R &= ~0x4;
    TIMER1_TBMR_R |= 0x2;
    TIMER1_TBMATCHR_R = 320000;

    TIMER1_CTL_R |= 0x0100;
}



void servo_move_to(int pulse_width) {
    int cycles = pulse_width + 320000;
    
    TIMER1_TBILR_R &= ~0xFFFF;
    TIMER1_TBILR_R |= (cycles) & 0xFFFF;
    TIMER1_TBPR_R &= ~0xFF;
    TIMER1_TBPR_R |= (cycles >> 16) & 0xFF;
}



