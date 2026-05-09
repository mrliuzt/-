#include "stm32f10x.h"                  // Device header
/*
引脚配置 GPIOB
 MISO  GPIO_Pin_14
 MOSI  GPIO_Pin_15
 SCK   GPIO_Pin_13
 SS	  GPIO_Pin_12
*/
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12,(BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_15,(BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_13,(BitAction)BitValue);
}


uint8_t MySPI_R_MISO(void)
{
	
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
	
}



void MySPI_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//配置io口（输出引脚为推挽输出，输入引脚为浮空输入或上拉输入)
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//SPI模式0初始化引脚
	MySPI_W_SS(1);//默认不选择从机
	MySPI_W_SCK(0);// 模式0默认SCK为低电平

	
}
//SPI起始信号
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}
//SPI终止信号
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}
//模式0软件模拟SPI
//SPI交换一个字节
//方法一
//优点:保留原来的参数
//缺点:效率慢
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i;
	uint8_t ByteRecevie = 0x00;//用来读取从机交换过来的数据
	//在SS下降沿后移出数据
	for(i = 0;i<8;i++)
	{
		MySPI_W_MOSI(ByteSend & (0x80>>i));//SPI高位先行,移出最高位
		MySPI_W_SCK(1);//上升沿后移入数据
		if(MySPI_R_MISO()==1)
		{
			ByteRecevie |= (0x80>>i);//移入到最高位
		}
		MySPI_W_SCK(0);//下降沿后移出数据
	}
	
	
	return ByteRecevie;
	
}
//方法二
//优点:效率快
//缺点:改变了原来的参数
uint8_t MySPI_SwapByte_better(uint8_t ByteSend)
{
	uint8_t i;
	//在SS下降沿后移出数据
	for(i = 0;i<8;i++)//移动8次
	{
		MySPI_W_MOSI(ByteSend & 0x80);//SPI高位先行,移出最高位
		ByteSend <<= 1;//左移一位，低位补零
		MySPI_W_SCK(1);//上升沿后移入数据
		if(MySPI_R_MISO()==1)
		{
			ByteSend |= 0x01;//移入的数据放最低位
		}
		MySPI_W_SCK(0);//下降沿后移出数据
	}
	
	return ByteSend;
}














