#include "stm32f10x.h"                  // Device header
#include "math.h"


uint32_t password;
extern uint8_t per;

void password_make(uint8_t num)
{
	
	if(per<9)
	{
		password = (num*pow(10,(8-per))) + password;//ÏÔÊ¾´Ó×óµ½ÓÒ
	}
}

void password_del(void)
{
		password = password-(int)password%(int)(pow(10,(10-per)));
	
}

void password_clear(void)
{
		password = 0;
}




















