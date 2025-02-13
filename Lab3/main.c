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
#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "uart.h"
#include "Scan.h"



int main() {
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                      // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    cyBot_uart_init();
    cyBOT_init_Scan(0b011);

    double *data = scan_range(0, 180);

    int angle = (parse_scan_data(data)->angle) - 90;



    if(angle < 0) {
        turn_right(sensor_data, absoluteVal(angle));
    } else {
        turn_left(sensor_data, angle);
    }




//    while(1) {
//        char b = cyBot_getByte();
//        lcd_printf("%c", b);
//        char message[20];
//        sprintf(message, "%c recieved by cybot", b);
//
//        cyBot_sendString(message, strlen(message));
//    }



}



