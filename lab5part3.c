/*
 * lab5part3.c
 *
 *  Created on: Sep 30, 2026
 *      Author: jackbyb
 */
#include <stdio.h>
#include <math.h>
#include "open_interface.h"
#include "sensor-data5.h"
#include "cyBot_Scan.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "Timer.H"
#include "movement.h"

#define DATA 90
#define MAX_OBJECTS 10
#define OBJECT_THRESHOLD 120.0

typedef struct
{
    int start_angle;
    int end_angle;
    int center_angle;
    float distance;
    int radial_width;
    float linear_width;

} Object;
int i=0;
int angle =0;

    int object_count = 0;
    int start_index = 0;
    int end_index = 0;
    char output[100];
    int j = 0;
    oi_t *sensor_data = oi_alloc();



Object objects[MAX_OBJECTS];
float raw_data[DATA];

float clean_data[DATA];

void movement(int direction, int speed)
{
    double sum = 0;
    // Implement movement logic here
    switch (direction)
    {
    case 'w': // forward

        oi_setWheels(150, 150); // move forward; full speed
        sum = 0;
        while (sum < 50)
        {
            oi_update(sensor_data);
            sum += sensor_data->distance;
        }
        oi_setWheels(0, 0); // stop

        break;
    case 's': // backward

        oi_setWheels(-150, -150); // backward
        sum = 0;
        while (sum > -50)
        {
            oi_update(sensor_data);
            sum += sensor_data->distance;
        }
        oi_setWheels(0, 0); // stop
        oi_free(sensor_data);
        break;
    case 'a': // left

        oi_setWheels(100, -100); // left
        sum = 0;
        while (sum < 50)
        {
            oi_update(sensor_data);
            sum += sensor_data->distance;
        }
        oi_setWheels(0, 0); // stop
        oi_free(sensor_data);
        break;
    case 'd': // right

        oi_setWheels(-100, 100); // right
        sum = 0;
        while (sum > -50)
        {
            oi_update(sensor_data);
            sum += sensor_data->distance;
        }
        oi_setWheels(0, 0); // stop
        oi_free(sensor_data);
        break;


    case 'm': // right

//        char message[] = "Degrees distance (cm)\r\n";


        for (i = 0; message[i] != '\0'; i++)
        {
    cyBot_sendByte(message[i]);
    }
        for (angle = 0; angle <= 180; angle += 2)
        {
    cyBOT_Scan(angle, &scan);
    char output[50];

    sprintf(output, "%d %.2f\r\n", angle, scan.sound_dist);

    for (i = 0; output[i] != '\0'; i++)
    {
        cyBot_sendByte(output[i]);
    }
}


//void main() {
//oi_t *sensor_data = oi_alloc();
//oi_init(sensor_data);
//double sum = 0;
//oi_setWheels(500, 500); // move forward; full speed
//while (sum < 5000) {
//oi_update(sensor_data);
//sum += sensor_data->distance;
//}
//oi_setWheels(0, 0); // stop
//oi_free(sensor_data);
//}

void clean_sensor_data(float raw[], float clean[], int data_size) {//add brackt
int i = 0;

//for (i = 0; i < DATA; i++) {
//if(array_val < middle_val && array_val2 < middle_val) {
for (i = 0; i < data_size; i++)
{
    clean[i] = raw[i];

}
for (i = 1; i < data_size - 1; i++)
{
    float array_val = raw[i - 1];
    float array_val2 = raw[i + 1];
    float middle_val = raw[i];

    if (fabs(middle_val - array_val) > 12 && fabs(middle_val - array_val2) > 12
            && fabs(array_val - array_val2) < 12)
    {
        clean[i] = (array_val + array_val2) / 2.0;

    }


for (i = 0; i < data_size; i++)
{

    printf("%.2f\t%.2f\n", raw[i], clean[i]);
}
}

int detect_objects(float data[], int size, Object objects[])
{
int object_count = 0;
int in_object = 0;

int start_index = 0;
int end_index = 0;

int i;

for (i = 0; i < size; i++)
{

    if (data[i] > 0 && data[i] < OBJECT_THRESHOLD)
    {
        if (!in_object)
        {
            start_index = i;
            in_object = 1;
        }
    }

    else
    {
        if (in_object)
        {
            end_index = i - 1;

            if (object_count < MAX_OBJECTS)
            {
                objects[object_count].start_angle = (start_index * 180)
                        / (size - 1);

                objects[object_count].end_angle = (end_index * 180)
                        / (size - 1);

                objects[object_count].center_angle =
                        (objects[object_count].start_angle
                                + objects[object_count].end_angle) / 2;

                objects[object_count].distance = data[start_index];

                objects[object_count].linear_width =
                        objects[object_count].end_angle
                                - objects[object_count].start_angle;

                object_count++;
            }

            in_object = 0;
        }


}

if (in_object && object_count < MAX_OBJECTS)
{
    end_index = size - 1;

    objects[object_count].start_angle = (start_index * 180) / (size - 1);

    objects[object_count].end_angle = (end_index * 180) / (size - 1);

    objects[object_count].center_angle = (objects[object_count].start_angle
            + objects[object_count].end_angle) / 2;

    objects[object_count].distance = data[start_index];

    objects[object_count].linear_width = objects[object_count].end_angle
            - objects[object_count].start_angle;

    object_count++;
}

return object_count;

}

int find_smallest_object(Object objects[], int object_count)
{
int smallest;
int i;

if (object_count == 0)
{
    return -1;
}

smallest = 0;

for (i = 1; i < object_count; i++)
{
    if (objects[i].linear_width < objects[smallest].linear_width)
    {
        smallest = i;
    }
}

return smallest;
}

int main(void)
{

timer_init();
lcd_init();
cyBot_uart_init();
cyBOT_init_SCAN();

//allocating and initalizing open interface sensor data once

oi_t *sensor_data = oi_alloc();
oi_init(sensor_data);

while (1)
{
    // movement()
}
}

