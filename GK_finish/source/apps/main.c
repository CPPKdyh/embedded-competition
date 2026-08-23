/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>      //C语言标准库字符串处理（"board.h"已包含）
#include "board.h" 			 //LoRa 协议站
#include "hal_key.h"		 //按键输入
#include "tim-board.h"		//定时器2、3、4函数模块
#include "timer_handles.h" //定时器任务处理清单 （10ms执行一次判段按键状态，抖动2次确认状态）
#include "adc_reader.h"			//ADC采集并读取
#include "usart1-board.h"		//串口1模块
#include "sht3x.h"					//温湿度模块（吴用）
#include "hal_oled.h"				//OLED模块

float gz,GZ,tem,hum,Light;

uint8_t send_str1[13] = {0x01,0x10,0x00,0x04,0x00,0x02,0x04,0x00,0x00,0x00,0x00,0xf2,0x5c};
uint8_t send_str2[13] = {0x01,0x10,0x00,0x04,0x00,0x02,0x04,0x00,0x00,0x00,0x14,0xf2,0x53};
uint8_t send_str3[13] = {0x01,0x10,0x00,0x04,0x00,0x02,0x04,0x00,0x00,0x00,0x28,0xF2,0x42};
uint8_t send_str4[13] = {0x01,0x10,0x00,0x04,0x00,0x02,0x04,0x00,0x00,0x00,0x3C,0xF2,0x4D};
uint8_t send_str5[13] = {0x01,0x10,0x00,0x04,0x00,0x02,0x04,0x00,0x00,0x00,0x50,0xF2,0x60};
uint8_t send_str6[13] = {0x01,0x10,0x00,0x04,0x00,0x02,0x04,0x00,0x00,0x00,0x64,0xf3,0xb7};

void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);
}

int main( void )
{
    Init();
    OLED_Init();
    ADCS_Init();
    hal_temHumInit();
    USART1_Init(9600);
    OLED_ShowString(2,5,"Light:");
	

    while( 1 )
    {
        AdcScanChannel();
        gz=AdcReadCh0 ();
        GZ=(5/2.0)*(100.0*gz);
        HAL_Delay (75);
        call_sht11(&tem,&hum);
        OLED_ShowNum(48,1,tem,2,16);
        OLED_ShowNum(48,3,hum,2,16);
        OLED_ShowNum(48,5,GZ,4,16);

        if (GZ <= 70)
        {
            USART1_SendStr(send_str6,13);
        }
        else if (GZ<=100)
        {
            USART1_SendStr(send_str5,13);
        }
        else if (GZ<=120)
        {
            USART1_SendStr(send_str4,13);
        }
        else if (GZ<=140)
        {
            USART1_SendStr(send_str3,13);
        }
        else if (GZ<=160)
        {
            USART1_SendStr(send_str2,13);
        }
        else
        {
            USART1_SendStr(send_str1,13);
        }
    }
}
		
