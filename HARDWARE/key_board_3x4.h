/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-09-24 17:39:48
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-09-28 09:02:38
 * @FilePath: \矩阵键盘\HARDWARE\key_board_3x4.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __KEY_BOARD_3x4_H__
#define __KEY_BOARD_3x4_H__

#include "stm32f4xx.h"
#include "system.h"
#include "SysTick.h"
#include "beep.h"


 // 定义行和列的GPIO引脚
// #define ROW0 PAout(8)
// #define ROW1 PEout(6)
// #define ROW2 PBout(6)
// #define ROW3 PCout(9)

// #define COL0 PCin(7)
// #define COL1 PGin(15)
// #define COL2 PAin(4)
// #define COL3 PBin(7)


#define COL3 PAin(8)
#define COL2 PEin(6)
#define COL1 PBin(6)
#define COL0 PCin(9)

#define ROW3 PCout(7)
#define ROW2 PGout(15)
#define ROW1 PAout(4)
#define ROW0 PBout(7)




void key_board_Init(void);
s8 key_board_Scan(u8 mode);
void get_num(char* key_buf);



#endif
