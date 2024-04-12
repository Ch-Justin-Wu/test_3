#include "key.h"

struct key_t bkey[5];

uint8_t key_read(void)
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0)
        return 1;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
        return 2;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == 0)
        return 3;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
        return 4;
    else
        return 0;
}

void key_serv(void)
{
    uint8_t key_sta = key_read();
    if (key_sta != 0)
    {
        bkey[key_sta].age++;
        if (bkey[key_sta].age > 1)
        {
            bkey[key_sta].short_flag = 1;
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            bkey[i].age = 0;
        }
    }
}

// ������סʱִ��
void key_serv_long(void)
{
    uint8_t key_sta = key_read();
    if (key_sta != 0)
    {
        bkey[key_sta].age++;
        if (bkey[key_sta].age == 2)
        {
            bkey[key_sta].press = 1;
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (bkey[i].press == 1 && bkey[i].long_flag == 0)
            {
                bkey[i].short_flag = 1;
            }

            bkey[i].press = 0;
            bkey[i].age = 0;
            bkey[i].long_flag = 0;
        }
    }
    if (bkey[key_sta].age > 69)
    {
        bkey[key_sta].long_flag = 1;
    }
}

// ����̧���ִ��
void key_serv_long_(void)
{
    uint8_t key_sta = key_read();
    if (key_sta != 0)
    {
        bkey[key_sta].age++;
        if (bkey[key_sta].age == 2)
        {
            bkey[key_sta].press = 1;
        }
    }
    else
    {
        if (bkey[key_sta].age > 69)
        {
            bkey[key_sta].long_flag = 1;
        }
        for (int i = 0; i < 5; i++)
        {
            if (bkey[i].press == 1 && bkey[i].long_flag == 0)
            {
                bkey[i].short_flag = 1;
            }

            bkey[i].press = 0;
            bkey[i].age = 0;
        }
    }
}

void key_serv_double(void)
{
    uint8_t key_sta = key_read();
    if (key_sta != 0)
    {
        bkey[key_sta].age++;
        if (bkey[key_sta].age == 2)
        {
            bkey[key_sta].press = 1;
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (bkey[i].double_age_EN == 1 && bkey[i].press == 1)
            {
                bkey[i].double_flag = 1;
                bkey[i].double_age_EN = 0;
                bkey[i].press = 0;
            }
            if (bkey[i].press==1 && bkey[i].long_flag == 0)
            {
                bkey[i].double_age_EN = 1;
            }
            if (bkey[i].double_age_EN == 1)
            {
                bkey[i].double_age++;
            }
            if (bkey[i].double_age_EN == 1 && bkey[i].double_age > 20)
            {
                bkey[i].short_flag = 1;
                bkey[i].double_age_EN = 0;
                bkey[i].double_age = 0;
            }
            bkey[i].press = 0;
            bkey[i].age = 0;
            bkey[i].long_flag = 0;
        }
    }
    if (bkey[key_sta].age > 69)
    {
        bkey[key_sta].long_flag = 1;
    }
}

uint8_t my_key_read()
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) // B1
    {
        return 1;
    }
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) // B2
    {
        return 2;
    }
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) // B3
    {
        return 3;
    }
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0) // B4
    {
        return 4;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief �������¼��ĺ���
 * 
 * �ú������ڴ������¼������ݰ���״̬������Ӧ�Ĳ�����
 * 
 * @note �ú�������°�����״̬�ͱ�־λ��
 */
void my_key_serv()
{
    uint8_t key_sta = my_key_read(); // ��ȡ����״̬
    if (key_sta != 0)
    {
        bkey[key_sta].age++; // �������£����Ӱ���������
        if (bkey[key_sta].age == 2)
        {
            bkey[key_sta].press = 1; // ��������2�����ڣ����ð������±�־λ
        }
    }
    else
    {
        if (bkey[key_sta].age > 69)
        {
            bkey[key_sta].long_flag = 1; // ������������69�����ڣ����ð���������־λ
        }
        for (int i = 0; i < 5; i++)
        {
            if (bkey[i].press == 1 && bkey[i].long_flag == 0)
            {
                bkey[i].short_flag = 1; // �����̰������ð����̰���־λ
            }
            bkey[i].age = 0; // ���ð�������
            bkey[i].press = 0; // ���ð������±�־λ
        }
    }
}
