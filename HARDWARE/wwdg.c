/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-08 16:20:41
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-08 16:27:44
 * @FilePath: \HARDWARE\wwdg.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "wwdg.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include <misc.h>
#include <stm32f4xx_wwdg.h>
#include <stm32f4xx_rcc.h>

void init_wwdg(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //使能窗口看门狗时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    //设置窗口看门狗的频率 WWDG clock counter = (PCLK1 (42MHz)/4096)/8 = 1281 Hz (~780 us) 
    WWDG_SetPrescaler(WWDG_Prescaler_8);

    //设置窗口值
    WWDG_SetWindowValue(80);

    //使能窗口看门并设置计数值
    WWDG_Enable(127);
    //喂狗时间: ~780 * (127-80) = 36.6ms < refresh window < ~780 * (127-64) = 49.9ms

    //要提前清空中断标志位,否则不会触发中断
    WWDG_ClearFlag();

    //允许触发中断
    WWDG_EnableIT();

    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//中断服务函数
void WWDG_IRQHandler(void)
{
    //判断是否触发中断
    if (WWDG_GetFlagStatus() == SET)
    {
        //清空中断标志位
        WWDG_ClearFlag();
        //喂狗
        WWDG_SetCounter(127);
    }
}
