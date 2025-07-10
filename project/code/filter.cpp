#include"filter.h"

/**
  * @brief  指数加权移动平均滤波器（Exponential Moving Average, EMA）初始化
  * @param  filter 指向EMA_filter_Struct结构体的指针
  *         alpha 平滑因子，范围 (0, 1)
  * @note   
  * @retval None
  */
// 初始化EMA滤波器
/* alpha = Ts / (Ts + (1 / 2 * pi * fc))      Ts是采样周期;(1 / 2 * pi * fc)是时间常数
    fc < fN = fs / 2                          fN是奈奎斯特频率，fs是采样频率  
    根据奈奎斯特定理，数字信号处理中，为了避免混叠，信号的最高频率应低于奈奎斯特频率。
    编码器采样周期现在是1ms,α约为0.386，但是实际情况直接看波形吧

    较大的 α,即α（接近1）：滤波器对新数据反应更快，保留更多高频信息，但平滑效果较弱。
    较小的 α,即α（接近0）：滤波器响应较慢，平滑效果较强，但可能引入更大的延迟。
*/
void EMA_init(EMA_filter_Struct* filter, float alpha) {
    if (alpha <= 0.0 || alpha >= 1.0) {
        // 无效的alpha值，设置为默认值0.5
        filter->alpha = 0.5;
    } else {
        filter->alpha = alpha;
    }
    filter->init_flag = 0;          // 标记为未初始化
    filter->out_last = 0.0;         // 初始化滤波输出为0
}


/**
  * @brief  更新EMA滤波器并返回当前滤波值
  * @param  filter 指向EMA_filter_Struct结构体的指针
  *         new_value 新的输入值
  * @note   一个是滤波结果（out），一个是原始数据(value)
  * @retval  double 当前的滤波输出值
  */
float EMA_update(EMA_filter_Struct* filter, int new_value) {
    if (!filter->init_flag) {
        // 第一次调用，直接将新值作为初始输出
        filter->out_last = new_value;
        filter->init_flag = 1;
    } else {
        // 应用EMA公式
        filter->out_last = filter->alpha * new_value + (1.0 - filter->alpha) * filter->out_last;
    }
    return filter->out_last;
}


/**
  * @brief  重置EMA滤波器
  * @param  filter 指向EMA_filter_Struct结构体的指针
  *         init_value 重置时的初始值
  * @note   
  * @retval None
  */
void EMA_reset(EMA_filter_Struct* filter, float init_value) {
    filter->out_last = init_value;
    filter->init_flag = 1;
}

