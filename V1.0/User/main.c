#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Myview.h"
#include "TImer2.h"
#include "Store.h"
/*项目名：密码锁
 *功能：按键，指纹，ic卡，蓝牙解锁
 *简介：用户模式：IC卡解锁，蓝牙解锁，按键解锁
				15秒未触碰按键则锁屏
				无论用什么方式解锁，只要2次解锁失败则锁定10秒，5次失锁定30秒，6次失败自动报警
				报警只能用蓝牙进入管理员模式才能停止报警
				
	    管理模式：使用蓝牙+指纹验证才能进去，指纹6次不匹配则自动报警且不能停止。
				管理员可以录入指纹，更改IC卡，修改密码以及一键解锁
		锁屏界面：显示时间，日期
*/

extern uint8_t AS608_RxFlag;
extern uint8_t AS608_RxPacket[400];
uint8_t matrix_num = 0xFE;


int main(void)
{
	Timer_Init();//定时器初始化
	while(1)
	{
		Store_Init();//内部FLash存储
		View_Init();// 锁屏页面
		View_First();//主页面	
	}
}














