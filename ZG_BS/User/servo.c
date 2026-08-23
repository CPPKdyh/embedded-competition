#include "stm32f10x.h"
#include "servo.h"

// ????????
extern volatile uint16_t X_Angle;
extern volatile uint16_t Y_Angle;
extern int vertical_error, level_error;

// PWM???(TIM4)
void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ????
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 19999;       // 20ms
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;        // 1us??
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    // PWM????
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

// PWM?????
void PWM_SetCompare1(uint16_t Compare) {TIM_SetCompare1(TIM4, Compare);}
void PWM_SetCompare4(uint16_t Compare) {TIM_SetCompare4(TIM4, Compare);}

// ??????
void Level_Servo(float Angle)
{
    PWM_SetCompare1((uint16_t)(Angle / 270.0f * 2000 + 500));
}

void Vertical_Servo(float Angle)
{
    PWM_SetCompare4((uint16_t)(Angle / 180.0f * 2000 + 500));
}

// ??????
void Servo_Right(void)
{
    if (X_Angle < 265) {X_Angle += 1;}
    Level_Servo(X_Angle);
}

void Servo_Left(void)
{
    if (X_Angle > 20) {X_Angle -= 1;}
    Level_Servo(X_Angle);
}

void Servo_Up(void)
{
    if (Y_Angle > 30) {Y_Angle -= 1;}
    Vertical_Servo(Y_Angle);
}

void Servo_Down(void)
{
    if (Y_Angle < 150) {Y_Angle += 1;}
    Vertical_Servo(Y_Angle);
}

// ????????
void Level_Task(void)
{
    if (level_error >= 3)          Servo_Right();
    else if (level_error <= -3)    Servo_Left();
    else                           Level_Servo(X_Angle);
}

// ????????
void Vertical_Task(void)
{
    if (vertical_error >= 3)       Servo_Up();
    else if (vertical_error <= -3) Servo_Down();
    else                           Vertical_Servo(Y_Angle);
}
