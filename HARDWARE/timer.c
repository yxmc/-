/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-26 17:20:57
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-22 16:34:48
 * @FilePath: \ZHAO-Watchd:\ZM\GEC\share\code\summerVacation\STM32F4\指纹蓝牙门锁\HARDWARE\timer.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "timer.h"
#include "OLED.h"
#include <stm32f4xx_tim.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx.h> 
#include "dht11.h"
#include "stdio.h"
#include "sr04.h"
#include "usart.h"

char buf[5] = { 0 };

u8 distance_flag = 0;
u8 temperature_flag = 0;

void timer1_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //使能定时器1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    //配置时间参数(决定定时时间)
    //定时时间 = (1+TIM_Period)*(1+TIM_Prescaler)/定时器的时钟频率(168MHz)
    TIM_TimeBaseStructure.TIM_Period = arr - 1;     //重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //允许定时器触发中断(溢出/刷新中断)
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    //使能定时器1工作
    TIM_Cmd(TIM1, ENABLE);
    // TIM_Cmd(TIM1, DISABLE);

    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void timer2_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //使能定时器2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //配置时间参数(决定定时时间)
    //定时时间 = (1+TIM_Period)*(1+TIM_Prescaler)/定时器的时钟频率(84MHz)
    TIM_TimeBaseStructure.TIM_Period = arr - 1;     //重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //允许定时器触发中断(溢出/刷新中断)
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //使能定时器2工作
    TIM_Cmd(TIM2, ENABLE);

    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void timer3_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //使能定时器3的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //配置时间参数(决定定时时间)
    //定时时间 = (1+TIM_Period)*(1+TIM_Prescaler)/定时器的时钟频率(84MHz)
    TIM_TimeBaseStructure.TIM_Period = arr - 1;     //重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    //允许定时器触发中断(溢出/刷新中断)
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    //使能定时器3工作
    TIM_Cmd(TIM3, ENABLE);

    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void timer8_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //使能定时器8的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    //配置时间参数(决定定时时间)
    //定时时间 = (1+TIM_Period)*(1+TIM_Prescaler)/定时器的时钟频率(168MHz)
    TIM_TimeBaseStructure.TIM_Period = arr - 1;     //重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    //允许定时器触发中断(溢出/刷新中断)
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

    //使能定时器8工作
    TIM_Cmd(TIM8, ENABLE);


    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 基本定时器6
void timer6_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //使能定时器6的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    //配置时间参数(决定定时时间)
    //定时时间 = (1+TIM_Period)*(1+TIM_Prescaler)/定时器的时钟频率(84MHz)
    TIM_TimeBaseStructure.TIM_Period = arr - 1;     //重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    //允许定时器触发中断(溢出/刷新中断)
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    //使能定时器6工作
    TIM_Cmd(TIM6, ENABLE);

    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

// 基本定时器7
void timer7_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //使能定时器7的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    //配置时间参数(决定定时时间)
    //定时时间 = (1+TIM_Period)*(1+TIM_Prescaler)/定时器的时钟频率(84MHz)
    TIM_TimeBaseStructure.TIM_Period = arr - 1;     //重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    //允许定时器触发中断(溢出/刷新中断)
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

    //使能定时器7工作
    TIM_Cmd(TIM7, ENABLE);

    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    //判断是否触发中断
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        //执行中断任务
        LED1 = !LED1;

        //清空中断标志位
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

//中断服务函数
void TIM2_IRQHandler(void)
{
    //判断是否触发中断
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        //执行中断任务    
        OLED_Display_Off();
        TIM_Cmd(TIM2, DISABLE);
        temperature_flag = 1;
        //清空中断标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

//中断服务函数
void TIM3_IRQHandler(void)
{
    //判断是否触发中断
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        //执行中断任务
        LED3 ^= 1;


        //清空中断标志位
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

//中断服务函数
void TIM6_DAC_IRQHandler(void)
{
    //判断是否触发中断
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
    {
        //执行中断任务
        USART3_RX_STA |= 1 << 15;	//标记接收完成     1000 0000 0000 0000
        TIM_Cmd(TIM6, DISABLE);  //关闭TIM6

        //清空中断标志位
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

//中断服务函数
void TIM7_IRQHandler(void)
{
    //判断是否触发中断
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
    {
        static u8 count = 0;
        //执行中断任务
        LED1 = !LED1;
        if (++count == 2)
        {
            count = 0;
            distance_flag = 1;
        }



        //清空中断标志位
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}


//中断服务函数
void TIM8_UP_TIM13_IRQHandler(void)
{
    //判断是否触发中断
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)
    {
        //执行中断任务
        LED4 ^= 1;

        //清空中断标志位
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}

