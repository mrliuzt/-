#ifndef __AS608_H
#define __AS608_H

#define CharBuffer1 0x01	
#define CharBuffer2 0x02

typedef struct  
{
	u16 pageID;//指纹ID
	u16 mathscore;//匹配得分
}SearchResult;

void usart1_Init(void);
void AS608_SendByte(uint8_t Byte);
void SendHead(void);
void AS608_SendByte(uint8_t Byte);
void SendAddr(void);
void SendFlag(uint8_t flag);
void SendLength(int length);
void Sendcmd(uint8_t cmd);
void SendCheck(uint8_t check);
uint8_t Get_AS608_RxFlag(void);
//功能函数
uint8_t PS_GetImage(void);//录入图像 PS_GetImage
u8 PS_GenChar(u8 BufferID);//生成特征 PS_GenChar
u8 PS_Match(void);//精确比对两枚指纹特征 PS_Match
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//搜索指纹 PS_Search
u8 PS_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//高速搜索指纹
u8 PS_RegModel(void);//合并特征（生成模板）PS_RegModel
u8 PS_StoreChar(u8 BufferID,u16 PageID);//储存模板 PS_StoreChar
u8 PS_DeletChar(u16 PageID,u16 N);//删除模板 PS_DeletChar
u8 PS_Empty(void);//清空指纹库 PS_Empty
u8 PS_ValidTempleteNum(u16 *ValidN);//读有效模板个数 PS_ValidTempleteNum
char *EnsureMessage(u8 ensure);//模块应答包确认码信息解析

#endif
