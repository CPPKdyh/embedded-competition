/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "hal_key.h"

char light_status = GPIO_PIN_RESET;  //灯光默认关闭
char fan_status = GPIO_PIN_RESET;			//风扇默认关闭

void light_ctl_init(void)		//灯光初始化
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = LIGHT_CTL_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;        //推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;                	//上拉---默认高电平
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  //超高速输出
    HAL_GPIO_Init(LIGHT_CTL_GPIO, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LIGHT_CTL_GPIO, LIGHT_CTL_GPIO_PIN, (GPIO_PinState)light_status);
}

void light_ctl(char is_light_on) //light_ctl(1) 开灯  light_ctl(0) 关灯 
{
    light_status = is_light_on;
    HAL_GPIO_WritePin(LIGHT_CTL_GPIO, LIGHT_CTL_GPIO_PIN, (GPIO_PinState)light_status);
}

void fan_ctl_init(void)		//风扇初始化
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = FAN_CTL_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		
    GPIO_InitStruct.Pull = GPIO_PULLUP;						
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(FAN_CTL_GPIO, &GPIO_InitStruct);

    HAL_GPIO_WritePin(FAN_CTL_GPIO, FAN_CTL_GPIO_PIN, (GPIO_PinState)fan_status);
}

void fan_ctl(char is_fan_on)	//fan_ctl(1) 开风扇  fan_ctl(0) 关风扇
{
    fan_status = is_fan_on;
    HAL_GPIO_WritePin(FAN_CTL_GPIO, FAN_CTL_GPIO_PIN, (GPIO_PinState)fan_status);
}

void keys_init(void) //按键输入引脚初始化
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		//输入模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;  		 //上拉输入---按下接地
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(KEY2_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KEY3_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(KEY3_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KEY4_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(KEY4_GPIO, &GPIO_InitStruct);
}


volatile char is_key2_down = 0;
volatile char is_key3_down = 0;
volatile char is_key4_down = 0;

char isKey2Pressed(void){
	return is_key2_down;
}

char isKey3Pressed(void){
	return is_key3_down;
}

char isKey4Pressed(void){
	return is_key4_down;
}

void resetKey2(void){
	is_key2_down = 0;
}

void resetKey3(void){
	is_key3_down = 0;
}

void resetKey4(void){
	is_key4_down = 0;
}

#define DEBOUNCE_CNT	2
void scan_keys(void)
{
    static char key2_status = KEY_UP, key2_down_cnt = 0, key2_up_cnt = 0;
    static char key3_status = KEY_UP, key3_down_cnt = 0, key3_up_cnt = 0;
    static char key4_status = KEY_UP, key4_down_cnt = 0, key4_up_cnt = 0;
    char new_status;

    //KEY2
    new_status = HAL_GPIO_ReadPin(KEY2_GPIO, KEY2_GPIO_PIN);//读取按键状态（按下0：KEY_DOWN 未按1：KEY_UP）
    if((key2_status == KEY_UP) && (new_status == KEY_DOWN)) { //判断按下
        key2_down_cnt++;																			//变量计数消抖法 每抖一次变量++
        if(key2_down_cnt >= DEBOUNCE_CNT) {										//抖两次后 
            is_key2_down = 1;																	//置按键标志位为1
            key2_status = KEY_DOWN;														//旧状态 ->按下
        }
    } else {
        key2_down_cnt = 0;																		//如果没按下 按下变量清零
    }

    if((key2_status == KEY_DOWN) && (new_status == KEY_UP)) {	//判断松开
        key2_up_cnt++;																				//变量计数消抖法 每抖一次变量++
        if(key2_up_cnt >= DEBOUNCE_CNT) {											//抖两次后 
						is_key2_down = 0;																	//置按键标志位为0
            key2_status = KEY_UP;															//旧状态 ——>松开
        }
    } else {
        key2_up_cnt = 0;																			//如果没松开 松开变量清零
    }


    //KEY3
    new_status = HAL_GPIO_ReadPin(KEY3_GPIO, KEY3_GPIO_PIN);
    if((key3_status == KEY_UP) && (new_status == KEY_DOWN)) {
        key3_down_cnt++;
        if(key3_down_cnt >= DEBOUNCE_CNT) {
            is_key3_down = 1;
            key3_status = KEY_DOWN;
        }
    } else {
        key3_down_cnt = 0;
    }

    if((key3_status == KEY_DOWN) && (new_status == KEY_UP)) {
        key3_up_cnt++;
        if(key3_up_cnt >= DEBOUNCE_CNT) {
						is_key3_down = 0;
            key3_status = KEY_UP;;
        }
    } else {
        key3_up_cnt = 0;
    }


    //KEY4
    new_status = HAL_GPIO_ReadPin(KEY4_GPIO, KEY4_GPIO_PIN);
    if((key4_status == KEY_UP) && (new_status == KEY_DOWN)) {
        key4_down_cnt++;
        if(key4_down_cnt >= DEBOUNCE_CNT) {
            is_key4_down = 1;
            key4_status = KEY_DOWN;
        }
    } else {
        key4_down_cnt = 0;
    }

    if((key4_status == KEY_DOWN) && (new_status == KEY_UP)) {
        key4_up_cnt++;
        if(key4_up_cnt >= DEBOUNCE_CNT) {
						is_key4_down = 0;
            key4_status = KEY_UP;;
        }
    } else {
        key4_up_cnt = 0;
    }
}

