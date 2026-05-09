#include "stm32f10x.h"                  // Device header
#include "math.h"


uint32_t password;
extern uint8_t per;

void password_make(uint8_t num)
{
    if(per < 9)
    {
        password = password * 10 + num;  // 正确的拼接方式
    }
}

void password_del(void)
{
    password = password / 10;  // 删除最后一位
}

void password_clear(void)
{
    password = 0;
}
