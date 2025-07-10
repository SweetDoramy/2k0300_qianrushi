#include"motor.h"


// 在设备树中，设置的10000。如果要修改，需要与设备树对应。
#define MOTOR1_PWM_DUTY_MAX    (motor_1_pwm_info.duty_max)       
// 在设备树中，设置的10000。如果要修改，需要与设备树对应。 
#define MOTOR2_PWM_DUTY_MAX    (motor_2_pwm_info.duty_max)    


struct pwm_info motor_1_pwm_info;
struct pwm_info motor_2_pwm_info;     

//电机PID参数设计
PidIncreaseTypeDef MotorStruct_left;
PidIncreaseTypeDef MotorStruct_right;

// 滤波对照组
// PidIncreaseTypeDef MotorStruct_left_raw;
// PidIncreaseTypeDef MotorStruct_right_raw;

//速度等变量存储
MotorStructTypeDef Motor;

//编码器数值滤波
EMA_filter_Struct EMA_encoder_L;
EMA_filter_Struct EMA_encoder_R;

/**
  * @brief  电机设置
  * @param  None
  * @note   开环,目前左右轮反了，前右轮，后左轮
  * @retval None
  */
void motor_set(float duty_L,float duty_R)
{
    duty_L = -duty_L;
    duty_R = -duty_R;
    // 获取PWM设备信息
    pwm_get_dev_info(MOTOR1_PWM, &motor_1_pwm_info);
    pwm_get_dev_info(MOTOR2_PWM, &motor_2_pwm_info);

    if(duty_L > 0 && duty_R > 0)               // 正转
    {
        gpio_set_level(MOTOR1_DIR, 0);                                     
        pwm_set_duty(MOTOR1_PWM, duty_L);       // 计算占空比

        gpio_set_level(MOTOR2_DIR, 1);                                      
        pwm_set_duty(MOTOR2_PWM, duty_R);       // 计算占空比
    }
    else if(duty_L > 0 && duty_R <= 0) 
    {
        gpio_set_level(MOTOR1_DIR, 0);                                     
        pwm_set_duty(MOTOR1_PWM, duty_L);      // 计算占空比

        gpio_set_level(MOTOR2_DIR, 0);                                    
        pwm_set_duty(MOTOR2_PWM, -duty_R);      // 计算占空比
    }
    else if(duty_L <= 0 && duty_R > 0) 
    {
        gpio_set_level(MOTOR1_DIR, 1);                                    
        pwm_set_duty(MOTOR1_PWM, -duty_L);      // 计算占空比

        gpio_set_level(MOTOR2_DIR, 1);                                    
        pwm_set_duty(MOTOR2_PWM, duty_R);      // 计算占空比
    }
    else if(duty_L <= 0 && duty_R <= 0) 
    {
        gpio_set_level(MOTOR1_DIR, 1);                                    
        pwm_set_duty(MOTOR1_PWM, -duty_L);      // 计算占空比

        gpio_set_level(MOTOR2_DIR, 0);                                     
        pwm_set_duty(MOTOR2_PWM, -duty_R);      // 计算占空比
    }
}

/**
  * @brief  电机开环初始化
  * @param  
  * @note   设置左右期望开环速度
  *         我们自己在这里设置期望编码器的值，方便用按键调整
  * @retval 
  */
 void motor_open_control_Init(void)
 {
    Motor.open_duty = 2000;
 }


/**
  * @brief  电机闭环初始化
  * @param  
  * @note   pid编码器闭环，这里使用增量式    
  *         我们自己在这里设置期望编码器的值，方便用按键调整
  * @retval 
  */
 void motor_control_Init(void)
 {
    //参数初始化
    Pid_IncreaseStructInit(&MotorStruct_left);
    Pid_IncreaseStructInit(&MotorStruct_right);

    /* 滤波对照组 */
    // Pid_IncreaseStructInit(&MotorStruct_left_raw);
    // Pid_IncreaseStructInit(&MotorStruct_right_raw);
    // Pid_IncreaseParmSet(&MotorStruct_left_raw,6.5,0.27,0);

    Motor.duty_encoder_L = 0;
    Motor.duty_encoder_R = 0;

    //赋值
    Motor.expect_val_encoder = close_speed;
    
    // Pid_IncreaseParmSet(&MotorStruct_left,7.25,0.28,0);

    // Pid_IncreaseParmSet(&MotorStruct_right,7.95,0.35,0);
    Pid_IncreaseParmSet(&MotorStruct_left,7.25,0.2,0);

    Pid_IncreaseParmSet(&MotorStruct_right,7.95,0.3,0);

    //滤波器
    Motor.EMA_alpha = 0.386;
    EMA_init(&EMA_encoder_L, Motor.EMA_alpha);
    EMA_init(&EMA_encoder_R, Motor.EMA_alpha);

    //定时器
    // pit_ms_init(1, motor_control);
 }


/**
  * @brief  电机闭环
  * @param  
  * @note   PID电机闭环，这里使用增量式，编码器反馈
  *         放入定时器中断里面
  * @retval 
  */
void motor_control(void)
 {

      if(0){
        Motor.encoder_EMA_L = EMA_update(&EMA_encoder_L, Encoderstruct.encoder_left);
        Motor.encoder_EMA_R = EMA_update(&EMA_encoder_R, Encoderstruct.encoder_right);
      }
      else{
        Motor.encoder_EMA_L = (float)Encoderstruct.encoder_left;
        Motor.encoder_EMA_R = (float)Encoderstruct.encoder_right;
      }


      Motor.expect_val_encoder = close_speed;
      Motor.duty_encoder_L += Pid_IncreaseControl(&MotorStruct_left ,  Motor.expect_val_encoder,  -Motor.encoder_EMA_L);
      Motor.duty_encoder_R += Pid_IncreaseControl(&MotorStruct_right,  Motor.expect_val_encoder,   Motor.encoder_EMA_R);
    
      Range_Protect_f(&Motor.duty_encoder_L, 4000, -4000);
      Range_Protect_f(&Motor.duty_encoder_R, 4000, -4000);

      /* 滤波对照组 */
      // Motor.duty_raw_pid_L += Pid_IncreaseControl(&MotorStruct_left_raw,Motor.expect_val_encoder,Encoderstruct.encoder_left);
      // Motor.duty_raw_pid_R += Pid_IncreaseControl(&MotorStruct_right_raw,Motor.expect_val_encoder, -Encoderstruct.encoder_right);
      // Range_Protect_f(&Motor.duty_raw_pid_L, 9000, -9000);
      // Range_Protect_f(&Motor.duty_raw_pid_R, 9000, -9000);

      //目前左右轮反了，第一个参数是右轮，第二个参数是左轮，但是编码器是正的
      // motor_set(0, Motor.duty_encoder_L);      //duty是占空比
      // motor_set(Motor.duty_encoder_R, 0); 
      motor_set(Motor.duty_encoder_R, Motor.duty_encoder_L); 
      // printf("motor. \r\n");
 }



  /**
  * @brief  UI界面电机平衡控制参数
  * @param  None
  * @note   None
  * @retval None
  */
 void motor_control_param(float* motor_kp_L,float* motor_ki_L,float* motor_kd_L,float* motor_kp_R,float* motor_ki_R,float* motor_kd_R)
 {
  //左轮
    *motor_kp_L = MotorStruct_left.kp;
    *motor_ki_L = MotorStruct_left.ki;
    *motor_kd_L = MotorStruct_left.kd;
  //右轮
    *motor_kp_R = MotorStruct_right.kp;
    *motor_ki_R = MotorStruct_right.ki;
  //  *motor_kd_R = MotorStruct_right.kd;
    *motor_kd_R = Motor.EMA_alpha;
 }


 /**
  * @brief  UI界面电机平衡控制参数选择跳转
  * @param  mode 参数选择
  * @param  step 步进
  * @note   None
  * @retval None
  */
 void motor_control_param_set(uint8 mode, float step){
  switch(mode){
      case 3: MotorStruct_left.kp += step; break;
      case 4: MotorStruct_left.ki += step; break;
      case 5: MotorStruct_left.kd += step; break;
      case 6: MotorStruct_right.kp += step; break;
      case 7: MotorStruct_right.ki += step; break;
    //  case 8: MotorStruct_right.kd += step; break;
      case 8: Motor.EMA_alpha += step; break;
      default: break;
  }
}



 /**
  * @brief  UI界面速度参数
  * @param  None
  * @note   None
  */
 void speed_param(float* closed_speed,float* open_speed)
 {
    *closed_speed = Motor.expect_val_encoder;
    *open_speed = Motor.open_duty;
 }

 /**
  * @brief  UI界面舵机平衡控制参数选择跳转
  * @param  mode 参数选择
  * @param  step 步进
  * @note   None
  * @retval None
  */
void speed_param_set(uint8 mode, float step){
  switch(mode){
      case 9: Motor.expect_val_encoder += step; break;
      case 10: Motor.open_duty += step; break;
      default: break;
  }
}


 /**
  * @brief  目标值跳变测试闭环调参
  * @param  
  * @note   None
  * @retval None
  */
void test_close(void)
{
  Motor.expect_val_encoder = 300;
  system_delay_ms(3000);
  Motor.expect_val_encoder = 550;
  system_delay_ms(3000);
}