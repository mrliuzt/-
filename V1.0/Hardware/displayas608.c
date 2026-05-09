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

//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	OLED_Clear();//清屏
	OLED_ShowChinese(25,5,(char*)EnsureMessage(ensure));
			
	OLED_Update();//更新显示
	Delay_ms(1000);
}
uint8_t FR_GET_NUM(void)
{
		//蓝牙牵手
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
	uint8_t num;
	num = FR_num;
	return num;
}
//录指纹
void Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	u16 ID;
	usart1_Init();
	OLED_Init();
	OLED_Clear();//清屏
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
				OLED_Clear();//清屏
				OLED_ShowChinese(35,5,"录入指纹 ");
				OLED_ShowChinese(35,25,"请按指纹");	
				OLED_ShowChinese(99,48,"返回");	
				OLED_Update();//更新显示
				ensure=PS_GetImage();
				Delay_ms(400);
				if(ensure==0x00) //录入成功
				{
//					BEEP=1;//蜂鸣器响
					ensure=PS_GenChar(CharBuffer1);//生成特征
//					BEEP=0;//蜂鸣器关闭
					if(ensure==0x00)
					{
						Delay_ms(400);
						OLED_Clear();//清屏
						OLED_ShowChinese(35,5,"指纹正常 ");
						OLED_Update();//更新显示	
						Delay_ms(400);
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);
				OLED_Clear();//清屏
				break;	
			case 1:
				i++;
				Delay_ms(400);
				OLED_Clear();//清屏
				OLED_ShowChinese(5,5,"请再按一次指纹 ");
				OLED_Update();//更新显示		
				Delay_ms(400);
				ensure=PS_GetImage();
				Delay_ms(1000);
				if(ensure==0x00) 
				{
//					BEEP=1;
					ensure=PS_GenChar(CharBuffer2);//生成特征
//					BEEP=0;
					if(ensure==0x00)
					{
						OLED_Clear();//清屏
						OLED_ShowChinese(35,5,"指纹正常 ");
						OLED_Update();//更新显示	
						Delay_ms(400);
						i=0;
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				//OLED_Clear();//清屏
				break;

			case 2:		
				OLED_ShowChinese(15,5,"对比两次指纹 ");
				OLED_Update();//更新显示	
				Delay_ms(400);
				ensure=PS_Match();//精确比对两枚指纹特征 PS_Match
				if(ensure==0x00) 
				{
					OLED_Clear();//清屏
					OLED_ShowChinese(15,5,"两次指纹一样 ");
					OLED_Update();//更新显示	
					Delay_ms(400);
					processnum=3;//跳到第四步
				}
				else 
				{
					OLED_Clear();//清屏
					OLED_ShowChinese(5,5,"对比失败请重录 ");
					OLED_Update();//更新显示	
					ShowErrMessage(ensure);
					i=0;
					OLED_Clear();//清屏
					processnum=0;//跳回第一步		
				}
				Delay_ms(1200);
				OLED_Clear();//清屏
				break;
			case 3:
					OLED_ShowChinese(5,5,"生成指纹模板 ");
					OLED_Update();//更新显示	
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{			
					OLED_Clear();//清屏
					OLED_ShowChinese(35,5,"生成成功 ");
					OLED_Update();//更新显示	
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				Delay_ms(1200);
				break;		
			case 4:	
				OLED_Clear();//清屏
				OLED_ShowChinese(25,5,"请输入储存号 ");
				OLED_Update();//更新显示	
				do
					ID=FR_GET_NUM();//输入ID号
				while(!(ID<100));//输入ID必须小于模块容量最大的数值
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				FR_num = 0xFF;
				if(ensure==0x00) 
				{			
					OLED_Clear();//清屏
					OLED_ShowChinese(25,5,"录入成功 ");
					OLED_ShowNum(45,25,ID,2,OLED_8X16);
					OLED_ShowChinese(60,25,"号指纹");
					OLED_Update();//更新显示	
					wire_as = 0xFF;
					Delay_ms(1500);	
				}else {processnum=0;ShowErrMessage(ensure);}
				OLED_Clear();//清屏					
				break;	
		}
		if(wire_as == 0xFF)
		break;
	}
}
//刷指纹
uint8_t press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	uint8_t as608_key = 0xFF;//指纹解锁标志位
	usart1_Init();
				ensure=PS_GetImage();
//						OLED_Clear();
//						OLED_ShowChinese(5,5,"正在检测指纹 ");
//						OLED_Update();//更新显示	
//						Delay_ms(100);	
			if(ensure==0x00)//获取图像成功 
			{	
				ensure=PS_GenChar(CharBuffer1);
				if(ensure==0x00) //生成特征成功
				{		
					
					ensure=PS_HighSpeedSearch(CharBuffer1,0,100,&seach);//搜索0到100ID号
					if(ensure==0x00)//搜索成功
					{				
//						OLED_Clear();
////						OLED_ShowChinese(35,25,"解锁中 ");
////						OLED_Update();//更新显示	
////						Delay_ms(1800);
////						OLED_ShowChinese(35,45,"已解锁 ");
////						OLED_ShowHexNum(5,5,seach.pageID,3,OLED_8X16);
////						OLED_ShowHexNum(35,5,seach.mathscore,3,OLED_8X16);
//						OLED_Update();//更新显示			
//						Delay_ms(1800);
						as608_key = 1;
						
					}
					else {
						OLED_ClearArea(20, 47, 120, 16);
						OLED_ShowChinese(20, 47,"指纹不匹配");
						OLED_Update();//更新显示		
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

////删除指纹
//void Del_FR(void)
//{
//		static u16 ValidN;
//		u8  ensure;
//		u16 num=10;
//		OLED_Clear();
//		OLED_ShowChinese(35,5,"删除指纹 ");
//		OLED_ShowChinese(25,25,"输入指纹储存号");	
//		OLED_ShowChinese(99,48,"确认");	
//		OLED_Update();//更新显示
//		Delay_ms(1200);
////	num=GET_NUM();//获取返回的数值
//	if(num==0xFFFF)
//		goto MENU ; //返回主页面
//	else if(num==0xFF00)
//		ensure=PS_Empty();//清空指纹库
//	else 
//		ensure=PS_DeletChar(num,1);//删除单个指纹
//	if(ensure==0)
//	{
//		OLED_Clear();
//		OLED_ShowChinese(25,25,"删除指纹成功");	
//		OLED_ShowChinese(70,48,"剩余");	
//		Delay_ms(1200);
//		OLED_Update();//更新显示
//	}
//  else
//  {ShowErrMessage(ensure);	}


//	PS_ValidTempleteNum(&ValidN);//读库指纹个数
//	OLED_ShowNum(78,48,100-ValidN,3,OLED_8X16);
//    OLED_Update();//更新显示
//	Delay_ms(1200);
//	
//MENU:	
//	OLED_Clear();
//}

