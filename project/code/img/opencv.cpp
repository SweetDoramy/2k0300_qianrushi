#include"opencv.h"
struct imgdata img_data;
Mat cvimg;                               //初始化图像矩阵
Mat show(Size(IMG_WIDTH,IMG_HEIGHT),CV_8UC1,Scalar(0,0,0));
float right_r = 0,left_r = 0,mid_r = 0,circle_err = 0;

int state_stayin = STRAIGHT;      //状态标记位
int state_counter1 = 0;           //状态计数器1
int state_counter2 = 0;           //状态计数器2

float circle_ERROR = 0;          //拟合的误差值
int imgprocess_flag = 0;          //图像处理标志
float error_midline_flag = 0.7;   //图像前瞻范围
int counter_of_dead = 0;          //保护计数器

int counters = 0; 


// /*------------------------------------WIFI 图传边线 不允许改动!--------------------------------------*/
// extern uint16 leftLine[IMG_HEIGHT],rightLine[IMG_HEIGHT],midLine[IMG_HEIGHT];
// /*------------------------------------WIFI 图传边线 不允许改动!--------------------------------------*/

/*------------------------------------img_data.Longestline-test--------------------------------------*/
// 函数简介     寻找最长白列并返回中线
// 参数说明		传入的中线数组 img_data.Longestline[3][IMG_HEIGHT]
// 参数说明     布尔值 是否开启中线显示debug
// 返回参数     void
// 使用示例     LongestWhiteLineDraw(left_line.cen_line,right_line,true);
// 备注信息     需要在UseCanny函数后使用
/*------------------------------------img_data.Longestline-test--------------------------------------*/
float LongestWhiteLine(void) {
	if (Wait_second() == 1) return pre_error_servo;
	float error = 0;                  //计算中线误差,
	int left_throwline = 0,right_throwline = 0;          //丢线长度
	float lk = 0 , rk = 0 , ks = 0, bs = 0, rl = 0, rr = 0, rc = 0;    //最小二乘法拟合斜率截距
	FindLongestLine(0.1,0.1);     //寻找最长白列
	FindStartSearchPoint(img_data.leftline,img_data.rightline);       //寻找起始点
	//嵌入式小车初始化
	Ros_car_init();
	//侧边库识别
	if (state_stayin == STRAIGHT && !flag_finish){
		//SearchLeftLine_LongestWhiteline(0,IMG_HEIGHT-STARTY,img_data.minlength,img_data.templine);
		//least_squares(img_data.templine, STARTY +20, img_data.minlength - 20, &ks, &bs, &rl);
		//printf("\n rl： %f \n\n",rl);
		//if (rl < 100){
			SearchRightLine_LongestWhiteline(0,110,65,img_data.templine);
			for(int y = 110; y > 65;y--){
				if(img_data.templine[y+1]-img_data.templine[y] > 15){
					beep_run();
					state_stayin = FINDING;
					servo_command = MID_HIT_SLOW;
		 			mileage_left = 0;
		 			break;
		 		}
			}
		//}
	}
	else if(state_stayin == FINAL_FIND){
		Final_find();
	}
	else if(state_stayin == IN_GARAGE){
		In_garage();
	}
	else if(state_stayin == FINAL){
		Final_stop();
	}
	else if(state_stayin == OUT_SIDE_GARAGE_II){
		Out_side_garage_ii();
		return 0;
	}
	else if(state_stayin == OUT_SIDE_GARAGE){
		Out_side_garage();
		return 0;
	}
	else if(state_stayin == IN_SIDE_GARAGE_II){
		In_side_garage_ii();
		return 0;
	}
	else if(state_stayin == IN_SIDE_GARAGE){
		In_side_garage();
		return 0;
	}
	else if(state_stayin == OUT_GARAGE){
		Out_garage();
		return 0;
	}
	else if(state_stayin == FINDING){
		Find_side_garage();
		return 0;
	}
	SearchLeftLine_LongestWhiteline(0,IMG_HEIGHT-STARTY,img_data.minlength,img_data.leftline);
	SearchRightLine_LongestWhiteline(0,IMG_HEIGHT-STARTY,img_data.minlength,img_data.rightline);
	if(right_throwline>0.16*IMG_HEIGHT&&left_throwline>0.16*IMG_HEIGHT){
		FindLongestLine(0.35,0.35);     //寻找最长白列
		FindStartSearchPoint(img_data.leftline,img_data.rightline);       //寻找起始点
		SearchLeftLine_Craw(img_data.leftline,left_throwline);        //寻找左侧爬线
		SearchRightLine_Craw(img_data.rightline,right_throwline);       //寻找右侧爬
	}
	//拟合中线12
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		img_data.cenline[y] = (int)(0.5 * (img_data.leftline[y] + img_data.rightline[y]));
	}
	Test_number_of_throwline(left_throwline,right_throwline);  //检测丢线数量
	//最小二乘法计算平均残差
	least_squares(img_data.leftline, STARTY + left_throwline,  img_data.minlength, &ks, &bs, &rl);	    //cv::line(img, Point(0, (int)bs), Point(600, (int)(ks * 600 + bs)), Scalar(0, 255, 0), 4);                        //画线
	least_squares(img_data.rightline, STARTY + right_throwline, img_data.minlength, &ks, &bs, &rr); 	//cv::line(img, Point(0, (int)bs), Point(600, (int)(ks * 600 + bs)), Scalar(0, 255, 0), 4);
	least_squares(img_data.cenline, STARTY + (left_throwline > right_throwline ? left_throwline : right_throwline), img_data.minlength, &ks, &bs, &rc);
	if(isnan(rr))right_r = 0;    else right_r = rr;
	if(isnan(rl))left_r = 0;     else left_r = rl;
	if(isnan(rc))mid_r = 0;      else mid_r = rc;

	//计算出误差
	for (int y = img_data.minlength; y < IMG_HEIGHT - STARTY - (int)((1-error_midline_flag)*(IMG_HEIGHT - STARTY  - img_data.minlength)); y++) {
		error += img_data.cenline[y];
	}
	error /= (float)(error_midline_flag*(IMG_HEIGHT - STARTY  - img_data.minlength));	


	DebugofImg();
	show = IMG;
	if (img_data.minlength > IMG_HEIGHT - STOPY - 15)  return 1;
	else pre_error_servo = error;
	return error;
}

/*----------------------------------------------Color-------------------------------------------------------*/
// 函数简介     分离图像的不同颜色
// 参数说明     选取颜色 int color
// 返回参数     void
// 使用示例     GetColorImg(PURPLE);
// 备注信息     通过访问指针操作图像IMG；
/*----------------------------------------------Color-------------------------------------------------------*/
void GetColorImg(int color) {
	if (color == BLACK) {
		cvtColor(IMG, IMG, COLOR_BGR2GRAY);
		img_data.thre = threshold(IMG, IMG, 0, 255, THRESH_BINARY | THRESH_OTSU);    // 大津法自动寻求全局阈值。

		return;
	}
	Scalar lower, upper;
	Mat imgHSV, mask;
	GaussianBlur(IMG, IMG, Size(7, 7), 7, 7);             //高斯模糊,高斯模糊内核越大，就有越多的图像参与进来就越平滑。  GaussianBlur(原图像，生成图，内核大小，x轴标准差，y轴标准差)；
	switch (color) {
	case PURPLE:    lower = Scalar(132, 49, 92); upper = Scalar(166, 240, 252); break;
	case GREEN:     lower = Scalar(33, 49, 70); upper = Scalar(78, 227, 220);   break;
	case YELLOW:    lower = Scalar(10, 56, 168); upper = Scalar(26, 255, 252);  break;
	case BLUE:		lower = Scalar(82, 40, 60); upper = Scalar(130, 240, 226);  break;
	default:		lower = Scalar(0, 0, 0); upper = Scalar(255, 255, 255);
	}
	cv::cvtColor(IMG, imgHSV, COLOR_BGR2HSV);
	inRange(imgHSV, lower, upper, IMG);
	for (int i = 0; i < IMG_HEIGHT; i++) {
		for (int j = 0; j < IMG_WIDTH; j++) {
			IMG.at<uchar>(i, j) = 255 - IMG.at<uchar>(i, j);   //对每一个像素反转
		}
	}
	//Canny(IMG, IMG, 25, 120, 3, false);                 //Canny 边缘检测是一种经典的、多阶段的边缘检测算法 threshold1: 第一个阈值（低阈值），用于边缘跟踪。  threshold2: 第二个阈值（高阈值），用于边缘检测。  apertureSize : Sobel 算子的孔径大小（默认为 3，通常使用 3 或 5）。 它影响梯度计算的精度和噪声抑制程度。  L2gradient : 一个标志，指示是否使用更精确的 L2 范数来计算图像梯
}

/*------------------------------------try_get_image--------------------------------------*/
// 函数简介     阻塞式获取图像
// 参数说明    	void
// 返回参数     void
// 使用示例     try_get_image(void);
// 备注信息     这个都是搬运的。第二个函数需要在初始化后运行
/*------------------------------------try_get_image-------------------------------------*/
VideoCapture capture;
int8 camera_init(const char *path)
{
    capture.open(path);
    if(!capture.isOpened()){
        printf("find uvc camera error.\r\n");
        return -1;
    } 
    else{
        printf("find uvc camera Successfully.\r\n");
    }

	// 设置视频流编码器
	capture.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
	capture.set(CAP_PROP_FRAME_WIDTH, IMG_WIDTH);     // 设置摄像头宽度
    capture.set(CAP_PROP_FRAME_HEIGHT, IMG_HEIGHT);    // 设置摄像头高度
    capture.set(CAP_PROP_FPS, IMG_FPS);              // 显示屏幕帧率

    return 0;
}

void try_get_image(void){
    try {// 阻塞式等待图像刷新
        capture >> IMG;// capture.read(frame_rgb);
        if (IMG.empty()) {
            std::cerr << "未获取到有效图像帧" << std::endl;
            return ;
        }
    } 
    catch (const cv::Exception& e) {
        std::cerr << "OpenCV 异常: " << e.what() << std::endl;
        return ;
    }
	// capture.read(IMG);
}



/*------------------------------------------------------------------------------------------------------*/
float LongestWhiteLine_back(void) {
	float error = 0;                  //计算中线误差,
	int left_throwline = 0,right_throwline = 0;          //丢线长度
	float lk = 0 , rk = 0 , ks = 0, bs = 0, rl = 0, rr = 0, rc = 0;    //最小二乘法拟合斜率截距
	FindLongestLine(0.1,0.1);     //寻找最长白列
	FindStartSearchPoint(img_data.leftline,img_data.rightline);       //寻找起始点
	if (state_stayin == STRAIGHT && !flag_finish){
		SearchRightLine_LongestWhiteline(0,110,65,img_data.templine);
		for(int y = 110; y > 65;y--){
			if(img_data.templine[y+1]-img_data.templine[y] > 15){
				beep_run();
				state_stayin = FINDING;
				servo_command = MID_HIT_SLOW;
				mileage_left = 0;
		 		break;
			}
		}
	}
	else if(state_stayin == IN_GARAGE){
		In_garage();
		return 0;
	}
	else if(state_stayin == FINAL){
		Final_stop();
		return 0;
	}

	else if(state_stayin == FINDING){
		Final_find();
		return 0;
	}
	SearchLeftLine_LongestWhiteline(0,IMG_HEIGHT-STARTY,img_data.minlength,img_data.leftline);
	SearchRightLine_LongestWhiteline(0,IMG_HEIGHT-STARTY,img_data.minlength,img_data.rightline);
	if(right_throwline>0.16*IMG_HEIGHT&&left_throwline>0.16*IMG_HEIGHT){
		FindLongestLine(0.35,0.35);     //寻找最长白列
		FindStartSearchPoint(img_data.leftline,img_data.rightline);       //寻找起始点
		SearchLeftLine_Craw(img_data.leftline,left_throwline);        //寻找左侧爬线
		SearchRightLine_Craw(img_data.rightline,right_throwline);       //寻找右侧爬
	}
	//拟合中线12
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		img_data.cenline[y] = (int)(0.5 * (img_data.leftline[y] + img_data.rightline[y]));
	}
	Test_number_of_throwline(left_throwline,right_throwline);  //检测丢线数量
	//最小二乘法计算平均残差
	least_squares(img_data.leftline, STARTY + left_throwline,  img_data.minlength, &ks, &bs, &rl);	    //cv::line(img, Point(0, (int)bs), Point(600, (int)(ks * 600 + bs)), Scalar(0, 255, 0), 4);                        //画线
	least_squares(img_data.rightline, STARTY + right_throwline, img_data.minlength, &ks, &bs, &rr); 	//cv::line(img, Point(0, (int)bs), Point(600, (int)(ks * 600 + bs)), Scalar(0, 255, 0), 4);
	least_squares(img_data.cenline, STARTY + (left_throwline > right_throwline ? left_throwline : right_throwline), img_data.minlength, &ks, &bs, &rc);
	if(isnan(rr))right_r = 0;    else right_r = rr;
	if(isnan(rl))left_r = 0;     else left_r = rl;
	if(isnan(rc))mid_r = 0;      else mid_r = rc;

	//计算出误差
	for (int y = img_data.minlength; y < IMG_HEIGHT - STARTY - (int)((1-error_midline_flag)*(IMG_HEIGHT - STARTY  - img_data.minlength)); y++) {
		error += img_data.cenline[y];
	}
	error /= (float)(error_midline_flag*(IMG_HEIGHT - STARTY  - img_data.minlength));	


	DebugofImg();
	show = IMG;
	if (img_data.minlength > IMG_HEIGHT - STOPY)  return 1;
	else pre_error_servo = error;
	return error;
}
