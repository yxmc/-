#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f4xx.h"
#include <misc.h>              

void StsTick_init(void);
void delay_ms(u32 ms);
void delay_us(u32 us);

#endif
