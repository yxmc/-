/*
 * @Author: yxmc 2737647040@qq.com
 * @Date: 2024-10-17 11:03:00
 * @LastEditors: yxmc 2737647040@qq.com
 * @LastEditTime: 2024-10-28 11:42:37
 * @FilePath: \指纹蓝牙门锁\USER\main.c
 * @Description:
 *
 * Copyright (c) 2024 by yxmc, All Rights Reserved.
 */

#include "main.h"

#define INFRARED PEin(5)

 //蓝牙初始化
void set_bluetooth(void)
{
    //恢复出厂设置
    usart_sendstr(USART2, "AT+ORGL\r\n");
    delay_ms(1000);
    //设置名字
    usart_sendstr(USART2, "AT+NAME=Moon\r\n");
    delay_ms(1000);

    //设置配对密码
    usart_sendstr(USART2, "AT+PSWD=\"1234\"\r\n");
    delay_ms(1000);

    //重启模块
    usart_sendstr(USART2, "AT+RESET\r\n");

}

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //循环发送,直到发送完毕   
    USART_SendData(USART1, c);
}

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun, u8* data, u8 len)
{
    u8 send_buf[32];
    u8 i;
    if (len > 28)return;	//最多28字节数据 
    send_buf[len + 3] = 0;	//校验数置零
    send_buf[0] = 0X88;	//帧头
    send_buf[1] = fun;	//功能字
    send_buf[2] = len;	//数据长度
    for (i = 0;i < len;i++)send_buf[3 + i] = data[i];			//复制数据
    for (i = 0;i < len + 3;i++)send_buf[len + 3] += send_buf[i];	//计算校验和	
    for (i = 0;i < len + 4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}

//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz)
{
    u8 tbuf[12];
    tbuf[0] = (aacx >> 8) & 0XFF;
    tbuf[1] = aacx & 0XFF;
    tbuf[2] = (aacy >> 8) & 0XFF;
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF;
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF;
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF;
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF;
    tbuf[11] = gyroz & 0XFF;
    usart1_niming_report(0XA1, tbuf, 12);//自定义帧,0XA1
}

//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度. -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度.-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz, short roll, short pitch, short yaw)
{
    u8 tbuf[28];
    u8 i;
    for (i = 0;i < 28;i++)tbuf[i] = 0;//清0
    tbuf[0] = (aacx >> 8) & 0XFF;
    tbuf[1] = aacx & 0XFF;
    tbuf[2] = (aacy >> 8) & 0XFF;
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF;
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF;
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF;
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF;
    tbuf[11] = gyroz & 0XFF;
    tbuf[18] = (roll >> 8) & 0XFF;
    tbuf[19] = roll & 0XFF;
    tbuf[20] = (pitch >> 8) & 0XFF;
    tbuf[21] = pitch & 0XFF;
    tbuf[22] = (yaw >> 8) & 0XFF;
    tbuf[23] = yaw & 0XFF;
    usart1_niming_report(0XAF, tbuf, 28);//飞控显示帧,0XAF
}


int32_t unlock_flag = -1;

// 开锁成功
void lock_success(void)
{
    // OLED_Clear();
    // OLED_ShowChinese(32, 10, "开锁成功");
    // OLED_Update();
    BEEP = 1;
    delay_ms(1000);
    BEEP = 0;
    // delay_ms(1000);

    // OLED_Clear();
    // OLED_Update();

}

// 开锁失败
void lock_unsuccess(void)
{
    OLED_Clear();
    OLED_ShowChinese(32, 10, "开锁失败");
    OLED_Update();
    BEEP = 1;
    delay_ms(50);
    BEEP = 0;
    delay_ms(50);
    BEEP = 1;
    delay_ms(50);
    BEEP = 0;
    delay_ms(50);
    BEEP = 1;
    delay_ms(50);
    BEEP = 0;
    delay_ms(1750);
    TIM_Cmd(TIM2, ENABLE);

}


// 蓝牙解锁
void bluetooth_unlock(void)
{
    u8 unlock_pwd[5] = "1234";

    //判断是否为蓝牙解锁帧
    if (g_usart2_recvbuf[0] == '*' && g_usart2_recvbuf[5] == '*')
    {
        OLED_Display_On();
        OLED_Clear();
        OLED_ShowChinese(32, 10, "蓝牙解锁");
        OLED_Update();
        TIM_Cmd(TIM2, ENABLE);

        //判断密码是否正确
        for (u8 i = 1; i < 5; i++)
        {
            if (g_usart2_recvbuf[i] != unlock_pwd[i - 1])
            {
                unlock_flag = 0;
                break;
            }
        }

        //解锁成功
        unlock_flag = 1;
    }


}

// // 密码解锁
// void cipher_unlock(void)
// {
//     u8 unlock_pwd[5] = "1234";
//     char key_buf[10] = { 0 };
//     get_num(key_buf);
//     //判断密码是否正确
//     for (u8 i = 0; i < 5; i++)
//     {
//         if (key_buf[i] != unlock_pwd[i])
//         {
//             unlock_flag = 0;
//             return;
//         }
//     }
//     //解锁成功
//     unlock_flag = 1;
//     OLED_Clear();
//     OLED_ShowChinese(32, 10, "密码解锁");
//     OLED_Update();
//     OLED_Display_On();
//     TIM_Cmd(TIM2, ENABLE);
// }

// 密码解锁
void cipher_unlock(void)
{
    u32 unlock_pwd = *(__IO u32*)0x08010004;
    u32 pwd = 0;

    char key_buf[10] = { 0 };
    int8_t key_value;
    u32 key_len = 0;
    u8 flag = 1;

    while (flag)
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
        else if (key_len == 0)
        {
            flag = 0;
        }
    }


    if (flag == 1)
    {
        //判断密码是否正确
        pwd = atoi(key_buf);
        printf("pwd = %d\n", pwd);
        printf("unlock_pwd = %d\n", unlock_pwd);

        if (pwd != unlock_pwd)
        {
            unlock_flag = 0;
        }
        else
        {
            //解锁成功
            unlock_flag = 1;
            OLED_Clear();
            OLED_ShowChinese(32, 10, "密码解锁");
            OLED_Update();
            OLED_Display_On();
            TIM_Cmd(TIM2, ENABLE);
        }


    }



}




void Human_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //开启GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //配置引脚参数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			  //指定引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   	 //模式选择,输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 //输出类型,推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	  //上下拉电阻
    GPIO_Init(GPIOE, &GPIO_InitStructure);		          //初始化PF9引脚	
}



int main(void)
{
    //中断优先级设置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    StsTick_init();
    LED_Init();
    BEEP_Init();
    EXTI_KEY_Init();
    key_board_Init();
    usart1_Init(115200);   		//串口1初始化,115200波特率 
    usart2_Init(9600);  		//串口2初始化,9600波特率 蓝牙
    usart3_Init(57600);   		//指纹模块
    timer7_Init(5000, 8400);  	//定时器7初始化,500ms周期  主板状态LED闪烁
    timer2_Init(30000, 8400);
    TIM_Cmd(TIM2, DISABLE);
    Human_Init();
    flash_Init();
    sr04_Init();
    // IIC_Init();
    PS_StaGPIO_Init();    //初始化指纹模块TCH
    rtc_Init();
    // alarmA_Init();
    OLED_Init();
    MFRC522_Init();			    //初始化MFRC522
    // MPU_Init();				//初始化MPU6050


    while (PS_HandShake(&AS608Addr))//与AS608模块握手
    {
        delay_ms(400);

        printf("未检测到AS608模块!!!\n");
        delay_ms(800);

        printf("尝试重新连接AS608模块!!!\n");
    }
    printf("AS608 ok.\t\n");

    //    while (mpu_dmp_init())
    //    {
    //        printf("尝试重新连接MPU6050模块.\n");
    //        delay_ms(200);
    //    }
    //    printf("MPU6050 ok.\t\n");

        // char str[100];
    char buf[20] = { 0 };				//时间缓存区
    // u8  ensure;
    u32 distance;
    // u16 ValidN;//模块内有效指纹个数

    // ensure = PS_ValidTempleteNum(&ValidN);//读库指纹个数
    // if (ensure != 0x00)
    // 	ShowErrMessage(ensure);//显示确认码错误信息	
    // ensure = PS_ReadSysPara(&AS608Para);  //读参数 
    // if (ensure == 0x00)
    // {
    // 	memset(str, 0, 50);
    // 	sprintf(str, "库容量:%d     对比等级: %d", AS608Para.PS_max - ValidN, AS608Para.PS_level);
    // 	printf("%s\n", str);
    // }
    // else
    // 	ShowErrMessage(ensure);
    int ble = 0;

    PS_ReadSysPara(&AS608Para);  //读参数 

    OLED_Display_On();
    TIM_Cmd(TIM2, ENABLE);

    while (1)
    {
        //开锁成功或失败
        if (unlock_flag != -1)
        {
            if (unlock_flag == 1)
                lock_success();
            else
                lock_unsuccess();

            unlock_flag = -1;
        }

        OLED_Clear();
        rtc_get_datatime(buf);
        char* p = strtok(buf, " ");
        p = strtok(NULL, " ");
        OLED_Printf(32, 35, OLED_8X16, "%s", p);
        OLED_ShowImage(0, 0, 10, 10, signal);
        OLED_ShowImage(10, 0, 10, 10, battery);
        OLED_ShowChinese(24, 15, "指纹蓝牙锁");
        if (BLE_STATE == 1)
        {
            OLED_ShowImage(10 + 10, 0, 10, 10, bluetooth);
        }
        OLED_Update();
        OLED_Clear();


        //指纹解锁
        if (PS_Sta)	    //检测PS_Sta状态,如果有手指按下
        {
            press_FR(); //刷指纹			
        }

        //录指纹和删指纹 
        if (key_board_flag == 1)
        {
            key_board_flag = 0;
            work_fr();
        }

        if (BLE_STATE == 1) //蓝牙连接成功
        {
            if (ble == 0)
            {
                ble = 1;
            }
        }
        else
        {
            if (ble == 1)
            {
                ble = 0;
            }
        }



        //蓝牙解锁
        if (g_usart2_recvflag)
        {
            g_usart2_recvflag = 0;
            bluetooth_unlock();

        }

        // 密码解锁
        cipher_unlock();

        //		// RFID解锁
        //        if (distance_flag && INFRARED)            
        //        {
        //            distance_flag = 0;
        //            distance = get_sr04_distance();
        //            if (distance < 500 && INFRARED)
        //            {
        //                OLED_Display_On();
        //                TIM_Cmd(TIM2, ENABLE);
        //				MFRC522_Init();			    //初始化MFRC522
        //                rfid_unlock();
        //            }

        //        }

        // RFID解锁
        if (distance_flag)
        {
            distance_flag = 0;
            distance = get_sr04_distance();
            if (distance < 500)
            {
                TIM_Cmd(TIM2, ENABLE);
                OLED_Display_On();
                MFRC522_Init();			    //初始化MFRC522
                rfid_unlock();
            }

        }
    }


}

