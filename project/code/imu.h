#ifndef _imu_h
#define _imu_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"


#define DIV_180_PI       (float)(57.295779513f)      //角度制与弧度制的转换
#define GRAVITY          (float)(9.8f)

#define IMU_MODE         1 // 0-EKF or 1-Mahony

typedef struct 
{
    float pitch;                        // 俯仰角
    float roll;                         // 翻滚角
    float yaw;                          // 偏航角

    float gyroX;                        // X轴角速度
    float gyroY;                        // Y轴角速度
    float gyroZ;                        // Z轴角速度
    float accX;
    float accY;
    float accZ;
    float magX;
    float magY;
    float magZ;

    float yaw_T;                       // 陀螺仪积分阈值
    float yaw_start;                   // 陀螺仪积分初始值

    uint8 integral_yaw_flag;           //陀螺仪偏航角积分标志位    1-开始积分;0-不积分
    uint8 integral_pitch_flag;         //陀螺仪俯仰角积分标志位
    uint8 integral_roll_flag;          //陀螺仪翻滚角积分标志位

    uint8 yaw_end_flag;                //陀螺仪偏航角积分工作完成标志位,用于图像处理圆环
}ImuStructTypeDef;

typedef struct{
    float twoKp;                        // 2倍Kp
    float twoKi;                        // 2倍Ki

    float q[4];                         // 机体四元数

    float integralFBx;                  // x积分项
    float integralFBy;                  // y积分项
    float integralFBz;                  // z积分项

    float sampleFreq;                   // 采样频率

    void (*filter)(float,float,float,   // Mahony更新姿态函数指针
                   float,float,float);
}MahonyStructTypeDef;

typedef struct {
    float period;                       // 姿态更新周期
    float half_T;                       // 姿态更新周期的1/2      单位：s

    float I_matrix[16];                 // 单位矩阵              4 * 4
    float Q_matrix[16];                 // 过程噪声协方差矩阵      4 * 4
    float R_matrix[9];                  // 观测噪声协方差矩阵      3 * 3
    float A[16];                        // 状态转移矩阵           4 * 4
    float H[12];                        // 观测矩阵              3 * 4
    float p_hat[16];                    // 误差协方差矩阵         4 * 4
    float K[12];                        // 卡尔曼增益矩阵         4 * 3
    float Kq[4];                        // 四元数卡尔曼增益        4 * 1
    float T[3];                         // 残差向量               3 * 1
    float q[4];                         // 机体四元数             4 * 1
    float q_k[4];                       // 优化后的四元数          4 * 1

    float vector_hat[3];                // 观测重力加速度          3 * 1

    void (*filter)(float,float,float,   // EKF更新姿态函数指针
                   float,float,float);
}EKFStructTypeDef;

extern ImuStructTypeDef ImuStruct;
//extern uint8 counter_yaw;
void integral_yaw_start(void);
void integral_yaw_run(void);
void integral_yaw_stop(void);

void imu_update(void);
void imu_init(void);


#endif