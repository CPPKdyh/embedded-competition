#include "stm32f10x.h"
#include "ADC.h"
#include "servo.h"
#include "timer.h"
#include "Delay.h"

volatile uint16_t X_Angle = 135;
volatile uint16_t Y_Angle = 90;
int Light_value1, Light_value2, Light_value3, Light_value4;
int left_error, right_error, up_error, down_error;
int vertical_error, level_error;

#define FILTER_LEN 5
uint16_t light_history[4][FILTER_LEN];
uint8_t hist_index = 0;

void Light_Task(void);
void Caluate_Task(void);

int main(void)
{
    Analog_Init();
    PWM_Init();
    TIM1_PWM_Init(199, 7199);
    
    while (1)
    {
        Light_Task();
        Caluate_Task();
        Delay_ms(10);
    }
}

void Light_Task(void)
{
    uint16_t raw1 = 100 - Light_GetValue(ADC_Channel_6);
    uint16_t raw2 = 100 - Light_GetValue(ADC_Channel_7);
    uint16_t raw3 = 100 - Light_GetValue(ADC_Channel_8);
    uint16_t raw4 = 100 - Light_GetValue(ADC_Channel_9);

    light_history[0][hist_index] = raw1;
    light_history[1][hist_index] = raw2;
    light_history[2][hist_index] = raw3;
    light_history[3][hist_index] = raw4;
    hist_index = (hist_index + 1) % FILTER_LEN;

    uint32_t sum1=0, sum2=0, sum3=0, sum4=0;
    for (int i = 0; i < FILTER_LEN; i++)
    {
        sum1 += light_history[0][i];
        sum2 += light_history[1][i];
        sum3 += light_history[2][i];
        sum4 += light_history[3][i];
    }
    Light_value1 = sum1 / FILTER_LEN;
    Light_value2 = sum2 / FILTER_LEN;
    Light_value3 = sum3 / FILTER_LEN;
    Light_value4 = sum4 / FILTER_LEN;
}

void Caluate_Task(void)
{
    up_error = (Light_value1 + Light_value2) / 2;
    down_error = (Light_value3 + Light_value4) / 2;
    left_error = (Light_value1 + Light_value3) / 2;
    right_error = (Light_value2 + Light_value4) / 2;

    vertical_error = up_error - down_error;
    level_error = left_error - right_error;
}

void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        Level_Task();
        Vertical_Task();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
