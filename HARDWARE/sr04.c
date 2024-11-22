/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-08 09:48:05
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-22 20:05:33
 * @FilePath: \指纹蓝牙门锁\HARDWARE\sr04.c
 * @Description: 超声波SR04驱动程序
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "sr04.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

void sr04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOA, ENABLE);


	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = SR04_TRIG_GPIO_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;

	GPIO_Init(SR04_TRIG_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = SR04_ECHO_GPIO_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;

	GPIO_Init(SR04_ECHO_PORT, &GPIO_InitStruct);


	//初始电平状态,这个具体要看要连接的硬件模块时序图.
	SR04_TRIG = 0;

}

// 返回值 单位: mm
uint32_t get_sr04_distance(void)
{
	uint32_t t = 0;

	SR04_TRIG = 1;
	delay_us(15);
	SR04_TRIG = 0;

	//等待电平拉高
	while (SR04_ECHO == 0)
		;

	//测量引脚高电平的持续时间
	while (SR04_ECHO)
	{
		t++;
		delay_us(9);	//有多少个9us,就是有多少个3mm
	}

	return t * 3 / 2;
}

