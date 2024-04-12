 #ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#include "key.h"

// ö�ٰ����ж�״̬
// 0: �����ɿ�
// 1: �������� ����
// 2: �������²�����һ��ʱ��
enum enum_judge_status
{
    KEY_UP = 0,
    KEY_DOWN = 1,
    KEY_HOLD = 2
};

enum enum_key_status
{
    DOWN = 0,
    UP = 1
};

struct keys
{
    uint8_t judge_status; // �жϽ��е���һ��
    bool key_status;      // ��������Ϊ0���ɿ�Ϊ1
    bool single_flag;      // �̰���Ϊ1
    bool long_flag;       // ������Ϊ1 (700ms)
    bool double_flag;     // ˫����־
    uint16_t last_press_time; // ��һ�ΰ�����ʱ��
    uint16_t hold_time;   // �������µ�ʱ��
};

struct pwm_capture
{
    uint32_t high_val;
    uint32_t ccr1_val;
    uint32_t frq;
    float duty;
    
    
};

struct car
{
    int8_t type[5];
    int8_t data[5];
    int8_t time[13];

};

extern int8_t rx_data[30];
extern uint8_t rx_buffer;
extern uint8_t rx_ptr;
extern struct keys key[4];
extern struct pwm_capture pwm_capture[2];

#endif // __INTERRUPT_H
