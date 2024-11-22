/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-08 09:42:53
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-08 09:51:10
 * @FilePath: \倒车雷达\HARDWARE\DHT11.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"


void DHT11_Init(void);

int8_t  DHT11_read_data(char* buf);

#endif
