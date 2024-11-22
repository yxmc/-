/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-22 15:25:44
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-22 15:44:18
 * @FilePath: \ZHAO-Watchd:\ZM\GEC\share\code\summerVacation\STM32F4\指纹蓝牙门锁\HARDWARE\AS608\as608.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __AS608_H__
#define __AS608_H__
#include <stdio.h>
#include "stm32f4xx.h" 
#include "system.h"
#include "SysTick.h"

#define PS_Sta   PAin(6)//读指纹模块状态引脚
#define CharBuffer1 0x01
#define CharBuffer2 0x02

#define BEEP PFout(8)

extern u32 AS608Addr;//模块地址


typedef struct
{
	u16 pageID;//指纹ID
	u16 mathscore;//匹配得分
}SearchResult;

typedef struct
{
	u16 PS_max;//指纹最大容量
	u8  PS_level;//安全等级
	u32 PS_addr;
	u8  PS_size;//通讯数据包大小
	u8  PS_N;//波特率基数N
}SysPara;

extern SysPara AS608Para;//指纹模块AS608参数

void PS_StaGPIO_Init(void);//初始化PA6读状态引脚

u8 PS_GetImage(void); //录入图像 

u8 PS_GenChar(u8 BufferID);//生成特征 

u8 PS_Match(void);//精确比对两枚指纹特征 

u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult* p);//搜索指纹 

u8 PS_RegModel(void);//合并特征(生成模板) 

u8 PS_StoreChar(u8 BufferID, u16 PageID);//储存模板 

u8 PS_DeletChar(u16 PageID, u16 N);//删除模板 

u8 PS_Empty(void);//清空指纹库 

u8 PS_WriteReg(u8 RegNum, u8 DATA);//写系统寄存器 

u8 PS_ReadSysPara(SysPara* p); //读系统基本参数 

u8 PS_SetAddr(u32 addr);  //设置模块地址 

u8 PS_WriteNotepad(u8 NotePageNum, u8* content);//写记事本 

u8 PS_ReadNotepad(u8 NotePageNum, u8* note);//读记事 

u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult* p);//高速搜索 

u8 PS_ValidTempleteNum(u16* ValidN);//读有效模板个数 

u8 PS_HandShake(u32* PS_Addr); //与AS608模块握手

const char* EnsureMessage(u8 ensure);//确认码错误信息解析

void ShowErrMessage(u8 ensure);

void press_FR(void);

void add_FR(void);

// void del_FR(void);

void work_fr(void);

#endif







