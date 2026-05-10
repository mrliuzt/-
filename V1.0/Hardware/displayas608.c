#include "stm32f10x.h"                  // Device header
#include "AS608.h"
#include "oled.h"
#include "Delay.h"
#include <stdio.h>
#include "Myview.h"
#include "Serial.h"
extern uint8_t AS608_RxFlag;
extern uint8_t wire_as;
uint8_t FR_num = 0xFF;

//œ‘ æ»∑»œ¬Î¥ÌŒÛ–≈œ¢
void ShowErrMessage(u8 ensure)
{
	OLED_Clear();//«Â∆¡
	OLED_ShowChinese(25,5,(char*)EnsureMessage(ensure));
			
	OLED_Update();//∏¸–¬œ‘ æ
	Delay_ms(1000);
}
uint8_t FR_GET_NUM(void)
{
		//¿∂—¿«£ ÷
		if(Get_Serial_RxFlag())
		{
			switch(checkPack)
			{
				//0
				case 0:
					FR_num = 0;
				break;
				//1
				case 11:
					FR_num = 1;
				break;
				//2
				case 12:
					FR_num = 2;
				break;
				//3
				case 13:
					FR_num = 3;
				break;
				//4
				case 14:
					FR_num = 4;
				break;
				//5
				case 15:
					FR_num = 5;
				break;
				//6
				case 16:
					FR_num = 6;
				break;
				//7
				case 17:
					FR_num = 7;
				break;
				//8
				case 18:
					FR_num = 8;
				break;
				//9
				case 19:
					FR_num = 9;
				break;
				//back
				case 50:
					FR_num = 0xFE;
				break;
			}
		}

		//¿∂—¿º√¥√”¥Û
		extern uint8_t matrix_num;
		extern uint8_t per;
		if(matrix_num <= 9)
		{
			FR_num = matrix_num;
			matrix_num = 0xFF;
			per = 0;
		}
		//¿∂—¿DELµ„
		if(matrix_num == 10)
		{
			FR_num = 0xFF;
			matrix_num = 0xFF;
			per = 0;
		}

	uint8_t num;
	num = FR_num;
	return num;
}
//¬º÷∏Œ∆
void Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	u16 ID;
	usart1_Init();
	OLED_Init();
	OLED_Clear();//«Â∆¡
	while(1)
	{
		Matrix_row();
		HC_XR();
		if(Get_Serial_RxFlag())
		{
			switch(checkPack)
			{	
				//Back/
				case 50:
					wire_as = 0xFF;
				break;
			}
		}
		switch (processnum)
		{
			case 0:
				i++;
				OLED_Clear();//«Â∆¡
				OLED_ShowChinese(35,5,"¬º»Î÷∏Œ∆ ");
				OLED_ShowChinese(35,25,"«Î∞¥÷∏Œ∆");	
				OLED_ShowChinese(99,48,"∑µªÿ");	
				OLED_Update();//∏¸–¬œ‘ æ
				ensure=PS_GetImage();
				Delay_ms(400);
				if(ensure==0x00) //¬º»Î≥…π¶
				{
//					BEEP=1;//∑‰√˘∆˜œÏ
					ensure=PS_GenChar(CharBuffer1);//…˙≥…Ãÿ’˜
//					BEEP=0;//∑‰√˘∆˜πÿ±’
					if(ensure==0x00)
					{
						Delay_ms(400);
						OLED_Clear();//«Â∆¡
						OLED_ShowChinese(35,5,"÷∏Œ∆’˝≥£ ");
						OLED_Update();//∏¸–¬œ‘ æ	
						Delay_ms(400);
						i=0;
						processnum=1;//Ã¯µΩµ⁄∂˛≤Ω						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);
				OLED_Clear();//«Â∆¡
				break;	
			case 1:
				i++;
				Delay_ms(400);
				OLED_Clear();//«Â∆¡
				OLED_ShowChinese(5,5,"«Î‘Ÿ∞¥“ª¥Œ÷∏Œ∆ ");
				OLED_Update();//∏¸–¬œ‘ æ		
				Delay_ms(400);
				ensure=PS_GetImage();
				Delay_ms(1000);
				if(ensure==0x00) 
				{
//					BEEP=1;
					ensure=PS_GenChar(CharBuffer2);//…˙≥…Ãÿ’˜
//					BEEP=0;
					if(ensure==0x00)
					{
						OLED_Clear();//«Â∆¡
						OLED_ShowChinese(35,5,"÷∏Œ∆’˝≥£ ");
						OLED_Update();//∏¸–¬œ‘ æ	
						Delay_ms(400);
						i=0;
						processnum=2;//Ã¯µΩµ⁄»˝≤Ω
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				//OLED_Clear();//«Â∆¡
				break;

			case 2:		
				OLED_ShowChinese(15,5,"∂‘±»¡Ω¥Œ÷∏Œ∆ ");
				OLED_Update();//∏¸–¬œ‘ æ	
				Delay_ms(400);
				ensure=PS_Match();//æ´»∑±»∂‘¡Ω√∂÷∏Œ∆Ãÿ’˜ PS_Match
				if(ensure==0x00) 
				{
					OLED_Clear();//«Â∆¡
					OLED_ShowChinese(15,5,"¡Ω¥Œ÷∏Œ∆“ª—˘ ");
					OLED_Update();//∏¸–¬œ‘ æ	
					Delay_ms(400);
					processnum=3;//Ã¯µΩµ⁄Àƒ≤Ω
				}
				else 
				{
					OLED_Clear();//«Â∆¡
					OLED_ShowChinese(5,5,"∂‘±» ß∞‹«Î÷ÿ¬º ");
					OLED_Update();//∏¸–¬œ‘ æ	
					ShowErrMessage(ensure);
					i=0;
					OLED_Clear();//«Â∆¡
					processnum=0;//Ã¯ªÿµ⁄“ª≤Ω		
				}
				Delay_ms(1200);
				OLED_Clear();//«Â∆¡
				break;
			case 3:
					OLED_ShowChinese(5,5,"…˙≥…÷∏Œ∆ƒ£∞Â ");
					OLED_Update();//∏¸–¬œ‘ æ	
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{			
					OLED_Clear();//«Â∆¡
					OLED_ShowChinese(35,5,"…˙≥…≥…π¶ ");
					OLED_Update();//∏¸–¬œ‘ æ	
					processnum=4;//Ã¯µΩµ⁄ŒÂ≤Ω
				}else {processnum=0;ShowErrMessage(ensure);}
				Delay_ms(1200);
				break;		
			case 4:	
				OLED_Clear();//«Â∆¡
				OLED_ShowChinese(25,5,"«Î ‰»Î¥¢¥Ê∫≈ ");
				OLED_Update();//∏¸–¬œ‘ æ	
				do
					ID=FR_GET_NUM();// ‰»ÎID∫≈
				while(!(ID<100));// ‰»ÎID±ÿ–Î–°”⁄ƒ£øÈ»›¡ø◊Ó¥Ûµƒ ˝÷µ
				ensure=PS_StoreChar(CharBuffer2,ID);//¥¢¥Êƒ£∞Â
				FR_num = 0xFF;
				if(ensure==0x00) 
				{			
					OLED_Clear();//«Â∆¡
					OLED_ShowChinese(25,5,"¬º»Î≥…π¶ ");
					OLED_ShowNum(45,25,ID,2,OLED_8X16);
					OLED_ShowChinese(60,25,"∫≈÷∏Œ∆");
					OLED_Update();//∏¸–¬œ‘ æ	
					wire_as = 0xFF;
					Delay_ms(1500);	
				}else {processnum=0;ShowErrMessage(ensure);}
				OLED_Clear();//«Â∆¡					
				break;	
		}
		if(wire_as == 0xFF)
		break;
	}
}
//À¢÷∏Œ∆
uint8_t press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	uint8_t as608_key = 0xFF;//÷∏Œ∆Ω‚À¯±Í÷æŒª
	usart1_Init();
				ensure=PS_GetImage();
//						OLED_Clear();
//						OLED_ShowChinese(5,5,"’˝‘⁄ºÏ≤‚÷∏Œ∆ ");
//						OLED_Update();//∏¸–¬œ‘ æ	
//						Delay_ms(100);	
			if(ensure==0x00)//ªÒ»°ÕºœÒ≥…π¶ 
			{	
				ensure=PS_GenChar(CharBuffer1);
				if(ensure==0x00) //…˙≥…Ãÿ’˜≥…π¶
				{		
					
					ensure=PS_HighSpeedSearch(CharBuffer1,0,100,&seach);//À—À˜0µΩ100ID∫≈
					if(ensure==0x00)//À—À˜≥…π¶
					{				
//						OLED_Clear();
////						OLED_ShowChinese(35,25,"Ω‚À¯÷– ");
////						OLED_Update();//∏¸–¬œ‘ æ	
////						Delay_ms(1800);
////						OLED_ShowChinese(35,45,"“—Ω‚À¯ ");
////						OLED_ShowHexNum(5,5,seach.pageID,3,OLED_8X16);
////						OLED_ShowHexNum(35,5,seach.mathscore,3,OLED_8X16);
//						OLED_Update();//∏¸–¬œ‘ æ			
//						Delay_ms(1800);
						as608_key = 1;
						
					}
					else {
						OLED_ClearArea(20, 47, 120, 16);
						OLED_ShowChinese(20, 47,"÷∏Œ∆≤ª∆•≈‰");
						OLED_Update();//∏¸–¬œ‘ æ		
						Delay_s(1);
						wire_time_count(15);
						add_fail_count();
						OLED_Clear();
					}				
			  }
				else
					ShowErrMessage(ensure);		
			}

		return as608_key;
}

////…æ≥˝÷∏Œ∆
//void Del_FR(void)
//{
//		static u16 ValidN;
//		u8  ensure;
//		u16 num=10;
//		OLED_Clear();
//		OLED_ShowChinese(35,5,"…æ≥˝÷∏Œ∆ ");
//		OLED_ShowChinese(25,25," ‰»Î÷∏Œ∆¥¢¥Ê∫≈");	
//		OLED_ShowChinese(99,48,"»∑»œ");	
//		OLED_Update();//∏¸–¬œ‘ æ
//		Delay_ms(1200);
////	num=GET_NUM();//ªÒ»°∑µªÿµƒ ˝÷µ
//	if(num==0xFFFF)
//		goto MENU ; //∑µªÿ÷˜“≥√Ê
//	else if(num==0xFF00)
//		ensure=PS_Empty();//«Âø’÷∏Œ∆ø‚
//	else 
//		ensure=PS_DeletChar(num,1);//…æ≥˝µ•∏ˆ÷∏Œ∆
//	if(ensure==0)
//	{
//		OLED_Clear();
//		OLED_ShowChinese(25,25,"…æ≥˝÷∏Œ∆≥…π¶");	
//		OLED_ShowChinese(70,48," £”‡");	
//		Delay_ms(1200);
//		OLED_Update();//∏¸–¬œ‘ æ
//	}
//  else
//  {ShowErrMessage(ensure);	}


//	PS_ValidTempleteNum(&ValidN);//∂¡ø‚÷∏Œ∆∏ˆ ˝
//	OLED_ShowNum(78,48,100-ValidN,3,OLED_8X16);
//    OLED_Update();//∏¸–¬œ‘ æ
//	Delay_ms(1200);
//	
//MENU:	
//	OLED_Clear();
//}

