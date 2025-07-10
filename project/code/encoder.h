#ifndef _encoder_h
#define _encoder_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"

#define ENCODER_1           "/dev/zf_encoder_1"
#define ENCODER_2           "/dev/zf_encoder_2"

typedef struct 
{    
    int16 encoder_left;
    int16 encoder_right;   
}EncoderStructTypeDef;

extern EncoderStructTypeDef Encoderstruct;

void pit_callback_encoder(void);
void encoder_init(void);

#endif