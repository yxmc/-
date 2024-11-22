/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-24 17:39:48
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-09-26 16:05:00
 * @FilePath: \中断_练习\HARDWARE\led.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"


#define  LED1  PFout(9)   
#define  LED2  PFout(10) 
#define  LED3  PEout(13)
#define  LED4  PEout(14)


void LED_Init(void);
void LED_Blink(u32 time);



#endif
