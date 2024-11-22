#ifndef __IIC_H__
#define __IIC_H__
#include "system.h"

#define SCL         PBout(8)
#define SDA_OUT     PBout(9)
#define SDA_IN      PBin(9)


#define ACK 0
#define NACK 1

void IIC_Init(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_mode(GPIOMode_TypeDef GPIO_Mode);
void IIC_send_byte(u8 send);
u8 IIC_wait_ack(void);
u8 IIC_recv_byte(void);
void IIC_send_ack(u8 ack);
#endif
