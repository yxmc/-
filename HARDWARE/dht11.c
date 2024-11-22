/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-29 16:46:34
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-23 15:10:13
 * @FilePath: \指纹蓝牙门锁\HARDWARE\dht11.c
 * @Description: 温湿度DHT11
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "dht11.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>



void DHT11_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //开启GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			  //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   	 //模式选择,输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	  //上下拉电阻
    GPIO_Init(GPIOG, &GPIO_InitStructure);		         //初始化引脚	

    //空闲状态时,PG9引脚为高电平
    PGout(9) = 1;
}

//切换模式
void DHT11_mode(GPIOMode_TypeDef GPIO_Mode)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			  //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;   	 //模式选择,输出模式
    GPIO_Init(GPIOG, &GPIO_InitStructure);		         //初始化引脚	
}

//开始信号
int8_t DHT11_start(void)
{
    DHT11_mode(GPIO_Mode_OUT);

    //PG9输出低电平
    PGout(9) = 0;


    u32 tmp;
    u32 ms = 20;
    while (ms--)
    {
        //设置计数值,从设置的数开始递减
        SysTick->LOAD = SystemCoreClock / 1000 / 8;     //延时1ms
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

    //PG9输出高电平
    PGout(9) = 1;


    u32 us = 30;
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


    DHT11_mode(GPIO_Mode_IN);

    //判断是否有响应   无响应返回-1 
    if (PGin(9) == 0)
    {
        us = 90;
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

        if (PGin(9) == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

}

//读取一个字节(8位)
u8 DHT11_read_byte(void)  //高位先出 MSB
{
    u8 byte = 0;  //0000 0000

    for (int i = 0;i < 8;i++)
    {

        while (PGin(9) == 1);	//等待高电平结束
        while (PGin(9) == 0);	//等待低电平结束

        u32 tmp;
        u32 us = 35;
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


        if (PGin(9) == 1)        //如果获取的数据是1
        {
            byte |= 0x1 << (7 - i);
        }
        else        //如果获取的数据是0
        {
            byte &= ~(0x1 << (7 - i));
        }
    }

    return byte;
}

//读取温湿度数据
int8_t  DHT11_read_data(char* buf)
{
    while (DHT11_start());

    for (int i = 0;i < 5;i++)
    {
        buf[i] = DHT11_read_byte();
    }

    // 判断数据是否有效 (校验和)   有效返回0  无效返回-1
    if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
