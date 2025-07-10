#ifndef __GARAGE_H__
#define __GARAGE_H__
#include "code_headfiles.h"

#define NONE         0
#define LEFT_HIT     1
#define RIGHT_HIT    2
#define MID_HIT      3
#define LEFT_HIT_B   4
#define RIGHT_HIT_B  5
#define STOP         6
#define MID_HIT_SLOW 7
#define LEFT_HIT_SL  8
#define RIGHT_HIT_SL 9
#define MID_HIT_SB   10


extern int servo_command;
extern int counter_ms;
extern float start_yaws;
extern int flag_finish;


void Ros_car_init(void);
void Out_garage(void);
int  Wait_second(void);
void Find_side_garage(void);
void In_side_garage(void);
void In_side_garage_ii(void);
void Out_side_garage(void);
void Out_side_garage_ii(void);
void In_garage(void);
void Final_find(void);
void Final_stop(void);

#endif