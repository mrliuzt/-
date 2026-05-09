#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "string.h"
#include "AS608.h"
#include "timer.h"
uint8_t ensure_ttt;
uint8_t  RxPacket_check_1;
uint8_t  RxPacket_check_2;
uint32_t AS608Addr = 0XFFFFFFFF; //AS608默认地址
uint8_t AS608_RxPacket[400];
uint16_t RxPacket_count;
u8 AS608_RxFlag;
void usart1_Init(void)
{
	//打开usart1的rcc
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//打开gpioa的rcc
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置io口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//配置TX为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置RX为上拉输入或浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置USART
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 57600;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;//硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//串口模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//校验位
	USART_InitStruct.USART_StopBits =USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长，不需要校验，字长写8位
	
	USART_Init(USART1, &USART_InitStruct);
	
	//配置接收部分中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//打开接收的串口中断
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组占比，一个程序只能一种
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//开通通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级
	NVIC_Init(&NVIC_InitStructure);//设置通道和优先级
	
	//串口使能
	USART_Cmd(USART1,ENABLE);
	
	TIM3_Int_Init(1000-1,8400-1);		//100ms中断
	RxPacket_count=0;		//清零
	TIM_Cmd(TIM3, DISABLE); //关闭定时器

}

 void AS608_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	//没有发送完就卡在这里
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//USART_FLAG_TXE:  发送寄存器空标志位,发送完置高电平,写DR就不需要手动清零
}

//写AS608发送的数据包格式

//发送包头
 void SendHead(void)
{
	AS608_SendByte(0xEF);
	AS608_SendByte(0x01);
}

//发送地址
 void SendAddr(void)
{
	AS608_SendByte(AS608Addr>>24);
	AS608_SendByte(AS608Addr>>16);
	AS608_SendByte(AS608Addr>>8);
	AS608_SendByte(AS608Addr);
}

//发送包标识,
 void SendFlag(uint8_t flag)
{
	AS608_SendByte(flag);
}

//发送包长度
 void SendLength(int length)
{
	AS608_SendByte(length>>8);
	AS608_SendByte(length);
}

//发送指令码
 void Sendcmd(uint8_t cmd)
{
	AS608_SendByte(cmd);
}

//发送校验和
 void SendCheck(uint8_t check)
{
	AS608_SendByte(check>>8);
	AS608_SendByte(check);
}
//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
 u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0]=0xef;					str[1]=0x01;
	str[2]=AS608Addr>>24;	str[3]=AS608Addr>>16;		
	str[4]=AS608Addr>>8;	str[5]=AS608Addr;				
	str[6]=0x07;					str[7]='\0';
	RxPacket_count=0;
	while(--waittime)
	{
		Delay_ms(1);
		if(RxPacket_count&0x8000)//接收到一次数据
		{
			RxPacket_count=0;
			data=strstr((const char*)AS608_RxPacket,(const char*)str);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}
//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于 ImageBuffer。
//模块返回确认字
//确认码=00H 表示录入成功； 
//确认码=01H 表示收包有错； 
//确认码=02H 表示传感器上无手指； 
//确认码=03H 表示录入不成功；
u8 PS_GetImage(void)
{
	    u16 temp;
		u8 ensure;
		u8  *data;
		SendHead();
		SendAddr();
		SendFlag(0x01);//命令包标识
		SendLength(0x03);
		Sendcmd(0x01);
		temp =  0x01+0x03+0x01;
		SendCheck(temp);
		data=JudgeStr(2000);
		if(data)
		{
			ensure=data[9];
		}
		else
			ensure=0xff;
		return ensure;
}

//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2			 
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//模块返回确认字
//确认码=00H 表示生成特征成功； 
//确认码=01H 表示收包有错； 
//确认码=06H 表示指纹图像太乱而生不成特征；
u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
    u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	AS608_SendByte(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];//确认码
	}
	else
		ensure=0xff;
	return ensure;
}
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
//确认码=00H 表示指纹匹配； 
//确认码=01H 表示收包有错； 
//确认码=08H 表示指纹不匹配；
u8 PS_Match(void)
{
	u16 temp;
    u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];//确认码
	}
	else
		ensure=0xff;
	return ensure;
}

//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2	
//说明:  模块返回确认字
//确认码=00H 表示合并成功； 
//确认码=01H 表示收包有错； 
//确认码=0aH 表示合并失败（两枚指纹不属于同一手指）；
u8 PS_RegModel(void)
{
	u16 temp;
    u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x05);//指令码
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];//确认码
	}
	else
		ensure=0xff;	
	return ensure;
}
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。			
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
//确认码=00H 表示储存成功； 
//确认码=01H 表示收包有错； 
//确认码=0bH 表示 PageID 超出指纹库范围； 
//确认码=18H 表示写 FLASH 出错；
u8 PS_StoreChar(u8 BufferID,u16 PageID)
{
	u16 temp;
    u8  ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	AS608_SendByte(BufferID);
	AS608_SendByte(PageID>>8);
	AS608_SendByte(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];//确认码
	}
	else
		ensure=0xff;	
	return ensure;	
}
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
//确认码=00H 表示删除模板成功； 
//确认码=01H 表示收包有错； 
//确认码=10H 表示删除模板失败；
u8 PS_DeletChar(u16 PageID,u16 N)
{
	u16 temp;
    u8  ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	AS608_SendByte(PageID>>8);
	AS608_SendByte(PageID);
	AS608_SendByte(N>>8);
	AS608_SendByte(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];//确认码
	}
	else
		ensure=0xff;	
	return ensure;	
}
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
//确认码=00H 表示清空成功； 
//确认码=01H 表示收包有错； 
//确认码=11H 表示清空失败；
u8 PS_Empty(void)
{
	u16 temp;
    u8 ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];//确认码
	}
	else
		ensure=0xff;	
	return ensure;	
}
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。			
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
//确认码=00H 表示搜索到； 
//确认码=01H 表示收包有错； 
//确认码=09H 表示没搜索到；此时页码与得分为 0
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
    u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	AS608_SendByte(BufferID);
	AS608_SendByte(StartPage>>8);
	AS608_SendByte(StartPage);
	AS608_SendByte(PageNum>>8);
	AS608_SendByte(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;		
}
//高速搜索PS_HighSpeedSearch
//功能：以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
//		  若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
//		  很好的指纹，会很快给出搜索结果。
//参数:  BufferID， StartPage(起始页)，PageNum（页数）
//说明:  模块返回确认字+页码（相配指纹模板）
//确认码=00H 表示搜索到； 
//确认码=01H 表示收包有错； 
//确认码=09H 表示没搜索到；此时页码与得分为 0
u8 PS_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
    u8  ensure;
	u8* data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	AS608_SendByte(BufferID);
	AS608_SendByte(StartPage>>8);
	AS608_SendByte(StartPage);
	AS608_SendByte(PageNum>>8);
	AS608_SendByte(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//读有效模板个数 PS_ValidTempleteNum
//功能：读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
u8 PS_ValidTempleteNum(u16 *ValidN)
{
	u16 temp;
    u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01+0x03+0x1d;
	SendCheck(temp);
    data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	return ensure;
}
//模块应答包确认码信息解析
//功能：解析确认码错误信息返回信息
//参数: ensure
char *EnsureMessage(u8 ensure) 
{
	char *p;
	switch(ensure)
	{
		case  0x00:
			p="成功";break;		
		case  0x01:
			p="数据包接收错误";break;
		case  0x02:
			p="没有手指按下";break;
		case  0x03:
			p="录入指纹图像失败";break;
		case  0x04:
			p="指纹图像太干、太淡而生不成特征";break;
		case  0x05:
			p="指纹图像太湿、太糊而生不成特征";break;
		case  0x06:
			p="指纹图像太乱而生不成特征";break;
		case  0x07:
			p="指纹特征点太少";break;
		case  0x08:
			p="指纹不匹配";break;
		case  0x09:
			p="没搜索到指纹!";break;
		case  0x0a:
			p="特征合并失败";break;
		case  0x0b:
			p="访问指纹库时地址序号超出指纹库范围";
		case  0x10:
			p="删除模板失败";break;
		case  0x11:
			p="清空指纹库失败";break;	
		case  0x15:
			p="缓冲区内没有有效原始图而生不成图像";break;
		case  0x19:
			p="未定义错误";break;
		case  0x1a:
			p="无效寄存器号";break;
		case  0x1f:
			p="指纹库满";break;
		case  0x20:
			p="地址错误";break;
		default :
			p="模块确认码有误";break;
	}
 return p;	
}
uint8_t Get_AS608_RxFlag(void)
{
	if(AS608_RxFlag == 1)
	{
		Delay_ms(200);
		AS608_RxFlag=0;
		return 1;
	}
	return 0;
}


//通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度   	 
void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	 
 
		res =USART_ReceiveData(USART1);		
		if((RxPacket_count&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 		
			if(RxPacket_count<400)		//还可以接收数据
			{
				TIM_SetCounter(TIM3,0);//计数器清空        				 
				if(RxPacket_count==0)		
					TIM_Cmd(TIM3, ENABLE);  //使能定时器
				AS608_RxPacket[RxPacket_count++]=res;		//记录接收到的值	 
			}else 
			{
				RxPacket_count|=1<<15;					//强制标记接收完成
			} 
		}  	
	}										 
}  
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		RxPacket_count|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			TIM_Cmd(TIM3, DISABLE);  //关闭TIM3
		}
}




////接收AS608应答的数据包格式
//void USART1_IRQHandler(void)
//{
//	static uint8_t RxState = 0;
//	static uint8_t RxPacket_count = 0;
//	static uint16_t RxPacket_check = 0x07;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);
//		if(RxState==0)//判断是不是包头
//		{
//			
//			if(RxData == 0xEF)
//			{
//		
//				RxState = 1;
//				RxPacket_count = 0;
//			}
//		}
//		else if(RxState ==1)
//		{
//			if(RxData == 0x01)
//			{
//				
//				RxState = 2;
//			}
//		}
//		else if(RxState ==2)
//		{
//			if(RxData == 0xFF)
//			{
//				RxState = 3;
//			}
//		}
//		else if(RxState ==3)
//		{
//			if(RxData == 0xFF)
//			{
//				RxState = 4;
//			}
//		}
//		else if(RxState ==4)
//		{
//			if(RxData == 0xFF)
//			{
//				RxState = 5;
//			}
//		}
//		else if(RxState ==5)
//		{
//			if(RxData == 0xFF)
//			{
//				RxState = 6;
//			}
//		}
//		else if(RxState ==6)
//		{
//			if(RxData == 0x07)
//			{
//			
//				RxState = 7;
//			}
//		}
//		else if(RxState ==7)
//		{
//			if(RxData == 0x00)
//			{
//			
//				RxState = 8;
//			}
//		}
//		else if(RxState == 8)
//		{
//			if (RxData == ((RxPacket_check & 0xFF00)>>8))
//			{
//			
//				RxState = 9;
//			}
//			else
//			{
//				AS608_RxPacket[RxPacket_count] = RxData;
//				RxPacket_check = RxPacket_check + AS608_RxPacket[RxPacket_count];
//				RxPacket_count ++;
//			}
//		}
//		else if(RxState == 9)
//		{
//			if(RxData == (RxPacket_check & 0x00FF))
//			{
//				RxState = 0;
//				
//				AS608_RxFlag = 1;
//			}
//		}
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	}
//}
