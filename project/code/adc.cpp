#include"adc.h"

ADCStructTypeDef ADCStruct;
/**
  * @brief  ADC电压值计算
  * @param  None
  * @note   adc_reg 为ADC的数字量;
  *         adc_reg * adc_scale 这两个相乘，得到的是实际的电压值，单位mv
  *         这里的11为硬件分压电阻,计算公式为: R37+R38/R38 = 11
  * @retval 如果测量实时电压值应该要定时器或者放while循环里面
  */
void ADC_get(void)
{
    ADCStruct.adc_reg = adc_convert(ADC_REG_PATH);
    ADCStruct.adc_scale = adc_get_scale(ADC_SCALE_PATH);
    ADCStruct.battery_vol = ADCStruct.adc_reg * ADCStruct.adc_scale * 11;

    if(ADCStruct.battery_vol <= 11.1) beep_run();
}

/**
  * @brief  ADC电压值定时器
  * @param  None
  * @note   10ms采集一次值
  * @retval 如果测量实时电压值应该要定时器或者放while循环里面
  */
 void ADC_init(void)
 {
    pit_ms_init(10, ADC_get);
 }