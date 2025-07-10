#include"servo.h"
#include <zf_driver_delay.h>
#include"img_control.h"
#include "code_headfiles.h"

struct pwm_info servo_pwm_info;
PidPlaceTypeDef PIDServoStruct;
ServoTypeDef ServoStruct;

EMA_filter_Struct Gyro_filter;

// 定义主板上舵机频率  请务必注意范围 50-300
// 如果要修改，需要直接修改设备树。
#define SERVO_MOTOR_FREQ            (servo_pwm_info.freq)                       

// 在设备树中，默认设置的10000。如果要修改，需要直接修改设备树。
#define PWM_DUTY_MAX                (servo_pwm_info.duty_max)       



// ------------------ 舵机占空比计算方式 ------------------
// 
// 舵机对应的 0-180 活动角度对应 控制脉冲的 0.5ms-2.5ms 高电平
// 
// 那么不同频率下的占空比计算方式就是
// PWM_DUTY_MAX/(1000/freq)*(0.5+Angle/90) 在 50hz 时就是 PWM_DUTY_MAX/(1000/50)*(0.5+Angle/90)
//
//就是占空比最大值*百分比,这个1000就是将周期单位换算成ms
//
//所以这里只需要设置角度即自动算出占空比
// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_MOTOR_DUTY(x)  ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))


/**
  * @brief  自定义 map() 线性映射函数函数,速度联动补偿
  * @param  x:当前车速
  *         in_min:设定的最小车速
  *         in_max:设定的最大车速
  *         out_min:设定最小车速对应的比例系数
  *         out_max:设定最大车速对应的比例系数
  * @note   车速越高，舵机响应也越强（即 PID 输出更大）
  *         高速过弯时： 由于惯性大，车辆如果转向不够激烈，容易冲出弯道；
  *         低速行驶时： 如果转向太敏感，反而会导致抖动或过度摆动；
  * @retval None
  */
float map_link(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
  * @brief  舵机打角
  * @param  float servo_duty：舵机动作角度
  * @note   开环
  * @retval None
  */
void servo_set(float servo_duty)   //舵机动作角度
{
    // 获取PWM设备信息
    pwm_get_dev_info(SERVO_MOTOR1_PWM, &servo_pwm_info);

    //设置舵机打角值
    pwm_set_duty(SERVO_MOTOR1_PWM, (uint16)SERVO_MOTOR_DUTY(servo_duty));

    if(servo_duty >= SERVO_MOTOR_R_MAX)
    {
        servo_duty = SERVO_MOTOR_R_MAX;
    }
   
    if(servo_duty <= SERVO_MOTOR_L_MAX)
    {
        servo_duty = SERVO_MOTOR_L_MAX;
    } 


}

/**
  * @brief  舵机平衡控制
  * @param  None
  * @note   放在定时器中断中,目前先不搞陀螺仪，置为0
  * @retval None
  */
void pit_callback_servo_balance(void)
 {
  // 开环,二选一
  // if(state_stayin == TURN){
  //   if(ImgControl.MidLine_value - Mid_img < 0 )  motor_set( Motor.open_duty + (int)(ImgControl.MidLine_value/30), Motor.open_duty);
  //   else   motor_set( Motor.open_duty , Motor.open_duty + (int)(ImgControl.MidLine_value/30));
  // }
  // else  motor_set( Motor.open_duty, Motor.open_duty);

  // 闭环,二选一
  // if(state_stayin == TURN){
  //   if(ImgControl.MidLine_value - Mid_img < 0 )  motor_set( Motor.duty_encoder_R + (int)(ImgControl.MidLine_value/30), Motor.duty_encoder_L);
  //   else   motor_set( Motor.duty_encoder_R , Motor.duty_encoder_L + (int)(ImgControl.MidLine_value/30));
  // }
  // else  motor_set( Motor.duty_encoder_R, Motor.duty_encoder_L);

  frequence_control_counter++;
 
  //期望值画面中值，当前值中线数值，陀螺仪当前为0，用的位置式pid
    ServoStruct.PWM_Servo_pid = Pid_PlaceControlS(&PIDServoStruct,Mid_img,ImgControl.MidLine_value);

    // //弯道加上速度联动补偿
    // if(state_stayin == TURN){
    //   if(fabs(ImuStruct.gyroZ) < 7.5f) ServoStruct.gyro_GKD = 0.27;
    //   ServoStruct.PWM_Servo_pid = Pid_PlaceControlS(&PIDServoStruct,Mid_img,ImgControl.MidLine_value) * ServoStruct.speed_factor + ImuStruct.gyroZ * ServoStruct.gyro_GKD;
    // }
 
    //现在中线每偏10，返回的pid值隔76（之前那个是150），成线性关系
    //ServoStruct.PWM_Servo_update =  ServoStruct.PWM_Servo_pid / 38;
    ServoStruct.PWM_Servo_update =  ServoStruct.PWM_Servo_pid / 60.0;
    //舵机PWM输出量，这里实际含义是角度
    ServoStruct.PWM_Servo = MID_VALUE + ServoStruct.PWM_Servo_update;     
  

   // 限幅      
   Range_Protect_f(&ServoStruct.PWM_Servo, SERVO_MOTOR_R_MAX, SERVO_MOTOR_L_MAX);
  //  if(state_stayin == STRAIGHT) Range_Protect_f(&ServoStruct.PWM_Servo, 100, 92);
  if(servo_command == NONE){
    servo_set(ServoStruct.PWM_Servo);
    motor_set(2500,2500);
  } 
  else if(servo_command == LEFT_HIT ){
    servo_set(SERVO_MOTOR_R_MAX);
    motor_set(2600,2300);
  }
  else if(servo_command == RIGHT_HIT){
    servo_set(SERVO_MOTOR_L_MAX);
    motor_set(2300,2600);
  }   
  else if(servo_command == MID_HIT){
    servo_set(MID_VALUE);
    motor_set(2450,2450);
  }   
  else if(servo_command == LEFT_HIT_B){
    servo_set(SERVO_MOTOR_R_MAX);
    motor_set(-2500,-2200);
  }
  else if(servo_command == RIGHT_HIT_B){
    servo_set(SERVO_MOTOR_L_MAX);
    motor_set(-2200,-2500);
  }
  else if(servo_command == STOP){
    servo_set(MID_VALUE);
    motor_set(0,0);
  }
  else if(servo_command == MID_HIT_SLOW){
    servo_set(MID_VALUE);
    motor_set(2300,2300);
  }   
    else if(servo_command == LEFT_HIT_SL){
    servo_set(SERVO_MOTOR_R_MAX);
    motor_set(2500,2200);
  }
  else if(servo_command == RIGHT_HIT_SL){
    servo_set(SERVO_MOTOR_L_MAX);
    motor_set(2200,2500);
  }
  else if(servo_command == MID_HIT_SB){
    servo_set(SERVO_MOTOR_L_MAX);
    motor_set(-2300,-2300);
  }
  if(abs(Encoderstruct.encoder_left) > 100){
    motor_set(0,0);
  }
  if(ImgControl.MidLine_value == 1){
      servo_set(MID_VALUE);
      motor_set(1500,1500);
      system_delay_ms(500);
      extern void cleanup(void);
      cleanup();
      exit(0);
  }
 }


 /**
  * @brief  舵机初始化
  * @param  None
  * @note   创建一个定时器10ms周期，回调函数为pit_callback,每10ms采集一次数据
  * @retval None
  */
 void servo_pit_init(void)
 {
    Pid_PlaceStructInit(&PIDServoStruct);
    // pit_ms_init(9, pit_callback_servo_balance);
    // Pid_PlaceParmSet(&PIDServoStruct,7.4,0,55,10000);     // 2000
    Pid_PlaceParmSet(&PIDServoStruct,7.60,0,60,10000);      // 2200
  //  ServoStruct.gyro_GKD = 0.15;       // 2000
    //ServoStruct.gyro_GKD = 0.2; 

    //闭环
    // ServoStruct.speed_factor = map_link(Motor.expect_val_encoder,200,500,1,1.6);
    //开环
    ServoStruct.speed_factor = map_link(Motor.open_duty,2000,5000,1,2.2);
 }


 /**
  * @brief  UI界面舵机平衡控制参数
  * @param  None
  * @note   None
  * @retval None
  */
 void servo_control_param(float* servo_kp,float* servo_ki,float* servo_kd)
 {
    *servo_kp = PIDServoStruct.kp;
    *servo_ki = ServoStruct.gyro_GKD;
    *servo_kd = PIDServoStruct.kd;
 }

 /**
  * @brief  UI界面舵机平衡控制参数选择跳转
  * @param  mode 参数选择
  * @param  step 步进
  * @note   None
  * @retval None
  */
void servo_control_param_set(uint8 mode, float step){
  switch(mode){
      case 0: PIDServoStruct.kp += step; break;
      case 1: ServoStruct.gyro_GKD += step; break;
      case 2: PIDServoStruct.kd += step; break;
      default: break;
  }
}