#include "distance_control.h"
int dis_state_flag = 0;
float mileage_left  = 0;
void distancepit_encoderrequire_callback(void){
    if(state_stayin == FINDING){
        dis_state_flag = 1;
        mileage_left  += Encoderstruct.encoder_left*0.01;
    }
    else {
        mileage_left = 0;
    }
}

void distance_pit_init(void){
    pit_ms_init(5,distancepit_encoderrequire_callback);
}