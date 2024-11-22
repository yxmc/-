/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-29 14:45:26
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-29 15:06:53
 * @FilePath: \LEDd:\ZM\GEC\share\code\summerVacation\STM32F4\zw2\HARDWARE\OLED_7\OLED.h
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */
#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "OLED_Data.h"

 /*参数宏定义*********************/

/*单片机速度过快时,OLED显示可能出现延迟,因此需要设置延时时间,否则显示不正常*/
/**/
#define OLED_DELAY  delay_us(0)

 /*OLED端口*/
#define     OLED_D0     GPIOB
#define     OLED_D1     GPIOD
#define     OLED_RES    GPIOD
#define     OLED_DC     GPIOE
#define     OLED_CS     GPIOE

/*OLED端口引脚*/
#define     OLED_D0_IO  GPIO_Pin_15
#define     OLED_D1_IO  GPIO_Pin_10
#define     OLED_RES_IO GPIO_Pin_8
#define     OLED_DC_IO  GPIO_Pin_14
#define     OLED_CS_IO  GPIO_Pin_12

/*FontSize参数取值*/
/*此参数值不仅用于判断,而且用于计算横向字符偏移,默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
  
void OLED_Display_On(void);    
void OLED_Display_Off(void);
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char* String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowChinese(int16_t X, int16_t Y, char* Chinese);
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t* Image);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char* format, ...);

/*绘图函数*/
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
