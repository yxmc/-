/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-14 09:49:13
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-14 11:34:37
 * @FilePath: \ADC\HARDWARE\flash.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f4xx.h"

#define  ADDR_FLASH_SECTOR_0   ((uint32_t)0x08000000) 
#define  ADDR_FLASH_SECTOR_1   ((uint32_t)0x08004000) 
#define  ADDR_FLASH_SECTOR_2   ((uint32_t)0x08008000) 
#define  ADDR_FLASH_SECTOR_3   ((uint32_t)0x0800C000) 
#define  ADDR_FLASH_SECTOR_4   ((uint32_t)0x08010000) 
#define  ADDR_FLASH_SECTOR_5   ((uint32_t)0x08020000) 
#define  ADDR_FLASH_SECTOR_6   ((uint32_t)0x08040000) 
#define  ADDR_FLASH_SECTOR_7   ((uint32_t)0x08060000) 


void flash_Init(void);
int32_t write_flash(char *temp,u32 count);
void read_flash(char *temp,u32 count);
void clear_flash(void);

#endif
