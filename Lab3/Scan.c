/*
 * Scan.c
 *
 *  Created on: Feb 12, 2025
 *      Author: ahogrady
 */

#define SCAN_RESOLUTION_ 2
#define SIG_DELTA_ 5
#define SIG_DIST_ 150
#define SIG_ANGULAR_WIDTH_ 5

#include "Scan.h"
#include "Timer.h"
#include "sleep.h"

//Servo tries to read values before getting to 0, causing noise
//function gives servo time to start at initial angle before full sweep
void init_scan(int initial_angle, cyBOT_Scan_t *scan_data) {
    cyBOT_Scan(0, scan_data);

    sleep_millis(2000);
}

//returns array of distances
double *scan_range(int min_angle, int max_angle) {
    cyBOT_Scan_t *scan_data = calloc(1, sizeof(cyBOT_Scan_t));
    double *data = (double *) malloc(((max_angle - min_angle) / SCAN_RESOLUTION_ + 3) * sizeof(double));
    data[0] = min_angle;
    data[1] = (max_angle - min_angle) / SCAN_RESOLUTION_ + 1;

    cyBot_sendString("Degrees    ", 11);
    cyBot_sendString("PING Distance (cm)\r\n", 19);

    //Servo tries to read values before set at 0, causing noise.
    init_scan(0, scan_data);

    int i = 2;
    while(min_angle <= max_angle) {
        cyBOT_Scan(min_angle, scan_data);

        data[i] = scan_data->sound_dist;

        char msg[100];
//
        sprintf(msg, "%d %lf\r\n", min_angle, scan_data->sound_dist);
//
        cyBot_sendString(msg, strlen(msg));


        min_angle += SCAN_RESOLUTION_;
        i++;
    }

    free(scan_data);

    return data;
}

obj_t *parse_scan_data(double *data) {
    int min_angle = (int) data[0];
    int len = (int) data[1];
    int obj_count = 0;
    int i;
    float *delta = (float *) malloc((len - 1) * sizeof(float));


    // calculates changes in distance
    for(i = 0; i < len - 1; i++) {
        delta[i] = absoluteVal(data[i + 2] - data[i + 3]);
    }



    // debug prints angle, distance, delta_difference
    cyBot_sendString("\r\n\n\n", 4);
    for(i = 0; i < len - 1; i++) {
        char msg[100];

        sprintf(msg, "%d, %lf, %lf\r\n", min_angle + i * SCAN_RESOLUTION_, data[i + 2], delta[i]);

        cyBot_sendString(msg, strlen(msg));
    }
    cyBot_sendString("\r\n\n\n", 4);


    // filters long-distance noise
    for(i = 0; i < len - 1; i++) {
        if(delta[i] > SIG_DELTA_ && (data[i + 2] > SIG_DIST_ && data[i + 3] > SIG_DIST_) ) { delta[i] = 0; }
    }


    // classify objects
    obj_t *head = calloc(1, sizeof(obj_t));
    obj_t *curr = head;
    int _i_width = 0;
    int _start_angle = min_angle;

    for(i = 0; i < len - 1; i++, _i_width++) {
        if(delta[i] > SIG_DELTA_) {
            curr->angular_width = _i_width * SCAN_RESOLUTION_;
            curr->angle = _start_angle + curr->angular_width / 2;
            curr->dist = data[(curr->angle - min_angle) / SCAN_RESOLUTION_ + 2];
            curr->next = calloc(1, sizeof(obj_t));
            curr->index = i;

            char msg[100];

            sprintf(msg, "Created object with angle %d, width %d, and distance %lf\r\n", curr->angle, curr->angular_width, curr->dist);

            cyBot_sendString(msg, strlen(msg));


            _i_width = 0;
            _start_angle += curr->angular_width;
            curr = curr->next;
        }
    }

    cyBot_sendString("1\r\n", -1);

    curr->angular_width = _i_width * SCAN_RESOLUTION_;
    curr->angle = _start_angle + curr->angular_width / 2;
    curr->dist = data[(curr->angle - min_angle) / SCAN_RESOLUTION_ + 2];
    curr->index = i;

    cyBot_sendString("2\r\n", -1);

    // filter far objects and noise
    curr = head;
    obj_t *_prev = NULL;

    while(curr) {
        if(curr->dist > SIG_DIST_ || curr->angular_width < SIG_ANGULAR_WIDTH_) { curr->index = -1; }

        curr = curr->next;
    }

//    while(curr) {
//        obj_t *_bad_obj = NULL;
//
//        if(curr->dist > SIG_DIST_ || curr->angular_width < SIG_ANGULAR_WIDTH_) {
//            if(_prev) { _prev->next = curr->next; }
//
//            _bad_obj = curr;
//        }
//
//
//        if(!_prev && !_bad_obj) { _prev = head; }
//        else if(_prev) { _prev = _prev->next; }
//        else { head = curr->next; }
//
//        curr = curr->next;
//
//        if(_bad_obj) { free(_bad_obj); }
//    }

    cyBot_sendString("3\r\n", -1);



    curr = head;


    while(curr) {
        if(curr->index >= 0) {
            char msg[100];

            sprintf(msg, "Angle: %d, width: %d, dist: %lf\r\n", curr->angle, curr->angular_width, curr->dist);

            cyBot_sendString(msg, strlen(msg));
        }
        curr = curr->next;
    }




    // pick narrowest angular width
    curr = head;
    obj_t *narrowest = curr;
    curr = curr->next;

    while(curr) {
        cyBot_sendString("4\r\n", -1);
        if(curr->index >= 0 && curr->angular_width < narrowest->angular_width) {
            free(narrowest);
            narrowest = curr;
            curr = curr->next;

        } else {
            obj_t *_o = curr;
            curr = curr->next;
            free(_o);
        }
    }

    cyBot_sendString("5\r\n", -1);


    free(delta);
    free(data);

    char msg[100];

    sprintf(msg, "Angle: %d, width: %d\r\n", narrowest->angle, narrowest->angular_width);

    cyBot_sendString(msg, strlen(msg));

    return narrowest;
}



