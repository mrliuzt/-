#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"
//读写Flash的最后一页


//数组的第一位是标志位，不能去更改
uint16_t Store_Data[512];//正好对应一页1024字节

//Flash最后一页初始化
//第一个半字为0xA5A5,其他都为0
void Store_Init(void)
{
	if(MyFLASH_ReadHalfWord(0x0800FC00) != 0xA5A5)
	{
		MyFLASH_ErasePage(0x800FC00);
		MyFLASH_ProgramHalfWord(0x800FC00,0xA5A5);//置标志位
		//i从1开始，防止标志位清零
		for(uint16_t i = 1;i < 512; i++)
		{
			MyFLASH_ProgramHalfWord(0x800FC00 + i*2,0x0000);//一个半字占用2个地址，一个地址是8位
		}
	}
	
	//上电的时候转存到SRAM数组里(实现掉电不丢失）
	for(uint16_t i = 0;i < 512; i++)
	{
		Store_Data[i] = MyFLASH_ReadHalfWord(0x800FC00 + i*2);
	}
	
	
}

//保存数据
//每次更改数据后都要保存一下
void Store_Save(void)
{
	MyFLASH_ErasePage(0x800FC00);
	for(uint16_t i = 0;i < 512; i++)
	{
		MyFLASH_ProgramHalfWord(0x800FC00 + i*2,Store_Data[i]);//备份数组里的数据到FLASH
	}
}
//数据清零
void Store_Clear(void)
{
	//i从1开始，防止标志位清零
	for(uint16_t i = 1;i < 512; i++)
	{
		Store_Data[i] = 0x0000;
	}
	Store_Save();
}






