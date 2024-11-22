/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-22 11:37:23
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-22 19:11:22
 * @FilePath: \指纹蓝牙门锁\HARDWARE\sr04.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __SR04_H__
#define __SR04_H__
#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"

#define SR04_TRIG PGout(9)
#define SR04_TRIG_PORT GPIOG
#define SR04_TRIG_GPIO_OUT GPIO_Pin_9

#define SR04_ECHO PAin(8)
#define SR04_ECHO_PORT GPIOA
#define SR04_ECHO_GPIO_IN GPIO_Pin_8


void sr04_Init(void);
u32 get_sr04_distance(void);

#endif
