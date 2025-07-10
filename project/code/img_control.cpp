#include"img_control.h"

ImgControlStruct ImgControl;

/**
  * @brief  图像返回中线
  * @param  None
  * @note   放在定时器中断中
  * @retval None
  */
void pit_callback_img_midline(void)
 {
    try_get_image();
    GetColorImg(BLACK);
    ImgControl.MidLine_value = LongestWhiteLine();
    pit_callback_servo_balance();     //舵机闭环
    send_img_to_tcp(IMG);
    fps_counter ++;
   //  printf("img = %d.\r\n",  fps_counter);
 }


 /**
  * @brief  图像处理初始化
  * @param  None
  * @note   创建一个定时器10ms周期，回调函数为pit_callback,每10ms采集一次数据
  * @retval None
  */
 void img_midline_pit_init(void)
 {
    pit_ms_init(0, pit_callback_img_midline);

 }