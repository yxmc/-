#include "iwdg.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include <stm32f4xx_iwdg.h>

void init_iwdg(void)
{
    //解除写保护
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    //LSI分频,32000/256=125Hz
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    //设置计数值,也就是说250/125=2s中之内要进行喂狗,否则就会导致独立看门狗复位cpu
    IWDG_SetReload(250);

    //重载计数值(喂狗)
    IWDG_ReloadCounter();

    //使能独立看门狗
    IWDG_Enable();
}
