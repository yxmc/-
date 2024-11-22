/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-24 17:39:33
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-22 20:30:47
 * @FilePath: \指纹蓝牙门锁\SYSTEM\usart.c
 * @Description: STM32F407ZET6 USART串口初始化及发送接收函数
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "usart.h"  
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_usart.h>
#include <misc.h>
#include "stdio.h"
#include <stm32f4xx_rcc.h>
#include "timer.h"


char g_usart1_recvbuf[100];
u32 g_usart1_recvflag;
u32 usart1_recvlen;

char g_usart2_recvbuf[100];
u32 g_usart2_recvflag;
u32 usart2_recvlen;

char g_usart3_recvbuf[100];
u32 g_usart3_recvflag;
u32 usart3_recvlen;


u16 USART3_RX_STA = 0;
u8 USART3_RX_BUF[100];


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{
    x = x;
}
#endif

//重写fputc函数,使得printf函数可以正常输出到串口
int fputc(int ch, FILE* fp)
{
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}


/*	@brief  函数功能 串口1初始化
    @param  baud
            波特率
    @return 返回值
    @property : 说明
*/
void usart1_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //使能GPIOA的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //使能串口1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //连接引脚和串口,目的是告诉CPU用的哪个复用功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);


    //配置引脚PA9->Tx  PA10->Rx  复用功能模式为USART1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置串口参数
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);

    //使能串口1的接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    //配置串口1的NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //开启串口1
    USART_Cmd(USART1, ENABLE);
}


/*	@brief  函数功能 串口2初始化
    @param  baud
            波特率
    @return 返回值
    @property : 说明
*/
void usart2_Init(u32 baud)
{

    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能GPIOA的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //使能串口2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    //连接引脚和串口,目的是告诉CPU用的哪个复用功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    //配置引脚PA9->Tx  PA10->Rx  复用功能模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //复用功能模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//用于蓝牙的STATE引脚，用于判断是否连接蓝牙
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOB,&GPIO_InitStructure); 
	

    //配置串口参数
    USART_InitStructure.USART_BaudRate = baud;  //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;  //无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送/接收模式
    USART_Init(USART2, &USART_InitStructure);

    //允许触发中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //使能工作
    USART_Cmd(USART2, ENABLE);
}

/*	@brief  函数功能 串口3初始化
    @param  baud
            波特率
    @return 返回值
    @property : 说明
*/
void usart3_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //使能GPIOB的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //使能串口3的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //连接引脚和串口,目的是告诉CPU用的哪个复用功能
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    //配置引脚PC10->Tx  PC11->Rx  复用功能模式为USART3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //设置GPIO速度为100Mhz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //配置串口参数
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;  //无校验位
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //发送/接收模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
    USART_Init(USART3, &USART_InitStructure);

    //使能串口3的接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    //配置串口3的NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //设置优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //开启串口3
    USART_Cmd(USART3, ENABLE);


    timer6_Init(1000, 8400);		//100ms中断
    USART3_RX_STA = 0;		        //清零
    TIM_Cmd(TIM6, DISABLE);         //关闭定时器6
}


/*	@brief  函数功能 串口发送字符串
    @param  USARTx
            串口号
    @param  str
            要发送的字符串
    @return 返回值
    @property : 说明
*/
void usart_sendstr(USART_TypeDef* USARTx, char* str)   //hello
{
    while (str && *str) //str != NULL &&  *str != '\0'
    {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

/*	@brief  函数功能 串口1接收中断服务函数
    @param  无
    @return 返回值
    @property : 说明
*/
void USART1_IRQHandler(void)
{
    u16 data;
    //判断是否触发中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        //执行中断任务
        data = USART_ReceiveData(USART1);  //接收数据

        //将数据保存到数组中
        g_usart1_recvbuf[usart1_recvlen++] = data;
        if (data == '#' || usart1_recvlen >= 100)   //如果收到#,或者数据长度>=100,就表示数据接收完毕
        {
            g_usart1_recvbuf[usart1_recvlen - 1] = '\0';  //将#变成\0,将数据变成字符串
            usart1_recvlen = 0;     //长度清零
            g_usart1_recvflag = 1;  //数据完成标志
        }



        // 清空中断标志位
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/*	@brief  函数功能 串口2接收中断服务函数
    @param  无
    @return 返回值
    @property : 说明
*/
void USART2_IRQHandler(void)
{

    u16 data;
    //判断是否触发中断
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        // 执行中断任务
        data = USART_ReceiveData(USART2);  //接收数据

        //将数据保存到数组中
        g_usart2_recvbuf[usart2_recvlen++] = data;
        if (data == '#' || usart2_recvlen >= 100)   //如果收到#,或者数据长度>=100,就表示数据接收完毕
        {
            g_usart2_recvbuf[usart2_recvlen - 1] = '\0';  //将#变成\0,将数据变成字符串
            usart2_recvlen = 0;     //长度清零
            g_usart2_recvflag = 1;  //数据完成标志
        }



        // 清空中断标志位
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}




// /*	@brief  函数功能 串口3接收中断服务函数
//     @param  无
//     @return 返回值
//     @property : 说明
// */
// void USART3_IRQHandler(void)
// {

//     u16 data;
//     //判断是否触发中断
//     if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//     {
//         //执行中断任务
//         data = USART_ReceiveData(USART3);  //接收数据

//         //将数据保存到数组中
//         g_usart3_recvbuf[usart3_recvlen++] = data;
//         if (data == '#' || usart3_recvlen >= 100)   //如果收到#,或者数据长度>=100,就表示数据接收完毕
//         {
//             g_usart3_recvbuf[usart3_recvlen - 1] = '\0';  //将#变成\0,将数据变成字符串
//             usart3_recvlen = 0;     //长度清零
//             g_usart3_recvflag = 1;  //数据完成标志
//         }



//         // 清空中断标志位
//         USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//     }

// }


//通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//USART3_RX_STA[15]:0,没有接收到数据;1,接收到了一批数据.
//USART3_RX_STA[14:0]:接收到的数据长度
void USART3_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 
        res = USART_ReceiveData(USART3);		
        if((USART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
        { 		
            if(USART3_RX_STA<100)		    //还可以接收数据
            {
                TIM_SetCounter(TIM2,0);     //计数器清空        				 
                if(USART3_RX_STA == 0)		
                    TIM_Cmd(TIM6, ENABLE);  //使能定时器6
                USART3_RX_BUF[USART3_RX_STA++]=res;		//记录接收到的值	 
            }
            else 
            {
                USART3_RX_STA|=1<<15;					//强制标记接收完成
            }
        }  	
    }										 
} 
