#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "RCC522.h"
#include "oled.h"
#include "Myview.h"
#include "Serial.h"
#include "Store.h"

extern uint8_t wire_ic;

//RC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
//u8  card_numberbuf0[5]={0x9A,0x63,0x01,0x01,0xF9};
u8  card_numberbuf0[5];
u8  card_numberbuf1[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];

void Add_Init(void)
{
		for(uint8_t p = 3;p < 7; p++)
		{
			card_numberbuf0[p-3] = Store_Data[p]%256;
			card_numberbuf0[p+1-3] = Store_Data[p]/256;
		}
}

//录入新卡
void Add_Rfid(void)
{
			RCC522_Init();
			u8 status=1;
			u8 card_size;
			OLED_Clear();
			OLED_ShowChinese(35,5,"录入卡片 ");
			OLED_ShowChinese(35,25,"请放入新卡片");	
			OLED_ShowChinese(99,48,"返回");	
			OLED_Update();//更新显示	
			RC522_Initializtion();			//初始化RC522
	while(1)
	{
		if(Get_Serial_RxFlag())
		{
			switch(checkPack)
			{	
				//Back/
				case 50:
					wire_ic = 0xFF;
				break;
			}
		}
		AntennaOn();//开启天线,
		status=RC522_Request(0x52, card_pydebuf);			//寻卡
		if(status==0)		//如果读到卡
		{
			OLED_Clear();
			OLED_ShowChinese(35,25,"读卡成功");	
			OLED_Update();//更新显示	
			status=RC522_Anticoll(card_numberbuf0);			//防撞处理			
			card_size=RC522_SelectTag(card_numberbuf0);	//选卡
			status=RC522_Auth(0x60, 45, card_key0Abuf, card_numberbuf0);	//验卡
			if(status == 0)
			{
				status=RC522_Write(45, card_writebuf);				//写卡（写卡要小心，特别是各区的块3）
				status=RC522_Read(45, card_readbuf);					//读卡	
				AntennaOff();
				OLED_Clear();
				OLED_ShowChinese(45,48,"录入成功");
				OLED_Update();//更新显示	
				for(uint8_t w = 3;w < 7; w++) 
				{Store_Data[w] = card_numberbuf0[w-3]+card_numberbuf0[w+1-3]*256;}
				Store_Save();
				wire_ic = 0xFF;
				Delay_ms(1000);
			}
		}
		if(wire_ic==0xFF)
		{
			OLED_Clear();
			break;
		}
	}
}


//rfid卡锁
u8 RC522_lock(void)
{
		RCC522_Init();
		u8 i,j,status=1,card_size;
		uint8_t ic_key = 0xFF;//ic卡解锁标志位
		u8 count0 = 0,count1 = 0;
		RC522_Initializtion();	
		//ic卡解锁
		AntennaOn();
		status=RC522_Request(0x52, card_pydebuf);			//寻卡
		if(status==0)		//如果读到卡
		{
			status=RC522_Anticoll(card_numberbuf1);			//防撞处理			
			card_size=RC522_SelectTag(card_numberbuf1);	//选卡
			status=RC522_Auth(0x60, 45, card_key0Abuf, card_numberbuf1);	//验卡
//			if(status == 0)
//			{
				status=RC522_Read(45, card_readbuf);					//读卡
				AntennaOff();
				for(i=0;i<5;i++)
				{
					if(card_numberbuf1[i]==card_numberbuf0[i])
					{
						count0++;	
					}				
				}
				for(j=0;j<18;j++)
				{
					if(card_readbuf[j]==card_writebuf[j])
					{
						count1++;	
					}				
				}
				if(count0>=5&&count1>=16)
				{
					count0=0;
					count1=0;
					
					Delay_ms(500);
					ic_key = 1;//解锁成功
					
				}
				else 
				{
					OLED_ClearArea(20, 47, 120, 16);
					OLED_ShowChinese(23, 47,"卡号不匹配");
					OLED_Update();
					Delay_s(1);
					AntennaOff();
					count0=0;
					count1=0;
					wire_time_count(15);
					add_fail_count();
					
				}
//			}	
		}
	
	return ic_key;
	
}

