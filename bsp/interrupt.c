#include "interrupt.h"
#include "usart.h"
#include "led.h"
#include "my_main.h"

struct keys key[4] = {0, 0, 0, 0};
struct pwm_capture pwm_capture[2] = {0, 0};

extern product r37, r38;

uint16_t led_count = 0;
uint8_t led_values[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
uint8_t led_index = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM6)
    {
        if(r37.tim_flag == 1)
        {
            r37.tim_cnt++;
            if (r37.tim_cnt>=100)
            {
                r37.tim_flag = 0;
                r37.tim_cnt = 0;
            }
        }
        if (r38.tim_flag == 1)
        {
            r38.tim_cnt++;
            if (r38.tim_cnt >= 100)
            {
                r38.tim_flag = 0;
                r38.tim_cnt = 0;
            }
        }
        // key_serv_long();
        key_serv_double();
        // // 读取四个按键的状态
        // key[0].key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
        // key[1].key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
        // key[2].key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
        // key[3].key_status = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        // // 对每个按键进行消抖操作
        // for (int i = 0; i < 4; i++)
        // {
        //     switch (key[i].judge_status)
        //     {
        //     case KEY_UP: // 按键未被按下
        //     {
        //         if (key[i].key_status == DOWN) // 如果按键被按下
        //         {
        //             key[i].judge_status = KEY_DOWN; // 更改状态为按键已被按下
        //         }
        //     }
        //     break;
        //     case KEY_DOWN: // 按键已被按下
        //     {
        //         if (key[i].key_status == DOWN) // 如果按键仍然被按下
        //         {

        //             key[i].judge_status = KEY_HOLD; // 更改状态为按键被持续按下
        //         }
        //         else // 如果按键已经释放
        //         {
        //             key[i].judge_status = KEY_UP; // 更改状态为按键未被按下
        //         }
        //     }
        //     break;
        //     case KEY_HOLD: // 按键被持续按下
        //     {
        //         if (key[i].key_status == UP) // 如果按键已经释放
        //         {
        //             key[i].judge_status = KEY_UP; // 更改状态为按键未被按下
        //             if (key[i].hold_time < 70)    // 如果按键按下的时间小于700ms
        //             {
        //                 key[i].single_flag = 1; // 设置短按标志
        //             }
        //             key[i].hold_time = 0; // 重置按键按下的时间
        //         }
        //         else // 如果按键仍然被按下
        //         {
        //             key[i].hold_time++;         // 按键按下的时间加一
        //             if (key[i].hold_time >= 70) // 如果按键按下的时间超过700ms
        //             {
        //                 key[i].long_flag = 1; // 设置长按标志
        //             }
        //         }
        //     }
        //     break;

        //     default:
        //         break;
        //     }
        // }
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) // 如果这个中断是由定时器2触发的
    {
        // 如果这个中断是由通道1触发的
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            // 读取通道1的捕获值
            pwm_capture[0].ccr1_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            // 读取通道2的捕获值
            pwm_capture[0].high_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
            // 将定时器的计数器重置为0
            __HAL_TIM_SetCounter(htim, 0);
            // 计算PWM信号的频率，假设定时器的时钟频率为80MHz，预分频器的分频系数为80
            pwm_capture[0].frq = 80000000 / 80 / (pwm_capture[0].ccr1_val + 1);
            // 计算PWM信号的占空比
            pwm_capture[0].duty = (float)(pwm_capture[0].high_val + 1) / (pwm_capture[0].ccr1_val + 1);
            // 重新启动通道1的输入捕获中断
            HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
            // 重新启动通道2的输入捕获中断
            HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_2);
        }
    }
    if (htim->Instance == TIM3)
    {
        // 直接输入通道
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            pwm_capture[1].ccr1_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            pwm_capture[1].high_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
            __HAL_TIM_SetCounter(htim, 0);
            pwm_capture[1].frq = 80000000 / 80 / (pwm_capture[1].ccr1_val + 1);
            pwm_capture[1].duty = (float)(pwm_capture[1].high_val + 1) / (pwm_capture[1].ccr1_val + 1);
            HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_2);
        }
    }
}

// int8_t rx_data[30];
// uint8_t rx_buffer;
// uint8_t rx_ptr;

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     rx_data[rx_ptr++] = rx_buffer;
//     HAL_UART_Receive_IT(&huart1, &rx_buffer, 1);
// }
