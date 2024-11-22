#include "IIC.h"
#include "stm32f4xx.h"
#include "SysTick.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_i2c.h>

void IIC_Init(void)
{
    // 软件模拟IIC初始化
    GPIO_InitTypeDef  GPIO_InitStructure;

    // //开启GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // //总线空闲时时钟和数据都为高
    SCL = 1;
    SDA_OUT = 1;


    // 硬件IIC初始化
//    I2C_InitTypeDef  I2C_InitStructure;

//    GPIO_InitTypeDef  GPIO_InitStructure;
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);

//    /*!< I2C configuration */
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    I2C_InitStructure.I2C_OwnAddress1 = 0xA0;	//地址
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//    I2C_InitStructure.I2C_ClockSpeed = 100000;	//100KHz

//    /* 使能I2C硬件 */
//    I2C_Cmd(I2C1, ENABLE);

//    /* I2C硬件初始化 */
//    I2C_Init(I2C1, &I2C_InitStructure);

//    delay_ms(200);


}

//切换SDA引脚模式(输入/输出)
void IIC_mode(GPIOMode_TypeDef GPIO_Mode)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//起始信号
void IIC_start(void)
{
    //将SDA引脚设为输出模式
    IIC_mode(GPIO_Mode_OUT);

    //空闲状态时,数据线和时钟线都是高电平
    SCL = 1;
    delay_us(3);

    SDA_OUT = 1;
    delay_us(3);

    //将SDA拉低
    SDA_OUT = 0;
    delay_us(3);

    //再将SCL拉低
    SCL = 0;
    delay_us(3);
}

//停止信号
void IIC_stop(void)
{
    //将SDA引脚设为输出模式
    IIC_mode(GPIO_Mode_OUT);


    SCL = 0;
    SDA_OUT = 0;
    delay_us(3);

    SCL = 1;     //将时钟线拉高
    delay_us(3); //延时的目的是让CPU识别到电平的变化

    SDA_OUT = 1; //再将数据线拉高
    delay_us(3);
}

//发送数据
void IIC_send_byte(u8 send) //1010 0101
{
    //将SDA引脚设为输出模式
    IIC_mode(GPIO_Mode_OUT);

    for (int i = 0;i < 8;i++)
    {

        if (send & (0x1 << (7 - i)))  //判断每个bit的值
        {
            SDA_OUT = 1;        //如果为1,则将SDA拉高
        }
        else
        {
            SDA_OUT = 0;        //如果为1,则将SDA拉低
        }
        delay_us(3); //【电平变化后要加延时】

        SCL = 1;  //将时钟线拉高
        delay_us(3);

        SCL = 0;  //将时钟线拉低
        delay_us(3);
    }

    // SCL = 0;  //将时钟线拉低,让从机发送数据
    // delay_us(3);

}

//等待应答
u8 IIC_wait_ack(void)
{
    u8 ack;
    //将SDA引脚设为输入模式
    IIC_mode(GPIO_Mode_IN);

    SCL = 1;  //将时钟线拉高
    delay_us(3);

    ack = SDA_IN; //0:应答  1:非应答

    SCL = 0;  //将时钟线拉高
    delay_us(3);

    return ack;
}

//接收数据
u8 IIC_recv_byte(void)
{
    u8 recv = 0;
    //将SDA引脚设为输入模式
    IIC_mode(GPIO_Mode_IN);

    for (int i = 0;i < 8;i++)
    {

        SCL = 1;  //将时钟线拉高
        delay_us(3);

        if (SDA_IN == 1)
        {
            recv |= (0x1 << (7 - i));
        }
        else
        {
            recv &= ~(0x1 << (7 - i));
        }

        SCL = 0;  //将时钟线拉低
        delay_us(3);
    }

    return recv;
}

//发送应答
void IIC_send_ack(u8 ack)
{
    //将SDA引脚设为输出模式
    IIC_mode(GPIO_Mode_OUT);

    SCL = 0;  //将时钟线拉低
    delay_us(3);

    SDA_OUT = ack;
    delay_us(3); //【电平变化后要加延时】

    SCL = 1;  //将时钟线拉高
    delay_us(3);

    //将SCL拉低,在低电平期间可以改变数据
    SCL = 0;
    delay_us(3);
}



