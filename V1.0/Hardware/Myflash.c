#include "stm32f10x.h"                  // Device header




//读取全字
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));
}

//读取半字
uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));
}

//读取一个字节
uint8_t MyFLASH_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));
}
//全擦除
void MyFLASH_EraseAllPages(void)
{
	FLASH_Unlock();//解锁
	FLASH_EraseAllPages();//全擦除
	FLASH_Lock();//锁上
}

//页擦除
void MyFLASH_ErasePage(uint32_t PageAddress)
{
	FLASH_Unlock();//解锁
	FLASH_ErasePage(PageAddress);//页擦除
	FLASH_Lock();//锁上
}
//写全字
void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data)
{
	FLASH_Unlock();//解锁
	FLASH_ProgramWord(Address,Data);
	FLASH_Lock();//锁上
}
//写半字
void MyFLASH_ProgramHalfWord(uint32_t Address,uint32_t Data)
{
	FLASH_Unlock();//解锁
	FLASH_ProgramHalfWord(Address,Data);
	FLASH_Lock();//锁上
}












