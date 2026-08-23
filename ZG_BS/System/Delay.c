#include "stm32f10x.h"
#include "Delay.h"

void Delay_ms(uint16_t nms)
{
    uint32_t i, j;
    for(i = nms; i > 0; i--)
    {
        for(j = 7200; j > 0; j--);
    }
}

