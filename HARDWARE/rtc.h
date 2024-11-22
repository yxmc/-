/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-09 19:47:40
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-10 15:09:28
 * @FilePath: \RTC唤醒中断\HARDWARE\rtc.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef _RTC_H
#define _RTC_H

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"
#include "stdio.h"
#include "string.h"
 // #include "usart.h" 

extern u8 g_rtc_wkp_flag;
extern u8 g_rtc_alarmA_flag;

void rtc_Init(void);

void rtc_get_datatime(char* buf);

void rtc_set_datatime(char* buf, u8 flag);

void alarmA_Init(void);

void alarmB_Init(void);

void set_alarm(char* buf);

u32 getBCD(const char* str);

#endif
