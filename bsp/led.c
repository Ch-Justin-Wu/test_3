#include "led.h"

void led_disp(uint8_t ds_led)
{ // ����LEDϨ��
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
    // �������
    HAL_GPIO_WritePin(GPIOC, ds_led << 8, GPIO_PIN_RESET);
    // ʹ��������
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    // �ر�������
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

// ��������һλled
void led_chg(uint8_t led_sta, uint8_t num, uint8_t status) // num:1-8
{
    uint8_t pos = 0x01 << (num - 1); // �����Ҫ�ı��LED��led_sta�е�λ��

    // ��led_sta�ж�Ӧλ�õ�LED״̬��Ϊstatus
    led_sta = (led_sta & (~pos) | (pos * status));

    led_disp(led_sta); // ��ʾ�µ�LED״̬
}

void my_led_disp(uint8_t addr)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, addr << 8, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}



void my_led_chg(uint8_t addr,uint8_t num,uint8_t status)
{
    uint8_t pos = 0x01 << (num - 1);

    addr = ((addr & (~pos)) | (pos * status));

    my_led_disp(addr);
}











