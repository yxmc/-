/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-29 16:46:24
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-22 15:39:30
 * @FilePath: \ZHAO-Watchd:\ZM\GEC\share\code\summerVacation\STM32F4\指纹蓝牙门锁\HARDWARE\timer.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"
#include "led.h"

extern char buf[5];

extern u8 distance_flag;
extern u8 temperature_flag;

void timer1_Init(u16 arr, u16 psc);

void timer2_Init(u16 arr, u16 psc);

void timer3_Init(u16 arr, u16 psc);

void timer6_Init(u16 arr, u16 psc);

void timer7_Init(u16 arr, u16 psc);

void timer8_Init(u16 arr, u16 psc);




#endif
