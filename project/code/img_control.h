#ifndef _Img_control_h
#define _Img_control_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "opencv.h"
#include "ui.h"
#include "servo.h"

#define Mid_img 160  //图像中值

typedef struct img_control
{
    uint16 MidLine_value;              //中线坐标
    int cen_line[180];
    int right_line[180];
    int left_line[180];
    
}ImgControlStruct;

extern ImgControlStruct ImgControl;

void pit_callback_img_midline(void);
void img_midline_pit_init(void);

#endif