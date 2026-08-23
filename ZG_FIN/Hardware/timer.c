#include "stm32f10x.h"                

// 初始化TIM1定时器，设置为每20ms触发一次中断  
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;  
      
    // 使能TIM1时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
      
    // 定时器参数配置  
    // 假设系统时钟为72MHz，TIM1时钟源为APB2，设置预分频值为7199，则TIM1的计数频率为72MHz/(7199+1)=1KHz  
    // 设置自动重载值为199，则中断周期为(199+1)*(1/1KHz)=20ms  
    TIM_TimeBaseStructure.TIM_Period = arr;  
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  
      
    // 使能TIM1的更新中断  
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  
      
    // 中断优先级配置  
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
      
    // 启动TIM1  
    TIM_Cmd(TIM1, ENABLE);  
}  
