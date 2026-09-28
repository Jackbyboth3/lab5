#include <stdio.h>

#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"


int main(void)
{
    cyBOT_Scan_t scan;

    timer_init();
    lcd_init();
    cyBot_uart_init();

    cyBOT_init_Scan(0b0111);

    //temp for now
    right_calibration_value = 311500;
    left_calibration_value = 1309000;

    while (1)
    {
        char command;

        command = cyBot_getByte();

        if (command == 'm')
        {
            int angle;
            char message[] = "Degrees distance (cm)\r\n";

            int i = 0;
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
        }
    }

}
