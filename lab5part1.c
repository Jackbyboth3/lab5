/*
 * lab5part1.c
 *
 *  Created on: Sep 27, 2026
 *      Author: jackbyb
 *
 *      NOTES:
 *      goal is to clean up a data set gathered by the cybot
 */
#include <stdio.h>
#include <math.h>

#include "sensor-data5.h"

#include "lcd.h"
#include "cyBot_uart.h"
#include "Timer.H"
#define DATA 90


float clean_data[DATA];


void clean_sensor_data(float raw[], float clean[], int data_size) {
    int i=0;

    //for (i = 0; i < DATA; i++) {
    //if(array_val < middle_val && array_val2 < middle_val) {
    for (i = 0; i < data_size; i++) {
        clean[i] = raw[i];

    }
    for (i = 1; i < data_size - 1; i++){
        float array_val = raw[i-1];
        float array_val2 = raw[i+1];
        float middle_val = raw[i];

        if (fabs(middle_val - array_val) > 12 && fabs(middle_val - array_val2) > 12 && fabs(array_val- array_val2) < 12) {
            clean[i] = (array_val + array_val2) / 2.0;

        }

    }
    for (i = 0; i < data_size; i++) {

    printf("%.2f\t%.2f\n", raw[i], clean[i]);
    }
    
}
