#ifndef _motor_h
#define _motor_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "pid.h"
#include "encoder.h"
#include "oscilloscope.h"
#include "filter.h"

#define MOTOR1_DIR   "/dev/zf_driver_gpio_motor_1"
#define MOTOR1_PWM   "/dev/zf_device_pwm_motor_1"

#define MOTOR2_DIR   "/dev/zf_driver_gpio_motor_2"
#define MOTOR2_PWM   "/dev/zf_device_pwm_motor_2"

#define close_speed 100

extern struct pwm_info motor_1_pwm_info;
extern struct pwm_info motor_2_pwm_info;

typedef struct motor
{
    float encoder_EMA_L;
    float encoder_EMA_R;            // 经过EMA滤波后的编码器值
    float EMA_alpha;
    
    float duty_encoder_L;            //PID返回的对应编码器数值
    float duty_encoder_R;
    //测试
    // float duty_raw_pid_L;
    // float duty_raw_pid_R;
    float expect_val_encoder;        //闭环期望速度，这里我们给编码器对应数值

    float open_duty;
}MotorStructTypeDef;

void motor_set(float duty_L,float duty_R);
void motor_open_control_Init(void);
void motor_control_Init(void);
void motor_control(void);
void motor_control_param(float* motor_kp_L,float* motor_ki_L,float* motor_kd_L,float* motor_kp_R,float* motor_ki_R,float* motor_kd_R);
void motor_control_param_set(uint8 mode, float step);
void speed_param(float* closed_speed,float* open_speed);
void speed_param_set(uint8 mode, float step);

void test_close(void);

extern MotorStructTypeDef Motor;

#endif