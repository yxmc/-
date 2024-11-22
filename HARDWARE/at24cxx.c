#include "at24cxx.h"
#include "stm32f4xx.h"
#include "IIC.h"
#include "stdio.h"

//写入数据
void at24cxx_write_data(u8* data, u8 addr, u8 len)
{
    //起始信号
    IIC_start();

    //发送写操作的设备地址
    IIC_send_byte(0xA0);

    //等待应答
    if (IIC_wait_ack() != ACK)
    {
        printf("发送写操作的设备地址失败\n");
        return;
    }

    //发送写数据的地址
    IIC_send_byte(addr);

    //等待应答
    if (IIC_wait_ack() != ACK)
    {
        printf("发送写数据的地址失败\n");
        return;
    }

    //发送数据
    while (len--)
    {
        IIC_send_byte(data[len]);
        //等待应答
        if (IIC_wait_ack() != ACK)
        {
            printf("发送数据失败\n");
            return;
        }

    }

    //停止信号
    IIC_stop();

    printf("写入数据成功\n");
}

//读取数据
void at24cxx_read_data(u8* data, u8 addr, int32_t len)
{
    //起始信号
    IIC_start();

    //发送写操作的设备地址
    IIC_send_byte(0xA0);

    //等待应答
    if (IIC_wait_ack() != ACK)
    {
        printf("发送写操作的设备地址失败\n");
        return;
    }
    //发送读数据的地址
    IIC_send_byte(addr);

    //等待应答
    if (IIC_wait_ack() != ACK)
    {
        printf("发送读数据的地址失败\n");
        return;
    }
    //起始信号
    IIC_start();

    //发送读操作的设备地址
    IIC_send_byte(0xA1);

    //等待应答
    if (IIC_wait_ack() != ACK)
    {
        printf("发送写操作的设备地址失败\n");
        return;
    }

    for (u32 i = 0;i < len;i++)
    {
        data[len - i - 1] = IIC_recv_byte(); //接收数据
        if (i != len - 1)
            IIC_send_ack(ACK); //发送应答
    }

    //接收非应答
    if (IIC_wait_ack() == NACK)
    {
        printf("读取成功\n");
    }

    //停止信号
    IIC_stop();

}


