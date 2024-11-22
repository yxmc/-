/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-29 16:46:24
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-14 11:18:05
 * @FilePath: \ADC\HARDWARE\pwm.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __PWM_H__
#define __PWM_H__
#include "stm32f4xx.h"
#include "SysTick.h"
#include "system.h"



void PWM1_Init(void);

void PWM14_Init(void);

void set_compare(void);

void PWM13_Init(void);

void tone(u32 cont, u32 cmp);


#endif
