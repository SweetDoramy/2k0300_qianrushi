#ifndef _ui_h
#define _ui_h

#include "code_headfiles.h"

#define UI_PAGE_SIZE    3
#define X_ips_shift     8              //x方向一个字符占8位
#define Y_ips_shift     16             //y方向一个字符占16位


//页面信息函数
void ui_show_page1(void);
void ui_show_page2(void);
void ui_show_page3(void);

//调参函数
void ui_param_set_toggle(uint8 dir);
void ui_show_main(void);
void ui_shift(uint8 dir);                 //页面滑动
void ui_param_set_dir(uint8 dir);         //上下滑动
void ui_param_set(uint8 dir);             //左右调参
void ui_clear(void);

extern float right_r,left_r,mid_r,circle_err;

#endif