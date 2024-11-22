/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-15 14:55:30
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-19 15:32:40
 * @FilePath: \IIC\HARDWARE\spi.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "spi.h"
#include "stm32f4xx.h"
#include <stm32f4xx_spi.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>


#ifdef SIMULATION //仿真模式

void spi_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    //使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //PB14->CS(输出)  PB5->MOSI(输出) //PB3->CLK(输出)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //空闲时,要将片选引脚拉高
    CS = 1;

    // PB4->MISO(输入)  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

#ifdef MODE0
    //模式0 CPOL = 0,CPHA = 0
    // CPOL 时钟极性
    // CPHA 时钟相位
    CLK = 0;  //CLK在空闲时是低电平

#elif MODE1
    //模式1 CPOL = 0,CPHA = 1
    // CPOL 时钟极性
    // CPHA 时钟相位
    CLK = 0;  //CLK在空闲时是低电平

#elif MODE2
    //模式2 CPOL = 1,CPHA = 0
    // CPOL 时钟极性
    // CPHA 时钟相位
    CLK = 1;  //CLK在空闲时是高电平

#elif MODE3
    //模式3 CPOL = 1,CPHA = 1    
    // CPOL 时钟极性
    // CPHA 时钟相位
    CLK = 1;  //CLK在空闲时是高电平

#endif

}


#ifdef MODE0 

//收发数据
uint8_t spi_send_recv_byte(uint8_t send) //0010 1101
{
    uint8_t recv = 0; //0000 0000
    for (int i = 0; i < 8; i++)  //MSB
    {

        if (send & (0x1 << (7 - i)))  //如果发送的位是1
        {
            MOSI = 1;
        }
        else                    //如果发送的位是0
        {
            MOSI = 0;
        }
        delay_us(2);

        CLK = 1; //第一个跳变沿(上升沿),接收数据
        delay_us(2);
        if (MISO == 1)  //如果MISO引脚是高电平,说明接收的数据是1
        {
            recv |= (0x1 << (7 - i));
        }
        else          //如果MISO引脚是低电平,说明接收的数据是0
        {
            recv &= ~(0x1 << (7 - i));
        }

        CLK = 0;    //第二个跳变沿(下降沿),发送数据
        delay_us(2);
    }

    // for (int i = 7; i >= 0; i--)
    // {
    //     //MSB,最高有效位发送数据
    //     if (send & (1 << i))
    //         MOSI = 1;
    //     else
    //         MOSI = 0;

    //     //设置SCLK引脚为高电平,告诉从机,我开始采集MISO引脚电平
    //     CLK = 1;
    //     delay_us(2);

    //     //主机采集MISO引脚电平
    //     if (MISO)
    //         d |= 1 << i;

    //     //设置SCLK引脚为低电平,这个时候,从机会开始读取MOSI引脚电平
    //     CLK = 0;
    //     delay_us(2);
    // }


    return recv;
}

#elif MODE1

//收发数据
uint8_t spi_send_recv_byte(uint8_t send) //0010 1101
{
    uint8_t recv = 0; //0000 0000
    for (int i = 0; i < 8; i++)  //MSB
    {
        CLK = 0;    //第二个跳变沿(下降沿),发送数据
        delay_us(2);

        if (send & (0x1 << (7 - i)))  //如果发送的位是1
        {
            MOSI = 1;
        }
        else                    //如果发送的位是0
        {
            MOSI = 0;
        }
        delay_us(2);

        CLK = 1; //第一个跳变沿(上升沿),接收数据
        delay_us(2);
        if (MISO == 1)  //如果MISO引脚是高电平,说明接收的数据是1
        {
            recv |= (0x1 << (7 - i));
        }
        else          //如果MISO引脚是低电平,说明接收的数据是0
        {
            recv &= ~(0x1 << (7 - i));
        }


    }

    return recv;
}

#elif MODE2

//收发数据
uint8_t spi_send_recv_byte(uint8_t send) //0010 1101
{
    uint8_t recv = 0; //0000 0000
    for (int i = 0; i < 8; i++)  //MSB
    {
        CLK = 1; //第一个跳变沿(上升沿),接收数据
        delay_us(2);
        if (send & (0x1 << (7 - i)))  //如果发送的位是1
        {
            MOSI = 1;
        }
        else                    //如果发送的位是0
        {
            MOSI = 0;

        }
        delay_us(2);

        CLK = 0;    //第二个跳变沿(下降沿),发送数据
        delay_us(2);
        if (MISO == 1)  //如果MISO引脚是高电平,说明接收的数据是1
        {
            recv |= (0x1 << (7 - i));
        }
        else          //如果MISO引脚是低电平,说明接收的数据是0
        {
            recv &= ~(0x1 << (7 - i));
        }

    }

    return recv;
}

#elif MODE3

//收发数据
uint8_t spi_send_recv_byte(uint8_t send) //0010 1101
{
    uint8_t recv = 0; //0000 0000
    for (int i = 0; i < 8; i++)  //MSB
    {
        CLK = 0;    //第二个跳变沿(下降沿),发送数据
        delay_us(2);

        if (send & (0x1 << (7 - i)))  //如果发送的位是1
        {
            MOSI = 1;
        }
        else                    //如果发送的位是0
        {
            MOSI = 0;
        }
        delay_us(2);

        CLK = 1; //第一个跳变沿(上升沿),接收数据
        delay_us(2);
        if (MISO == 1)  //如果MISO引脚是高电平,说明接收的数据是1
        {
            recv |= (0x1 << (7 - i));
        }
        else          //如果MISO引脚是低电平,说明接收的数据是0
        {
            recv &= ~(0x1 << (7 - i));
        }
    }

    // for (int i = 7; i >= 0; i--)
    // {
    //     //MSB,最高有效位发送数据
    //     if (send & (1 << i))
    //         MOSI = 1;
    //     else
    //         MOSI = 0;

    //     //设置SCLK引脚为低电平,这个时候,从机会开始读取MOSI引脚电平
    //     CLK = 0;
    //     delay_us(2);

    //     //设置SCLK引脚为高电平,告诉从机,主机准备采集MISO电平
    //     CLK = 1;
    //     delay_us(2);

    //     //主机采集MISO引脚电平
    //     if (MISO)
    //         d |= 1 << i;
    // }

    return recv;
}

#endif

#else //真实硬件

void spi_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;

    //使能SPI时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    //使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //PB14->CS(输出模式)  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //空闲时,要将片选引脚拉高
    CS = 1;

    //PB3->CLK  PB4->MISO  PB5->MOSI(复用)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //复用功能连接引脚
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

    //SPI配置
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据
    //CPOL和CPHA决定了工作模式,00->模式0  01->模式1 10->模式2  11->模式3
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟极性
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//时钟相位,模式3
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  //软件触发
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //84MHz/4=21MHz
    //MSB:最高有效位优先   LSB:最低有效位优先
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC校验位,默认7
    SPI_Init(SPI1, &SPI_InitStructure);

    //使能SPI1
    SPI_Cmd(SPI1, ENABLE);
}

//收发数据
uint8_t spi_send_recv_byte(uint8_t send)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, send);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}


#endif
