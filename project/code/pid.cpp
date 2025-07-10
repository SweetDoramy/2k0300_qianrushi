#include "pid.h"

/**
  * @brief  限制 浮点数
  * @param  None
  * @note   None
  * @retval None
  */
 void Range_Protect_f(float* value, float max, float min){
    float v = *value;

    if(v > max)
    {
        *value = max;
    }
    else if (v < min)
    {
        *value = min;
    }
    else{
        *value = v;
    }
}

/**
  * @brief  限制 整型
  * @param  None
  * @note   None
  * @retval None
  */
 void Range_Protect_int(int* value, int max, int min){
    int v = *value;

    if(v > max)
    {
        *value = max;
    }
    else if (v < min)
    {
        *value = min;
    }
    else{
        *value = v;
    }
}

/**
  * @brief  限制 无符号整型
  * @param  None
  * @note   None
  * @retval None
  */
void Range_Protect_uint(unsigned long* value, unsigned long max, unsigned long min){
    unsigned long v = *value;

    if(v > max)
    {
        *value = max;
    }
    else if (v < min)
    {
        *value = min;
    }
    else{
        *value = v;
    }
}


/**
  * @brief  位置式PID参数结构体初始化
  * @param  PidPlaceStruct  位置式pid结构体指针
  * @note   None
  * @retval None
  */
 void Pid_PlaceStructInit(PidPlaceTypeDef *PidPlaceStruct)
 {
     PidPlaceStruct->kp = 0;
     PidPlaceStruct->ki = 0;
     PidPlaceStruct->kd = 0;
     PidPlaceStruct->Error = 0;
     PidPlaceStruct->Error_last = 0;
     PidPlaceStruct->Error_Sum = 0;
     PidPlaceStruct->limited = 0;
 }
 
 /**
  * @brief  增量式PID参数结构体初始化
  * @param  PidIncreaseStruct  增量式pid结构体指针
  * @note   None
  * @retval None
  */
void Pid_IncreaseStructInit(PidIncreaseTypeDef *PidIncreaseStruct)
{
    PidIncreaseStruct->kp = 0;
    PidIncreaseStruct->ki = 0;
    PidIncreaseStruct->kd = 0;
    PidIncreaseStruct->Error = 0;
    PidIncreaseStruct->Error_last = 0;
    PidIncreaseStruct->Error_last_last= 0;
}

/**
  * @brief  位置式PID控制
  * @param  Ps  位置式pid结构体指针
  * @param  Tar_Val         期望值
  * @param  Now_Val         当前值
  * @note   微分项为误差做差
  * @retval None
  */
 float Pid_PlaceControlS(PidPlaceTypeDef *Ps,float Tar_Val,float Now_Val)
 {
     float Out_Value;
     Ps->Error = (Tar_Val - Now_Val);              // 计算当前误差


     /******************************************** 舵机调整处 **********************************/
     if (fabs(Ps->Error) < 3.0f && fabs(ImuStruct.gyroZ) < 1.0f) {
        Ps->Error = 0; // 进入直道小幅偏差，压制摆动
    }


    // 快速将舵机输出拉回中心值
    if (fabs(Ps->Error) < 5.0f && fabs(ImuStruct.gyroZ) < 1.5f) {
        ServoStruct.PWM_Servo = MID_VALUE;
    }
    



    /******************************************** 舵机调整处 **********************************/
     Ps->Error_Sum += Ps->Error;
 
     Range_Protect_f(&(Ps->Error_Sum), Ps->limited, -Ps->limited);
 
     Out_Value = Ps->kp * Ps->Error + Ps->ki * Ps->Error_Sum + Ps->kd * (Ps->Error - Ps->Error_last);
 
     Ps->Error_last = Ps->Error;                    // 保存这次误差为下次一的上一次误差
 
     return Out_Value;
 }

/**
  * @brief  位置式PID控制
  * @param  Ps  位置式pid结构体指针
  * @param  Tar_Val         期望值
  * @param  Now_Val         当前值
  * @param  Kd_Target       微分项
  * @note   这里是自己给定微分项
  * @retval None
  */
 float Pid_PlaceControl(PidPlaceTypeDef *Ps,float Tar_Val,float Now_Val,float Kd_Target)
 {
     float Out_Value;
     Ps->Error = (Tar_Val - Now_Val);              // 计算当前误差
     Ps->Error_Sum += Ps->Error;
 
     Range_Protect_f(&(Ps->Error_Sum), Ps->limited, -Ps->limited);
 
     Out_Value = Ps->kp * Ps->Error + Ps->ki * Ps->Error_Sum + Ps->kd * Kd_Target;
 
     Ps->Error_last = Ps->Error;                    // 保存这次误差为下次一的上一次误差
 
     return Out_Value;
 }
 
/**
  * @brief  增量式PID控制
  * @param  Ps  增量式pid结构体指针
  * @param  Tar_Val         期望值
  * @param  Now_Val         当前值
  * @note   None
  * @retval None
  */
 float Pid_IncreaseControl(PidIncreaseTypeDef *Ps,float Tar_Val,float Now_Val)
 {
 
     float Out_Value;
     Ps->Error = Tar_Val - Now_Val;      //计算当前偏差
     Out_Value += Ps->kp * (Ps->Error - Ps->Error_last) + Ps->ki * Ps->Error + Ps->kd * (Ps->Error -2 * Ps->Error_last + Ps->Error_last_last);
     Ps->Error_last_last = Ps->Error_last;
     Ps->Error_last = Ps->Error;
 
     return Out_Value;
 }


 /**
  * @brief  位置式PID参数设置
  * @param  PidPlaceStruct  位置式pid结构体指针
  * @param  KP              比例系数
  * @param  KI              积分系数
  * @param  KD              微分系数
  * @note   None
  * @retval None
  */
void Pid_PlaceParmSet(PidPlaceTypeDef *PidPlaceStruct,float KP,float KI,float KD, float limited)
{
    PidPlaceStruct->kp = KP;
    PidPlaceStruct->ki = KI;
    PidPlaceStruct->kd = KD;
    PidPlaceStruct->limited = limited;
}

/**
  * @brief  增量式PID参数设置
  * @param  PidIncreaseStruct  增量式PID参数设置
  * @param  KP              比例系数
  * @param  KI              积分系数
  * @param  KD              微分系数
  * @note   None
  * @retval None
  */
void Pid_IncreaseParmSet(PidIncreaseTypeDef *PidIncreaseStruct,float KP,float KI,float KD)
{
    PidIncreaseStruct->kp = KP;
    PidIncreaseStruct->ki = KI;
    PidIncreaseStruct->kd = KD;
}


/**
  * @brief  误差清除，PID重置(位置)
  * @param  None
  * @note   主要是抗积分饱和
  * @retval None
  */
 void Pid_PlaceReset(PidPlaceTypeDef* Ps)
 {
     Ps->Error = 0;
     Ps->Error_last = 0;
     Ps->Error_Sum = 0;
 }
 
 /**
   * @brief  误差清除，PID重置(增量)
   * @param  None
   * @note   None
   * @retval None
   */
 void Pid_IncreaseReset(PidIncreaseTypeDef *Ps)
 {
     Ps->Error = 0;
     Ps->Error_last = 0;
     Ps->Error_last_last = 0;
 }