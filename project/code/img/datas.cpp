#include "datas.h"

extern PidPlaceTypeDef PIDServoStruct;

int pre_error_servo;
int weighted_array[IMG_HEIGHT]={

};

/*------------------------------------WIFI 图传边线 不允许改动!--------------------------------------*/
extern uint16 leftLine[IMG_HEIGHT],rightLine[IMG_HEIGHT],midLine[IMG_HEIGHT];
/*------------------------------------WIFI 图传边线 不允许改动!--------------------------------------*/

/*------------------------------------Find_Zebra_crossing--------------------------------------*/
// 函数简介     状态管理
// 参数说明     void
// 返回参数     void
// 使用示例     State_Manager();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------Find_Zebra_crossing--------------------------------------*/
void Test_left_circle(float& rl,float& rr,float& circle_ERROR,int& right_throwline){
	int circle_start_point = 0;       //圆环数组的起始位置
	int circle_end_point = 0;         //圆环数组的终止位置
	int circle_N = 0;                 //圆弧段的y的长度，例如 img_data.templine[cirlce_N]
	float kss=0,bss=0;
	if (rl > CIRCLE_ERROR_THRESHOLD * rr && rr < 70 && right_throwline<0.18*IMG_HEIGHT  && state_stayin != LEFT_TURN_CIRCLE && state_stayin != RIGHT_TURN_CIRCLE) {
		SearchRightLine_LongestWhiteline(false,IMG_HEIGHT - STARTY, img_data.minlength ,img_data.templine);
		least_squares(img_data.templine, STARTY + right_throwline, img_data.minlength, &kss, &bss, &rr);
		if(rr < 100){
			SearchLeftLine_LongestWhiteline(false,IMG_HEIGHT - STARTY, img_data.minlength ,img_data.templine);
			for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
				if (img_data.templine[y] == THROWPOINT && img_data.templine[y - 1] != THROWPOINT) {
					circle_start_point = y - 1;
					for (int yy = y; yy > img_data.minlength; yy--) {
						if (img_data.templine[yy] != THROWPOINT && img_data.templine[yy - 1] == THROWPOINT) {
							circle_end_point = yy;
							circle_N = circle_start_point - circle_end_point;
							break;
						}
						else circle_N = 0;
					}
					break;
				}
			}
			int ssssssssssssss = 0;
			SearchLeftLine_Craw(img_data.templine,ssssssssssssss);        //寻找左侧爬线
			if (circle_N != 0 && circle_N > CIRCLELONG_THRESHOLD) {
				least_squares(img_data.templine, IMG_HEIGHT - circle_start_point, circle_start_point - circle_N, &kss, &bss, &circle_ERROR);
				if(isnan(circle_ERROR))circle_err = 0;      else circle_err = circle_ERROR;
			}
			else circle_ERROR = 0;
			if (circle_ERROR > 10 && circle_ERROR < 160) {
				state_stayin = LEFT_TURN_CIRCLE;
				//integral_yaw_start();
				beep_run();
			}
		} 
	}
}


/*------------------------------------Find_Zebra_crossing--------------------------------------*/
// 函数简介     状态管理
// 参数说明     void
// 返回参数     void
// 使用示例     State_Manager();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------Find_Zebra_crossing--------------------------------------*/
void Test_right_circle(float& rl,float& rr,float& circle_ERROR,int& left_throwline){
	int circle_start_point = 0;       //圆环数组的起始位置
	int circle_end_point = 0;         //圆环数组的终止位置
	int circle_N = 0;                 //圆弧段的y的长度，例如 img_data.templine[cirlce_N]
	float kss=0,bss=0;
	if (rr > CIRCLE_ERROR_THRESHOLD * rl && rl < 70 && left_throwline<0.18*IMG_HEIGHT  && state_stayin != LEFT_TURN_CIRCLE && state_stayin != RIGHT_TURN_CIRCLE) {
		SearchLeftLine_LongestWhiteline(false,IMG_HEIGHT - STARTY, img_data.minlength ,img_data.templine);
		least_squares(img_data.templine, STARTY + left_throwline, img_data.minlength, &kss, &bss, &rl);
		if(rl < 100){
			SearchRightLine_LongestWhiteline(false,IMG_HEIGHT - STARTY, img_data.minlength ,img_data.templine);
			for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
				if (img_data.templine[y] == IMG_WIDTH - THROWPOINT && img_data.templine[y - 1] != IMG_WIDTH - THROWPOINT) {
					circle_start_point = y - 1;
					for (int yy = y; yy > img_data.minlength; yy--) {
						if (img_data.templine[yy] != IMG_WIDTH - THROWPOINT && img_data.templine[yy - 1] == IMG_WIDTH - THROWPOINT) {
							circle_end_point = yy;
							circle_N = circle_start_point - circle_end_point;
							break;
						}
						else circle_N = 0;
					}
					break;
				}
			}
			int ssssssssssssss = 0;
			SearchRightLine_Craw(img_data.templine,ssssssssssssss);        //寻找左侧爬线
			if (circle_N != 0 && circle_N > CIRCLELONG_THRESHOLD) {
				least_squares(img_data.templine, IMG_HEIGHT - circle_start_point, circle_start_point - circle_N, &kss, &bss, &circle_ERROR);
				if(isnan(circle_ERROR))circle_err = 0;      else circle_err = circle_ERROR;
			}
			else circle_ERROR = 0;
			if (circle_ERROR > 10 && circle_ERROR < 160) {
				state_stayin = RIGHT_TURN_CIRCLE;
				//integral_yaw_start();
				beep_run();
			}
		} 
	}
}

/*------------------------------------Find_Zebra_crossing--------------------------------------*/
// 函数简介     状态管理
// 参数说明     void
// 返回参数     void
// 使用示例     State_Manager();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------Find_Zebra_crossing--------------------------------------*/
int motorflag = 0;
void State_Manager(void){

	switch (state_stayin) {
		while(key_get_state(KEY_P13) == KEY_SHORT_PRESS){
			motorflag = 0;
		}
		case STRAIGHT:  
			putText(IMG, "straight", Point(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 2);
			// Motor.open_duty = 2000;
			Motor.expect_val_encoder = close_speed;
			// Pid_PlaceParmSet(&PIDServoStruct,7.5,0,60,10000);
			break;
		case TURN:		
			putText(IMG, "turn", Point(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 2);
			// Motor.open_duty = 2000;
			Motor.expect_val_encoder = 300;
			// Pid_PlaceParmSet(&PIDServoStruct,8.5,0,45,10000);
			break;
		case LEFT_TURN_CIRCLE:    
			putText(IMG, "LEFT_TURN_CIRCLE", Point(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 2);
			// Motor.open_duty = 2000;
			Motor.expect_val_encoder = 300;
			// Pid_PlaceParmSet(&PIDServoStruct,7.5,0.01,45,10000);
			break;
		case RIGHT_TURN_CIRCLE:   
			putText(IMG, "RIGHT_TURN_CIRCLE", Point(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 2);
			// Motor.open_duty = 2000;
			Motor.expect_val_encoder = 300;
			// Pid_PlaceParmSet(&PIDServoStruct,7.5,0.01,45,10000);
			break;
	}
	if(Find_Zebra_crossing()){
		putText(IMG, "ZEBRA_LINE", Point(100,50), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 2); 
		stop();
	}
}
void stop(){
	while(key_get_state(KEY_P13) != KEY_SHORT_PRESS){
		Motor.open_duty = 0;
		Motor.expect_val_encoder = 0;
		servo_set(MID_VALUE);
	}
}
/*------------------------------------Find_Zebra_crossing--------------------------------------*/
// 函数简介     寻找斑马线
// 参数说明     void
// 返回参数     找到了置于1,没找到置于0；
// 使用示例     Find_Zebra_crossing();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------Find_Zebra_crossing--------------------------------------*/
int Find_Zebra_crossing(){
	int counter_point = 0;
	for(int x = THROWPOINT + (int)(0.15*IMG_WIDTH); x < IMG_WIDTH - THROWPOINT - (int)(0.15*IMG_WIDTH); x++){
		if(IMG.at<uchar>(0.85*IMG_HEIGHT, x) == 255 && IMG.at<uchar>(0.85*IMG_HEIGHT, x+1) != 255){
			counter_point++;
		}
	}
	if(counter_point > 4){

		return 1;
	}
	else return 0; 
}

/*------------------------------------FindLongestLine--------------------------------------*/
// 函数简介     寻找最长白列
// 参数说明     leftlimit,rightlimit    左右舍弃行数（局部变量，取值在0-1）
//			   img_data.minlength   最长白列搜线长度(有传出值)
// 			   img_data.Longestline        最长白列所在行数(有传出值)
// 参数说明     THROWPOINT,STARTY,img_data.minlength,img_data.leftline,img_data.Longestline,rightline需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindLongestLine--------------------------------------*/
void FindLongestLine(float leftlimit,float rightlimit){
	//找 最长白列 和 最长白列的长度
	img_data.minlength = IMG_HEIGHT;        //最长白列的长度
	img_data.Longestline = -1;                   //最长白列的列号
	for (int x = THROWPOINT + (int)(leftlimit*IMG_WIDTH); x < IMG_WIDTH - THROWPOINT - (int)(rightlimit*IMG_WIDTH); x++) {
		if (IMG.at<uchar>(IMG_HEIGHT - STARTY, x) != 255)  continue;
		for (int y = IMG_HEIGHT - STARTY; y >  STARTY; y--) {
			if (IMG.at<uchar>(y, x) != 255) {
				if (y < img_data.minlength) { img_data.minlength = y; img_data.Longestline = x; }
				break;
			}
		}
	}
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     寻找搜线起始点
// 参数说明		img_data.leftline img_data.rightline 数组有修改
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void FindStartSearchPoint(int* leftline,int* rightline){
	//找左右搜线的起始点（其中，左丢线为x=THROWPOINT,右丢线为x=IMG_WIDTH-throwpoint）
	for (int x = img_data.Longestline; x >= THROWPOINT; x--) {
		if (IMG.at<uchar>(IMG_HEIGHT - STARTY, x) != 255 || x == THROWPOINT) { leftline[IMG_HEIGHT - STARTY] = x;  break; }
	}
	for (int x = img_data.Longestline; x <= IMG_WIDTH - THROWPOINT; x++) {
		if (IMG.at<uchar>(IMG_HEIGHT - STARTY, x) != 255 || x == IMG_WIDTH - THROWPOINT) { rightline[IMG_HEIGHT - STARTY] = x;  break; }
	}
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     爬线法搜线-左线
// 参数说明	
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void SearchLeftLine_Craw(int* linein,int& left_throwline){
	int counter = 0;
	int startflag = 0;
	int flag_l = 0;
	int store_y1 =STARTY,store_x1= THROWPOINT,store_y2 = STARTY,store_x2= THROWPOINT;
	//接下来开始向前搜线段(左)
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		//判断前方是黑块还是白块  （黑0，白255）
		startflag = 0;
		if (IMG.at<uchar>(y - 1, linein[y]) == 255) {
			counter = 0;
			do {
				counter++;
				if (IMG.at<uchar>(y - 1, linein[y] - counter) != 255 || linein[y] - counter < THROWPOINT){
					linein[y - 1] = linein[y] - counter + 1;
					break;
				}
			} while (counter < LINE_SEARCH_THRESHOLD);
			//找到左边拐点
			if (counter == LINE_SEARCH_THRESHOLD) {
				startflag = 1;
				linein[y - 1] = linein[y];
			}
		}
		//如果是黑块，那么就顺着找到边线
		else {
			for (int x = linein[y]; x <= IMG_WIDTH - THROWPOINT; x++) {
				if (IMG.at<uchar>(y - 1, x) != 0 || x == IMG_WIDTH - THROWPOINT) {
					linein[y - 1] = x;
					break;
				}
			}
		}
		//拐点状态机判断
		if (startflag != flag_l) {
			if (startflag == 1 && flag_l == 0)         store_x1 = linein[y], store_y1 = y;
			else if (startflag == 0 && flag_l == 1)    store_x2 = linein[y-1], store_y2 = y-1;
			if (store_y2 != STARTY ) {
				//检测是不是末端路线
				if(store_y2 > 1.1*img_data.minlength){
					drawline_list(linein,store_x1,store_x2,store_y1,store_y2);
					store_y1 = STARTY, store_x1 = THROWPOINT, store_y2 = STARTY, store_x2 = THROWPOINT;
				}
				//是的话对最末端调用最长白列
				else{
					for (int y = store_y1; y > store_y2; y--) {
						if (y > img_data.minlength) {
							for (int x = img_data.Longestline; x >= THROWPOINT; x--) {
								if (IMG.at<uchar>(y, x) != 255 || x == THROWPOINT) { linein[y] = x;  break; }
							}
						}
					}
				}
			}
		}
	flag_l = startflag;
	}
	//统计左侧丢线数
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		if (img_data.leftline[y] <= THROWPOINT ) continue;
		left_throwline = IMG_HEIGHT - STARTY - y;
		break;
	}
	//左侧最长白列
	if(left_throwline>LEFT_THRESHOLD)
	SearchLeftLine_LongestWhiteline(true,IMG_HEIGHT - STARTY,IMG_HEIGHT - STARTY - left_throwline - 5,img_data.leftline);
	else 
	SearchLeftLine_LongestWhiteline(false,IMG_HEIGHT - STARTY,IMG_HEIGHT - STARTY - left_throwline - 5,img_data.leftline);
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     爬线法搜线-右线
// 参数说明	
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void SearchRightLine_Craw(int* linein, int& right_throwline){
	int counter = 0,startflag = 0,flag_l = 0;  //局部计数器
	int store_y1 =STARTY,store_x1= THROWPOINT,store_y2 = STARTY,store_x2= THROWPOINT;  //用于爬线段斜率拟合
	//再如法炮制，找到右边线段
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		startflag = 0;
		//判断前方是黑块还是白块  （黑0，白255）
		if (IMG.at<uchar>(y - 1, linein[y]) == 255) {
			counter = 0;
			do {
				counter++;
				if (IMG.at<uchar>(y - 1, linein[y] + counter) != 255 || linein[y] + counter > IMG_WIDTH - THROWPOINT) {
					linein[y - 1] = linein[y] + counter - 1;
					break;
				}
			} while (counter < LINE_SEARCH_THRESHOLD);
			//找到右边拐点
			if (counter == LINE_SEARCH_THRESHOLD) {
				startflag = 1;
				linein[y - 1] = linein[y];
			}
		}
		//如果是黑块，那么就顺着找到边线
		else {
			for (int x = linein[y]; x >= THROWPOINT; x--) {
				if (IMG.at<uchar>(y - 1, x) != 0 || x == THROWPOINT) {
					linein[y - 1] = x;
					break;
				}
			}
		}
		if (startflag != flag_l) {
			if (startflag == 1 && flag_l == 0)         store_x1 = linein[y], store_y1 = y;
			else if (startflag == 0 && flag_l == 1)    store_x2 = linein[y-1], store_y2 = y-1;
			if (store_y2 != STARTY) {
				if(store_y2 > 1.1*img_data.minlength){
					drawline_list(linein,store_x1,store_x2,store_y1,store_y2);
					store_y1 = STARTY, store_x1 = THROWPOINT, store_y2 = STARTY, store_x2 = THROWPOINT;
				}
				else{
					for(int y = store_y1; y > store_y2; y--) {
						if (y > img_data.minlength) {
							for (int x = img_data.Longestline; x <=IMG_WIDTH - THROWPOINT; x++) {
								if (IMG.at<uchar>(y, x) != 255 || x == IMG_WIDTH - THROWPOINT) { linein[y] = x;  break; }
							}
						}
					}
				}
			}
		}
	flag_l = startflag;
	//rightLine[y - 1] = uint16(linein[y - 1]); // SPI图传边线更新
	}
	//统计右侧最底部丢线数
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		if (img_data.rightline[y] >= IMG_WIDTH - THROWPOINT) continue;
		right_throwline = IMG_HEIGHT - STARTY - y;
		break;
	}
	//右侧底边寻线
	if(right_throwline < RIGHT_THRESHOLD)
	SearchRightLine_LongestWhiteline(true,IMG_HEIGHT - STARTY,IMG_HEIGHT - STARTY - right_throwline - 5,img_data.rightline);
	else
	SearchRightLine_LongestWhiteline(false,IMG_HEIGHT - STARTY,IMG_HEIGHT - STARTY - right_throwline - 5,img_data.rightline);
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     最长白列算法-右线
// 参数说明	    start = IMG_HEIGHT - STARTY
// 参数说明     end = img_data.minlength
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void SearchRightLine_LongestWhiteline(bool choose,int start,int end,int* linein){
	//Point a;
	//最长白列算法
	for (int y = start; y > end; y--) {
		if (y > img_data.minlength) {
			for (int x = img_data.Longestline; x <= IMG_WIDTH - THROWPOINT; x++) {
				if (IMG.at<uchar>(y, x) != 255 || x == IMG_WIDTH - THROWPOINT) { linein[y] = x;  break; }
			}
		}
	}
	if(choose){
		//基础补线
		for (int y = IMG_HEIGHT - STARTY; y > (int)(1.1 * img_data.minlength); y--) {
			if (linein[y] != IMG_WIDTH - THROWPOINT && linein[y + 1] == IMG_WIDTH - THROWPOINT) {
				do {
					y--;
				} while (abs(linein[y] - linein[y - 2] + linein[y - 1] - linein[y - 3]) > 8 && y > (int)(1.1 * img_data.minlength));
	
				//if (y == (int)(1.1 * img_data.minlength) || y < 30) break;
				float k = (int)((abs(linein[y] - linein[y - 5] + linein[y - 1] - linein[y - 6] + linein[y - 2] - linein[y - 7] + linein[y - 3] - linein[y - 8] + linein[y - 4] - linein[y - 9])) * 0.022);
				//if (k == 0 || k > 7)break;
				for (int cur = y; cur < IMG_HEIGHT; cur++) {
					if (linein[cur - 1] - k > IMG_WIDTH - THROWPOINT) break;
					linein[cur] = linein[y] + (int)(k*(cur - y));
				}
				//a.x = linein[y];
				//a.y = y;
				//circle(IMG, a, 9, Scalar(0, 0, 0), 2);
				break;
			}
		}
	}
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     最长白列算法-左线
// 参数说明	
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
//右侧
void SearchLeftLine_LongestWhiteline(bool choose,int start,int end,int* linein){
	//最长白列算法
	for (int y = start; y > end; y--) {
		if (y > img_data.minlength) {
			for (int x = img_data.Longestline; x >= THROWPOINT; x--) {
				if (IMG.at<uchar>(y, x) != 255 || x == THROWPOINT) { linein[y] = x;  break; }
			}
		}
	}
	if(choose){
		//基础补线
		for (int y = IMG_HEIGHT - STARTY; y > (int)(1.1 * img_data.minlength); y--) {
			if (linein[y] != THROWPOINT && linein[y + 1] == THROWPOINT) {
				do {
					y--;
				} while (abs(linein[y] - linein[y - 2] + linein[y - 1] - linein[y - 3]) > 8 && y > (int)(1.1 * img_data.minlength));
				if (y == (int)(1.1 * img_data.minlength) || y < 20) break;
				float k = (int)((abs(linein[y] - linein[y - 5] + linein[y - 1] - linein[y - 6] + linein[y - 2] - linein[y - 7] + linein[y - 3] - linein[y - 8] + linein[y - 4] - linein[y - 9])) * 0.022);
				//if (k == 0 || k > 7)break;
				for (int cur = y; cur < IMG_HEIGHT; cur++) {
					if (linein[cur - 1] - k < THROWPOINT) break;
					linein[cur] = linein[y] - (int)(k*(cur - y));
				}
				//a.x = linein[y];
				//a.y = y;
				//circle(IMG, a, 9, Scalar(0, 0, 0), 2);
				break;
			}
		}
	}
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     最长白列算法-右线
// 参数说明	
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void DebugofImg(){
	Point a;                 //作图用点
	//画出左边线，右边线，和中线段
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		a.x = img_data.rightline[y];
		a.y = y;
		circle(IMG, a, 1, Scalar(0, 0, 0), -1);
	}
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		a.x = img_data.leftline[y];
		a.y = y;
		circle(IMG, a, 1, Scalar(0, 0, 0), -1);
	}
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		a.x = img_data.cenline[y];
		a.y = y;
		circle(IMG, a, 1, Scalar(0, 0, 0), -1);
	}
	//rectangle(img, (0,0), (80,100), (0,0,0), -1, 4);
	//putText(img, "", (200, 100), cv2.FONT_HERSHEY_COMPLEX, 2.0, (100, 200, 200), 5)
}
/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     最长白列算法-右线
// 参数说明	
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     FindLongestLineAndStartpoin();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void Test_number_of_throwline(int& left_throwline , int& right_throwline){
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		if (img_data.leftline[y] <= THROWPOINT ) continue;
		left_throwline = IMG_HEIGHT - STARTY - y;
		break;
	}
	//统计右侧最底部丢线数
	for (int y = IMG_HEIGHT - STARTY; y > img_data.minlength; y--) {
		if (img_data.rightline[y] >= IMG_WIDTH - THROWPOINT) continue;
		right_throwline = IMG_HEIGHT - STARTY - y;
		break;
	}
}





/*------------------------------------FindStartSearchPoint--------------------------------------*/
// 函数简介     数组补线
// 参数说明     需要外部调用到opencv.h
// 返回参数     void
// 使用示例     drawline_list();
// 备注信息     opencv.cpp的内部调用函数
/*------------------------------------FindStartSearchPoint--------------------------------------*/
void drawline_list (int* destline ,int startx,int endx,int starty,int endy) {
	if (startx != endx && starty != endy ){
		float k = (float)(starty - endy) / (float)(startx - endx);
		float b = endy - k * endx;
		for (int sy = starty; sy >= endy; sy--) {
			destline[sy] = (int)((sy - b) / k);
		}
	}
}



/*
 * 函数：least_squares
 * 功能：计算最小二乘法拟合的直线参数 y = ax + b
 * 参数：
 *      points - 存储数据点的数组
 *      n      - 数据点个数
 *      a      - 指向斜率的指针（输出）
 *      b      - 指向截距的指针（输出）
 */
void least_squares(int* point , int starty , int minlength ,float* a, float* b,float* r) {
	float sum_x = 0.0, sum_y = 0.0;
	float sum_x2 = 0.0, sum_xy = 0.0;
	// 计算所有统计量
	for (int y = IMG_HEIGHT - starty; y > minlength; y--) {
		sum_x += point[y];
		sum_y += y;
		sum_x2 += point[y] * point[y];
		sum_xy += point[y] * y;
	}
	int n = IMG_HEIGHT - starty - minlength;
	// 计算斜率 a
	*a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);

	// 计算截距 b
	*b = (sum_y - (*a) * sum_x) / n;
	for (int y = IMG_HEIGHT - starty-2; y > minlength+2; y--) {
		*r += (point[y] - ((y - (*b)) / (*a)))*(point[y] - ((y - (*b)) / (*a)));
	}
	*r =  *r / n;
}
/**
 * 最小二乘法拟合圆
 * 拟合出的圆以圆心坐标和半径的形式表示
 * 此代码改编自 newsmth.net 的 jingxing 在 Graphics 版贴出的代码。
 * 版权归 jingxing， 我只是搬运工外加一些简单的修改工作。
 * 在本程序中完全没有用的哦
 */
double circleLeastFit(int* line, int N, int start_point, double& center_x, double& center_y, double& radius) {
	center_x = 0.0f;
	center_y = 0.0f;
	radius = 0.0f;

	double sum_x = 0.0f, sum_y = 0.0f;
	double sum_x2 = 0.0f, sum_y2 = 0.0f;
	double sum_x3 = 0.0f, sum_y3 = 0.0f;
	double sum_xy = 0.0f, sum_x1y2 = 0.0f, sum_x2y1 = 0.0f;

	for (int i = start_point; i >= start_point - N; i--) {
		double x = line[i];
		double y = i;
		double x2 = x * x;
		double y2 = y * y;
		sum_x += x;
		sum_y += y;
		sum_x2 += x2;
		sum_y2 += y2;
		sum_x3 += x2 * x;
		sum_y3 += y2 * y;
		sum_xy += x * y;
		sum_x1y2 += x * y2;
		sum_x2y1 += x2 * y;
	}

	double C, D, E, G, H;
	double a, b, c;

	C = N * sum_x2 - sum_x * sum_x;
	D = N * sum_xy - sum_x * sum_y;
	E = N * sum_x3 + N * sum_x1y2 - (sum_x2 + sum_y2) * sum_x;
	G = N * sum_y2 - sum_y * sum_y;
	H = N * sum_x2y1 + N * sum_y3 - (sum_x2 + sum_y2) * sum_y;
	a = (H * D - E * G) / (C * G - D * D);
	b = (H * C - E * D) / (D * D - G * C);
	c = -(a * sum_x + b * sum_y + sum_x2 + sum_y2) / N;

	double errors = 0.0f;
	center_x = a / (-2);
	center_y = b / (-2);
	radius = sqrt(a * a + b * b - 4 * c) / 2;
	for (int i = start_point; i >= start_point - N; i--) {
		errors += abs(sqrt(abs((line[i] - center_x) * (line[i] - center_x) + (i - center_y) * (line[i] - center_x))) - radius * radius);
	}
	errors /= N;
	return errors;
}

