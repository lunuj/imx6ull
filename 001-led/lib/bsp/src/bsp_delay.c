#include "bsp_delay.h"
// 空操作，演示大约1ms
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

void delay(volatile unsigned int n)
{
    while(n--){delay_short(0x7ff);}
}