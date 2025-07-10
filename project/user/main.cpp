#include "code_headfiles.h"

extern int fps_while;

#define IPS200      "/dev/fb0"                          //屏幕
#define UVC         "/dev/video0"                       //摄像头
float lf,lr =0.0f;
void high_frequency_tasks(){
    imu_update();                 //陀螺仪
    // motor_control();              //电机闭环
    // pit_callback_servo_balance();     //舵机闭环
}

void low_frequency_tasks(){
    key_scanner();                //按键扫描
    key_callback();               //按键轮询
    ADC_get();                    //电压值采集
}

void sigint_handler(int signum) {
    printf("收到Ctrl+C，程序即将退出\n");
    exit(0);
}

void cleanup(){
    printf("程序异常退出，执行清理操作\n");
    pwm_set_duty(MOTOR1_PWM, 0);   
    pwm_set_duty(MOTOR2_PWM, 0);   
    pwm_set_duty(SERVO_MOTOR1_PWM, 0);   
    pwm_set_duty(Suction_PWM_1, 0);   
    pwm_set_duty(Suction_PWM_2, 0);    
    gpio_set_level(BEEP, 0x0);
}
void counter_pit_callback(){
    mileage_left += Encoderstruct.encoder_left*0.01;
}

int main(int, char**) 
{
    // 注册清理函数
    atexit(cleanup);

    // 注册SIGINT信号的处理函数
    signal(SIGINT, sigint_handler);

 /********************************************************************************
 *                 定义变量部分
 ********************************************************************************/
  ips200_init(IPS200);

 /********************************************************************************
 
 *                  初始化部分
 ********************************************************************************/
    //陀螺仪初始化
    imu_init();
    integral_yaw_start();
    //开环电机初始化
    //  motor_open_control_Init();
    //  motor_set(Motor.open_duty,Motor.open_duty);
    if(1){
      motor_set(1000,1000);
    }
    else{
      motor_control_Init(); // 闭环电机初始化
    }


    //图像初始化
    camera_init(UVC);
    tcp_init(1);
    img_midline_pit_init();
    fps_pit_init();
    encoder_init();
    pit_ms_init(5,counter_pit_callback);

    //舵机初始化，是PID控制定时器
    servo_set(MID_VALUE);
    servo_pit_init();

    //里程计


 /*******************************************************************************/
    // 实例化定时器
    // 全局定时器管理器实例
    TimerManager timerManager;

/********************************************************************************
 *                 定时器任务部分
 ********************************************************************************/
    // timerManager.addTimer(std::chrono::milliseconds(1000), [](){
    //   std::cout << "   1s "  << std::endl;
    // });
    timerManager.addTimer(std::chrono::milliseconds(20), high_frequency_tasks);
    // timerManager.addTimer(std::chrono::milliseconds(50), low_frequency_tasks);


 /*******************************************************************************/ 
    while(1){ 
      //if(Encoderstruct.encoder_left > 100)  lf++;
      //if(Encoderstruct.encoder_left < 100)  lf--;
      //if(Encoderstruct.encoder_right > 100) lr++;
      //if(Encoderstruct.encoder_right < 100) lr--;
      printf("state_stayin:%d\n",state_stayin);
      printf("mileage_left:%f\n",mileage_left);
      //printf("start_yaws:%f\n",start_yaws);
      printf("yaw = %f\r\n",ImuStruct.yaw);
      //printf("img_data.thre = %f\r\n",img_data.thre);
      //printf("img_err: %d\r\n",ImgControl.MidLine_value);
      //motor_control();
      printf("Encoderstruct.encoder_left = %d.\r\n",  Encoderstruct.encoder_left);
      //printf("Encoderstruct.encoder_right = %d.\r\n",  Encoderstruct.encoder_right);
      system_delay_ms(200);
      ui_show_main();
      fps_while ++;
     
     /*************************** oscilloscope uart *********************************************/

  // 写入需要发送的数据，有几个通道就写多少个数据
        // 这里仅写入4个通道数据
        // seekfree_assistant_oscilloscope_data.data[0] = ImuStruct.yaw_end_flag;
        // seekfree_assistant_oscilloscope_data.data[1] = ImuStruct.integral_yaw_flag;
        // seekfree_assistant_oscilloscope_data.data[2] = ImuStruct.yaw_start;
        // seekfree_assistant_oscilloscope_data.data[3] = ImuStruct.yaw;
        // seekfree_assistant_oscilloscope_data.data[4] = ServoStruct.PWM_Servo_pid;
        // seekfree_assistant_oscilloscope_data.data[5] = ServoStruct.PWM_Servo_update;

        // 设置本次需要发送几个通道的数据
        // seekfree_assistant_oscilloscope_data.channel_num = 4;
        
        // // // 这里进发送了4个通道的数据，最大支持8通道
        // seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
        
      //  system_delay_ms(100);
    //   vofa_send_justfloat_8ch(Motor.expect_val_encoder,
    //     Encoderstruct.encoder_left,
    //     - Encoderstruct.encoder_right,
    //     Motor.duty_encoder_L,
    //     Motor.duty_encoder_R,
    //     Motor.encoder_EMA_L,
    //     - Motor.encoder_EMA_R,
    //     0
    //   );
      // vofa_send_justfloat_3ch(Mid_img ,ImgControl.MidLine_value ,ServoStruct.PWM_Servo_pid);

    /*************************** oscilloscope uart *********************************************/

   }
}