#ifndef __DATAS_H__
#define __DATAS_H__
#include "zf_common_headfile.h"
#include "opencv.h"

//可调节参数
#define CIRCLE_ERROR_THRESHOLD  70  //判断圆环拟合误差的阈值
#define CIRCLE_K_THRESHOLD      800  //判断环岛的斜率比值阈值
#define STATE_JUDGE_THRESHOLD   30  //判断弯道直道的
#define STATE_TIME_DELAY        6   //延迟判定的帧数
#define THROWPOINT              3    //丢线点，设为3就行      for(;x >= THROWPOINT;)
#define LINE_SEARCH_THRESHOLD   5    //搜线偏差阈值           while (counter < LINE_SEARCH_THRESHOLD);
#define STARTY                  10   //开始搜线的y距离        for(#define y =IMG_HEIGHT - STARTY;;)
#define STOPY                   11   //保护距离
#define LEFT_THRESHOLD          5   //左边线丢线阈值
#define RIGHT_THRESHOLD         30   //右边线丢线阈值
#define CIRCLELONG_THRESHOLD    15   //环岛判定弧长阈值
extern int counter_of_dead;
extern struct imgdata img_data;
extern int pre_error_servo;
/*----------------------基础图像处理----------------------------------*/
void FindLongestLine(float leftlimit,float rightlimit);
void FindStartSearchPoint(int* leftline,int* rightline);

void SearchLeftLine_Craw(int* linein,int& );
void SearchRightLine_Craw(int* linein,int& );
void SearchLeftLine_LongestWhiteline(bool choose,int start,int end,int* linein);
void SearchRightLine_LongestWhiteline(bool choose,int start,int end,int* linein);

int Find_Zebra_crossing(void);
void State_Manager(void);
void Test_left_circle(float& rl,float& rr,float& circle_ERROR,int& right_throwline);
void Test_right_circle(float& rl,float& rr,float& circle_ERROR,int& left_throwline);

void DebugofImg(void);
void Test_number_of_throwline(int& , int&);
void drawline_list(int*,int,int,int,int);
/*----------------------基础数据处理----------------------------------*/
void least_squares(int* point , int starty , int minlength ,float* a, float* b,float* r) ;
double circleLeastFit(int* line, int N, int start_point, double& center_x, double& center_y, double& radius);
void stop(void);
#endif