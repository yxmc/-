/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-15 14:54:53
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-15 19:04:17
 * @FilePath: \SPI\SYSTEM\SysTick.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "SysTick.h"

 //选择系统定时器时钟源
void StsTick_init(void)
{
    //滴答定时器的时钟源 = 168MHz/8=21MHz:1s计数21M次,最大延时时间能够达到798ms
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//毫秒级延时
void delay_ms(u32 ms)
{
    u32 tmp;
    while (ms--)
    {
        //设置计数值,从设置的数开始递减
        SysTick->LOAD = SystemCoreClock / 1000 / 8;     //延时1ms
        //清零当前值寄存器 计数器清零
        SysTick->VAL = 0;
        //开启滴答定时器
        // SysTick->CTRL |= 0x1;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //同上
        do
        {
            tmp = SysTick->CTRL;
        } while ((tmp & 0x1) && !(tmp & (0x1 << 16)));  //等待时间到达
        //清零当前值寄存器 关闭计数器
        SysTick->VAL = 0;
        //关闭滴答定时器
        // SysTick->CTRL &= ~0x1;
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //同上
    }
}

//微秒级延时
void delay_us(u32 us)
{
    u32 tmp;
    while (us--)
    {
        //设置计数值,从设置的数开始递减
        SysTick->LOAD = SystemCoreClock / 1000000 / 8;     //延时1us
        //清零当前值寄存器
        SysTick->VAL = 0;
        //开启滴答定时器
        SysTick->CTRL |= 0x1;
        do
        {
            tmp = SysTick->CTRL;
        } while ((tmp & 0x1) && !(tmp & (0x1 << 16)));  //等待时间到达
        //清零当前值寄存器
        SysTick->VAL = 0;
        //关闭滴答定时器
        SysTick->CTRL &= ~0x1;
    }
}
