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

//ÏÔÊ¾È·ÈÏÂë´íÎóĞÅÏ¢
void ShowErrMessage(u8 ensure)
{
	OLED_Clear();//ÇåÆÁ
	OLED_ShowChinese(25,5,(char*)EnsureMessage(ensure));
			
	OLED_Update();//¸üĞÂÏÔÊ¾
	Delay_ms(1000);
}
uint8_t FR_GET_NUM(void)
{
		//À¶ÑÀÇ£ÊÖ
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

		//À¶ÑÀ¼Ã´ÃÓ´ó
		extern uint8_t matrix_num;
		extern uint8_t per;
		if(matrix_num <= 9 && per > 0)
		{
			FR_num = matrix_num;
			matrix_num = 0xFF;
			per = 0;
		}
		//À¶ÑÀDELµã
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
//Â¼Ö¸ÎÆ
void Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	u16 ID;
	usart1_Init();
	OLED_Init();
	OLED_Clear();//ÇåÆÁ
	while(1)
	{
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
				OLED_Clear();//ÇåÆÁ
				OLED_ShowChinese(35,5,"Â¼ÈëÖ¸ÎÆ ");
				OLED_ShowChinese(35,25,"Çë°´Ö¸ÎÆ");	
				OLED_ShowChinese(99,48,"·µ»Ø");	
				OLED_Update();//¸üĞÂÏÔÊ¾
				ensure=PS_GetImage();
				Delay_ms(400);
				if(ensure==0x00) //Â¼Èë³É¹¦
				{
//					BEEP=1;//·äÃùÆ÷Ïì
					ensure=PS_GenChar(CharBuffer1);//Éú³ÉÌØÕ÷
//					BEEP=0;//·äÃùÆ÷¹Ø±Õ
					if(ensure==0x00)
					{
						Delay_ms(400);
						OLED_Clear();//ÇåÆÁ
						OLED_ShowChinese(35,5,"Ö¸ÎÆÕı³£ ");
						OLED_Update();//¸üĞÂÏÔÊ¾	
						Delay_ms(400);
						i=0;
						processnum=1;//Ìøµ½µÚ¶ş²½						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);
				OLED_Clear();//ÇåÆÁ
				break;	
			case 1:
				i++;
				Delay_ms(400);
				OLED_Clear();//ÇåÆÁ
				OLED_ShowChinese(5,5,"ÇëÔÙ°´Ò»´ÎÖ¸ÎÆ ");
				OLED_Update();//¸üĞÂÏÔÊ¾		
				Delay_ms(400);
				ensure=PS_GetImage();
				Delay_ms(1000);
				if(ensure==0x00) 
				{
//					BEEP=1;
					ensure=PS_GenChar(CharBuffer2);//Éú³ÉÌØÕ÷
//					BEEP=0;
					if(ensure==0x00)
					{
						OLED_Clear();//ÇåÆÁ
						OLED_ShowChinese(35,5,"Ö¸ÎÆÕı³£ ");
						OLED_Update();//¸üĞÂÏÔÊ¾	
						Delay_ms(400);
						i=0;
						processnum=2;//Ìøµ½µÚÈı²½
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				//OLED_Clear();//ÇåÆÁ
				break;

			case 2:		
				OLED_ShowChinese(15,5,"¶Ô±ÈÁ½´ÎÖ¸ÎÆ ");
				OLED_Update();//¸üĞÂÏÔÊ¾	
				Delay_ms(400);
				ensure=PS_Match();//¾«È·±È¶ÔÁ½Ã¶Ö¸ÎÆÌØÕ÷ PS_Match
				if(ensure==0x00) 
				{
					OLED_Clear();//ÇåÆÁ
					OLED_ShowChinese(15,5,"Á½´ÎÖ¸ÎÆÒ»Ñù ");
					OLED_Update();//¸üĞÂÏÔÊ¾	
					Delay_ms(400);
					processnum=3;//Ìøµ½µÚËÄ²½
				}
				else 
				{
					OLED_Clear();//ÇåÆÁ
					OLED_ShowChinese(5,5,"¶Ô±ÈÊ§°ÜÇëÖØÂ¼ ");
					OLED_Update();//¸üĞÂÏÔÊ¾	
					ShowErrMessage(ensure);
					i=0;
					OLED_Clear();//ÇåÆÁ
					processnum=0;//Ìø»ØµÚÒ»²½		
				}
				Delay_ms(1200);
				OLED_Clear();//ÇåÆÁ
				break;
			case 3:
					OLED_ShowChinese(5,5,"Éú³ÉÖ¸ÎÆÄ£°å ");
					OLED_Update();//¸üĞÂÏÔÊ¾	
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{			
					OLED_Clear();//ÇåÆÁ
					OLED_ShowChinese(35,5,"Éú³É³É¹¦ ");
					OLED_Update();//¸üĞÂÏÔÊ¾	
					processnum=4;//Ìøµ½µÚÎå²½
				}else {processnum=0;ShowErrMessage(ensure);}
				Delay_ms(1200);
				break;		
			case 4:	
				OLED_Clear();//ÇåÆÁ
				OLED_ShowChinese(25,5,"ÇëÊäÈë´¢´æºÅ ");
				OLED_Update();//¸üĞÂÏÔÊ¾	
				do
					ID=FR_GET_NUM();//ÊäÈëIDºÅ
				while(!(ID<100));//ÊäÈëID±ØĞëĞ¡ÓÚÄ£¿éÈİÁ¿×î´óµÄÊıÖµ
				ensure=PS_StoreChar(CharBuffer2,ID);//´¢´æÄ£°å
				FR_num = 0xFF;
				if(ensure==0x00) 
				{			
					OLED_Clear();//ÇåÆÁ
					OLED_ShowChinese(25,5,"Â¼Èë³É¹¦ ");
					OLED_ShowNum(45,25,ID,2,OLED_8X16);
					OLED_ShowChinese(60,25,"ºÅÖ¸ÎÆ");
					OLED_Update();//¸üĞÂÏÔÊ¾	
					wire_as = 0xFF;
					Delay_ms(1500);	
				}else {processnum=0;ShowErrMessage(ensure);}
				OLED_Clear();//ÇåÆÁ					
				break;	
		}
		if(wire_as == 0xFF)
		break;
	}
}
//Ë¢Ö¸ÎÆ
uint8_t press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	uint8_t as608_key = 0xFF;//Ö¸ÎÆ½âËø±êÖ¾Î»
	usart1_Init();
				ensure=PS_GetImage();
//						OLED_Clear();
//						OLED_ShowChinese(5,5,"ÕıÔÚ¼ì²âÖ¸ÎÆ ");
//						OLED_Update();//¸üĞÂÏÔÊ¾	
//						Delay_ms(100);	
			if(ensure==0x00)//»ñÈ¡Í¼Ïñ³É¹¦ 
			{	
				ensure=PS_GenChar(CharBuffer1);
				if(ensure==0x00) //Éú³ÉÌØÕ÷³É¹¦
				{		
					
					ensure=PS_HighSpeedSearch(CharBuffer1,0,100,&seach);//ËÑË÷0µ½100IDºÅ
					if(ensure==0x00)//ËÑË÷³É¹¦
					{				
//						OLED_Clear();
////						OLED_ShowChinese(35,25,"½âËøÖĞ ");
////						OLED_Update();//¸üĞÂÏÔÊ¾	
////						Delay_ms(1800);
////						OLED_ShowChinese(35,45,"ÒÑ½âËø ");
////						OLED_ShowHexNum(5,5,seach.pageID,3,OLED_8X16);
////						OLED_ShowHexNum(35,5,seach.mathscore,3,OLED_8X16);
//						OLED_Update();//¸üĞÂÏÔÊ¾			
//						Delay_ms(1800);
						as608_key = 1;
						
					}
					else {
						OLED_ClearArea(20, 47, 120, 16);
						OLED_ShowChinese(20, 47,"Ö¸ÎÆ²»Æ¥Åä");
						OLED_Update();//¸üĞÂÏÔÊ¾		
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

////É¾³ıÖ¸ÎÆ
//void Del_FR(void)
//{
//		static u16 ValidN;
//		u8  ensure;
//		u16 num=10;
//		OLED_Clear();
//		OLED_ShowChinese(35,5,"É¾³ıÖ¸ÎÆ ");
//		OLED_ShowChinese(25,25,"ÊäÈëÖ¸ÎÆ´¢´æºÅ");	
//		OLED_ShowChinese(99,48,"È·ÈÏ");	
//		OLED_Update();//¸üĞÂÏÔÊ¾
//		Delay_ms(1200);
////	num=GET_NUM();//»ñÈ¡·µ»ØµÄÊıÖµ
//	if(num==0xFFFF)
//		goto MENU ; //·µ»ØÖ÷Ò³Ãæ
//	else if(num==0xFF00)
//		ensure=PS_Empty();//Çå¿ÕÖ¸ÎÆ¿â
//	else 
//		ensure=PS_DeletChar(num,1);//É¾³ıµ¥¸öÖ¸ÎÆ
//	if(ensure==0)
//	{
//		OLED_Clear();
//		OLED_ShowChinese(25,25,"É¾³ıÖ¸ÎÆ³É¹¦");	
//		OLED_ShowChinese(70,48,"Ê£Óà");	
//		Delay_ms(1200);
//		OLED_Update();//¸üĞÂÏÔÊ¾
//	}
//  else
//  {ShowErrMessage(ensure);	}


//	PS_ValidTempleteNum(&ValidN);//¶Á¿âÖ¸ÎÆ¸öÊı
//	OLED_ShowNum(78,48,100-ValidN,3,OLED_8X16);
//    OLED_Update();//¸üĞÂÏÔÊ¾
//	Delay_ms(1200);
//	
//MENU:	
//	OLED_Clear();
//}

