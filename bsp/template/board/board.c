#include "RayOS.h"
#include "board.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)

//Disable gobal interrupt
void OS_ENTER_CRITICAL(void)
{
    // Put your code here

}
//Enable gobal interrupt
void OS_EXIT_CRITICAL(void)
{
    // Put your code here
}

//Reload Timer0
void Timer_Reload(void)
{
    // Put your code here
}

//Initialize timer0
static void Timer_config(void)
{
    // Put your code here

}

//Initialize System
void SystemInit(void)
{
    Timer_config();
    OS_EXIT_CRITICAL();
}
