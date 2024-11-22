/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-15 14:55:30
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-17 09:09:52
 * @FilePath: \RFID\HARDWARE\spi.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __SPI_H__
#define __SPI_H__

#include "system.h"
#include "SysTick.h"


 // #define CS      PBout(14)
 // #define CLK     PBout(3)
 // #define MOSI    PBout(5)
 // #define MISO    PBin(4)

#define CS      PDout(6)
#define CLK     PDout(7)
#define MOSI    PCout(6)
#define MISO    PBin(4)
#define RST     PCout(11)

#define SIMULATION
#define MODE0 1
 // #define MODE1 1
 // #define MODE2 1
 // #define MODE3 1


void spi_Init(void);
uint8_t spi_send_recv_byte(uint8_t byte);

#endif
