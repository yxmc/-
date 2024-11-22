/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-25 11:21:03
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-23 09:09:33
 * @FilePath: \指纹蓝牙门锁\HARDWARE\key.c
 * @Description:A0、E2、E3、E4四个按键的驱动程序
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "key.h"
#include <stm32f4xx_exti.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_syscfg.h>
#include "beep.h"
#include <stm32f4xx_gpio.h>

u8 key_board_flag = 0;

void KEY_Init(void)
{
    // PA0
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PE2 PE3 PE4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*	@brief  函数功能 : 按键扫描函数
    @param  mode
            mode=0  单次扫描
            mode=1  连续扫描
    @return 返回值
    @property : 说明
*/
u8 KEY_Scan(u8 mode)
{
    static u8 key = 1;
    if (key == 1 && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)) //任意一个按键按下
    {
        delay_ms(15);  //消抖
        if (KEY1 == 0)
        {
            key = 0;
            return KEY1_PRESSED;
        }
        else if (KEY2 == 0)
        {
            key = 0;
            return KEY2_PRESSED;
        }
        else if (KEY3 == 0)
        {
            key = 0;
            return KEY3_PRESSED;
        }
        else if (KEY4 == 0)
        {
            key = 0;
            return KEY4_PRESSED;
        }
    }
    else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1)    //无按键按下
    {
        key = 1;
    }
    if (mode == 1) //连续按键按下
    {
        key = 1;
    }
    return 0;
}

/*	@brief  函数功能 : 按键中断初始化函数
    @param
    @param
    @return 返回值
    @property : 说明
*/
void EXTI_KEY_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //使能SYSCFG的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    //告诉CPU是由PA0触发外部中断0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

    //外部中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;  //指定外部中断
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //触发方式(上升、下降、边沿) 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    //NVIC配置,任何有关中断服务函数的硬件都需要设置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //响应优先级(子优先级)
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_Init(&NVIC_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
    // 检测外部中断线0(EXTI_Line0)的中断状态
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        //清除中断标志位
        EXTI_ClearITPendingBit(EXTI_Line0);

        //消抖
        delay_ms(15);
        if (KEY1 == 0)
        {
            key_board_flag = 1;
            BEEP = 1;
            delay_ms(50);
            BEEP = 0;
        }

    }
}

void EXTI2_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2);

        delay_ms(15);
        if (KEY2 == 0)
        {
            key_board_flag = 2;
            // LED3 = 0;

            BEEP = 1;
            delay_ms(50);
            BEEP = 0;
        }

    }
}

void EXTI3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
        delay_ms(15);
        if (KEY3 == 0)
        {
            // key_board_flag = 1;
            // LED3 = 0;
            BEEP = 1;
            delay_ms(50);
            BEEP = 0;
        }
    }
}

void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4);

        delay_ms(15);
        if (KEY4 == 0)
        {
            // key_board_flag = 1;
            // LED3 = 0;
            BEEP = 1;
            delay_ms(50);
            BEEP = 0;
        }
    }
}


