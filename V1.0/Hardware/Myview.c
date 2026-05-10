#include "stm32f10x.h"                  // Device header
#include "RTC.h"
#include "oled.h"
#include "Myview.h"
#include "matrix.h"
#include "Serial.h"
#include "Delay.h"
#include "password.h"
#include "RCC522.h"
#include "displayrc522.h"
#include "displayas608.h"
#include "Bee.h"
#include "Store.h"

uint8_t wire_as = 0xFF;//¼��ָ�Ʊ�־λ
uint8_t wire_ic = 0xFF;//¼�뿨�ű�־λ
uint8_t root = 0xFF,ASGO = 0xFF;//����Ա�����־λ
uint8_t amend = 0xFF,again = 0xFF;//�޸����빦�ܱ�־λ
uint8_t hid = 0xFF;//���ر�־λ
uint8_t ok = 0xFF;//������־λ
uint8_t sos = 0xFF; // ������־λ

uint8_t fail_count = 0;//������־λ
uint8_t per = 0;//λ

extern uint32_t password;
extern uint8_t matrix_num;
extern uint8_t row;//��
extern uint8_t card_numberbuf0[5];
uint32_t password_key;//��ʼ����

uint8_t time_count = 15;//��������

//���ⲿ�����޸�������ʱ
void wire_time_count(uint8_t count)
{
	time_count = count;
}
//���ⲿ���ô������
void add_fail_count(void)
{
	fail_count++;
}

//����ģ���ʼ��&���ݴ���
void HC_XR(void)
{
	Serial_Init();
	//����ǣ��
		if(Get_Serial_RxFlag())
		{
			switch(checkPack)
			{
				//0
				case 10:
					matrix_num = 0xFE;
					time_count = 15;
				break;
				//0
				case 0:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 0;
					password_make(0);
					per++;
				}
				break;
				//1
				case 11:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 1;
					password_make(1);
					per++;
				}
				break;
				//2
				case 12:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 2;
					password_make(2);
					per++;
				}
				break;
				//3
				case 13:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 3;
					password_make(3);
					per++;
				}
				break;
				//4
				case 14:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 4;
					password_make(4);
					per++;
				}
				break;
				//5
				case 15:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 5;
					password_make(5);
					per++;
				}
				break;
				//6
				case 16:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 6;
					password_make(6);
					per++;
				}
				break;
				//7
				case 17:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 7;
					password_make(7);
					per++;
				}
				break;
				//8
				case 18:
					Delay_ms(400);
					time_count = 15;
				if(per<9)
				{
					matrix_num = 8;
					password_make(8);
					per++;
				}
				break;
				//9
				case 19:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 9;
					password_make(9);
					per++;
				}
				break;
				//DEL
				case 20:
					time_count = 15;
					matrix_num = 10;
					Delay_ms(400);
					password_del();
					if(per!=0)
					per--;
				break;
				//Clear
				case 30:
					time_count = 15;
					matrix_num = 20;
					Delay_ms(250);
					password_clear();
					if(per!=0)
					per = 0;
				break;
				//amend//�޸�����
				case 40:
					time_count = 15;
					matrix_num = 30;
					Delay_ms(250);
					password_clear();
					per = 0;
					amend = 1;
				break;
				//Back//����
				case 50:
					matrix_num = 40;
					Delay_ms(400);
					time_count = 15;
				if(hid == 0xFF)
				{
					hid = 1;
				}
				else
				{
					hid = 0xFF;
				}
				break;
				//OK
				case 60:
					matrix_num = 50;
					if(password == password_key)
					{
						ok = 1;
					}
					else
					{
						ok = 0;
					}
					time_count = 15;
				break;
				//Root
				case 70:
					matrix_num = 60;
					Delay_ms(400);
					time_count = 15;
					sos = 0xFF;
					root = 1;
				break;
				
				
			}
		}
}
//��������
void View_Init(void)
{
		Matrix_Init();
		MyRTC_Init();
		OLED_Init();
		OLED_Clear();
	while(matrix_num == 0xFE)
	{
		password_key = Store_Data[1]+ Store_Data[2]*65535;//���ڲ�flash��ȡ���룬ʵ�ֵ��粻��ʧ
		Add_Init();//���ڲ�flash��ȡ���ţ�ʵ�ֵ��粻��ʧ
		Bee_Init();
		kaisuo_OFF();//��ͷ�رգ���ֹ��
		TIM_Cmd(TIM2,DISABLE);
		time_count = 15;
		password_clear();
		per = 0;
		HC_XR();//����ǣ��
		Matrix_row();//�������������ɨ��
		MyRTC_ReadTime();
		OLED_ShowString(34,11,"    -  -  ",OLED_6X8);
		OLED_ShowString(40,25,"  :  :  ",OLED_6X8);
		OLED_DrawRectangle( 1, 1, 125, 61, OLED_UNFILLED);
		OLED_ShowNum(34,11, MyRTC_Time[0],4,OLED_6X8);
		OLED_ShowNum(34+6*5,11, MyRTC_Time[1],2,OLED_6X8);
		OLED_ShowNum(34+6*5+6*3,11, MyRTC_Time[2],2,OLED_6X8);
		OLED_ShowNum(40,25, MyRTC_Time[3],2,OLED_6X8);
		OLED_ShowNum(40+6*3,25, MyRTC_Time[4],2,OLED_6X8);
		OLED_ShowNum(40+6*3+6*3,25, MyRTC_Time[5],2,OLED_6X8);
		switch(MyRTC_Time[6])
		{
			case 0:	OLED_ShowChinese(40,40,"������");break;
			case 1:	OLED_ShowChinese(40,40,"����һ");break;
			case 2: OLED_ShowChinese(40,40,"���ڶ�");break;
			case 3: OLED_ShowChinese(40,40,"������");break;
			case 4: OLED_ShowChinese(40,40,"������");break;
			case 5: OLED_ShowChinese(40,40,"������");break;
			case 6: OLED_ShowChinese(40,40,"������");break;
			default:OLED_ShowChinese(40,40,"������");break;
		}
		OLED_Update();
	}
}
//��ҳ��
void View_First(void)
{
	OLED_Init();
	Serial_Init();
	Matrix_Init();
	MyRTC_Init();
	OLED_Clear();
	while(matrix_num != 0xFE)
	{
		
		TIM_Cmd(TIM2,ENABLE);
		//����ǣ��
		if(Get_Serial_RxFlag())
		{
			switch(checkPack)
			{
				//0
				case 0:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 0;
					password_make(0);
					per++;
				}
				break;
				//1
				case 11:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 1;
					password_make(1);
					per++;
				}
				break;
				//2
				case 12:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 2;
					password_make(2);
					per++;
				}
				break;
				//3
				case 13:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 3;
					password_make(3);
					per++;
				}
				break;
				//4
				case 14:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 4;
					password_make(4);
					per++;
				}
				break;
				//5
				case 15:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 5;
					password_make(5);
					per++;
				}
				break;
				//6
				case 16:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 6;
					password_make(6);
					per++;
				}
				break;
				//7
				case 17:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 7;
					password_make(7);
					per++;
				}
				break;
				//8
				case 18:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 8;
					password_make(8);
					per++;
				}
				break;
				//9
				case 19:
					time_count = 15;
					Delay_ms(400);
				if(per<9)
				{
					matrix_num = 9;
					password_make(9);
					per++;
				}
				break;
				//DEL
				case 20:
					time_count = 15;
					matrix_num = 10;
					Delay_ms(400);
					password_del();
					if(per!=0)
					per--;
				break;
				//Clear
				case 30:
					time_count = 15;
					matrix_num = 20;
					Delay_ms(250);
					password_clear();
					if(per!=0)
					per = 0;
				break;
				//amend//�޸�����
//				case 40:
//					time_count = 15;
//					matrix_num = 30;
//					Delay_ms(250);
//					if(root == 1)
//					{
//						password_clear();
//						per = 0;
//						amend = 1;
//					}
//				break;
				//Back//����
				case 50:
					time_count = 15;
					matrix_num = 40;
					Delay_ms(400);
				if(root == 1)
				{
					ASGO = 0xFF;
					root = 0xFF;
					OLED_Clear();
				}
				if(hid == 0xFF)
				{
					hid = 1;
				}
				else
				{
					hid = 0xFF;
				}
				break;
				//OK
				case 60:
					time_count = 15;
					matrix_num = 50;
					if(password == password_key)
					{
						ok = 1;
					}
					else
					{
						ok = 0;
					}
					
				break;
				//Root
				case 70:
					time_count = 15;
					matrix_num = 60;
					Delay_ms(400);
					sos = 0xFF;
					root = 1;
					OLED_Clear();
				break;
				
			}
		}
	//�û�ģʽ-------------------------
		if(root == 0xFF)
		{
			ASGO = 0xFF;
			Matrix_row();//�������������ɨ��
			MyRTC_ReadTime();//��ȡʱ��
			//ʱ����ʾ
			OLED_ShowString(5,5,"    -  -  ",OLED_6X8);
			OLED_ShowString(80,5,"  :  :  ",OLED_6X8);
			OLED_ShowNum(5,5, MyRTC_Time[0],4,OLED_6X8);
			OLED_ShowNum(5+6*5,5, MyRTC_Time[1],2,OLED_6X8);
			OLED_ShowNum(5+6*5+6*3,5, MyRTC_Time[2],2,OLED_6X8);
			OLED_ShowNum(80,5, MyRTC_Time[3],2,OLED_6X8);
			OLED_ShowNum(80+6*3,5, MyRTC_Time[4],2,OLED_6X8);
			OLED_ShowNum(80+6*3+6*3,5, MyRTC_Time[5],2,OLED_6X8);
			//������ʾ
			OLED_DrawRectangle( 21, 21, 85, 20, OLED_UNFILLED);
			OLED_ShowNum(28, 23, password, 9, OLED_8X16);
			if(hid == 0xFF)
			{
				switch(per)
				{
					case 1:OLED_ShowString(28, 23,"*",OLED_8X16);break;
					case 2:OLED_ShowString(28, 23,"**",OLED_8X16);break;
					case 3:OLED_ShowString(28, 23,"***",OLED_8X16);break;
					case 4:OLED_ShowString(28, 23,"****",OLED_8X16);break;
					case 5:OLED_ShowString(28, 23,"*****",OLED_8X16);break;
					case 6:OLED_ShowString(28, 23,"******",OLED_8X16);break;
					case 7:OLED_ShowString(28, 23,"*******",OLED_8X16);break;
					case 8:OLED_ShowString(28, 23,"********",OLED_8X16);break;
					case 9:OLED_ShowString(28, 23,"*********",OLED_8X16);break;
				}
			}
			OLED_ClearArea(28+per*8, 23, 8*(9-per), 16);
			OLED_Update();
			if(matrix_num != 50)
			{
				OLED_ClearArea(18, 47, 120, 16);
				OLED_ShowImage(53, 47,16,16,suo);
				OLED_Update();
			}
			else if(matrix_num == 50)
			{
				OLED_ShowChinese(33, 47,"���ڽ���");
				OLED_Update();
				Delay_s(1);
				
				if(ok == 1)
				{
					OLED_ClearArea(18, 47, 120, 16);
					OLED_ShowImage(53, 47,16,16,suokai);
					matrix_num = 0xFF;
					ok = 0xFF;
					per = 0;
					password_clear();
					OLED_Update();
					Delay_s(1);
					while(1)
					{
						Bee_Init();
						OLED_ClearArea(18, 47, 120, 16);
						OLED_ShowChinese(33, 47,"�����ɹ�");
						fail_count = 0;
						OLED_Update();
						kaisuo_ON();
						Delay_s(3);
						kaisuo_OFF();
						break;
					}
				}
				else if(ok==0)
				{
					OLED_ClearArea(18, 47, 120, 16);
					OLED_ShowChinese(33, 47,"����ʧ��");
					matrix_num = 0xFF;
					ok = 0xFF;
					per = 0;
					password_clear();
					OLED_Update();
					Delay_s(1);
					fail_count++;
				}
			}
			//ic ��
			if(RC522_lock()==1)
			{
				time_count = 15;
				matrix_num = 0xFF;
				OLED_ClearArea(18, 47, 120, 16);
				OLED_ShowChinese(33, 47,"���ڽ���");
				OLED_Update();
				Delay_s(1);
				OLED_ClearArea(18, 47, 120, 16);
				OLED_ShowImage(53, 47,16,16,suokai);
				OLED_Update();
				Delay_s(1);
				while(1)
				{
					Bee_Init();
					OLED_ClearArea(18, 47, 120, 16);
					OLED_ShowChinese(33, 47,"�����ɹ�");
					OLED_Update();
					kaisuo_ON();
					Delay_s(3);
					kaisuo_OFF();
					fail_count = 0;
					matrix_num = 0xFF;
					password_clear();
					ok = 0xFF;
					per = 0;
					break;
				}	
			}
			
			//��������
			if(fail_count==10)
			{
				sos = 1;
				Bee_Init();
				while(sos==1)
				{
					time_count = 15;
					OLED_Clear();
					Bee_ON();
					Delay_ms(250);
					Bee_OFF();
					Delay_ms(250);
					OLED_ShowChinese(15, 5,"���澯��");
					//Serial_printf("SOS SOS 110");
                    OLED_Update();
					//����ǣ��
					if(Get_Serial_RxFlag())
					{
						switch(checkPack)
						{	
							//Root
							case 70:
                                Bee_OFF();
                                fail_count = 0;
								time_count = 15;
								matrix_num = 60;
								Delay_ms(400);
								sos = 0xFF;
								root = 1;
							break;
						}
					}
					
				}
			}
			 else if(fail_count==7)//�������6��
			{
				fail_count = 3;//���ü�������
				while(fail_count--)
				{
					time_count = 15;
					Delay_ms(1000);
					OLED_Clear();
					OLED_ShowChinese(15, 5,"��ʣ�����λ���");
					OLED_ShowNum(20,25,fail_count,2,OLED_8X16);
					OLED_ShowChinese(45,25,"�������");
					OLED_Update();
				}
				OLED_Clear();
				OLED_Update();
				fail_count = 8;
				matrix_num = 0xFF;
			}
			else if(fail_count ==2)//�������2��
			{
				fail_count = 2;//���ü�������
				while(fail_count--)
				{
					time_count = 15;
					Delay_ms(1000);
					OLED_Clear();
					OLED_ShowChinese(15, 5,"��ʣ�����λ���");
					OLED_ShowNum(20,25,fail_count,2,OLED_8X16);
					OLED_ShowChinese(45,25,"�������");
					OLED_Update();
				}
				OLED_Clear();
				OLED_Update();
				fail_count = 3;
				matrix_num = 0xFF;
			}
		}
		
		
//rootģʽ-----------------------------

		if(root == 1)
		{
			TIM_Cmd(TIM2,DISABLE);
			time_count = 15;

			// 检查指纹库是否为空，如果为空则跳过指纹验证
			u16 ValidN = 0;
			usart1_Init();
			PS_ValidTempleteNum(&ValidN);

			// 如果没有录入指纹，显示提示并跳过验证
			if(ValidN == 0)
			{
				OLED_ShowChinese(15, 5,"请录入指纹");
				OLED_ShowChinese(15, 25,"确认操作");
				OLED_Update();
				Delay_s(1);
				ASGO = 0;  // 跳过指纹验证
			}

			//ָ�ƽ���
			while(ASGO == 0xFF)
			{
				if(Get_Serial_RxFlag())
				{
					switch(checkPack)
					{	
						//Back/
						case 50:
							wire_as = 0xFF;
							wire_ic = 0xFF;
							ASGO = 0;
							root = 0xFF;
							
						break;
					}
				}
				OLED_ShowChinese(15,5,"���ڼ��ָ�� ");
				OLED_Update();//������ʾ	
					if(press_FR() == 1)
					{
						time_count = 15;
						matrix_num = 0xFF;
						OLED_ClearArea(30, 47, 120, 16);
						OLED_ShowChinese(33, 47,"���ڽ���");
						OLED_Update();
						Delay_s(1);
						OLED_ClearArea(30, 47, 120, 16);
						OLED_ShowImage(53, 47,16,16,suokai);
						OLED_Update();
							Bee_Init();
							OLED_ClearArea(30, 47, 120, 16);
							OLED_ShowChinese(33, 47,"�����ɹ�");
							fail_count = 0;
							OLED_Update();
							matrix_num = 0xFF;
							password_clear();
							ok = 0xFF;
							per = 0;		
							ASGO = 0;
							OLED_Clear();
					}
					//��������
					if(fail_count==8)
					{
						sos = 1;
						Bee_Init();
						while(sos==1)
						{
							time_count = 15;
							OLED_Clear();
							Bee_ON();
							Delay_ms(250);
							Bee_OFF();
							Delay_ms(250);
							OLED_ShowChinese(15, 5,"���澯��");
							Serial_printf("SOS SOS 110");
						}
					}
					 else if(fail_count==6)
					{
						fail_count = 30;//���ü�������
						while(fail_count--)
						{
							time_count = 15;
							Delay_ms(1000);
							OLED_Clear();
							OLED_ShowChinese(15, 5,"��ʣ��һ�λ���");
							OLED_ShowNum(20,25,fail_count,2,OLED_8X16);
							OLED_ShowChinese(45,25,"�������");
							OLED_Update();
						}
						OLED_Clear();
						OLED_Update();
						fail_count = 7;
						matrix_num = 0xFF;
					}
					else if(fail_count ==2)
					{
						fail_count = 10;//���ü�������
						while(fail_count--)
						{
							time_count = 15;
							Delay_ms(1000);
							OLED_Clear();
							OLED_ShowChinese(15, 5,"��ʣ�����λ���");
							OLED_ShowNum(20,25,fail_count,2,OLED_8X16);
							OLED_ShowChinese(45,25,"�������");
							OLED_Update();
						}
						OLED_Clear();
						OLED_Update();
						fail_count = 3;
						matrix_num = 0xFF;
					}
				}
			
			//ҳ����ʾ���ʼ��
			Matrix_row();//�������������ɨ��
			MyRTC_ReadTime();//��ȡʱ��
			OLED_ShowChinese(35, 5,"һ������");
			OLED_ShowChinese(35, 25,"¼��ָ��");
			OLED_ShowChinese(35, 45,"���Ŀ���");
			OLED_DrawRectangle(5, 3, 120, 60, OLED_UNFILLED);
			OLED_Update();
		//����ǣ��----------------------------------
					if(Get_Serial_RxFlag())
					{
						switch(checkPack)
						{	
							//Back/
							case 50:
								wire_as = 0xFF;
								wire_ic = 0xFF;
								ASGO = 0xFF;
								root = 0xFF;
								OLED_Clear();
								
							break;
							//¼��ָ��
							case 20:
								OLED_ReverseArea(35, 25,4*16, 16);
								OLED_Update();
								Delay_s(1);
								OLED_ReverseArea(35, 25,4*16, 16);
								OLED_Update();
								wire_as = 1;
								
							break;
							//���Ŀ���
							case 30:
								OLED_ReverseArea(35, 45,4*16, 16);
								OLED_Update();
								Delay_s(1);
								OLED_ReverseArea(35, 45,4*16, 16);
								OLED_Update();
								wire_ic = 1;
							break;
							//һ������
							case 60:
								OLED_ReverseArea(35, 5,4*16, 16);
								OLED_Update();
								Delay_s(1);
								Bee_Init();
								kaisuo_ON();
								OLED_ReverseArea(35, 5,4*16, 16);
								OLED_Update();
								Delay_s(3);
								kaisuo_OFF();
							break;
							//amend//�޸�����
							case 40:
								time_count = 15;
								matrix_num = 30;
								Delay_ms(250);
								if(root == 1)
								{
									password_clear();
									per = 0;
									amend = 1;
								}
							break;
						}
						
					}
            if(amend != 1)
            {
            //¼��ָ��
                while(wire_as == 1)
                {
                     Add_FR();
                }
        
            
            
            //¼�뿨��
                while(wire_ic == 1)
                {
                    Add_Rfid();
                }
            }	
			//�޸�����
			if(amend == 1)
			{
				OLED_Clear();
				
					while(1)
					{
							//����ǣ��
						if(Get_Serial_RxFlag())
						{
							switch(checkPack)
							{
								
								//0
								case 0:
									Delay_ms(400);
									time_count = 15;
								if(per<9)
								{
									matrix_num = 0;
									password_make(0);
									per++;
								}
								break;
								//1
								case 11:
									Delay_ms(400);
									time_count = 15;
								if(per<9)
								{
									matrix_num = 1;
									password_make(1);
									per++;
								}
								break;
								//2
								case 12:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 2;
									password_make(2);
									per++;
								}
								break;
								//3
								case 13:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 3;
									password_make(3);
									per++;
								}
								break;
								//4
								case 14:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 4;
									password_make(4);
									per++;
								}
								break;
								//5
								case 15:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 5;
									password_make(5);
									per++;
								}
								break;
								//6
								case 16:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 6;
									password_make(6);
									per++;
								}
								break;
								//7
								case 17:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 7;
									password_make(7);
									per++;
								}
								break;
								//8
								case 18:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 8;
									password_make(8);
									per++;
								}
								break;
								//9
								case 19:
									time_count = 15;
									Delay_ms(400);
								if(per<9)
								{
									matrix_num = 9;
									password_make(9);
									per++;
								}
								break;
								//DEL
								case 20:
									time_count = 15;
									matrix_num = 10;
									Delay_ms(400);
									password_del();
									if(per!=0)
									per--;
								break;
								//Clear
								case 30:
									time_count = 15;
									matrix_num = 20;
									Delay_ms(400);
									password_clear();
									if(per!=0)
									per = 0;
								break;
								//Back//����
								case 50:
									time_count = 15;
									matrix_num = 40;
									Delay_ms(400);
								break;
								//OK
								case 60:
									time_count = 15;
									matrix_num = 50;
									if(password == password_key)
									{
										ok = 1;
									}
									else
									{
										ok = 0;
									}
								break;
								
							}
						}
						
						Matrix_row();//�������������ɨ��
						MyRTC_ReadTime();//��ȡʱ��
						OLED_ShowString(5,5,"    -  -  ",OLED_6X8);
						OLED_ShowString(80,5,"  :  :  ",OLED_6X8);
						OLED_ShowNum(5,5, MyRTC_Time[0],4,OLED_6X8);
						OLED_ShowNum(5+6*5,5, MyRTC_Time[1],2,OLED_6X8);
						OLED_ShowNum(5+6*5+6*3,5, MyRTC_Time[2],2,OLED_6X8);
						OLED_ShowNum(80,5, MyRTC_Time[3],2,OLED_6X8);
						OLED_ShowNum(80+6*3,5, MyRTC_Time[4],2,OLED_6X8);
						OLED_ShowNum(80+6*3+6*3,5, MyRTC_Time[5],2,OLED_6X8);
						OLED_DrawRectangle( 21, 21, 85, 20, OLED_UNFILLED);
						OLED_ShowNum(28, 23, password, 9, OLED_8X16);
						OLED_ClearArea(28+per*8, 23, 8*(9-per), 16);
						OLED_ClearArea(30, 47, 120, 16);
						OLED_ShowChinese(20, 47,"ȷ��");
						OLED_ShowChinese(72, 47,"�˳�");
						OLED_Update();
						if(matrix_num == 50)
						{
							OLED_ShowChinese(20, 47,"ȷ��");
							OLED_ShowChinese(72, 47,"�˳�");
							OLED_ReverseArea(20, 47,16*2,16);
							OLED_Update();
							Delay_ms(250);
							password_key = password;//��������
							OLED_ClearArea(20, 47, 120, 16);
							OLED_ShowChinese(33, 47,"���óɹ�");
							matrix_num = 0xFF;
							amend = 0xFF;
							password = 0;
							per = 0;
							OLED_Update();
							Delay_s(1);
							OLED_ClearArea(20, 47, 120, 16);
							OLED_ShowChinese(33, 47,"�ٴ�����");
							OLED_Update();
							Delay_ms(1000);
							while(1)
							{
									//����ǣ��
									if(Get_Serial_RxFlag())
									{
										switch(checkPack)
										{
											//0
											case 0:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 0;
												password_make(0);
												per++;
											}
											break;
											//1
											case 11:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 1;
												password_make(1);
												per++;
											}
											break;
											//2
											case 12:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 2;
												password_make(2);
												per++;
											}
											break;
											//3
											case 13:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 3;
												password_make(3);
												per++;
											}
											break;
											//4
											case 14:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 4;
												password_make(4);
												per++;
											}
											break;
											//5
											case 15:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 5;
												password_make(5);
												per++;
											}
											break;
											//6
											case 16:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 6;
												password_make(6);
												per++;
											}
											break;
											//7
											case 17:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 7;
												password_make(7);
												per++;
											}
											break;
											//8
											case 18:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 8;
												password_make(8);
												per++;
											}
											break;
											//9
											case 19:
												time_count = 15;
												Delay_ms(400);
											if(per<9)
											{
												matrix_num = 9;
												password_make(9);
												per++;
											}
											break;
											//DEL
											case 20:
												time_count = 15;
												matrix_num = 10;
												Delay_ms(400);
												password_del();
												if(per!=0)
												per--;
											break;
											//Clear
											case 30:
												time_count = 15;
												matrix_num = 20;
												Delay_ms(400);
												password_clear();
												if(per!=0)
												per = 0;
											break;
											//Back//����
											case 50:
												time_count = 15;
												matrix_num = 40;
												Delay_ms(400);
											break;
											//OK
											case 60:
												time_count = 15;
												matrix_num = 50;
												if(password == password_key)
												{
													ok = 1;
												}
												else
												{
													ok = 0;
												}
											break;
											
												
										}
									}
								
								OLED_ClearArea(30, 47, 120, 16);
								OLED_ShowChinese(20, 47,"ȷ��");
								OLED_ShowChinese(72, 47,"�˳�");
								OLED_Update();
								
								Matrix_row();//�������������ɨ��
								MyRTC_ReadTime();//��ȡʱ��
								OLED_ShowString(5,5,"    -  -  ",OLED_6X8);
								OLED_ShowString(80,5,"  :  :  ",OLED_6X8);
								OLED_ShowNum(5,5, MyRTC_Time[0],4,OLED_6X8);
								OLED_ShowNum(5+6*5,5, MyRTC_Time[1],2,OLED_6X8);
								OLED_ShowNum(5+6*5+6*3,5, MyRTC_Time[2],2,OLED_6X8);
								OLED_ShowNum(80,5, MyRTC_Time[3],2,OLED_6X8);
								OLED_ShowNum(80+6*3,5, MyRTC_Time[4],2,OLED_6X8);
								OLED_ShowNum(80+6*3+6*3,5, MyRTC_Time[5],2,OLED_6X8);
								OLED_DrawRectangle( 21, 21, 85, 20, OLED_UNFILLED);
								OLED_ShowNum(28, 23, password, 9, OLED_8X16);
								OLED_ClearArea(28+per*8, 23, 8*(9-per), 16);
								OLED_ClearArea(30, 47, 120, 16);
								OLED_ShowChinese(20, 47,"ȷ��");
								OLED_ShowChinese(72, 47,"�˳�");
								OLED_Update();
								if(matrix_num == 50)
								{
									OLED_ShowChinese(20, 47,"ȷ��");
									OLED_ShowChinese(72, 47,"�˳�");
									OLED_ReverseArea(20, 47,16*2,16);
									OLED_Update();
									Delay_ms(250);
									if(password_key == password)
									{
										again=1;
										matrix_num = 0xFF;
										amend = 0xFF;
										password = 0;
										per = 0;
										break;
									}
									else
									{
										OLED_ClearArea(30, 47, 120, 16);
										OLED_ShowChinese(20, 47,"���벻һ��");
										OLED_Update();
										Delay_s(1);
										OLED_ShowChinese(20, 47,"����ͷ����");
										OLED_Update();
										Delay_s(1);
										password_key = Store_Data[2];
										matrix_num = 0xFF;
										password = 0;
										per = 0;
										again = 0xFF;
										break;
									}
								}
								
								if(matrix_num == 40)
								{
									OLED_ClearArea(30, 47, 120, 16);
									OLED_ShowChinese(20, 47,"ȷ��");
									OLED_ShowChinese(72, 47,"�˳�");
									OLED_ReverseArea(72, 47,16*2,16);
									matrix_num = 0xFF;
									password = 0;
									per = 0;
									OLED_Update();
									Delay_ms(250);
									OLED_Clear();
									break;
								}
							}
							if(again==1)
							{
								OLED_ClearArea(20, 47, 120, 16);
								OLED_ShowChinese(33, 47,"���óɹ�");
								OLED_Update();
								Store_Data[2] = password_key/65535;
								Store_Data[1] = password_key%65535;
								Store_Save();
								Delay_ms(1000);
								OLED_Clear();
								break;
							}
						}
						if(matrix_num == 40)
						{
							OLED_ClearArea(30, 47, 120, 16);
							OLED_ShowChinese(20, 47,"ȷ��");
							OLED_ShowChinese(72, 47,"�˳�");
							OLED_ReverseArea(72, 47,16*2,16);
							matrix_num = 0xFF;
							amend = 0xFF;
							password = 0;
							per = 0;
							OLED_Update();
							Delay_ms(250);
							OLED_Clear();
							break;
						}
					}
			}
			if(root==0xFF)
				OLED_Clear();
			
		}
		
		
		
		
		
		
		
		
		
	}	
}




void EXTI0_IRQHandler(void)//������
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{ 
		switch(row)
		{
			case 1: 
				time_count = 15;
				matrix_num = 10;
				Delay_ms(150);
				password_del();
				if(per!=0)
				{
					per--;
				}
				
			
			break;
			
			case 2: 
				time_count = 15;
				matrix_num = 20;
				Delay_ms(250);
				password_clear();
				if(per!=0)
				per = 0;
			break;
			case 3:
				time_count = 15;
				matrix_num = 30;
				Delay_ms(250);
				if(root == 1)
				{
					password_clear();
					per = 0;
					amend = 1;
				}
			break;
			case 4:
				time_count = 15;
				matrix_num = 60;
//				root = 1;
				
			break;
		}
	}
		
		EXTI_ClearITPendingBit(EXTI_Line0);
}
void EXTI1_IRQHandler (void)//������
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{ 
		switch(row)
		{
			case 1: 
				time_count = 15;
				matrix_num = 9;
				Delay_ms(400);
				if(per<9)
				{
					password_make(9);
					per++;
				}
			break;
			
			case 2: 
				time_count = 15;
				matrix_num = 6;
				Delay_ms(400);
				if(per<9)
				{
					password_make(6);
					per++;
				}
			break;
			case 3: 
				time_count = 15;
				matrix_num = 3;
				Delay_ms(400);
				if(per<9)
				{
					password_make(3);
					per++;
				}
			break;
			case 4: 
				time_count = 15;
				
			if(root==0xFF)
			{
				matrix_num = 50;
				if(password == password_key)
				{
					ok = 1;
				}
				else
				{
					ok = 0;
				}
			}
				//һ������--------------------------------------
				if(root == 1)
				{
					if(amend != 1)
					{
						OLED_ReverseArea(35, 5,4*16, 16);
						OLED_Update();
						Delay_s(1);
						Bee_Init();
						kaisuo_ON();
						OLED_ReverseArea(35, 5,4*16, 16);
						OLED_Update();
						Delay_s(3);
						kaisuo_OFF();
					}
				}
			break;
		}
	}
		
		EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI2_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{ 
		switch(row)
		{
			case 1: 
				time_count = 15;
				matrix_num = 8;
				Delay_ms(250);
				if(per<9)
				{
					password_make(8);
					per++;
				}
			break;
			case 2: 
				time_count = 15;
				matrix_num = 5;
				Delay_ms(250);
				if(per<9)
				{
					password_make(5);
					per++;
				}
			break;
			case 3: 
				time_count = 15;
				matrix_num = 2;
				Delay_ms(400);
				if(per<9)
				{
					password_make(2);
					per++;
				}
			break;
			case 4: 
				time_count = 15;
				matrix_num = 0;
				Delay_ms(400);
				if(per<9)
				{
					password_make(0);
					per++;
				}
			break;
		}
	}
		
		EXTI_ClearITPendingBit(EXTI_Line2);
}
void EXTI3_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{ 
		switch(row)
		{
			case 1: 
				time_count = 15;
				matrix_num = 7;
				Delay_ms(400);
				if(per<9)
				{
					password_make(7);
					per++;
				}
			break;
			case 2: 
				time_count = 15;
				matrix_num = 4;
				Delay_ms(400);
				if(per<9)
				{
					password_make(4);
					per++;
				}
			break;
			case 3: 
				time_count = 15;
				matrix_num = 1;
				Delay_ms(400);
				if(per<9)
				{
					password_make(1);
					per++;
				}
			break;
			case 4: 
				time_count = 15;
				Delay_ms(400);
				matrix_num = 40;
//				if(root == 1)
//				{
//					ASGO = 0xFF;
//					root = 0xFF;
//					OLED_Clear();
//				}
				if(hid == 0xFF)
				{
					hid = 1;
				}
				else
				{
					hid = 0xFF;
				}
			
			break;
		}
	}
	
		EXTI_ClearITPendingBit(EXTI_Line3);
}


void TIM2_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)// ��ȡ�Ƿ�TIM2������ж�
   {
	    time_count--;
	    if(time_count==0)
		{
			time_count = 15;
			matrix_num = 0xFE;
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//Ӳ����1.�Լ���0
   }
	
}

















