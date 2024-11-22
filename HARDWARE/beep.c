/*
  * @Author: yxmc 2737647040@qq.com
  * @Date: 2024-09-24 18:51:40
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-09-27 16:14:45
 * @FilePath: \矩阵键盘\HARDWARE\beep.c
  * @Description:PF8蜂鸣器驱动
  *
  * Copyright (c) 2024 by yxmc, All Rights Reserved.
  */

#include "beep.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>


void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    //配置PF8为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    BEEP = 0;
    //关闭蜂鸣器
    // BEEP_Off();

}


/*	@brief  函数功能 : 蜂鸣器发出声音
    @param  freq
            频率 单位Hz
    @return 返回值
    @property : 说明
*/
void BEEP_Tone(uint16_t freq)
{

}


void BEEP_Off(void)
{

}
