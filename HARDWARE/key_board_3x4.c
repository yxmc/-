/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-24 17:39:33
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-23 11:36:26
 * @FilePath: \指纹蓝牙门锁\HARDWARE\key_board_3x4.c
 * @Description: 3x4矩阵键盘驱动程序
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "key_board_3x4.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "stdio.h"
#include "OLED.h"


void key_board_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
        RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE |
        RCC_AHB1Periph_GPIOG, ENABLE);

    // 列 输入模式:PC9、PB6、PE6、PA8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // 行 输出模式模式:PB7、PA4、PG15、PC7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ROW0 = 1;
    ROW1 = 1;
    ROW2 = 1;
    ROW3 = 1;
    COL0 = 1;
    COL1 = 1;
    COL2 = 1;

}



/*  @brief  函数功能 : 按键扫描函数,扫描3x4矩阵键盘的按键,并返回按键的坐标.
    @param  X
            参数1说明
    @param  y
            参数2说明
    @return 键值
    @property : 说明
*/
s8 key_board_Scan(u8 mode)
{
    static u8 key_flag = 1; // 按键状态标志
    static s8 key_value = -1;

    if (key_flag == 1) // 只有在按键未被按下时才处理
    {
        // 逐行逐列扫描按键
        for (int row = 0; row < 4; row++)
        {
            ROW0 = (row == 0) ? 0 : 1;
            ROW1 = (row == 1) ? 0 : 1;
            ROW2 = (row == 2) ? 0 : 1;
            ROW3 = (row == 3) ? 0 : 1;

            // 检测列按键
            if (COL0 == 0 || COL1 == 0 || COL2 == 0)
            {
                delay_ms(15); // 去抖动

                if (COL0 == 0 || COL1 == 0 || COL2 == 0)
                {
                    key_flag = 0; // 标记按键已按下

                    if (COL0 == 0)
                    {
                        key_value = (row * 3) + 1; // 1, 4, 7, *
                        if (key_value == 10)
                        {
                            key_value = '*' - '0'; //  *键特殊处理
                        }


                    }
                    else if (COL1 == 0)
                    {
                        key_value = (row * 3) + 2; // 2, 5, 8, 0
                        if (key_value == 11)
                        {
                            key_value = 0; // 0键特殊处理
                        }

                    }
                    else if (COL2 == 0)
                    {
                        key_value = (row * 3) + 3; // 3, 6, 9, #
                        if (key_value == 12)
                        {
                            key_value = '#' - '0'; //  #键特殊处理
                        }

                    }

                    BEEP = 1; // 发声
                    delay_ms(50);
                    BEEP = 0;

                    while (COL0 == 0);
                    while (COL1 == 0);
                    while (COL2 == 0);

                    return key_value + '0'; // 返回键值,避免继续扫描

                }
            }
        }
    }

    // 检查是否无按键按下
    else  if (COL0 == 1 && COL1 == 1 && COL2 == 1)
    {
        // 重置行状态
        ROW0 = 1;
        ROW1 = 1;
        ROW2 = 1;
        ROW3 = 1;

        key_value = -1;

        key_flag = 1; // 按键释放,允许下次按键检测
    }

    if (mode == 1) // 连续按键按下
    {
        key_flag = 1; // 如果需要重新启用
    }

    return key_value; // 如果没有按键按下, 返回 -1
}

//从键盘获取数据
void get_num(char* key_buf)  //123456\0
{
    int8_t key_value;
    u32 key_len = 0;
    while (1)
    {
        key_value = key_board_Scan(0);
        if (key_value != -1)
        {
            if (key_value == '#')        //确认
            {
                key_len = 0;
                break;
            }
            else if (key_value == '*')   //删除
            {
                if (key_len > 0)
                {
                    key_len--;
                    key_buf[key_len] = '\0';  //在最后添加\0
                }
            }
            else                        //其他字符作为输入(0~9)
            {
                key_buf[key_len++] = key_value;
                key_buf[key_len] = '\0';  //在最后添加\0
            }
            key_value = -1;
        }
    }
}



