/*
 * Scan.c
 *
 *  Created on: Feb 12, 2025
 *      Author: ahogrady
 */

#define SCAN_RESOLUTION_ 1
#define SIG_DELTA_ 5

#include "Scan.h"



double *scan_range(int min_angle, int max_angle) {
    cyBOT_Scan_t *scan_data = calloc(1, sizeof(cyBOT_Scan_t));
    double *data = (double *) malloc(((max_angle - min_angle) / SCAN_RESOLUTION_ + 3) * sizeof(double));
    data[0] = min_angle;
    data[1] = (max_angle - min_angle) / SCAN_RESOLUTION_ + 1;

    cyBot_sendString("Degrees    ", 11);
    cyBot_sendString("PING Distance (cm)\n", 19);

    int i = 2;
    while(min_angle <= max_angle) {
        cyBOT_Scan(min_angle, scan_data);

        data[i] = scan_data->sound_dist;

//        char msg[100];
//
//        sprintf(msg, "%d %lf\n\r", min_angle, scan_data->sound_dist);
//
//        cyBot_sendString(msg, strlen(msg));


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

    float *delta = (float *) malloc((len - 1) * sizeof(float));

    int i;
    // calculates changes in distance
    for(i = 0; i < len - 1; i++) {
        delta[i] = absoluteVal(data[i + 2] - data[i + 3]);
    }

    cyBot_sendString("\r\n\n\n", 4);
//
    for(i = 0; i < len - 1; i++) {
        char msg[100];

        sprintf(msg, "%d, %lf, %lf\r\n", min_angle + i * SCAN_RESOLUTION_, data[i + 2], delta[i]);

        cyBot_sendString(msg, strlen(msg));
    }

    cyBot_sendString("\r\n\n\n", 4);

    // filters long-distance noise
    for(i = 0; i < len - 1; i++) {
        if(delta[i] > SIG_DELTA_ && data[i + 2] > 175 && data[i + 3] > 175) { delta[i] = 0; }
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

            curr->next = calloc(1, sizeof(obj_t));


            _i_width = 0;
            _start_angle += curr->angular_width;
            curr = curr->next;
        }
    }

    curr->angular_width = _i_width * SCAN_RESOLUTION_;
    curr->angle = _start_angle + curr->angular_width / 2;

    // filter far objects and noise
    curr = head;

    while(curr) {
        if(data[(curr->angle - min_angle) / SCAN_RESOLUTION_ + 2] > 200 || curr->angular_width < 3) {
            curr->index = -1;
        }

        curr = curr->next;
    }




    curr = head;


    while(curr) {
        if(curr->index >= 0) {
        char msg[100];

        sprintf(msg, "Angle: %d, width: %d\r\n", curr->angle, curr->angular_width);

        cyBot_sendString(msg, strlen(msg));
        }
        curr = curr->next;
    }




    // pick narrowest angular width
    curr = head;

    while(curr && curr->index < 0) {
        obj_t *_o = curr->next;
        free(curr);
        curr = _o;
    }


    obj_t *narrowest = curr;

    while(curr) {
        if(curr->index >= 0 && curr->angular_width < narrowest->angular_width) {
            free(narrowest);
            narrowest = curr;

        } else {
            obj_t *_o = curr;
            curr = curr->next;
            free(_o);
        }
    }


    free(delta);

    char msg[100];

            sprintf(msg, "Angle: %d, width: %d\r\n", narrowest->angle, narrowest->angular_width);

            cyBot_sendString(msg, strlen(msg));

    return narrowest;
}



