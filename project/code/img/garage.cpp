#include "garage.h"
int servo_command;
int counter_ms = 0;
float start_yaws = 0;
int init_flag = 1;
int flag_finish = 0;
int white_test = 0;
void Ros_car_init(){
    if(init_flag == 1) {
        servo_command = MID_HIT;
        state_stayin = TURN;
        int black_test = 0;
        for (int x = THROWPOINT ; x < IMG_WIDTH - THROWPOINT; x++) {
            if (IMG.at<uchar>(96, x) == 0)     black_test ++;
        }
        if(black_test >= 0.6*IMG_WIDTH){
            state_stayin = OUT_GARAGE;
            start_yaws = ImuStruct.yaw;
            black_test = 0;
            init_flag = 0;
        }
    }
}
void Out_garage(void){
    servo_command = RIGHT_HIT;
    if(abs(ImuStruct.yaw - start_yaws) >= 75){
        beep_run();
        state_stayin = STRAIGHT;
        start_yaws = 0;
        servo_command = NULL;

    }
}
void Find_side_garage(void){
    if(abs(mileage_left) > GARAGE_SIDE){
        beep_run();
        servo_command = STOP;
        //counter_ms = 0;
        state_stayin = IN_SIDE_GARAGE;
        start_yaws = ImuStruct.yaw;
        servo_command = RIGHT_HIT_B;
    }
}
void In_side_garage(void){
    if(abs(ImuStruct.yaw - start_yaws) >= 78){
        beep_run();
        servo_command = STOP;
        counter_ms = 0;
        state_stayin = IN_SIDE_GARAGE_II;
        start_yaws = ImuStruct.yaw;
        servo_command = LEFT_HIT_B;
    }
}
void In_side_garage_ii(void){
    if(abs(ImuStruct.yaw - start_yaws) >= 74){
        beep_run();
        start_yaws = 0;
        counter_ms = 30;
    }
}
void Out_side_garage(void){
    if(abs(ImuStruct.yaw - start_yaws) >= 70){
        beep_run();
        start_yaws = ImuStruct.yaw;
        state_stayin = OUT_SIDE_GARAGE_II;
        servo_command = RIGHT_HIT_SL;
    }
}
void Out_side_garage_ii(void){
    if(abs(ImuStruct.yaw - start_yaws) >= 45){
        beep_run();
        start_yaws = 0;
        state_stayin = STRAIGHT;
        servo_command = NONE;
        flag_finish = 1;
    }
}
void Final_find(void){
    if(abs(mileage_left) > INTEGRATE_GARAGE){
        beep_run();
        servo_command = STOP;
        //counter_ms = 0;
        state_stayin = IN_GARAGE;
        start_yaws = ImuStruct.yaw;
        servo_command = RIGHT_HIT_B;
    }
}
void In_garage(void){
    if(abs(ImuStruct.yaw - start_yaws) >= 90){
        beep_run();
        servo_command = STOP;
        counter_ms = 0;
        state_stayin = FINAL;
        start_yaws = ImuStruct.yaw;
        servo_command = MID_HIT_SB;
    }
}
void Final_stop(void){
    for (int x = THROWPOINT ; x < IMG_WIDTH - THROWPOINT; x++) {
            if (IMG.at<uchar>(53, x) == 255)     white_test ++;
        }
        if(white_test >= 0.6*IMG_WIDTH){
            start_yaws = ImuStruct.yaw;
            white_test = 0;
            extern void cleanup(void);
            cleanup();
            exit(0);
        }
}
int Wait_second(void){
    if(counter_ms == 1)  {
        state_stayin = OUT_SIDE_GARAGE;
        start_yaws = ImuStruct.yaw;
        servo_command = LEFT_HIT_SL;
        counter_ms = 0; 
        return 1;
    }
    if(counter_ms == 0) return 0;
    counter_ms --;
    servo_command = STOP;
    return 1;
}
