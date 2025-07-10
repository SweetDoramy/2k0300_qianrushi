#ifndef _filter_h
#define _filter_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
 
typedef struct filter
{
    float   alpha;          // 平滑因子,可算
    float   out_last;       // 上一次的滤波输出
    uint8    init_flag;      // 初始化标志
}EMA_filter_Struct;


void EMA_init(EMA_filter_Struct* filter, float alpha);
float EMA_update(EMA_filter_Struct* filter, int new_value);
void EMA_reset(EMA_filter_Struct* filter, float init_value);

#endif