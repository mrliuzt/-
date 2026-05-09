#ifndef __Myflash_H
#define __Myflash_H


//¶ÁÈ¡È«×Ö
uint32_t MyFLASH_ReadWord(uint32_t Address);
//¶ÁÈ¡°ë×Ö
uint16_t MyFLASH_ReadHalfWord(uint32_t Address);
//¶ÁÈ¡Ò»¸ö×Ö½Ú
uint8_t MyFLASH_ReadByte(uint32_t Address);
//È«²Á³ý
void MyFLASH_EraseAllPages(void);
//Ò³²Á³ý
void MyFLASH_ErasePage(uint32_t PageAddress);
//Ð´È«×Ö
void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data);
//Ð´°ë×Ö
void MyFLASH_ProgramHalfWord(uint32_t Address,uint32_t Data);

#endif
