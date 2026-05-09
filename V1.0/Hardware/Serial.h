#ifndef __SERIAL_H
#define __SERIAL_H
#include "stdio.h"


#define checkPack Serial_RxPacket[0]+Serial_RxPacket[1]+Serial_RxPacket[2]+Serial_RxPacket[3]+Serial_RxPacket[4]+Serial_RxPacket[5]+Serial_RxPacket[6]+Serial_RxPacket[7]+Serial_RxPacket[8]+Serial_RxPacket[9]+Serial_RxPacket[10]+Serial_RxPacket[11]+Serial_RxPacket[12]+Serial_RxPacket[13]+Serial_RxPacket[14]+Serial_RxPacket[15]

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X,uint32_t Y);
void Serial_SendNumber(uint32_t Number,uint8_t Length);
void Serial_printf(char *format, ...);
uint8_t Get_Serial_RxFlag(void);
void Serial_SendPacket(void);

#endif
