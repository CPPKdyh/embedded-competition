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

	
	

    
    while (1)
    {

		
    }
}

void Light_Task(void)
{

	for(int i;i< FILTER_LEN;i++)
	{


	
	}
}

void Caluate_Task(void)
{


}
void TIM1_UP_IRQHandler(void)
{
	
}
