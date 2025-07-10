#ifndef _beep_h
#define _beep_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"

#define BEEP        "/dev/zf_driver_gpio_beep"          //蜂鸣器

void beep_run(void);


#endif