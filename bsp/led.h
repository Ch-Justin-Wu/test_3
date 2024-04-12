#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

    void led_disp(uint8_t ds_led);
    void led_chg(uint8_t led_sta, uint8_t num, uint8_t sta);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
