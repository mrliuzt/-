#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"

void Key_Init_A(uint16_t mode,uint16_t pins,uint8_t bit);
void Key_Init_B(uint16_t mode,uint16_t pins,uint8_t bit);
void Key_Init_C(uint16_t mode,uint16_t pins,uint8_t bit);

uint8_t Key_Read_A(uint16_t pins);
uint8_t Key_Read_B(uint16_t pins);
uint8_t Key_Read_C(uint16_t pins);

uint8_t Key_GetNum_A(uint16_t pins);
uint8_t Key_GetNum_B(uint16_t pins);
uint8_t Key_GetNum_C(uint16_t pins);

#endif
