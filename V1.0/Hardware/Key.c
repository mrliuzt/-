#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/**
  * GPIO初始化
  * 
  * 输入参数：
  *          mode：需要配置的模式  //项目这里要配置推挽输出
  *          pins：需要配置的（GPIOA里的)IO口  //如PA7就填GPIO_Pin_7
  *          bit： 默认要拉高电平还是低电平。1位高，0为低。  //项目这里开关接地，所以低电平表示按下
  * 返回值：无
  */
void Key_Init_A(uint16_t mode,uint16_t pins,uint8_t bit){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)mode;
	GPIO_InitStructure.GPIO_Pin = pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	if(bit==1)
	{
		GPIO_SetBits(GPIOA, pins);
	}
	else if(bit==0)
	{
		GPIO_ResetBits(GPIOA, pins);
	}
}

void Key_Init_B(uint16_t mode,uint16_t pins,uint8_t bit){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)mode;
	GPIO_InitStructure.GPIO_Pin = pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	if(bit==1)
	{
		GPIO_SetBits(GPIOB, pins);
	}
	else if(bit==0)
	{
		GPIO_ResetBits(GPIOB, pins);
	}
}

void Key_Init_C(uint16_t mode,uint16_t pins,uint8_t bit){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)mode;
	GPIO_InitStructure.GPIO_Pin = pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	if(bit==1)
	{
		GPIO_SetBits(GPIOC, pins);
	}
	else if(bit==0)
	{
		GPIO_ResetBits(GPIOC, pins);
	}
}

 uint8_t Key_GetNum_A(uint16_t pins)
{
    uint8_t KeyNum =1;
	//GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x );//读取某端口某一位输入的值是什么
	if(GPIO_ReadInputDataBit(GPIOA,pins)==0)//记得改------------------------------------------原理图是接gnd
	{
		Delay_ms(20);//按键消抖
		while(GPIO_ReadInputDataBit(GPIOA,pins)==0);//检测是否松手
		Delay_ms(20);//按键消抖
		KeyNum=0;
	}
	return KeyNum;
}

 uint8_t Key_GetNum_B(uint16_t pins)
{
    uint8_t KeyNum =1;
	//GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x );//读取某端口某一位输入的值是什么
	if(GPIO_ReadInputDataBit(GPIOB,pins)==0)
	{
		Delay_ms(20);//按键消抖
		while(GPIO_ReadInputDataBit(GPIOB,pins)==0);//检测是否松手
		Delay_ms(20);//按键消抖
		KeyNum=0;
	}
	return KeyNum;
}
 uint8_t Key_GetNum_C(uint16_t pins)
{
    uint8_t KeyNum =1;
	//GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x );//读取某端口某一位输入的值是什么
	if(GPIO_ReadInputDataBit(GPIOC,pins)==0)
	{
		Delay_ms(20);//按键消抖
		while(GPIO_ReadInputDataBit(GPIOC,pins)==0);//检测是否松手
		Delay_ms(20);//按键消抖
		KeyNum=0;
	}
	return KeyNum;
}
/**
  * @brief  
  * @param  
  * @param  
  * @param  
  * @param  
  * @retval 
  */

