#include "stm32f10x.h"                  // Device header

/*设置中断函数，什么通道要用哪一个去启动文件找*/
/*
void TIM2_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)// 读取是否TIM2进入的中断
   {
	    
	   
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//硬件置1.自己置0
   }
	
}
*/
extern uint16_t Num;//声明要用的外部变量,跨文件使用

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//配置RCC使能，time2在APB1总线上
	
	TIM_InternalClockConfig(TIM2);//将TIM2设置为使用内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//定义结构体，准备配置时基单元
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1 ; //对时基单元关系不大，随便配一个
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;//设置向上计数
	/*定时1s*/
	TIM_TimeBaseInitStruct.TIM_Period=(10000-1);//ARR自动重装的值 计数到了就进入中断
	TIM_TimeBaseInitStruct.TIM_Prescaler=(7200-1);//PSC预分频器的值 计数频率
	
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//高级计数器有的，通用计数器直接置0；
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);//配置时基单元参数
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//更新中断标志位清零，避免初始化刚上电进入中断
	/* 使能中断 */
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);//开启更新中断到NVIC
	
	/*配置NVIC*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组占比，一个程序只能一种
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel =TIM2_IRQn ;//Time2通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//响应优先级
	NVIC_Init(&NVIC_InitStruct);//设置通道和优先级
	
	/*启动定时器*/
//	TIM_Cmd(TIM2,ENABLE);
}
