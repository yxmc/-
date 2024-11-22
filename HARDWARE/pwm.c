/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-28 17:25:36
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-14 14:43:42
 * @FilePath: \ADC\HARDWARE\pwm.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "pwm.h"
#include <stm32f4xx_tim.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>


void PWM1_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    //使能定时器1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;	//指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   	    //模式选择,复用功能模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	  //上下拉电阻
    GPIO_Init(GPIOE, &GPIO_InitStructure);		 //初始化PF13、14引脚	

    //连接引脚与复用功能
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);


    //配置时间参数(决定频率和计数值)
    TIM_TimeBaseStructure.TIM_Period = 10000 / 100;    //频率是100HZ,计数值100
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //PWM模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //允许输出
    TIM_OCInitStructure.TIM_Pulse = 0;  //比较初值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//高电平有效
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    //允许定时器1通道3、4工作4自动重装载
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    //开启定时器1通道1自动重装载
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    //使能定时器1工作
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    //使能定时器1工作
    TIM_Cmd(TIM1, ENABLE);
}

//LED1
void PWM14_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    //使能定时器14的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			  //指定引脚  LED1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   	    //模式选择,复用功能模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	  //上下拉电阻
    GPIO_Init(GPIOF, &GPIO_InitStructure);		 //初始化PF9引脚	

    //连接引脚与复用功能
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);

    //配置时间参数(决定频率和计数值)
    TIM_TimeBaseStructure.TIM_Period = 10000 / 100;    //频率是100HZ,计数值100
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //PWM模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //允许输出
    TIM_OCInitStructure.TIM_Pulse = 0;  //比较初值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//高电平有效
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);

    //允许定时器14通道1自动重装载
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

    //开启定时器14通道1自动重装载
    TIM_ARRPreloadConfig(TIM14, ENABLE);

    //使能定时器14工作
    TIM_Cmd(TIM14, ENABLE);
}

void set_compare(void)
{
    for (int cmp = 0;cmp <= 100;cmp++)
    {
        TIM_SetCompare3(TIM1, cmp);
        TIM_SetCompare4(TIM1, cmp);
        TIM_SetCompare1(TIM14, cmp);
        delay_ms(20);
    }
    for (int cmp = 100;cmp >= 0;cmp--)
    {
        TIM_SetCompare3(TIM1, cmp);
        TIM_SetCompare4(TIM1, cmp);
        TIM_SetCompare1(TIM14, cmp);
        delay_ms(20);
    }
}

void PWM13_Init(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    //使能定时器13的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			  //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   	    //模式选择,复用功能模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	  //上下拉电阻
    GPIO_Init(GPIOF, &GPIO_InitStructure);		 //初始化PF8引脚	

    //连接引脚与复用功能
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);

    //配置时间参数(决定频率和计数值)
    TIM_TimeBaseStructure.TIM_Period = 10000 / 100;    //频率是100HZ,计数值100
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;  //预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //二次分频(407不支持)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //PWM模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //允许输出
    TIM_OCInitStructure.TIM_Pulse = 0;  //比较初值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//高电平有效
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);

    //允许定时器13通道1自动重装载
    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
    //开启定时器13通道1自动重装载
    TIM_ARRPreloadConfig(TIM13, ENABLE);

    //使能定时器13工作
    TIM_Cmd(TIM13, ENABLE);

}

void tone(u32 cont, u32 cmp)
{
    TIM_SetAutoreload(TIM13, cont);
    TIM_SetCompare1(TIM13, cmp);

}
