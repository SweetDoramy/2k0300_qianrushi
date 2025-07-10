#include"ui.h"
extern int state_stayin;

typedef void (*ui_show)(void);

ui_show ui_array[UI_PAGE_SIZE] = {
    ui_show_page1,
    ui_show_page2,
    ui_show_page3
};

static uint8 param_set_flag = 0;          // 调参标志位   0-页面滑动;1-调参增减
static uint8 ui_array_idx = 0;            // ui界面页码
uint8 ui_clear_flag = 0;                  // 清屏标志位
static float step[4] = {0.01, 0.1, 1, 10};         // 调参步进
static uint8 step_idx = 0;                         // 调参步进索引            
static uint16 ui_page2_param[12] = {7,8,9,10,11,12,13,14,15,16,17,18};   //page2调参界面坐标索引，就是从刨除掉图像的部分开始   
static uint8 ui_page2_param_idx = 0;        //对应0~11             

#define Page2_index 11                //page2的需要调参的步进索引

/**
  * @brief  当前页面调参标志
  * @param  None
  * @note   param_set_flag是调参标志位   0-页面滑动;1-调参增减
  * @retval None
  */
 void ui_param_set_toggle(uint8 dir)
 {
    if(dir)
        param_set_flag = 1;
    else 
        param_set_flag = 0;
}



/**
  * @brief  信息界面1(只显示,不可更改)
  * @param  None
  * @note   x方向一个字符占8位，y方向一个字符占16位
  * @retval None
  */
 void ui_show_page1(void)
 {
    ips200_show_string(0 , Y_ips_shift * 8, "PWM_Servo_update:");
    ips200_show_float(X_ips_shift * 17 ,Y_ips_shift * 8 , ServoStruct.PWM_Servo_update ,3 ,3);

    ips200_show_string(0 , Y_ips_shift * 9, "PWM_Servo_pid:");
    ips200_show_float(X_ips_shift * 14 ,Y_ips_shift * 9 , ServoStruct.PWM_Servo_pid ,3 ,3);

    // ips200_show_string(0 , Y_ips_shift * 8, "encoder_left:");
    // ips200_show_float(X_ips_shift * 13 ,Y_ips_shift * 8 , Encoderstruct.encoder_left ,5 ,0);

    // ips200_show_string(0 , Y_ips_shift * 9, "encoder_right:");
    // ips200_show_float(X_ips_shift * 14 ,Y_ips_shift * 9 , Encoderstruct.encoder_right ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 10, "MidLine_value:");
    ips200_show_float(X_ips_shift * 14 ,Y_ips_shift * 10 , ImgControl.MidLine_value ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 11, "PWM_Servo:");
    ips200_show_float(X_ips_shift * 10 ,Y_ips_shift * 11 , ServoStruct.PWM_Servo ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 12, "battery_vol:");
    ips200_show_float(X_ips_shift * 12 ,Y_ips_shift * 12 , ADCStruct.battery_vol ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 13, "right_r:");
    ips200_show_float(X_ips_shift * 8 ,Y_ips_shift * 13 , right_r ,6 ,3);

    ips200_show_string(0 , Y_ips_shift * 14, "left_r:");
    ips200_show_float(X_ips_shift * 7 ,Y_ips_shift * 14 , left_r ,6 ,3);

    ips200_show_string(0 , Y_ips_shift * 15, "fps:");
    ips200_show_float(X_ips_shift * 4 ,Y_ips_shift * 15 , (float)fps_img ,6 ,3);

    ips200_show_string(0 , Y_ips_shift * 16, "circle_err:");
    ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * 16 , circle_err ,6 ,3);
    
    ips200_show_string(0 , Y_ips_shift * 17, "f_control:");
    ips200_show_float(X_ips_shift * 10 ,Y_ips_shift * 17 , (float)frequence_control ,6 ,3);

    ips200_show_string(0 , Y_ips_shift * 18, "element:");
    ips200_show_float(X_ips_shift * 8 ,Y_ips_shift * 18 , (float)state_stayin ,6 ,3);

    // ips200_show_string(0 , Y_ips_shift * 13, "Motor.duty_R:");
    // ips200_show_float(X_ips_shift * 13 ,Y_ips_shift * 13 , Motor.duty_R ,5 ,0);


    //检查措施
    //printf("This is page 1\n");
 }



/**
  * @brief  调参界面(平衡控制相关)
  * @param  None
  * @note   x方向一个字符占8位，y方向一个字符占16位,屏幕只有16*19行，从0开始就是最多20行
  * @retval None
  */
 void ui_show_page2(void)
 {
    //舵机调参部分
    float servo_kp = 0,servo_ki = 0,servo_kd = 0;
    servo_control_param(&servo_kp,&servo_ki,&servo_kd);

    ips200_show_string(0 , Y_ips_shift * ui_page2_param[1], "servo_kp:");
    ips200_show_float(X_ips_shift * 9 ,Y_ips_shift * ui_page2_param[1] , servo_kp ,3,2);
    ips200_show_string(0 , Y_ips_shift * ui_page2_param[2], "gyro_GKD:");
    ips200_show_float(X_ips_shift * 9 ,Y_ips_shift * ui_page2_param[2] , servo_ki ,2,3);
    ips200_show_string(0 , Y_ips_shift * ui_page2_param[3], "servo_kd:");
    ips200_show_float(X_ips_shift * 9 ,Y_ips_shift * ui_page2_param[3] , servo_kd ,3,2);

    //电机调参部分
    float motor_kp_L=0, motor_ki_L=0, motor_kd_L=0, motor_kp_R=0, motor_ki_R=0, motor_kd_R=0;
    motor_control_param(&motor_kp_L,&motor_ki_L,&motor_kd_L,&motor_kp_R,&motor_ki_R,&motor_kd_R);

    ips200_show_string(0 , Y_ips_shift * ui_page2_param[4], "motor_kp_L:");
    ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * ui_page2_param[4] , motor_kp_L ,3,2);
    ips200_show_string(0 , Y_ips_shift * ui_page2_param[5], "motor_ki_L:");
    ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * ui_page2_param[5] , motor_ki_L ,3,2);
    ips200_show_string(0 , Y_ips_shift * ui_page2_param[6], "motor_kd_L:");
    ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * ui_page2_param[6] , motor_kd_L ,3,2);

    ips200_show_string(0 , Y_ips_shift * ui_page2_param[7], "motor_kp_R:");
    ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * ui_page2_param[7] , motor_kp_R ,3,2);
    ips200_show_string(0 , Y_ips_shift * ui_page2_param[8], "motor_ki_R:");
    ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * ui_page2_param[8] , motor_ki_R ,3,2);
    // ips200_show_string(0 , Y_ips_shift * ui_page2_param[8], "motor_kd_R:");
    // ips200_show_float(X_ips_shift * 11 ,Y_ips_shift * ui_page2_param[8] , motor_kd_R ,3,2);

    // ips200_show_string(0 , Y_ips_shift * ui_page2_param[8], "EMA_alpha:");
    // ips200_show_float(X_ips_shift * 10 ,Y_ips_shift * ui_page2_param[8] , motor_kd_R ,2,3);

    //速度
    float closed_speed = 0,open_speed = 0;
    speed_param(&closed_speed,&open_speed);
    ips200_show_string(0 , Y_ips_shift * ui_page2_param[9], "Motor.closed.speed:");
    ips200_show_float(X_ips_shift * 19 ,Y_ips_shift * ui_page2_param[9] , closed_speed ,3,2);

    ips200_show_string(0 , Y_ips_shift * ui_page2_param[10], "Motor.open.speed:");
    ips200_show_float(X_ips_shift * 17 ,Y_ips_shift * ui_page2_param[10] , open_speed ,5,1);

    //调整区域
    ips200_show_string(0,Y_ips_shift * 18,"step:");
    ips200_show_float(X_ips_shift * 5,Y_ips_shift * 18,step[step_idx],3,3);

    ips200_show_string(X_ips_shift * 28 , Y_ips_shift * ui_page2_param[ui_page2_param_idx],"<-");

    //检查措施
    //printf("This is page 2\n");
 }


 /**
  * @brief  信息界面3(只显示,不可更改)
  * @param  imu
  * @note   x方向一个字符占8位，y方向一个字符占16位
  * @retval None
  */
 void ui_show_page3(void)
 {
    ips200_show_string(0 , Y_ips_shift * 8, "yaw_start:");
    ips200_show_float(X_ips_shift * 10 ,Y_ips_shift * 8 , ImuStruct.yaw_start ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 9, "yaw_end_flag:");
    ips200_show_float(X_ips_shift * 13 ,Y_ips_shift * 9 , ImuStruct.yaw_end_flag ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 10, "yaw_T:");
    ips200_show_float(X_ips_shift * 6 ,Y_ips_shift * 10 , ImuStruct.yaw_T ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 11, "integral_yaw_flag:");
    ips200_show_float(X_ips_shift * 18 ,Y_ips_shift * 11 , ImuStruct.integral_yaw_flag ,5 ,0);

    ips200_show_string(0 , Y_ips_shift * 12, "ImuStruct.gyroZ:");
    ips200_show_float(X_ips_shift * 16 ,Y_ips_shift * 12 , ImuStruct.gyroZ , 3 ,3);

    ips200_show_string(0 , Y_ips_shift * 13, "yaw:");
    ips200_show_float(X_ips_shift * 4 ,Y_ips_shift * 13 , ImuStruct.yaw ,5 ,0);
 }



/**
  * @brief  UI显示
  * @param  None
  * @note   主要是各个标志位的执行，基本界面信息的构建
  *         报错Segmentation Fault的可能原因：
  *     1.注意信息界面需要放在这个主函数前面，否则会识别为未初始化或定义；
  *     2.图像或者屏幕打印显示的某个东西越界了，比如横纵坐标超出屏幕范围,屏幕只有16*19行，从0开始就是最多20行;8*29列，就是最多30列
  * @retval None
  */
 void ui_show_main(void)
 {
     if(ui_clear_flag)          //执行清屏操作，标志位起作用
     {
         ui_clear_flag = 0;
         ips200_clear();
     }


     //函数指针的调用
     if (ui_array_idx < UI_PAGE_SIZE && ui_array[ui_array_idx] != NULL)
     {
        (*ui_array[ui_array_idx])();  

     }
     else {
        printf("Index out of bounds or NULL pointer\n");
    }

    //显示图形页面
    ips200_show_string(0,Y_ips_shift * 19,"<-");
    ips200_show_string(X_ips_shift * 5 ,Y_ips_shift * 19,"page:");
    ips200_show_uint(X_ips_shift * 10 ,Y_ips_shift * 19, ui_array_idx + 1,2);
    ips200_show_string(X_ips_shift * 28 ,Y_ips_shift * 19,"->");


    if(param_set_flag)             //显示调参标志位状态
    {
    ips200_show_string(X_ips_shift * 18 ,Y_ips_shift * 19,"param:");
    ips200_show_uint(X_ips_shift * 24 ,Y_ips_shift * 19,1,2);
    }
    else{
    ips200_show_string(X_ips_shift * 18 ,Y_ips_shift * 19,"param:");
    ips200_show_uint(X_ips_shift * 24 ,Y_ips_shift * 19,0,2);
    }


   // 显示图像到屏幕上
   ips200_show(show);                  //X:160=8*20;Y:90=16*5.625,取16*6,其余的从7开始

}


 /**
  * @brief  页面滑动
  * @param  dir 0-左滑, 1-右滑
  * @note
  * @retval None
  */
void ui_shift(uint8 dir)
{
    if(!param_set_flag){
        if(dir == 0){
            if(ui_array_idx == 0){
                ui_array_idx = (uint8_t)(UI_PAGE_SIZE - 1);
            }
            else{
                ui_array_idx --;
            }
        }
        else{
            if(ui_array_idx == (UI_PAGE_SIZE - 1)){
                ui_array_idx = 0;
            }
            else{
                ui_array_idx ++;
            }
        }

        ui_clear_flag = 1;
    }
}


/**
  * @brief  调参指针
  * @param  dir 1- 上滑 0-下滑
  * @note   None
  * @retval None
  */
 void ui_param_set_dir(uint8 dir)
 {
    if(param_set_flag && ui_array_idx == 1)       //Page2
    {
        if(dir){
            if(ui_page2_param_idx  != 0){
                ui_page2_param_idx --;
            }
            else{
                ui_page2_param_idx = Page2_index;
            }
        }
        else{
            if(ui_page2_param_idx != Page2_index){
                ui_page2_param_idx ++;
            }
            else{
                ui_page2_param_idx = 0;
            }
        }

        ui_clear_flag = 1;
    }
 }



/**
  * @brief  调参左减右加
  * @param  dir 1- 右加 0-左减
  * @note   None
  * @retval None
  */
 void ui_param_set(uint8 dir)
 {
    if(param_set_flag && ui_array_idx == 1)
    {
        if(dir){
            if(ui_page2_param_idx != Page2_index){
                servo_control_param_set(ui_page2_param_idx, step[step_idx]);
                motor_control_param_set(ui_page2_param_idx, step[step_idx]);
                speed_param_set(ui_page2_param_idx, step[step_idx]);
            }
            else{
                if(step_idx != 3){
                    step_idx ++;
                }
                else step_idx = 0;
            }
        }
        else{
            if(ui_page2_param_idx != Page2_index){
                servo_control_param_set(ui_page2_param_idx, -step[step_idx]);
                motor_control_param_set(ui_page2_param_idx, -step[step_idx]);
                speed_param_set(ui_page2_param_idx, -step[step_idx]);
            }
            else{
                if(step_idx != 0){
                    step_idx --;
                }
                else step_idx = 3;
            }
        }
        ui_clear_flag = 1;
    }
 }


 /**
  * @brief  需要清屏
  * @param  None
  * @note   None
  * @retval None
  */
void ui_clear(void){
    ui_clear_flag = 1;
}
