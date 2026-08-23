#include "stm32f10x.h"
#include "Delay.h"

// ?????(??STM32F10x 72MHz??)
void Delay_ms(uint16_t nms)
{
    uint32_t i, j;
    for(i = nms; i > 0; i--)
    {
        for(j = 7200; j > 0; j--);
    }
}

// ??:?????(?????)
// void Delay_us(uint32_t nus)
// {
//     uint32_t temp;
//     SysTick->LOAD = 72 * nus;
//     SysTick->VAL = 0;
//     SysTick->CTRL = 0x01;
//     do
//     {
//         temp = SysTick->CTRL;
//     }while((temp & 0x01) && !(temp & (1 << 16)));
//     SysTick->CTRL = 0;
//     SysTick->VAL = 0;
// }