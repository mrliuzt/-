#ifndef __RTC_H
#define __RTC_H


extern int16_t MyRTC_Time[];

void MyRTC_Init(void);
void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);
	
#endif
