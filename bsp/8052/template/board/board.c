#include <REG52.H>
#include "RayOS.h"
#include "board.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)

//Disable gobal interrupt
void OS_ENTER_CRITICAL(void)
{
    // Put your code here
    EA = 0;
}
//Enable gobal interrupt
void OS_EXIT_CRITICAL(void)
{
    // Put your code here
    EA = 1;
}

//Reload Timer0
void Timer_Reload(void)
{
    TH0=T0VAL/256;
    TL0=T0VAL%256;
}

//Initialize timer0
static void Timer_config(void)
{
    // Put your code here
    TMOD=0x01;
    TH0=T0VAL/256;
    TL0=T0VAL%256;
    TR0=1;
}

//Initialize System
void SystemInit(void)
{
    Timer_config();
    OS_EXIT_CRITICAL();
}
