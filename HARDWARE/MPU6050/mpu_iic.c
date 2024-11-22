#include "mpu_iic.h"
#include "system.h"
#include "SysTick.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用,未经作者许可,不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本:V1.0
//版权所有,盗版必究.
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IIC
void  MPU_IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	MPU_IIC_SCL = 1;
	MPU_IIC_SDA = 1;
}
//产生IIC起始信号
void  MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //sda线输出
	MPU_IIC_SDA = 1;
	MPU_IIC_SCL = 1;
	delay_us(4);
	MPU_IIC_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MPU_IIC_SCL = 0;//钳住I2C总线,准备发送或接收数据 
}
//产生IIC停止信号
void  MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda线输出
	MPU_IIC_SCL = 0;
	MPU_IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	MPU_IIC_SCL = 1;
	MPU_IIC_SDA = 1;//发送I2C总线结束信号
	delay_us(4);
}
//等待应答信号到来
//返回值:1,接收应答失败
//        0,接收应答成功
u8  MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	MPU_SDA_IN();      //SDA设置为输入  
	MPU_IIC_SDA = 1;delay_us(1);
	MPU_IIC_SCL = 1;delay_us(1);
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL = 0;//时钟输出0 	   
	return 0;
}
//产生ACK应答
void  MPU_IIC_Ack(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 0;
	delay_us(2);
	MPU_IIC_SCL = 1;
	delay_us(2);
	MPU_IIC_SCL = 0;
}
//不产生ACK应答		    
void  MPU_IIC_NAck(void)
{
	MPU_IIC_SCL = 0;
	MPU_SDA_OUT();
	MPU_IIC_SDA = 1;
	delay_us(2);
	MPU_IIC_SCL = 1;
	delay_us(2);
	MPU_IIC_SCL = 0;
}
//IIC发送一个字节
//返回从机有无应答
//1,有应答
//0,无应答			  
void  MPU_IIC_Send_Byte(u8 txd)
{
	u8 t;
	MPU_SDA_OUT();
	MPU_IIC_SCL = 0;//拉低时钟开始数据传输
	for (t = 0;t < 8;t++)
	{
		MPU_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(2);   //对TEA5767这三个延时都是必须的
		MPU_IIC_SCL = 1;
		delay_us(2);
		MPU_IIC_SCL = 0;
		delay_us(2);
	}
}
//读1个字节,ack=1时,发送ACK,ack=0,发送nACK   
u8  MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	MPU_SDA_IN();//SDA设置为输入
	for (i = 0;i < 8;i++)
	{
		MPU_IIC_SCL = 0;
		delay_us(2);
		MPU_IIC_SCL = 1;
		receive <<= 1;
		if (READ_SDA)receive++;
		delay_us(1);
	}
	if (!ack)
		MPU_IIC_NAck();//发送nACK
	else
		MPU_IIC_Ack(); //发送ACK   
	return receive;
}















