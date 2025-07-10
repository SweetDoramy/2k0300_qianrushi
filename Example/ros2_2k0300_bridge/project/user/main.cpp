


#include "zf_common_headfile.h"

#include "class_thread_wrapper.h"
#include "class_posix_pit.h"

#include "get_encoder.h"
#include "get_radar.h"
#include "get_imu.h"
#include "tcp_send_ladar.h"
#include "tcp_send_sensor.h"
#include "tcp_recv_control.h"

#include "get_config.h"
#include "user_pid.h"
#include "motor_control.h"

TimerFdWrapper *timer;

// 定义线程回调函数，用于处理定时器到期事件
void timer_callback(void) 
{
    imu_callback();

    encoder_callback();

    pid_inc_cal(&left_pid,  g_car_control.left_speed, g_encoder_left);
    pid_inc_cal(&right_pid, g_car_control.right_speed, g_encoder_right);

    pwm_control(left_pid.out_value, right_pid.out_value);

    servo_control(g_car_control.servo_duty);
}

void sigint_handler(int signum) 
{
    printf("收到Ctrl+C，程序即将退出\n");
    exit(0);
}


void cleanup()
{
    printf("程序异常退出，执行清理操作\n");
    // 处理程序退出！！！
    // 这里需要关闭电机，关闭电调等。
    timer->stop();
    system_delay_ms(10);

    pwm_set_duty(MOTOR1_PWM, 0);  
    pwm_set_duty(MOTOR2_PWM, 0);  
    pwm_set_duty(SERVO_MOTOR1_PWM, 0);
    close_tcp_send_ladar();
    close_tcp_send_sensor();
    close_tcp_recv_control();
}




int main(int, char**) 
{

    // 注册清理函数
    atexit(cleanup);

    // 注册SIGINT信号的处理函数
    signal(SIGINT, sigint_handler);

    // 创建周期定时器 周期10ms
    timer = new TimerFdWrapper(10, timer_callback);
    timer->start();

    // 创建并运行线程
    // 创建新线程

    // 获取六轴数据和编码器数据，10ms发送一次。
    ThreadWrapper thd_0(tcp_send_sensor_thd_entry);
    // 串口接收雷达数据，通过TCP发送到ubuntu中。
    ThreadWrapper thd_1(tcp_send_ladar_thd_entry);
    // TCP接收ROS2的数据，控制车模运动和转向
    ThreadWrapper thd_2(tcp_recv_control_thd_entry);

    while(1)
    {
        printf("left = %d \n\r",g_encoder_left);
        printf("right = %d \n\r",g_encoder_right);
        system_delay_ms(1000);
    // }
    // {
    //     encoder_thd_entry();
    //     imu_thd_entry();
    }


}