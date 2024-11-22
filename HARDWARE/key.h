/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-26 11:28:56
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-09-26 17:41:08
 * @FilePath: \Timer\HARDWARE\key.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"
#include "led.h"


#define KEY1 PAin(0)
#define KEY2 PEin(2)
#define KEY3 PEin(3)
#define KEY4 PEin(4)

#define KEY1_PRESSED 0x01
#define KEY2_PRESSED 0x02
#define KEY3_PRESSED 0x04
#define KEY4_PRESSED 0x08


extern u8 key_board_flag;

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
void EXTI_KEY_Init(void);

#endif
