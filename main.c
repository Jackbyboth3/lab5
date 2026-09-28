/*
 * main.c
 *
 *  Created on: Sep 8, 2026
 *      Author: jackbyb
 */

#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "cyBot_uart.h"
#include "lcd.h"
#include "button.h"
#include <stdio.h>

void cyBot_sendString(char *message)
{
    while (*message != '\0')
    {
        cyBot_sendByte(*message);
        message++;
    }
}
int main(void)
{
    button_init();
    lcd_init();
    cyBot_uart_init();
    timer_init();

    uint8_t button = 0;
    uint8_t last_button = 0;
    char buffer[50];

    while (1)
    {
        button = button_getButton();

        if (button != last_button && button != 0)
        {

            lcd_printf("Button: %d", button);
            timer_waitMillis(200);

            sprintf(buffer, "Button %d pressed\r\n", button);
            cyBot_sendString(buffer);
        }

        last_button = button;
    }
}

