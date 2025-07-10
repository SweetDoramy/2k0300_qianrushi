#ifndef _servo_h
#define _servo_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "pid.h"
#include "imu.h"
#include "motor.h"

#define SERVO_MOTOR1_PWM            "/dev/zf_device_pwm_servo"

#define MID_VALUE 99          //舵机中值，需要测试
// 定义主板上舵机活动范围 角度                                                                                                         
#define SERVO_MOTOR_L_MAX           (78)                       
#define SERVO_MOTOR_R_MAX           (122)       

extern struct pwm_info servo_pwm_info;

typedef struct servo
{
    float PWM_Servo_pid;     //pid值
    float PWM_Servo_update;         //转换后的最终值
    float PWM_Servo;
    float gyro_GKD;          //陀螺仪角速度增益
    float speed_factor;      //速度转换比例系数 
}ServoTypeDef;

extern ServoTypeDef ServoStruct;

float map_link(float x, float in_min, float in_max, float out_min, float out_max);

void servo_set(float servo_duty);
void pit_callback_servo_balance(void);
void servo_pit_init(void);
void servo_control_param(float* servo_kp,float* servo_ki,float* servo_kd);
void servo_control_param_set(uint8 mode, float step);

#endif