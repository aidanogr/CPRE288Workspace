/*
 * main.c
 *
 *  Created on: Jan 30, 2025
 *      Author: ahogrady
 *
 *      LAST COMMIT: FIXED INITIAL DISTANCE IN "move_forward(...)"
 *                   ADDED "move_forward1(...)" FOR EASE-IN-OUT ALGORITHM
 */

#include <stdio.h>
#include <math.h>
#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "Scan.h"
#include "sleep.h"
#include "adc.h"






int main() {
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    //oi_init(sensor_data); // do this only once at start of main()

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                      // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    uart_interrupt_init();
    cyBOT_init_Scan(0b111);

    //cyBOT_SERVO_cal();
    right_calibration_value = 238000;
    left_calibration_value = 1230250;

    adc_init();
    cyBOT_Scan(90, NULL);
    int32_t sum = 0;
    lcd_printf("");
    int counter = 0;
    while(counter < 16) {
        sum += adc_read();
      // lcd_printf("%d", result);
        sleep_millis(50);
        counter++;
    }

    int32_t avg = sum/16;

    // calculate distance
    double a = 791398.7;
    double b = 2.603902;
    double c = 24.562;
    double d = 7.474858;
    double dist = d + (a-d)/(1+pow(avg/c,b));

    lcd_printf("ADC: %d\nDistance: %lf", avg, dist);



    //oi_free(sensor_data);
}



