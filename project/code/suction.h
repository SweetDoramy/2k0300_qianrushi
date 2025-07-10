#ifndef _suction_h
#define _suction_h

#include "zf_common_headfile.h"

#define Suction_PWM_1           "/dev/zf_device_pwm_esc_1"
#define Suction_PWM_2           "/dev/zf_device_pwm_esc_2"

void suction_init(uint16 duty_1,uint16 duty_2);

#endif