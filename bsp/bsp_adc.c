#include "bsp_adc.h"

struct adc adc;
float get_adc(ADC_HandleTypeDef *hadc)
{
    uint32_t adc; // 定义一个变量用于存储ADC的读数

    HAL_ADC_Start(hadc); // 启动ADC

    adc = HAL_ADC_GetValue(hadc); // 获取ADC的读数

    // 将ADC的读数转换为电压值，假设ADC的参考电压为3.3V，分辨率为12位（即4096级）
    return adc * 3.3f / 4096.0f;
}
