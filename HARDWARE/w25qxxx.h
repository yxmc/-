#ifndef __W25QXXX_H__
#define __W25QXXX_H__

#include "stm32f4xx.h"
#include "SysTick.h"

void w25qxxx_read_id(u8* m_id, u8* d_id);

void w25qxxx_write_enable(void);

void w25qxxx_write_disable(void);

void w25qxxx_erase_sector(u32 addr);

void w25qxxx_read_data(u8* data, u32 addr, u32 len);

void w25qxxx_write_data(u8* data, u32 addr, u32 len);

#endif
