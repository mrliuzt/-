#include "stm32f10x.h"                  // Device header
#include "key.h"



void Bee_Init(void)
{
	 Key_Init_A(GPIO_Mode_Out_PP,GPIO_Pin_7,1);
	 Key_Init_B(GPIO_Mode_Out_PP,GPIO_Pin_8,0);
}


void Bee_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
}

void Bee_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}

void kaisuo_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void kaisuo_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}








