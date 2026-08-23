#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
}

void Level_Servo(float Angle)
{
	PWM_SetCompare3(Angle / 270 * 2000 + 500);
}

void Vertical_Servo(float Angle)
{
	PWM_SetCompare4(Angle / 180 * 2000 + 500);
}


