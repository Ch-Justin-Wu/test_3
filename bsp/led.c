#include "led.h"

void led_disp(uint8_t ds_led)
{ // 所有LED熄灭
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
    // 输出数据
    HAL_GPIO_WritePin(GPIOC, ds_led << 8, GPIO_PIN_RESET);
    // 使能锁存器
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    // 关闭锁存器
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

// 单独控制一位led
void led_chg(uint8_t led_sta, uint8_t num, uint8_t status) // num:1-8
{
    uint8_t pos = 0x01 << (num - 1); // 计算出要改变的LED在led_sta中的位置

    // 将led_sta中对应位置的LED状态改为status
    led_sta = (led_sta & (~pos) | (pos * status));

    led_disp(led_sta); // 显示新的LED状态
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











