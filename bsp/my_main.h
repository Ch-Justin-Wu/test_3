#ifndef __MY_MAIN_H
#define __MY_MAIN_H

#include "main.h"

typedef struct product
{
    float volt;
    float s_low;
    float s_high;
    
    float pass;

    uint8_t tim_cnt;
    uint8_t tim_flag;

    uint16_t pass_num;
    uint16_t total;
} product;

enum set_flag
{
    r37_h = 0,
    r37_l,
    r38_h,
    r38_l
};

void loop(void);
void setup(void);

#endif /* __MY_MAIN_H */
