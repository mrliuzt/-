#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h"

#define headPack 0xA5
#define tailPack 0x5A

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[16];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
//只发送的话配置--------------------------
	//打开RCC,UART和GPIO的
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//配置io口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//配置TX为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//配置接收部分
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置RX为上拉输入或浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//配置接收部分
	//配置USART
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;//硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//串口模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//校验位
	USART_InitStruct.USART_StopBits =USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长，不需要校验，字长写8位
	
	USART_Init(USART3, &USART_InitStruct);
	
//配置接收部分中断
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//打开接收的串口中断
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组占比，一个程序只能一种
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//开通通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);//设置通道和优先级
	
//配置接收部分
	//串口使能
	USART_Cmd(USART3,ENABLE);
//只发送的话配置--------------------------
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3,Byte);
	//没有发送完就卡在这里
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);//USART_FLAG_TXE:  发送寄存器空标志位,发送完置高电平,写DR就不需要手动清零
}

void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i = 0;i< Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}
//字符串自带一个结束标志位，所以不用再定义长度.字符'\0' 等价于 数字0
//使用例子：Serial_SendString("OK");
//如果需要换行，需在后面加上 \r\n  例如Serial_SendString("OK\r\n");
void Serial_SendString(char *String)
{
	uint16_t i;
	for(i = 0;String[i] != '\0' ;i++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint16_t i;
	for(i = 0;i<Length ;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10 + '0');
	}
}
//printf的第一种方法，只能重定向在一个UART
//将printf函数重定向到串口
//因为fputc函数是底层函数，printf就是将ch一个个打印出来的
//使用例子：printf("Num = %d\r\n",666);
//添加<stdio.h>

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}
//printf的第二种方法
//	char String[100];
//	sprintf(String,"Num = %d\r\n",666);//指定打印位置
//	Serial_SendString(String);


//printf的第三种方法
//例子：Serial_printf("Num = %d\r\n",666);
//添加<stdarg.h>//添加<stdio.h>
void Serial_printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}

//数据包发送
void Serial_SendPacket(void)
{
	Serial_SendByte(headPack);
	Serial_SendArray(Serial_TxPacket,4);
	Serial_SendByte(tailPack);
}

uint8_t Get_Serial_RxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}



void USART3_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t RxPacket_count = 0;

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);
		
		if(RxState==0)//判断是不是包头
		{
			if(RxData == headPack)
			{
				RxState = 1;
				RxPacket_count = 0;
			}
		}
		else if(RxState == 1)
		{
			Serial_RxPacket[RxPacket_count] = RxData;
			RxPacket_count++;
			if(RxPacket_count>=16)//接收16次
			{
				RxState = 2;
			}
		}
		else if(RxState == 2)//判断校验位
		{
			if(RxData == (checkPack & 0x000000FF))
			{
				RxState = 3;
			}
		}
		else if(RxState == 3)
		{
			if(RxData == tailPack)
			{
				RxState = 0;
				Serial_RxFlag = 1;
			}
		}
		
	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}
