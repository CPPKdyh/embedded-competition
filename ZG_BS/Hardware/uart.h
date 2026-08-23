#ifndef __UART_H_
#define __UART_H_

#include "stm32f10x.h"                  // Device header
#include "stdio.h"

void UART1_Init(int bound);
void UART3_Init(int bound);
void coordinate_transformation(void);

#endif
