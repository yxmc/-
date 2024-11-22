/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-26 15:36:11
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-09-26 15:55:29
 * @FilePath: \中断_练习\HARDWARE\beep.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"

#define  BEEP  PFout(8)

void BEEP_Init(void);

void BEEP_Tone(uint16_t freq);

void BEEP_Off(void);

#endif
