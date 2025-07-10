#ifndef _oscilloscope_h
#define _oscilloscope_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"

/*---------------------------------type-def--------------------------------------------*/

static void pit_callback_oscilloscope_tcp_client(void);
void oscilloscope_tcp_client_init(void);
void vofa_send_justfloat_2ch(float data1, float data2);
void vofa_send_justfloat_3ch(float data1, float data2, float data3);
void vofa_send_justfloat_8ch(float data1, float data2,float data3, float data4,float data5, float data6,float data7, float data8);

#endif