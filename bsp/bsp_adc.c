#include "bsp_adc.h"

struct adc adc;
float get_adc(ADC_HandleTypeDef *hadc)
{
    uint32_t adc; // ����һ���������ڴ洢ADC�Ķ���

    HAL_ADC_Start(hadc); // ����ADC

    adc = HAL_ADC_GetValue(hadc); // ��ȡADC�Ķ���

    // ��ADC�Ķ���ת��Ϊ��ѹֵ������ADC�Ĳο���ѹΪ3.3V���ֱ���Ϊ12λ����4096����
    return adc * 3.3f / 4096.0f;
}
