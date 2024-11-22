/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-15 14:55:38
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-17 09:06:17
 * @FilePath: \RFID\HARDWARE\w25qxxx.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "w25qxxx.h"
#include "stm32f4xx.h"
#include "spi.h"

void w25qxxx_read_id(u8* m_id, u8* d_id)
{
    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令90H
    spi_send_recv_byte(0x90);

    //发送24位地址(000000)
    spi_send_recv_byte(0);
    spi_send_recv_byte(0);
    spi_send_recv_byte(0);

    //接收从机返回的厂商和设备ID
    *m_id = spi_send_recv_byte(0xff);  //发送任意数据
    *d_id = spi_send_recv_byte(0xff);  //发送任意数据

    //spi工作完成时,需要先片选引脚拉高
    CS = 1;
}

/*	@brief  函数功能 读取数据
    @param  data
            存放读取到的数据
    @param  addr
            起始地址
    @param  len
            读取长度
    @return 无
    @note   无
*/
void w25qxxx_read_data(u8* data, u32 addr, u32 len)
{
    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令03H
    spi_send_recv_byte(0x03);

    //发送3字节的地址
    spi_send_recv_byte((addr >> 16) & 0xff);
    spi_send_recv_byte((addr >> 8) & 0xff);
    spi_send_recv_byte(addr & 0xff);


    //接收从机返回的数据
    for (u32 i = 0; i < len; i++)
    {
        data[i] = spi_send_recv_byte(0xff);  //发送任意数据
    }

    //spi工作完成时,需要先片选引脚拉高
    CS = 1;
}

//写使能
void w25qxxx_write_enable(void)
{
    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令06H
    spi_send_recv_byte(0x06);

    //spi工作完成时,需要先片选引脚拉高
    CS = 1;
}

//写禁止
void w25qxxx_write_disable(void)
{
    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令04H
    spi_send_recv_byte(0x04);

    //spi工作完成时,需要先片选引脚拉高
    CS = 1;
}

//读取状态寄存器1
u8 w25qxxx_read_status_reg1(void)
{
    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令05H
    spi_send_recv_byte(0x05);

    //接收从机返回的状态寄存器值
    u8 status = spi_send_recv_byte(0xff);  //发送任意数据

    //spi工作完成时,需要先片选引脚拉高
    CS = 1;

    return status;
}

//等待写操作结束
void w25qxxx_wait_for_write_end(void)
{
    while (w25qxxx_read_status_reg1() & 0x01);
}

/* //擦除整个芯片
void w25qxxx_erase_chip(void)
{
    w25qxxx_write_enable();
    delay_ms(1);

    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令C7H
    spi_send_recv_byte(0xC7);

    //判断是否擦除成功
    if (w25qxxx_read_status_reg1() & 0x01)
    {
    }
    else
    {

        delay_ms(1); //延时1ms

        //spi工作完成时,需要先片选引脚拉高
        CS = 1;
        return;
    }
}

//擦除块
void w25qxxx_erase_block(u32 addr)
{
    w25qxxx_write_enable();
    delay_ms(1);

    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令52H
    spi_send_recv_byte(0x52);

    //发送3字节的地址
    spi_send_recv_byte((addr >> 16) & 0xff);
    spi_send_recv_byte((addr >> 8) & 0xff);
    spi_send_recv_byte(addr & 0xff);

    //判断是否擦除成功
    if (w25qxxx_read_status_reg1() & 0x01)
    {
    }
    else
    {

        delay_ms(1); //延时1ms

        //spi工作完成时,需要先片选引脚拉高
        CS = 1;
        return;
    }
}
 */


 /*	@brief  函数功能 擦除扇区
     @param  addr
             扇区地址
     @return 无
     @note   无
 */
void w25qxxx_erase_sector(u32 addr)
{
    w25qxxx_write_enable();
    delay_ms(1);

    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令D8H
    spi_send_recv_byte(0x20);

    //发送3字节的地址
    spi_send_recv_byte((addr >> 16) & 0xff);
    spi_send_recv_byte((addr >> 8) & 0xff);
    spi_send_recv_byte(addr & 0xff);


    delay_ms(1);

    //spi工作完成时,需要先片选引脚拉高
    CS = 1;

    //等待写入完成
    w25qxxx_wait_for_write_end();
    delay_ms(1);
    w25qxxx_write_disable();
}

/*	@brief  函数功能 写入数据
    @param  data
                待写入的数据
    @param  addr
                起始地址
    @param  len
                写入长度
    @return 无
    @note   无
*/
void w25qxxx_write_data(u8* data, u32 addr, u32 len)
{
    w25qxxx_write_enable();
    delay_ms(1);

    //spi开始工作时,需要先片选引脚拉低
    CS = 0;

    //发送指令02H
    spi_send_recv_byte(0x02);

    //发送3字节的地址
    spi_send_recv_byte((addr >> 16) & 0xff);
    spi_send_recv_byte((addr >> 8) & 0xff);
    spi_send_recv_byte(addr & 0xff);

    //发送写入的数据  高位在前
    while (len > 0)
    {
        spi_send_recv_byte(data[--len]);
    }

    //等待写入完成
    w25qxxx_wait_for_write_end();


    //spi工作完成时,需要先片选引脚拉高
    CS = 1;

    delay_ms(1);


    delay_ms(1);
    w25qxxx_write_disable();

}
