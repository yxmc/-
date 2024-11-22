#include "as608.h"
#include <string.h>	
#include "usart.h"
#include "key_board_3x4.h"
#include "stdlib.h"
#include "OLED.h"


u32 AS608Addr = 0XFFFFFFFF; //默认
SysPara AS608Para;

extern int32_t unlock_flag;

//初始化PA6为下拉输入		    
//读摸出感应状态(触摸感应时输出高电平信号)
void PS_StaGPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	//初始化读状态引脚GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
}

//串口发送一个字节
static void MYUSART_SendData(u8 data)
{
	while ((USART3->SR & 0X40) == 0);
	USART3->DR = data;
}

//发送包头
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}

//发送地址
static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr >> 24);
	MYUSART_SendData(AS608Addr >> 16);
	MYUSART_SendData(AS608Addr >> 8);
	MYUSART_SendData(AS608Addr);
}

//发送包标识,
static void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}

//发送包长度
static void SendLength(int length)
{
	MYUSART_SendData(length >> 8);
	MYUSART_SendData(length);
}

//发送指令码
static void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}

//发送校验和
static void SendCheck(u16 check)
{
	MYUSART_SendData(check >> 8);
	MYUSART_SendData(check);
}

//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间(单位1ms)
//返回值:数据包首地址
static u8* JudgeStr(u16 waittime)
{
	char* data;
	u8 str[8];
	str[0] = 0xef;					str[1] = 0x01;
	str[2] = AS608Addr >> 24;	str[3] = AS608Addr >> 16;
	str[4] = AS608Addr >> 8;	str[5] = AS608Addr;
	str[6] = 0x07;					str[7] = '\0';
	USART3_RX_STA = 0;
	while (--waittime)
	{
		delay_ms(1);
		if (USART3_RX_STA & 0X8000)//接收到一次数据
		{
			USART3_RX_STA = 0;
			data = strstr((const char*)USART3_RX_BUF, (const char*)str);
			if (data)
				return (u8*)data;
		}
	}
	return 0;
}
//录入图像 PS_GetImage
//功能:探测手指,探测到后录入指纹图像存于ImageBuffer. 
//模块返回确认字
u8 PS_GetImage(void)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();			//发送包头
	SendAddr();			//发送地址
	SendFlag(0x01);		//命令包标识
	SendLength(0x03);	//发送包长度
	Sendcmd(0x01);		//发送包标识
	temp = 0x01 + 0x03 + 0x01;
	SendCheck(temp);	//发送校验码
	data = JudgeStr(2000);//接收返回数据
	if (data)
		ensure = data[9];		//返回确认码
	else
		ensure = 0xff;
	return ensure;
}
//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2			 
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//模块返回确认字
u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01 + 0x04 + 0x02 + BufferID;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];  //返回确认码
	else
		ensure = 0xff;
	return ensure;
}
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
u8 PS_Match(void)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01 + 0x03 + 0x03;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到,则返回页码.			
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字,页码(相配指纹模板)
u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult* p)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage >> 8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum >> 8);
	MYUSART_SendData(PageNum);
	temp = 0x01 + 0x08 + 0x04 + BufferID
		+ (StartPage >> 8) + (u8)StartPage
		+ (PageNum >> 8) + (u8)PageNum;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		p->pageID = (data[10] << 8) + data[11];
		p->mathscore = (data[12] << 8) + data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}
//合并特征(生成模板)PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2	
//说明:  模块返回确认字
u8 PS_RegModel(void)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01 + 0x03 + 0x05;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置.			
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID(指纹库位置号)
//说明:  模块返回确认字
u8 PS_StoreChar(u8 BufferID, u16 PageID)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID >> 8);
	MYUSART_SendData(PageID);
	temp = 0x01 + 0x06 + 0x06 + BufferID
		+ (PageID >> 8) + (u8)PageID;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号),N删除的模板个数.
//说明:  模块返回确认字
u8 PS_DeletChar(u16 PageID, u16 N)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID >> 8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N >> 8);
	MYUSART_SendData(N);
	temp = 0x01 + 0x07 + 0x0C
		+ (PageID >> 8) + (u8)PageID
		+ (N >> 8) + (u8)N;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
u8 PS_Empty(void)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01 + 0x03 + 0x0D;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}
//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
u8 PS_WriteReg(u8 RegNum, u8 DATA)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	if (ensure == 0)
		printf("\r\n设置参数成功!");
	else
		printf("\r\n%s", EnsureMessage(ensure));
	return ensure;
}
//读系统基本参数 PS_ReadSysPara
//功能:  读取模块的基本参数(波特率,包大小等)
//参数:  无
//说明:  模块返回确认字 + 基本参数(16bytes)
u8 PS_ReadSysPara(SysPara* p)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01 + 0x03 + 0x0F;
	SendCheck(temp);
	data = JudgeStr(1000);
	if (data)
	{
		ensure = data[9];
		p->PS_max = (data[14] << 8) + data[15];
		p->PS_level = data[17];
		p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}
	else
		ensure = 0xff;
	if (ensure == 0x00)
	{
		// printf("\r\n模块最大指纹容量=%d", p->PS_max);
		// printf("\r\n对比等级=%d", p->PS_level);
		// printf("\r\n地址=%x", p->PS_addr);
		// printf("\r\n波特率=%d\n", p->PS_N * 9600);
	}
	else
		printf("\r\n%s", EnsureMessage(ensure));
	return ensure;
}
//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
u8 PS_SetAddr(u32 PS_addr)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr >> 24);
	MYUSART_SendData(PS_addr >> 16);
	MYUSART_SendData(PS_addr >> 8);
	MYUSART_SendData(PS_addr);
	temp = 0x01 + 0x07 + 0x15
		+ (u8)(PS_addr >> 24) + (u8)(PS_addr >> 16)
		+ (u8)(PS_addr >> 8) + (u8)PS_addr;
	SendCheck(temp);
	AS608Addr = PS_addr;//发送完指令,更换地址
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	AS608Addr = PS_addr;
	if (ensure == 0x00)
		printf("\r\n设置地址成功!");
	else
		printf("\r\n%s", EnsureMessage(ensure));
	return ensure;
}
//功能: 模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本,
//	该记事本逻辑上被分成 16 个页.
//参数:  NotePageNum(0~15),Byte32(要写入内容,32个字节)
//说明:  模块返回确认字
u8 PS_WriteNotepad(u8 NotePageNum, u8* Byte32)
{
	u16 temp;
	u8  ensure, i;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(36);
	Sendcmd(0x18);
	MYUSART_SendData(NotePageNum);
	for (i = 0;i < 32;i++)
	{
		MYUSART_SendData(Byte32[i]);
		temp += Byte32[i];
	}
	temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}
//读记事PS_ReadNotepad
//功能:  读取FLASH用户区的128bytes数据
//参数:  NotePageNum(0~15)
//说明:  模块返回确认字+用户信息
u8 PS_ReadNotepad(u8 NotePageNum, u8* Byte32)
{
	u16 temp;
	u8  ensure, i;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x19);
	MYUSART_SendData(NotePageNum);
	temp = 0x01 + 0x04 + 0x19 + NotePageNum;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		for (i = 0;i < 32;i++)
		{
			Byte32[i] = data[10 + i];
		}
	}
	else
		ensure = 0xff;
	return ensure;
}
//高速搜索PS_HighSpeedSearch
//功能:以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库.
//		  若搜索到,则返回页码,该指令对于的确存在于指纹库中 ,且登录时质量
//		  很好的指纹,会很快给出搜索结果.
//参数:  BufferID, StartPage(起始页),PageNum(页数)
//说明:  模块返回确认字+页码(相配指纹模板)
u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult* p)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage >> 8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum >> 8);
	MYUSART_SendData(PageNum);
	temp = 0x01 + 0x08 + 0x1b + BufferID
		+ (StartPage >> 8) + (u8)StartPage
		+ (PageNum >> 8) + (u8)PageNum;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		p->pageID = (data[10] << 8) + data[11];
		p->mathscore = (data[12] << 8) + data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}
//读有效模板个数 PS_ValidTempleteNum
//功能:读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
u8 PS_ValidTempleteNum(u16* ValidN)
{
	u16 temp;
	u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01 + 0x03 + 0x1d;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		*ValidN = (data[10] << 8) + data[11];
	}
	else
		ensure = 0xff;

	if (ensure == 0x00)
	{
		printf("\r\n有效指纹个数=%d", (data[10] << 8) + data[11]);
	}
	else
		printf("\r\n%s", EnsureMessage(ensure));
	return ensure;
}
//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址(正确地址)	
u8 PS_HandShake(u32* PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);
	delay_ms(200);
	if (USART3_RX_STA & 0X8000)//接收到数据
	{
		if (//判断是不是模块返回的应答包				
			USART3_RX_BUF[0] == 0XEF
			&& USART3_RX_BUF[1] == 0X01
			&& USART3_RX_BUF[6] == 0X07
			)
		{
			*PS_Addr = (USART3_RX_BUF[2] << 24) + (USART3_RX_BUF[3] << 16)
				+ (USART3_RX_BUF[4] << 8) + (USART3_RX_BUF[5]);
			USART3_RX_STA = 0;
			return 0;
		}
		USART3_RX_STA = 0;
	}
	return 1;
}

//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	printf("%s\n", (u8*)EnsureMessage(ensure));
}

//模块应答包确认码信息解析
//功能:解析确认码错误信息返回信息
//参数: ensure
const char* EnsureMessage(u8 ensure)
{
	const char* p;
	switch (ensure)
	{
	case  0x00:
		p = "OK";break;
	case  0x01:
		p = "数据包接收错误";break;
	case  0x02:
		p = "传感器上没有手指";break;
	case  0x03:
		p = "录入指纹图像失败";break;
	case  0x04:
		p = "指纹图像太干、太淡而生不成特征";break;
	case  0x05:
		p = "指纹图像太湿、太糊而生不成特征";break;
	case  0x06:
		p = "指纹图像太乱而生不成特征";break;
	case  0x07:
		p = "指纹图像正常,但特征点太少(或面积太小)而生不成特征";break;
	case  0x08:
		p = "指纹不匹配";break;
	case  0x09:
		p = "没搜索到指纹";break;
	case  0x0a:
		p = "特征合并失败";break;
	case  0x0b:
		p = "访问指纹库时地址序号超出指纹库范围";
	case  0x10:
		p = "删除模板失败";break;
	case  0x11:
		p = "清空指纹库失败";break;
	case  0x15:
		p = "缓冲区内没有有效原始图而生不成图像";break;
	case  0x18:
		p = "读写 FLASH 出错";break;
	case  0x19:
		p = "未定义错误";break;
	case  0x1a:
		p = "无效寄存器号";break;
	case  0x1b:
		p = "寄存器设定内容错误";break;
	case  0x1c:
		p = "记事本页码指定错误";break;
	case  0x1f:
		p = "指纹库满";break;
	case  0x20:
		p = "地址错误";break;
	default:
		p = "模块返回确认码有误";break;
	}
	return p;
}

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	// char str[100] = { 0 };
	ensure = PS_GetImage();
	if (ensure == 0x00)//获取图像成功 
	{
		BEEP = 1;//打开蜂鸣器	
		ensure = PS_GenChar(CharBuffer1);
		if (ensure == 0x00) //生成特征成功
		{
			BEEP = 0;//关闭蜂鸣器	
			ensure = PS_HighSpeedSearch(CharBuffer1, 0, AS608Para.PS_max, &seach);
			if (ensure == 0x00)//搜索成功
			{
				// printf("刷指纹成功\n");
				// sprintf(str, "确有此人,ID:%d  匹配得分:%d", seach.pageID, seach.mathscore);
				// printf("%s\n", str);

				OLED_Clear();
				OLED_ShowChinese(32, 10, "指纹解锁");
				OLED_Update();
				OLED_Display_On();
				TIM_Cmd(TIM2, ENABLE);

				unlock_flag = 1;

			}
			else
			{
				unlock_flag = 0;
				ShowErrMessage(ensure);
			}
		}
		else
			ShowErrMessage(ensure);
		BEEP = 0;//关闭蜂鸣器
		delay_ms(600);
	}

}

// 录指纹
void add_FR(void)
{
	u8 i, ensure, processnum = 0;
	u16 ID;
	char key_buf[10] = { 0 };
	while (1)
	{
		switch (processnum)
		{
		case 0:
			i++;
			OLED_Clear();
			OLED_ShowChinese(32, 10, "请按指纹");
			OLED_Update();
			OLED_Display_On();
			TIM_Cmd(TIM2, ENABLE);
			ensure = PS_GetImage();
			if (ensure == 0x00)
			{
				BEEP = 1;
				ensure = PS_GenChar(CharBuffer1);//生成特征
				BEEP = 0;
				if (ensure == 0x00)
				{
					OLED_Clear();
					OLED_ShowChinese(32, 10, "指纹正常");
					OLED_Update();
					OLED_Display_On();
					TIM_Cmd(TIM2, ENABLE);
					i = 0;
					processnum = 1;//跳到第二步						
				}
				else ShowErrMessage(ensure);
			}
			else ShowErrMessage(ensure);
			break;

		case 1:
			i++;
			OLED_Clear();
			OLED_ShowChinese(0, 20, "请按再按一次指纹");
			OLED_Update();
			OLED_Display_On();
			TIM_Cmd(TIM2, ENABLE);
			ensure = PS_GetImage();
			if (ensure == 0x00)
			{
				BEEP = 1;
				ensure = PS_GenChar(CharBuffer2);//生成特征
				BEEP = 0;
				if (ensure == 0x00)
				{
					OLED_Clear();
					OLED_ShowChinese(32, 10, "指纹正常");
					OLED_Update();
					OLED_Display_On();
					TIM_Cmd(TIM2, ENABLE);
					i = 0;
					processnum = 2;//跳到第三步
				}
				else ShowErrMessage(ensure);
			}
			else ShowErrMessage(ensure);
			break;

		case 2:
			OLED_Clear();
			OLED_ShowChinese(0, 10, "对比两次指纹");
			OLED_Update();
			OLED_Display_On();
			TIM_Cmd(TIM2, ENABLE);
			ensure = PS_Match();
			if (ensure == 0x00)
			{
				OLED_Clear();
				OLED_ShowChinese(32, 10, "对比成功");
				OLED_Update();
				OLED_Display_On();
				TIM_Cmd(TIM2, ENABLE);

				processnum = 3;//跳到第四步
			}
			else
			{
				OLED_Clear();
				OLED_ShowChinese(32, 10, "对比失败");
				OLED_ShowChinese(0, 26, "请重新录入指纹");
				OLED_Update();
				OLED_Display_On();
				TIM_Cmd(TIM2, ENABLE);

				ShowErrMessage(ensure);
				i = 0;
				processnum = 0;//跳回第一步		
			}
			delay_ms(1200);
			break;

		case 3:
			OLED_Clear();
			OLED_ShowChinese(0, 26, "生成指纹模板");
			OLED_Update();
			OLED_Display_On();
			TIM_Cmd(TIM2, ENABLE);
			ensure = PS_RegModel();
			if (ensure == 0x00)
			{
				OLED_Clear();
				OLED_ShowChinese(0, 26, "生成指纹模板成功");
				OLED_Update();
				delay_ms(1000);
				OLED_Display_On();
				TIM_Cmd(TIM2, ENABLE);
				processnum = 4;//跳到第五步
			}
			else { processnum = 0;ShowErrMessage(ensure); }
			delay_ms(1200);
			break;

		case 4:
			OLED_Clear();
			OLED_ShowChinese(0, 10, "请输入储存");
			OLED_ShowString(16 * 5, 26, "ID,", OLED_8X16);
			OLED_ShowString(0, 32, "0 =< ID <= 299", OLED_8X16);
			OLED_Update();
			OLED_Display_On();
			TIM_Cmd(TIM2, ENABLE);

			do
			{
				get_num(key_buf);
				ID = atoi(key_buf);
			} while (!(ID < 300));//输入ID必须小于模块容量最大的数值

			ensure = PS_StoreChar(CharBuffer2, ID);//储存模板
			if (ensure == 0x00)
			{

				OLED_Clear();
				OLED_ShowChinese(0, 26, "录入指纹成功");
				OLED_Update();
				OLED_Display_On();
				TIM_Cmd(TIM2, ENABLE);

				return;
			}
			else { processnum = 0;ShowErrMessage(ensure); }
			break;
		}

		delay_ms(400);
		if (i == 5)//超过5次没有按手指则退出
		{
			break;
		}
	}
}

// 删除指纹
void del_FR(void)
{
	u8  ensure;
	u16 num;
	char key_buf[10] = { 0 };
	OLED_Clear();
	OLED_ShowChinese(32, 0, "删除指纹");
	OLED_ShowChinese(0, 16, "请输入储存");
	OLED_ShowString(16 * 5, 16, "ID,", OLED_8X16);
	OLED_ShowString(0, 32, "0 =< ID <= 299", OLED_8X16);
	OLED_Update();
	OLED_Display_On();
	TIM_Cmd(TIM2, ENABLE);


	get_num(key_buf);//获取返回的数值
	num = atoi(key_buf);

	ensure = PS_DeletChar(num, 1);//删除单个指纹
	if (ensure == 0)
	{
		OLED_Clear();
		OLED_ShowChinese(0, 26, "删除指纹成功");
		OLED_Update();
		OLED_Display_On();
		TIM_Cmd(TIM2, ENABLE);
	}
	else
		ShowErrMessage(ensure);
}

void clear_FR(void)
{
	u8  ensure;
	ensure = PS_Empty();//清空指纹
	if (ensure == 0)
	{
		OLED_Clear();
		OLED_ShowChinese(0, 26, "清空指纹成功");
		OLED_Update();
		OLED_Display_On();
		TIM_Cmd(TIM2, ENABLE);

	}
	else
		ShowErrMessage(ensure);
}


// 密码操作
void cipher_operate(void)
{

	u32 unlock_pwd = *(__IO u32*)0x08010004;
	u32 pwd, pwd1;
	OLED_Clear();
	OLED_Display_On();
	OLED_ShowChinese(24, 10, "请输入密码");
	OLED_Update();
	char key_buf[10] = { 0 };
	get_num(key_buf);
	pwd = atoi(key_buf);

	OLED_ShowChinese(24 - 8, 10, "再次输入密码");
	OLED_Update();
	get_num(key_buf);
	pwd1 = atoi(key_buf);

	printf("pwd = %d, pwd1 = %d\n", pwd, pwd1);

	if (pwd != pwd1)
	{
		OLED_ShowChinese(24 - 8 - 8, 10, "两次密码不一致");
		OLED_Update();
		delay_ms(3000);
		return;
	}

	//解除写保护
	FLASH_Unlock();

	FLASH_ProgramWord(0x08010004, pwd);

	//添加写保护
	FLASH_Lock();
	OLED_Clear();
	OLED_ShowChinese(24 - 8, 10, "密码设置成功");
	OLED_Update();
	TIM_Cmd(TIM2, ENABLE);
	delay_ms(2000);

}


void work_fr(void)
{
	char key_buf[10] = { 0 };
	int select;
	// printf("1、录指纹\n2、删除指纹\n3、清空指纹\n4、修改密码\n5、返回\n");
	OLED_Clear();
	OLED_Printf(0, 0, OLED_8X16, "1");
	OLED_Printf(0, 16, OLED_8X16, "2");
	OLED_Printf(0, 32, OLED_8X16, "3");
	OLED_Printf(0, 48, OLED_8X16, "4");
	OLED_ShowChinese(16, 0, "、录指纹");
	OLED_ShowChinese(16, 16, "、删除指纹");
	OLED_ShowChinese(16, 32, "、清空指纹");
	OLED_ShowChinese(16, 48, "、修改密码");
	OLED_Update();
	OLED_Display_On();
	TIM_Cmd(TIM2, ENABLE);
	get_num(key_buf);
	select = atoi(key_buf);
	switch (select)
	{
	case 1:add_FR();break;
	case 2:del_FR();break;
	case 3:clear_FR();break;
	case 4:cipher_operate();break;
	case 5:break;
	}
}
