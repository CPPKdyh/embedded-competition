#include "stm32f10x.h" 
#include "task.h"     
#include "servo.h"
#include "OLED.h"
#include "AD.h"
#include "Delay.h"
#include "uart.h"

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
uint16_t X_Angle = 135;	
uint16_t Y_Angle = 90;	

void Light_Task(void)
{
	Light_value1 = 100 - Light_GetValue(ADC_Channel_6);	
	Light_value2 = 100 - Light_GetValue(ADC_Channel_7);	
	Light_value3 = 100 - Light_GetValue(ADC_Channel_8);	
	Light_value4 = 100 - Light_GetValue(ADC_Channel_9);	
		
}

void Caluate_Task(void)   
{
	up_error = (Light_value1 + Light_value2)/2;		
	down_error = (Light_value3 + Light_value4)/2;	
	left_error = (Light_value1 + Light_value3)/2;	
	right_error = (Light_value2 + Light_value4)/2;	
	
	vertical_error = up_error - down_error;			
	level_error = left_error - right_error;			
}

void Level_Task(void)
{
	if(level_error>=3)		
	{
		Servo_Left();
	}
	if(level_error<=-3)		
	{
		Servo_Right();		
	}
	if(level_error>=-3 && level_error<=3)	
	{
		Vertical_Servo(Y_Angle);
	}
}

void Vertical_Task(void)
{
	if(vertical_error>=3)	
	{
		Servo_Up();
	}
	if(vertical_error<=-3)	
	{
		Servo_Down();
	}
	if(vertical_error>=-3 && vertical_error<=3)	
	{
		Level_Servo(X_Angle);
	}
}

void Servo_Right(void)
{	
	X_Angle += 1;		
	if(X_Angle >= 250)	
	{
		X_Angle = 265;
	}
	Level_Servo(X_Angle);
}

void Servo_Left(void)	
{
	X_Angle -= 1;		
	if(X_Angle <= 20)	
	{
		X_Angle = 20;
	}
	Level_Servo(X_Angle);
}

void Servo_Up(void)
{
	Y_Angle -= 1;		
	if(Y_Angle <= 30)	
	{
		Y_Angle = 30;
	}
	Vertical_Servo(Y_Angle);
}


void Servo_Down(void)
{
	Y_Angle += 1;		
	if(Y_Angle >= 150)	
	{
		Y_Angle = 150;
	}
	Vertical_Servo(Y_Angle);
}
