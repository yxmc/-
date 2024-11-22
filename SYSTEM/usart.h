#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h" 
#include "led.h"
#include "DHT11.h"

#define  BLE_STATE PBin(9)

extern char g_usart1_recvbuf[100];
extern u32 g_usart1_recvflag;
extern u32 usart1_recvlen;

extern char g_usart2_recvbuf[100];
extern u32 g_usart2_recvflag;
extern u32 usart2_recvlen;

extern char g_usart3_recvbuf[100];
extern u32 g_usart3_recvflag;
extern u32 usart3_recvlen;

extern u16 USART3_RX_STA;
extern u8 USART3_RX_BUF[100];



void usart1_Init(u32 baud);
void usart2_Init(u32 baud);
void usart3_Init(u32 baud);


void usart_sendstr(USART_TypeDef* USARTx, char* str);

#endif











