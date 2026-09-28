#include <stdio.h>
#include <math.h>

#include "sensor-data5.h"
#include "cyBot_Scan.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "Timer.H"

#define MAX_OBJECTS 10
#define DATA 89
//change later

#define OBJECT_THRESHOLD 50.0

float calculate_actual_width(float left_distance, float right_distance,
                             int angle)
{
    float radians;

    radians = angle * M_PI / 180.0;

    return sqrt(left_distance * left_distance + right_distance * right_distance - 2.0 * left_distance * right_distance * cos(radians));
}
float linear_actual_width(float radial_width, float distance) {


    float radial_wid_rad = radial_width * (M_PI / 180.0);
    float linear_width = 2.0 * distance * tanf(radial_wid_rad / 2.0);

    return linear_width;

}
typedef struct
{
    int start_angle;
    int end_angle;
    int center_angle;
    float distance;
    int radial_width;
    float actual_width;
    float linear_width;

} Object;

Object objects[MAX_OBJECTS];


int main(void)
{
    int smallest_object = 0;


    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0011);

    cyBOT_Scan_t scan;
    int i;
    int object_count = 0;
    int in_object = 0;

    int start_index = 0;
    int end_index = 0;
    char output[100];
    int j = 0;
    // float angle_step = 1.0;
    //float angle_step = 180 / (DATA);

    right_calibration_value = 311500;
    left_calibration_value = 1309000;
    for (i = 0; i < DATA; i++)
    {
        float distance = sensor_data_array[i];

        if (distance > 0 && distance < OBJECT_THRESHOLD)
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
//                    objects[object_count].start_angle =
//                            sensor_data_array[start_index];
                    objects[object_count].start_angle = (start_index * 180)
                            / (DATA);

                    objects[object_count].end_angle = (end_index * 180)
                            / (DATA);

                    objects[object_count].center_angle =
                            (objects[object_count].start_angle
                                    + objects[object_count].end_angle) /2.0;

                    objects[object_count].distance =
                            sensor_data_array[start_index];

                    objects[object_count].linear_width =
                            objects[object_count].end_angle
                                    - objects[object_count].start_angle;

                    object_count++;
                }

                in_object = 0;
            }
        }
    }

    if (in_object && object_count < MAX_OBJECTS)
    {
        end_index = i - DATA;

        objects[object_count].start_angle = (start_index * 180) / (DATA);

        objects[object_count].end_angle = (end_index * 180) / (DATA);

        objects[object_count].center_angle = (objects[object_count].start_angle + objects[object_count].end_angle);

        objects[object_count].distance = sensor_data_array[start_index];

        objects[object_count].linear_width = objects[object_count].end_angle - objects[object_count].start_angle;

        object_count++;
    }

    sprintf(output, "Object#  Angle  Distance  Width\r\n");
    for (j = 0; output[j] != '\0'; j++)
    {
        cyBot_sendByte(output[j]);
    }

    for (i = 0; i < object_count; i++)
    {
        sprintf(output, "%d        %d      %.2f      %d\r\n", i + 1, objects[i].center_angle, objects[i].distance, abs(objects[i].linear_width));
        for (j = 0; output[j] != '\0'; j++) {
            cyBot_sendByte(output[j]);
        }

    }
    if (object_count > 0)
    {
        for (i = 1; i < object_count; i++)
        {
            if (objects[i].linear_width < objects[smallest_object].linear_width)
            {
                smallest_object = i;
            }
        }

        sprintf(output, "\nsmallest object is: %d\r\n", smallest_object + 1);
        for (j = 0; output[j] != '\0'; j++)
        {
            cyBot_sendByte(output[j]);
        }

        sprintf(output, "Point servo to angle: %d degrees\r\n", objects[smallest_object].center_angle);
        for (j = 0; output[j] != '\0'; j++)
        {
            cyBot_sendByte(output[j]);
        }

        cyBOT_Scan(objects[smallest_object].center_angle, &scan);
        //cyBOT_Scan(180, &scan);
    }

    return 0;
}
