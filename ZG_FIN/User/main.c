#include "stm32f10x.h"
#include "AD.h"
#include "servo.h"
#include "task.h"
#include "timer.h"
#include "Delay.h"
#include "PWM.h"


volatile uint16_t X_Angle = 135;    
volatile uint16_t Y_Angle = 90;     

int power;              
int Light_value1;
int Light_value2;
int Light_value3;
int Light_value4;
int left_error;
int right_error;
int up_error;
int down_error;
int vertical_error;
int level_error;


#define FILTER_LEN 5            
uint16_t light_history[4][FILTER_LEN]; 
uint8_t hist_index = 0;              


void Analog_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}


uint16_t ADC_GetValue(uint8_t ADC_Channel)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}
uint16_t Light_GetValue(uint8_t ADC_CHANNEL)
{
    uint16_t ADValue;
    ADValue = ((float)ADC_GetValue(ADC_CHANNEL)) / 4095 * 100;
    return ADValue;
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


    uint32_t sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
    for (int i = 0; i < FILTER_LEN; i++) {
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


void Level_Task(void)
{
    if (level_error >= 3)          
    {
        Servo_Left();
    }
    else if (level_error <= -3)  
    {
        Servo_Right();
    }
    else                       
    {
        Level_Servo(X_Angle);
    }
}


void Vertical_Task(void)
{
    if (vertical_error >= 3)      
    {
        Servo_Up();
    }
    else if (vertical_error <= -3)
    {
        Servo_Down();
    }
    else                         
    {
        Vertical_Servo(Y_Angle);
    }
}


void Servo_Right(void)
{
    if (X_Angle < 265)      
    {
        X_Angle += 1;
    }
    Level_Servo(X_Angle);
}

void Servo_Left(void)
{
    if (X_Angle > 20)      
    {
        X_Angle -= 1;
    }
    Level_Servo(X_Angle);
}

void Servo_Up(void)
{
    if (Y_Angle > 30)      
    {
        Y_Angle -= 1;
    }
    Vertical_Servo(Y_Angle);
}

void Servo_Down(void)
{
    if (Y_Angle < 150)      
    {
        Y_Angle += 1;
    }
    Vertical_Servo(Y_Angle);
}


void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM4);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;       
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;       
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);

    TIM_Cmd(TIM4, ENABLE);
}


void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM4, Compare);
}


void PWM_SetCompare4(uint16_t Compare)
{
    TIM_SetCompare4(TIM4, Compare);
}


void TIM1_PWM_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM1, ENABLE);
}


void Level_Servo(float Angle)
{
    PWM_SetCompare1((uint16_t)(Angle / 270.0f * 2000 + 500));
}

void Vertical_Servo(float Angle)
{
    PWM_SetCompare4((uint16_t)(Angle / 180.0f * 2000 + 500));
}


int main(void)
{
    Analog_Init();
    PWM_Init();
    TIM1_PWM_Init(200 - 1, 7200 - 1); 

    while (1)
    {
        Light_Task();   
        Caluate_Task(); 
 
    }
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
 