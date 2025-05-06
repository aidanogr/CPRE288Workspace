
#include <stdio.h>
#include <math.h>
#include "open_interface.h"
#include "movement.h"
#include "uart-interrupt.h"
#include "Scan.h"
#include "sleep.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "music.h"
#include "bno.h"
#include <stdint.h>

//global vars for scan_range()
int num_objects;
obj_t object_array[10];

bno_calib_t bno_calibration = {0xB, 0x2, 0xFFF7, 0x3EC, 0xFE52, 0xF8B5, 0xFFFE, 0xFFFE, 0xFFFF, 0x3E8, 0x2EF};
bno_t *bno;

/**
 * Points to right side of field before entering main function loop
 *  so gui direction lines up with actual cyBot direction
 *  @param degree to point cybot towards; ANGLE IS NOT THE SAME AS OI ANGLE, IMU VALUES ONLY
 */
void point_cybot(int degrees) {

    bno_update(bno);
    float target = bno->euler.heading / 16.;

    oi_setWheels(-100, 100);
    while( (target > (degrees + 1)) || (target < (degrees - 1)) ) {
        bno_update(bno);
        target = bno->euler.heading / 16.;
    }
    oi_setWheels(0, 0);
   // bno_free(bno);
    return;

}


/*
 * Prints off imu angles. Set desired angle to start with point_cybot(degrees);
 * Break from function by holding button 4 (frees struct)
 */
void callibrate_imu() {



}

void initialize_imu() {
    //lcd_printf("%lf", 5.3);
    bno = bno_alloc();
    bno_initCalib(&bno_calibration);

    bno_update(bno);

    return;
}

/**
 * handles command from uart interrupt
 */
void execute_command(uint8_t opcode, uint8_t param1, uint8_t param2) {

    if((char) opcode == 'w') {
        move_forward(((double) param1) * ((double) param2));

    } else if((char) opcode == 's') {
        move_forward(((double) param1) * -1 * ((double) param2));

    } else if((char) opcode == 'p') {
        scan_range(param1, param2, object_array, &num_objects);

    } else if((char) opcode == 'a') {
        turn_left(((double) param1) * param2);

    } else if((char) opcode == 'd') {
        turn_right((double) param1);
    } else if((char) opcode == 'b') {
        point_cybot();
    }

    uart_sendStr("done\n");
}



int main() {

    // === INITIALIZATIONS ===
    movement_init();
    oi_setWheels(0, 0);
    timer_init();
    button_init();
    lcd_init();
    uart_interrupt_init();
    cyBOT_init_scan();
    initialize_servo();
    num_objects = 0;    //used for scan_range()

    initialize_imu();
    short last_imu_angle = bno->euler.heading; //IMU angle for gui changes
    int last_angle_difference = 0;
    int delta_angle = 0;
    char str_buffer[20];

    int button_pressed = 0;

    // === CALIBRATIONS ===
    // callibrate_servo();
    servo_set_callibration(-84, 232);
    //callibrate_imu();


    // === MAIN FUNCTION LOOP ===
    while(1) {

        //wait for uart interrupt handler to populate Interrupt_Result and send change in angle since the start to gui
        //ToDo make sure cliff sensors are properly set for test field
        while(Interrupt_Ready != 1) {
            bno_update(bno);
            //lcd_printf("%hd", bno->euler.heading /16 );
            delta_angle = last_imu_angle/16 - bno->euler.heading /16;
            if(delta_angle > 0 || delta_angle < 0) {
                last_imu_angle = bno->euler.heading;
                sprintf(str_buffer, "turned,%d\n", -delta_angle);
                uart_sendStr(str_buffer);
            }

        }

        execute_command((uint8_t) ((Interrupt_Result & 0xFF0000) >> 16),
                        (uint8_t) ((Interrupt_Result & 0xFF00) >> 8),
                        (uint8_t) (Interrupt_Result & 0xFF));

        reset_Interrupt();
        bno_update(bno);

    }


}





