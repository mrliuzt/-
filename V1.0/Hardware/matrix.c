#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Keyboard.h"


uint8_t row;
//初始化
void Matrix_Init(void)
{
	//初始化Key5-8为高电平(行）配置为输出模式
	Key_Init_B(GPIO_Mode_Out_PP,GPIO_Pin_0,1);
	Key_Init_A(GPIO_Mode_Out_PP,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6,1);
	
	//初始化Key1-4，设置为外部中断(列）配置为输入模式
	Keyboard_Init();
}

//行扫描
void Matrix_row(void)
{
	uint8_t select_row;
	for(select_row=1;select_row<5;select_row++)
	{
			switch(select_row)
		{
			case 4:	
				row = 4;
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
			break;
			case 3:	
				row = 3;
				GPIO_SetBits(GPIOB, GPIO_Pin_0);
				GPIO_ResetBits(GPIOA, GPIO_Pin_6);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
			break;
			case 2:
				row = 2;
				GPIO_SetBits(GPIOB, GPIO_Pin_0);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5); 
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
			break;
			case 1:
				row = 1;
				GPIO_SetBits(GPIOB, GPIO_Pin_0);
				GPIO_SetBits(GPIOA, GPIO_Pin_6);
				GPIO_SetBits(GPIOA, GPIO_Pin_5); 
				GPIO_ResetBits(GPIOA, GPIO_Pin_4); 
			break;
		}
	}
}










