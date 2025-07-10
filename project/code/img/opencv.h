#ifndef __OPENCV_H__
#define __OPENCV_H__
#include "zf_common_headfile.h"

using namespace std;
using namespace cv;
/*--------------------------------------------------------------------------------------------------------------*/

extern Mat cvimg;
extern Mat show;
#define IMG         cvimg                 //摄像头图像的名称

//图像数据和信息
#define IMG_WIDTH  320			//160 // 320                    //图像的高度，由函数自动获取
#define IMG_HEIGHT 180			//120 // 180
#define IMG_FPS    120            
#define THE_UPPER_OF_IMG   0.35*IMG_HEIGHT

//颜色判断的定义
#define PURPLE   1                         //颜色选择的定义，用于GetColorImg()函数
#define BLUE     0
#define GREEN    2
#define YELLOW	 3
#define BLACK    4

//状态判断的定义
#define STRAIGHT 1
#define TURN	 2
#define LEFT_TURN_CIRCLE  12
#define RIGHT_TURN_CIRCLE 13
#define LEFT_TURN_CIRCLE2 14
#define ZEBRA_LINE        20

#define OUT_GARAGE         30
#define FINDING            31
#define IN_GARAGE          32
#define OUT_SIDE_GARAGE    33
#define IN_SIDE_GARAGE     35 
#define IN_SIDE_GARAGE_II  36
#define OUT_SIDE_GARAGE_II 37
#define FINAL              38
#define FINAL_FIND         39

#define GARAGE_OUT_DISTANCE    1
#define GARAGE_SIDE            150
#define INTEGRATE_GARAGE       160
#define INTEGRATE_GARAGE_SIDE  10

struct imgdata{
	int leftline[IMG_HEIGHT];          //左边线数组
	int rightline[IMG_HEIGHT];         //右边线数组
	int cenline[IMG_HEIGHT];           //中线数组
	int templine[IMG_HEIGHT];          //判别圆环时候的临时数组
	int minlength;                      //最长白列的长度
	int Longestline;                   //最长白列的列号
	double thre;
};
extern int state_stayin;
/*------------------------------------------------------------------------------------------------------------*/


void try_get_image(void);
int8 camera_init(const char*);
void GetColorImg(int color);                  //颜色选择，用于筛选出图片中的不同颜色
float LongestWhiteLine(void);      //并获取新边线和中线，返回一个二为维向量组，用于保存边线中线信息，0左边，1中线，2右边

#endif