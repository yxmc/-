#include "rtc.h"
#include <stm32f4xx_exti.h>
#include <stm32f4xx_rtc.h>
#include <stm32f4xx_rcc.h>


u8 g_rtc_wkp_flag = 0;
u8 g_rtc_alarmA_flag = 0;

u32 getBCD(const char* str)
{
    if (strlen(str) == 2)
    {
        return (str[0] - '0') * 16 + (str[1] - '0'); // 转换字符串的最后两位字符为十六进制数
    }

    else if (strlen(str) == 1)
    {
        // 转换字符串的最后一位字符为十六进制数
        return str[0] - '0';
    }

    else
    {
        return 0; // 如果字符串长度不为4,则返回0
    }
}

//初始化RTC
void rtc_Init(void)
{

    __IO uint32_t uwAsynchPrediv = 0;
    __IO uint32_t uwSynchPrediv = 0;

    // NVIC_InitTypeDef  NVIC_InitStructure;
    // EXTI_InitTypeDef  EXTI_InitStructure;
    RTC_InitTypeDef   RTC_InitStructure;
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;


    //使能电源时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    //允许访问备份寄存器
    PWR_BackupAccessCmd(ENABLE);

    //开启LSE
    RCC_LSEConfig(RCC_LSE_ON);

    //等待LSE稳定
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    //LSE作为RTC时钟源(32768Hz),比LSI更加精确
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    /* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
    //1Hz = 32768/128/256
    uwAsynchPrediv = 0x7F; //异步分频系数
    uwSynchPrediv = 0xFF;  //同步分频系数


    //使能RTC时钟
    RCC_RTCCLKCmd(ENABLE);

    //等待稳定
    RTC_WaitForSynchro();

    //读取备份寄存器的值,如果跟之前写入的数据不相同,就重新设置日期时间
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 3456)
    {
        //设置时间
        RTC_TimeStructure.RTC_Hours = 0x10;  //时
        RTC_TimeStructure.RTC_Minutes = 0x34;  //分
        RTC_TimeStructure.RTC_Seconds = 0x0;  //秒
        RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

        //设置日期
        RTC_DateStructure.RTC_Month = RTC_Month_October;
        RTC_DateStructure.RTC_Date = 0x10;
        RTC_DateStructure.RTC_Year = 0x23;
        RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Wednesday;
        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

        //日历配置
        uwSynchPrediv = 0xFF;  //同步分频系数
        uwAsynchPrediv = 0x7F; //异步分频系数
        RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; //24小时计时法
        RTC_Init(&RTC_InitStructure);

        //将设置好的数据保存到备份寄存器中
        RTC_WriteBackupRegister(RTC_BKP_DR0, 3456);
    }

    // //在配置唤醒中断时钟源和计数值之前要先关闭唤醒使能
    // RTC_WakeUpCmd(DISABLE);

    // //配置唤醒中断时钟源
    // RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits); //设置唤醒时钟源为CK_SPRE_16bits

    // //配置唤醒计数值
    // RTC_SetWakeUpCounter(0); //设置唤醒计数值 1

    // RTC_ClearITPendingBit(RTC_IT_WUT);      //清除RTC_IT_WUT中断标志位
    // EXTI_ClearITPendingBit(EXTI_Line22);    //清除EXTI_Line22中断标志位


    // RTC_ITConfig(RTC_IT_WUT, ENABLE);       //允许触发唤醒中断
    // RTC_WakeUpCmd(ENABLE);                  //使能唤醒中断

    // NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);


    // EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    // EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    // EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    // EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    // EXTI_Init(&EXTI_InitStructure);

}

// 获取当前日期时间
void rtc_get_datatime(char* buf)
{
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;
    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure); //获取时间
    RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure); //获取日期

    sprintf(buf, "%x/%x/%x %x:%x:%x\0", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date,
        RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

// 设置当前日期时间
void rtc_set_datatime(char* buf, u8 flag)
{
    // 解析命令
    const char s[2] = "-";
    char* token;

    // 修改日期:"DATE SET-2023-12-12-1"        //年-月-日-星期 
    // 修改时间:"TIME SET-10-20-30"            //时-分-秒 
    if (flag == 0)
    {
        RTC_DateTypeDef   RTC_DateStructure;

        token = strtok(NULL, s);
        RTC_DateStructure.RTC_Year = getBCD(token + 2);
        token = strtok(NULL, s);
        RTC_DateStructure.RTC_Month = getBCD(token);
        token = strtok(NULL, s);
        RTC_DateStructure.RTC_Date = getBCD(token);
        token = strtok(NULL, s);
        RTC_DateStructure.RTC_WeekDay = getBCD(token);

        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure); //设置日期    
    }

    else if (flag == 1)
    {
        RTC_TimeTypeDef   RTC_TimeStructure;

        token = strtok(NULL, s);
        RTC_TimeStructure.RTC_Hours = getBCD(token);
        token = strtok(NULL, s);
        RTC_TimeStructure.RTC_Minutes = getBCD(token);
        token = strtok(NULL, s);
        RTC_TimeStructure.RTC_Seconds = getBCD(token);

        RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); //设置时间 

    }

    char buf1[100] = { 0 };
    rtc_get_datatime(buf1);
    printf("%s\n", buf1);

}

// 初始化闹钟A
RTC_AlarmTypeDef RTC_AlarmStructureA;
void alarmA_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;

    //设置闹钟时间
    RTC_AlarmStructureA.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;  //下午
    RTC_AlarmStructureA.RTC_AlarmTime.RTC_Hours = 0x14;      //时
    RTC_AlarmStructureA.RTC_AlarmTime.RTC_Minutes = 0x20;    //分
    RTC_AlarmStructureA.RTC_AlarmTime.RTC_Seconds = 0x5;    //秒


    //设置何时生效
#if 1  //表示每天生效
    RTC_AlarmStructureA.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
    RTC_AlarmStructureA.RTC_AlarmDateWeekDay = RTC_Weekday_Monday; //星期一
#endif

#if 0  //表示几号生效
    RTC_AlarmStructureA.RTC_AlarmDateWeekDay = 0x10; //10号生效
    RTC_AlarmStructureA.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date; //日期
    RTC_AlarmStructureA.RTC_AlarmMask = RTC_AlarmMask_None; //不屏蔽星期和日期
#endif

#if 0     //表示星期几生效
    RTC_AlarmStructureA.RTC_AlarmDateWeekDay = RTC_Weekday_Thursday; //星期四生效
    RTC_AlarmStructureA.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; //日期
    RTC_AlarmStructureA.RTC_AlarmMask = RTC_AlarmMask_None; //不屏蔽星期和日期
#endif

    //先关闭闹钟使能
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

    //设置闹钟A
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructureA);

    //提前清空中断标志位
    RTC_ClearFlag(RTC_FLAG_ALRAF);
    EXTI_ClearITPendingBit(EXTI_Line17);

    //允许触发闹钟中断
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

    //使能闹钟中断
    // RTC_AlarmCmd(RTC_Alarm_A, ENABLE);


    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}

// 初始化闹钟B
RTC_AlarmTypeDef RTC_AlarmStructureB;
void alarmB_Init(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;

    //设置闹钟时间
    RTC_AlarmStructureB.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;  //下午
    RTC_AlarmStructureB.RTC_AlarmTime.RTC_Hours = 0x14;      //时
    RTC_AlarmStructureB.RTC_AlarmTime.RTC_Minutes = 0x20;    //分
    RTC_AlarmStructureB.RTC_AlarmTime.RTC_Seconds = 0x6;    //秒

    //设置何时生效
#if 1  //表示每天生效
    RTC_AlarmStructureB.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
#endif
#if 0  //表示几号生效
    RTC_AlarmStructureB.RTC_AlarmDateWeekDay = 0x10; //10号生效
    RTC_AlarmStructureB.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date; //日期
    RTC_AlarmStructureB.RTC_AlarmMask = RTC_AlarmMask_None;
#endif
#if 0     //表示星期几生效
    RTC_AlarmStructureB.RTC_AlarmDateWeekDay = RTC_Weekday_Thursday; //星期四生效
    RTC_AlarmStructureB.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; //日期
    RTC_AlarmStructureB.RTC_AlarmMask = RTC_AlarmMask_None;
#endif

    //先关闭闹钟使能
    RTC_AlarmCmd(RTC_Alarm_B, DISABLE);

    //设置闹钟B
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_B, &RTC_AlarmStructureB);

    //提前清空中断标志位
    RTC_ClearFlag(RTC_FLAG_ALRBF);
    EXTI_ClearITPendingBit(EXTI_Line17);

    //允许触发闹钟中断
    RTC_ITConfig(RTC_IT_ALRB, ENABLE);

    //使能闹钟中断
    // RTC_AlarmCmd(RTC_Alarm_B, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //中断模式 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}

RTC_AlarmTypeDef RTC_AlarmStructure;
void set_alarm(char* buf)
{
    // 解析命令
    const char s[2] = "-";
    char* token;
    token = strtok((char*)buf, s);

    char buf1[10] = { 0 };

    strcpy(buf1, token);

    // 设置闹钟时间
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;  //下午
    token = strtok(NULL, s);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = getBCD(token);      //时
    token = strtok(NULL, s);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = getBCD(token);    //分
    token = strtok(NULL, s);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = getBCD(token);    //秒


    // 设置何时生效
#if 1  //表示每天生效
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
#endif


    if (strcmp(buf1, "ALARMA") == 0)
    {
        //设置闹钟A
        RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

        //使能闹钟中断
        RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    }

    else if (strcmp(buf1, "ALARMB") == 0)
    {
        //设置闹钟A
        RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_B, &RTC_AlarmStructure);

        //使能闹钟中断
        RTC_AlarmCmd(RTC_Alarm_B, ENABLE);

    }

}

// RTC唤醒中断服务函数
void RTC_WKUP_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC_IT_WUT中断标志位
        EXTI_ClearITPendingBit(EXTI_Line22); //清除EXTI_Line22中断标志位

        g_rtc_wkp_flag = 1;
    }
}

//闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
    //判断是否触发中断
    if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        g_rtc_alarmA_flag = 1;

        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);
    }
}
