
/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-24 17:39:33
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-14 16:44:37
 * @FilePath: \ADC\HARDWARE\adc.c
 * @Description: ADC初始化
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "adc.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include <stm32f4xx_adc.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>

 // ADC1初始化 IN5口 可调电阻
void adc1_Init(void)
{
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    //开启ADC1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    //开启GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //配置引脚参数(PA5/ADC1_IN5-->可调电阻)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			  //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;   	 //模式选择,模拟模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	  //上下拉电阻
    GPIO_Init(GPIOA, &GPIO_InitStructure);		 //初始化引脚

    //ADC常规配置
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  //独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; //84MHz/8=10.5MHz
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//不适用DMA模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//采样间隔周期
    ADC_CommonInit(&ADC_CommonInitStructure);


    //ADC参数配置
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //分辨率12位(根据实际使用场景选择分辨率)
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;          //不需要扫描转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      //连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要通过外部定时器触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;  //选择常规组
    ADC_Init(ADC1, &ADC_InitStructure);

    //选择通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

    //使能ADC1
    ADC_Cmd(ADC1, ENABLE);

    //清空标志位
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

// ADC3初始化 IN5口 光敏传感器
void adc3_Init(void)
{
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    //开启ADC3的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
    //开启GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    //配置引脚参数(PC0/ADC3_IN5-->可调电阻)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			  //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;   	 //模式选择,模拟模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	  //上下拉电阻
    GPIO_Init(GPIOC, &GPIO_InitStructure);		 //初始化引脚

    //ADC常规配置
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; //84MHz/8=10.5MHz
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//不适用DMA模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//采样间隔周期
    ADC_CommonInit(&ADC_CommonInitStructure);

    //ADC参数配置
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //分辨率12位(根据实际使用场景选择分辨率)
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;          //不需要扫描转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      //连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要通过外部定时器触发          
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;  //选择常规组
    ADC_Init(ADC1, &ADC_InitStructure);
    //选择通道
    ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);

    //使能ADC3
    ADC_Cmd(ADC3, ENABLE);

    //清空标志位
    ADC_ClearFlag(ADC3, ADC_FLAG_EOC);
}

// ADC2初始化 IN4口 烟雾传感器
void adc1_fumes_Init(void)
{
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;

    //开启ADC1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    //开启GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

    //配置引脚参数(PA4/ADC1_IN4-->烟雾传感器AO引脚)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			 //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;   	     //模式选择,模拟模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	 //上下拉电阻
    GPIO_Init(GPIOA, &GPIO_InitStructure);		         //初始化引脚

    //配置引脚参数(PB7-->烟雾传感器DO引脚)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		    //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   	    //模式选择,模拟模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	    //上下拉电阻
    GPIO_Init(GPIOB, &GPIO_InitStructure);		        //初始化引脚

    //ADC常规配置
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  //独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; //84MHz/8=10.5MHz
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//不适用DMA模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//采样间隔周期
    ADC_CommonInit(&ADC_CommonInitStructure);

    //ADC参数配置
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //分辨率12位(根据实际使用场景选择分辨率)
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;          //不需要扫描转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      //连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要通过外部定时器触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;  //选择常规组
    ADC_Init(ADC1, &ADC_InitStructure);

    //选择通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);

    //使能ADC1
    ADC_Cmd(ADC1, ENABLE);
}

