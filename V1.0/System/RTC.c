#include "stm32f10x.h"                  // Device header
#include <time.h>
#include "RTC.h"
int16_t MyRTC_Time[] = {2026,1,16,21,54,3,0};//初始化年月日时分秒

void MyRTC_Init(void)
{
	//开启BKP 和 PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
	//配置DPB
	PWR_BackupAccessCmd(ENABLE);
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)//备份DR寄存器刚开始上电是0
	{
		//如果LSE无法起振导致程序卡死在初始化函数中
		//可将初始化函数替换为下述代码，使用LSI当作RTCCLK
		//LSI无法由备用电源供电，故主电源掉电时，RTC走时会暂停
		//开启内部LSI时钟
			RCC_LSICmd(ENABLE);
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)!=SET);
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
			RCC_RTCCLKCmd(ENABLE);

		//	//手动开启LSE时钟
		//	RCC_LSEConfig(RCC_LSE_ON);
		//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);//等待标志位置一，即时钟开启
		//	//配置LSE为RTC的时钟 并使能
		//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		//	RCC_RTCCLKCmd(ENABLE);

			//等待同步
			RTC_WaitForSynchro();
			//等待上一个写入操作完成
			RTC_WaitForLastTask();
			
		//往RTC预分频器中写值内部的晶振是40khz
			RTC_SetPrescaler(40000 - 1);
			//往RTC预分频器中写值,晶振是32.768khz
		//	RTC_SetPrescaler(32768 - 1);
			//等待上一个写入操作完成
			RTC_WaitForLastTask();//每一个写入操作后都调用一下

			MyRTC_SetTime();
		//	//时间初始化
		//	RTC_SetCounter(1732375145);
		//	//等待上一个写入操作完成
		//	RTC_WaitForLastTask();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		//如果是LSE当晶振不用以下代码
		RCC_LSICmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		//--------------------
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon  = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min  = MyRTC_Time[4];
	time_date.tm_sec  = MyRTC_Time[5];


	time_cnt = mktime(&time_date) - 8 * 60 * 60;//将年月日时分秒转换为秒数
	
	//时间设置
	RTC_SetCounter(time_cnt);
	//等待上一个写入操作完成
	RTC_WaitForLastTask();
	

}


void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter()+ 8 * 60 * 60;//获取时间戳
	
	time_date = *localtime(&time_cnt);//将秒数转换为年月日时分秒
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1; 
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;  
	MyRTC_Time[5] = time_date.tm_sec;  
	
	MyRTC_Time[6] = time_date.tm_wday;//星期是通过前面时间戳自动算的，不会受初始化值影响

	
}



