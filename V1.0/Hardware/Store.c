#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"


uint16_t Store_Data[512];//存储对应一页1024字节

void Store_Init(void)
{
	if(MyFLASH_ReadHalfWord(0x0800FC00) != 0xA5A5)
	{
		MyFLASH_ErasePage(0x800FC00);
		MyFLASH_ProgramHalfWord(0x800FC00,0xA5A5);
		for(uint16_t i = 1;i < 512; i++)
		{
			MyFLASH_ProgramHalfWord(0x800FC00 + i*2,0x0000);
		}
		// 设置默认密码为 123456
		Store_Data[1] = 123456 % 65535;        // 低16位
		Store_Data[2] = 123456 / 65535;        // 高16位
		Store_Save();
	}
	
	for(uint16_t i = 0;i < 512; i++)
	{
		Store_Data[i] = MyFLASH_ReadHalfWord(0x800FC00 + i*2);
	}
}

void Store_Save(void)
{
	MyFLASH_ErasePage(0x800FC00);
	for(uint16_t i = 0;i < 512; i++)
	{
		MyFLASH_ProgramHalfWord(0x800FC00 + i*2,Store_Data[i]);
	}
}

void Store_Clear(void)
{
	for(uint16_t i = 1;i < 512; i++)
	{
		Store_Data[i] = 0x0000;
	}
	Store_Save();
}
