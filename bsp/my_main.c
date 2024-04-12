#include "my_main.h"

#include "led.h"
#include "lcd.h"
#include "bsp_adc.h"
#include "string.h"
#include "stdio.h"
#include "adc.h"
#include "interrupt.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

char view_flag = 0;
char set_flag = r37_h;
uint8_t uart_rx[50];
uint8_t uart_tx[50];

uint32_t time_500ms;

const char r37_c[5] = "R37";
const char r38_c[5] = "R38";

struct product r37 = {0.0f, 1.2f, 2.2f}, r38 = {0.0f, 1.4f, 3.0f};

extern struct key_t bkey[];

void disp_process(void);
void key_process(void);

void setup(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)uart_rx, sizeof(uart_rx));

    led_disp(0x00);
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);

    time_500ms = uwTick;
}

void pass_clear(product *r)
{
    r->pass_num = 0;
    r->total = 0;
    r->pass = 0;
}

void loop(void)
{
    key_process();
    disp_process();

    if (uwTick - time_500ms >= 500) // 如果当前的系统滴答计数和上一次清除屏幕的时间之差大于或等于500
    {
        time_500ms = uwTick; // 更新上一次清除屏幕的时间为当前的系统滴答计数
        LCD_Clear(Black); // 清除LCD屏幕
    }
}



void disp_process(void)
{
    if (view_flag == 0)
    {
        led_disp(0x04);
        if (r37.tim_flag == 1)
        {
            led_chg(0x04, 1, 1);
        }

        if (r38.tim_flag == 1)
        {
            led_chg(0x04, 2, 1);
        }

        char text[30];
        sprintf(text, "       GOODS     ");
        LCD_DisplayStringLine(Line1, (uint8_t *)text);

        sprintf(text, "     R37:%.2fV", r37.volt = get_adc(&hadc2));
        LCD_DisplayStringLine(Line3, (uint8_t *)text);

        sprintf(text, "     R38:%.2fV", r38.volt = get_adc(&hadc1));
        LCD_DisplayStringLine(Line4, (uint8_t *)text);
    }
    if (view_flag == 1)
    {
        led_disp(0x08);
        char text[30];
        sprintf(text, "      STANDARD     ");
        LCD_DisplayStringLine(Line1, (uint8_t *)text);

        sprintf(text, "    SR37:%.1f-%.1f     ", r37.s_low, r37.s_high);
        LCD_DisplayStringLine(Line3, (uint8_t *)text);

        sprintf(text, "    SR38:%.1f-%.1f     ", r38.s_low, r38.s_high);
        LCD_DisplayStringLine(Line4, (uint8_t *)text);
    }
    if (view_flag == 2)
    {
        led_disp(0x10);
        char text[30];
        sprintf(text, "        PASS     ");
        LCD_DisplayStringLine(Line1, (uint8_t *)text);

        sprintf(text, "     PR37:%.1f%%    ", r37.pass);
        LCD_DisplayStringLine(Line3, (uint8_t *)text);

        sprintf(text, "     PR38:%.1f%%    ", r38.pass);
        LCD_DisplayStringLine(Line4, (uint8_t *)text);
    }
}

void key_process(void)
{
    if (bkey[1].short_flag == 1)
    {
        LCD_Clear(Black);
        view_flag++;
        if (view_flag == 3)
        {
            view_flag = 0;
        }
        bkey[1].short_flag = 0;
    }
    if (bkey[2].short_flag == 1)
    {
        if (view_flag == 0)
        {
            // 按下按键B2，检测产品R37是否合格
            if (r37.volt >= r37.s_low && r37.volt <= r37.s_high)
            {
                r37.pass_num++;
                r37.tim_flag = 1;
            }
            r37.total++;
            r37.pass = (float)r37.pass_num / r37.total * 100;
        }
        if (view_flag == 1)
        {
            // 选择需要调整产品R37、产品R38的检测标准上限或下限
            set_flag++;
            if (set_flag == 4)
            {
                set_flag = r37_h;
            }
        }
        bkey[2].short_flag = 0;
    }
    if (bkey[3].short_flag == 1)
    {
        if (view_flag == 0)
        {
            // 按下按键B3，检测产品R38是否合格
            if (r38.volt >= r38.s_low && r38.volt <= r38.s_high)
            {
                r38.pass_num++;
                r38.tim_flag = 1;
            }
            r38.total++;
            r38.pass = (float)r38.pass_num / r38.total * 100;
        }
        // 按键B3，选择的产品标准上限或下限加0.2
        if (view_flag == 1)
        {
            switch (set_flag)
            {
            case r37_h:
                pass_clear(&r37);
                r37.s_high += 0.2f;
                if (r37.s_high > 3.1f)
                {
                    r37.s_high = 2.2f;
                }
                break;
            case r37_l:
                pass_clear(&r37);
                r37.s_low += 0.2f;
                if (r37.s_low > 2.1f)
                {
                    r37.s_low = 1.2f;
                }
                break;
            case r38_h:
                pass_clear(&r38);
                r38.s_high += 0.2f;
                if (r38.s_high > 3.1f)
                {
                    r38.s_high = 2.2f;
                }
                break;
            case r38_l:
                pass_clear(&r38);
                r38.s_low += 0.2f;
                if (r38.s_low > 2.1f)
                {
                    r38.s_low = 1.2f;
                }
                break;
            default:
                break;
            }
        }
        bkey[3].short_flag = 0;
    }
    if (bkey[4].short_flag == 1)
    {
        if (view_flag == 2)
        { // 按下按键B4，清零产品R37、产品R38的合格率
            pass_clear(&r37);
            pass_clear(&r38);
        }
        if (view_flag == 1)
        {
            switch (set_flag)
            {
            case r37_h:
                pass_clear(&r37);
                r37.s_high -= 0.2f;
                if (r37.s_high < 2.1f)
                {
                    r37.s_high = 3.0f;
                }
                break;
            case r37_l:
                pass_clear(&r37);
                r37.s_low -= 0.2f;
                if (r37.s_low < 1.1f)
                {
                    r37.s_low = 2.0f;
                }
                break;
            case r38_h:
                pass_clear(&r38);
                r38.s_high -= 0.2f;
                if (r38.s_high < 2.1f)
                {
                    r38.s_high = 3.0f;
                }
                break;
            case r38_l:
                pass_clear(&r38);
                r38.s_low -= 0.2f;
                if (r38.s_low < 1.1f)
                {
                    r38.s_low = 2.0f;
                }
                break;
            default:
                break;
            }
        }
        bkey[4].short_flag = 0;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    char rx_buff[10]; // 定义一个字符数组用于存储从UART接收到的数据

    sscanf(uart_rx,"%s",rx_buff); // 从uart_rx中读取字符串并存储到rx_buff中

    // 如果接收到的字符串是"R37"
    if(strcmp(rx_buff,"R37")==0)
    {
        // 将"R37"后面的数据格式化输出到uart_tx中
        sprintf(uart_tx, "R37:%d,%d,%.1f%%\r\n", r37.total, r37.pass_num, r37.pass);
        // 通过DMA（直接内存访问）方式发送uart_tx中的数据
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart_tx, strlen(uart_tx));
        // 清空uart_rx，准备接收新的数据
        memset(uart_rx, 0, sizeof(uart_rx));
    }

    // 如果接收到的字符串是"R38"
    if (strcmp(rx_buff, "R38") == 0)
    {
        // 将"R38"后面的数据格式化输出到uart_tx中
        sprintf(uart_tx, "R38:%d,%d,%.1f%%\r\n", r38.total, r38.pass_num, r38.pass);
        // 通过DMA（直接内存访问）方式发送uart_tx中的数据
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart_tx, strlen(uart_tx));
        // 清空uart_rx，准备接收新的数据
        memset(uart_rx, 0, sizeof(uart_rx));
    }

    // 通过中断方式接收UART数据，当UART空闲时触发中断
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)uart_rx, sizeof(uart_rx));
}

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
// {
//     char rx_buff[10];
//     sscanf(uart_rx, "%s", rx_buff);
//     if(strcmp(rx_buff,"R37")==0)
//     {
//         sprintf(uart_tx, "XXX%%\r\n");
//         HAL_UART_Transmit_DMA(&huart1,(uint8_t*)uart_tx,strlen(uart_tx));
//         memset(uart_rx, 0, sizeof(uart_rx));
//     }
//     HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)uart_rx, sizeof(uart_rx));
// }
