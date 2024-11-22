/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-24 17:39:33
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-17 11:13:29
 * @FilePath: \IIC\HARDWARE\led.c
 * @Description: PF9、PF10、PE13、PE14的LED驱动代码
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "led.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>


void LED_Init(void)
{
    //Configure the GPIO pin for LED

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    //灯光初始化
    GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
    GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}


// 流水灯
void LED_Blink(u32 time)
{
    LED1 = 0;
    delay_ms(time);
    LED1 = 1;
    delay_ms(time);
    LED2 = 0;
    delay_ms(time);
    LED2 = 1;
    delay_ms(time);
    LED3 = 0;
    delay_ms(time);
    LED3 = 1;
    delay_ms(time);
    LED4 = 0;
    delay_ms(time);
    LED4 = 1;
    delay_ms(time);
}

