/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-21 15:18:57
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-21 15:26:46
 * @FilePath: \IIC\HARDWARE\MPU6050\mpu_iic.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __MPU_IIC_H__
#define __MPU_IIC_H__

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

 //IO方向设置
#define MPU_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define MPU_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
//IO操作函数	 
#define  MPU_IIC_SCL    PBout(8) //SCL
#define  MPU_IIC_SDA    PBout(9) //SDA	 
#define  READ_SDA        PBin(9)  //输入SDA 

//IIC所有操作函数
void  MPU_IIC_Init(void);                //初始化IIC的IO口				 
void  MPU_IIC_Start(void);				//发送IIC开始信号
void  MPU_IIC_Stop(void);	  			//发送IIC停止信号
void  MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8  MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8  MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void  MPU_IIC_Ack(void);					//IIC发送ACK信号
void  MPU_IIC_NAck(void);				//IIC不发送ACK信号

// void  MPU_IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
// u8  MPU_IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif
















