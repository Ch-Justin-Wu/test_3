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
        // // ��ȡ�ĸ�������״̬
        // key[0].key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
        // key[1].key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
        // key[2].key_status = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
        // key[3].key_status = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        // // ��ÿ������������������
        // for (int i = 0; i < 4; i++)
        // {
        //     switch (key[i].judge_status)
        //     {
        //     case KEY_UP: // ����δ������
        //     {
        //         if (key[i].key_status == DOWN) // �������������
        //         {
        //             key[i].judge_status = KEY_DOWN; // ����״̬Ϊ�����ѱ�����
        //         }
        //     }
        //     break;
        //     case KEY_DOWN: // �����ѱ�����
        //     {
        //         if (key[i].key_status == DOWN) // ���������Ȼ������
        //         {

        //             key[i].judge_status = KEY_HOLD; // ����״̬Ϊ��������������
        //         }
        //         else // ��������Ѿ��ͷ�
        //         {
        //             key[i].judge_status = KEY_UP; // ����״̬Ϊ����δ������
        //         }
        //     }
        //     break;
        //     case KEY_HOLD: // ��������������
        //     {
        //         if (key[i].key_status == UP) // ��������Ѿ��ͷ�
        //         {
        //             key[i].judge_status = KEY_UP; // ����״̬Ϊ����δ������
        //             if (key[i].hold_time < 70)    // ����������µ�ʱ��С��700ms
        //             {
        //                 key[i].single_flag = 1; // ���ö̰���־
        //             }
        //             key[i].hold_time = 0; // ���ð������µ�ʱ��
        //         }
        //         else // ���������Ȼ������
        //         {
        //             key[i].hold_time++;         // �������µ�ʱ���һ
        //             if (key[i].hold_time >= 70) // ����������µ�ʱ�䳬��700ms
        //             {
        //                 key[i].long_flag = 1; // ���ó�����־
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
    if (htim->Instance == TIM2)
    {
        // ֱ������ͨ��
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            pwm_capture[0].ccr1_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            pwm_capture[0].high_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
            __HAL_TIM_SetCounter(htim, 0);
            pwm_capture[0].frq = 80000000 / 80 / (pwm_capture[0].ccr1_val + 1);
            pwm_capture[0].duty = (float)(pwm_capture[0].high_val + 1) / (pwm_capture[0].ccr1_val + 1);
            HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_2);
        }
    }
    if (htim->Instance == TIM3)
    {
        // ֱ������ͨ��
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
