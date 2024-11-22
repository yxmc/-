/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-22 11:37:23
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-23 14:10:30
 * @FilePath: \指纹蓝牙门锁\HARDWARE\flash.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#include "flash.h"

#include "stdio.h"
#include <stm32f4xx_flash.h>


void flash_Init(void)
{

    u32 data = *(__IO u32*)0x08010000;

    if (data != 123456)
    { 
        //解除写保护
    FLASH_Unlock();

    /*清除待处理标志(如果有)*/
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
        FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    //Flash写入数据之前必须先擦除
    FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3);
    FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3);


    //写入数据
    FLASH_ProgramWord(0x08010000, 123456);
    FLASH_ProgramWord(0x08010004, 654321);

    //添加写保护
    FLASH_Lock();
    }

    // u32 data1 = *(__IO u32*)0x08010000;

    // printf("data1 = %d\n", data1);

}

//写一条数据（32字节）
int32_t write_flash(char *temp,u32 count)
{
    int32_t i = 0;
    u32 start_addr = ADDR_FLASH_SECTOR_4 + (count)*32;
    u32 end_addr = start_addr+32;
     //解除写保护
    FLASH_Unlock();
    
    while(start_addr < end_addr)
    {
        //每次写入一个字节的数据
        if(FLASH_ProgramByte(start_addr,temp[i]) == FLASH_COMPLETE)
        {
             start_addr++;
             i++;
        }
        else
        {
               //添加写保护
            FLASH_Lock(); 
            return -1;
        }
       
        //每次写入四个字节的数据
//        if(FLASH_ProgramWord(start_addr,*(u32 *)&temp[i]) != FLASH_COMPLETE)
//        {
//            return -1;
//        }
//        else
//        {
//            start_addr += 4;
//            i += 4;
//        }
       
    }
    
    //添加写保护
    FLASH_Lock(); 
    
    return 0;
}

//读一条数据（32字节）
void read_flash(char *temp,u32 count)
{
    int32_t i = 0;
    u32 start_addr = ADDR_FLASH_SECTOR_4 + count*32;
    u32 end_addr = start_addr+32;
    
    while(start_addr < end_addr)
    {
         //每次读取一个字节的数据
        temp[i] = *(u8 *)start_addr;
        start_addr++;
        i++;
        //每次读取四个字节的数据
//        *(u32 *)&temp[i] = *(u32 *)start_addr;
//        start_addr += 4;
//        i += 4;
    }
}

void clear_flash(void)
{
    //解除写保护
    FLASH_Unlock();
    
    //擦除扇区4
    if(FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3) != FLASH_COMPLETE)
    {
        printf("擦除失败\n");
    }
    
    //添加写保护
    FLASH_Lock(); 
    
}



