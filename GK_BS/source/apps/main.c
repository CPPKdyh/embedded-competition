/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>
#include "board.h"
#include "hal_key.h"
#include "tim-board.h"
#include "timer_handles.h"
#include "adc_reader.h"
#include "usart1-board.h"
#include "sht3x.h"
#include "hal_oled.h" 

float GZ,gz,tem,hum;

void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
}


int main()
	{
		Init();

   
    while( 1 )
    {
		

		
    }
}
	
