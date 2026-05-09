#include "stm32f10x.h"                  // Device header

void PWM_Init_1(void){
	//开启RCC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置定时器内部时钟
	TIM_InternalClockConfig(TIM2);
	//配置TIMx
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;  //ARR  Reso=1/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;  //PSC  Freq=72M/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//配置输出比较通道1
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);//初始化结构体，因为有些功能不用到，必须随便赋个值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//配置高电平为有效电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;  //CCR  Duty=CCR/(ARR+1)
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	//定时器使能
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_Init_2(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;  //ARR  Reso=1/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;  //PSC  Freq=72M/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;  //CCR  Duty=(CCR/(ARR+1))*100%
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}
/**
  * @brief  输出比较通道1占空比设置函数
  * @param  duty：需要设置的占空比
  * @retval 
  */
void PWM_SetDuty_1(uint16_t duty){
	TIM_SetCompare1(TIM2, duty);//设置CCR  CCR=(ARR+1)*Duty
}

void PWM_SetDuty_2(uint16_t duty){
	TIM_SetCompare2(TIM3, duty);
}
/**
  * @brief  输出比较通道1频率设置函数
  * @param  freq：需要设置的频率
  * @retval 
  */
void PWM_SetFreq_1(uint16_t freq){
	uint16_t psc = 720000/freq - 1;  //  PSC=（72M/Freq/(ARR+1)）-1
	TIM_PrescalerConfig(TIM2, psc, TIM_PSCReloadMode_Update);
}

void PWM_SetFreq_2(uint16_t freq){
	uint16_t psc = 720000/freq - 1;
	TIM_PrescalerConfig(TIM3, psc, TIM_PSCReloadMode_Update);
}
