#ifndef _adc_h
#define _adc_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "beep.h"

#define  ADC_REG_PATH "/sys/bus/iio/devices/iio:device0/in_voltage7_raw"
#define  ADC_SCALE_PATH "/sys/bus/iio/devices/iio:device0/in_voltage_scale"

typedef struct 
{
    uint16 adc_reg;
    float adc_scale;
    uint16 battery_vol;
}ADCStructTypeDef;

extern ADCStructTypeDef ADCStruct;

void ADC_get(void);
void ADC_init(void);

#endif