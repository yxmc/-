/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-17 14:58:04
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-17 19:31:14
 * @FilePath: \IIC\HARDWARE\at24cxx.h
 * @Description: 
 * 
 * Copyright (c) 2024 by yxmc, All Rights Reserved. 
 */
#ifndef __AT24CXX_H__
#define __AT24CXX_H__
#include "stm32f4xx.h"

void at24cxx_write_data(u8* data, u8 addr, u8 len);
void at24cxx_read_data(u8* data, u8 addr, int32_t len);

#endif
