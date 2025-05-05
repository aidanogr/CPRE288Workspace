
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



int num_objects;
obj_t object_array[10];
bno_calib_t bno_calibration = {0xB, 0x2, 0xFFF7, 0x3EC, 0xFE52, 0xF8B5, 0xFFFE, 0xFFFE, 0xFFFF, 0x3E8, 0x2EF};



void execute_command(uint8_t opcode, uint8_t param1, uint8_t param2) {
    //static int num_recieved = 0;

    //lcd_printf("%c,%.2f,%d", opcode, (double) ((int) param1), (int)param2);
    lcd_printf("3");
    //move_forward(200);
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
    }

    lcd_printf("4");

    uart_sendStr("done\n");
}

int main() {


    movement_init();

    timer_init();
    button_init();
    lcd_init();
    uart_interrupt_init();
    cyBOT_init_scan();
  //  cyBOT_init_Scan(0b100);
   // callibrate_servo();
    servo_set_callibration(-84, 232);
    num_objects = 0;




    bno_t *bno = bno_alloc();
    bno_initCalib(&bno_calibration);

    bno_update(bno);
    float target = bno->euler.heading / 16.;

    while(1) {
        bno_update(bno);
        target = bno->euler.heading / 16.;
        lcd_printf("%lf", target);
        timer_waitMillis(500);
    }













//    oi_setWheels(0, 0);


    while(1) {
        //wait for command from uart
        while(Interrupt_Ready != 1) { lcd_printf("%d", sensor_data->cliffFrontLeftSignal); oi_update(sensor_data); }
        lcd_printf("2");
        execute_command((uint8_t) ((Interrupt_Result & 0xFF0000) >> 16), (uint8_t) ((Interrupt_Result & 0xFF00) >> 8), (uint8_t) (Interrupt_Result & 0xFF) );
        lcd_printf("5");
        reset_Interrupt();
        lcd_printf("6");
    }
    //move_forward(sensor_data, 80);
    //scan_range(0,180, object_array, &num_objects);

/*
    while(1) {
        if(round > 1) {
            min_angle = 45;
            max_angle = 135;
        }

        scan_range(min_angle, max_angle, object_array, &num_objects);

        if(num_objects == 0) {
            lcd_printf("We are NOT finding this mf\n");
            break;
        }

        int skinniest_index = 0;
        for(i = 1; i < num_objects; i++) {
            if(object_array[i].width < object_array[skinniest_index].width) { skinniest_index = i; }
        }

        // found skinniest_index, turn to it with servo (find angle)
        int angle_of_skinniest_object = (object_array[skinniest_index].start_angle + object_array[skinniest_index].end_angle) / 2;
        cyBOT_Scan(angle_of_skinniest_object, NULL);
        lcd_printf("deg of skinniest: %d\n", angle_of_skinniest_object);

        //turn to angle with cybot      //TODO What does this do?
        if(angle_of_skinniest_object < 90) {
            turn_right(sensor_data, 90 - angle_of_skinniest_object - 3);
        } else {
            turn_left(sensor_data, angle_of_skinniest_object - 90 - 3);
        }

        int go_this_far = object_array[skinniest_index].distance * 10 - 190;    //TODO where does this value come from
        int went_this_far = move_to_obj(sensor_data, go_this_far);
        if(went_this_far >= go_this_far) { break; }                             //TODO i dont know how this termination works


        round++;
    }




*/




}





