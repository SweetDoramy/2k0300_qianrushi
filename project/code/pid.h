#ifndef _pid_h
#define _pid_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "imu.h"
#include "servo.h"

typedef struct{
    float kp;
    float ki;
    float kd;
    float Error;        // 当前误差
    float Error_last;      // 上次误差
    float Error_Sum;    // 误差总和
    float limited;      // 积分限幅
}PidPlaceTypeDef;       // 位置PID参数结构体

typedef struct{
    float kp;
    float ki;
    float kd;
    float Error;        // 当前误差
    float Error_last;      // 上一次误差
    float Error_last_last;      // 上上一次误差
}PidIncreaseTypeDef;    // 增量PID参数结构体

// 限幅函数
void Range_Protect_f(float* value, float max, float min);
void Range_Protect_int(int* value, int max, int min);
void Range_Protect_uint(unsigned long* value, unsigned long max, unsigned long min);

// 位置式PID相关接口
void  Pid_PlaceStructInit(PidPlaceTypeDef *PidPlaceStruct);
void  Pid_PlaceParmSet(PidPlaceTypeDef *PidPlaceStruct,float KP,float KI,float KD, float limited);
float Pid_PlaceControl(PidPlaceTypeDef *Ps,float Tar_Val,float Now_Val,float Kd_Target);
float Pid_PlaceControlS(PidPlaceTypeDef *Ps,float Tar_Val,float Now_Val);
void  Pid_PlaceReset(PidPlaceTypeDef* Ps);

// 增量式PID相关接口
void  Pid_IncreaseStructInit(PidIncreaseTypeDef *PidIncreaseStruct);
void  Pid_IncreaseParmSet(PidIncreaseTypeDef *PidIncreaseStruct,float KP,float KI,float KD);
float Pid_IncreaseControl(PidIncreaseTypeDef *Ps,float Tar_Val,float Now_Val);
void  Pid_IncreaseReset(PidIncreaseTypeDef *Ps);

#endif