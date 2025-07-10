#ifndef __IMG_TCP_H__
#define __IMG_TCP_H__
#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "opencv.h"
/*---------------------------------type-def--------------------------------------------*/

#define SERVER_IP "192.168.43.57"                       //服务器端地址
#define PORT 8086                                        //服务器端开放端口
#define BOUNDARY_NUM            (IMG_HEIGHT * 4 / 2)     // 边界的点数量远大于图像高度，便于保存回弯的情况

extern int fps_counter;
extern int fps_img;
extern int frequence_control;
extern int frequence_control_counter;
/*---------------------------------fonction-declare------------------------------------*/
void tcp_init(bool); 
void send_img_to_tcp(cv::Mat );
void ips200_show(cv::Mat );
void fps_pit_init(void);
void fps_callback(void);
#endif