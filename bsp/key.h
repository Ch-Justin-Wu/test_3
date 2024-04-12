#ifndef __KEY_H
#define __KEY_H

#include "main.h"

typedef struct key_t
{
    uint8_t age;
    uint8_t short_flag;
    uint8_t long_flag;
    uint8_t press;

    uint8_t double_age_EN;
    uint8_t double_age;
    uint8_t double_flag;
} key_t;

void key_serv(void);
void key_serv_long(void);
void key_serv_double(void);

#endif /* __KEY_H */
