#include <stdio.h>
#include "sensor-data.h"
//#define data = 46;

int main(void)
{
    int i;

    for (i = 0; i < 92; i++)
    {
        printf("%f\n", sensor_data_array[i]);
    }

}
