#include "Timer.h"
#include "lcd.h"
#include "button.h"
#include "cyBot_Scan.h"

int main(void)
{
    cyBOT_SERVRO_cal_t calibration;

    
    timer_init();
    lcd_init();
    button_init();

    
    cyBOT_init_Scan(0b0001);

    
    calibration = cyBOT_SERVO_cal();

    // display
    lcd_printf("Right: %d\nLeft: %d",
              calibration.right,
               calibration.left);

    while (1)
    {
    }

    return 0;
}
