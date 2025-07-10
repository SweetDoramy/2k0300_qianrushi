#include"beep.h"

void beep_run(void)
{
    gpio_set_level(BEEP, 0x1);
    system_delay_ms(50);
    gpio_set_level(BEEP, 0x0);
}