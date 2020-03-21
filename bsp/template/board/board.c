#include "STC15Fxxxx.H"
#include "RayOS.h"
#include "board.h"
#include "GPIO.h"
#include "USART.h"
#include "timer.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)

//Enable gobal interrupt
void OS_ENTER_CRITICAL(void)
{
    // Put your code here

}
//Disable gobal interrupt
void OS_EXIT_CRITICAL(void)
{
    // Put your code here
}

//Initialize GPIO
static void GPIO_config(void)
{
    // Put your code here

}

//Initialize timer
static void Timer_config(void)
{
    // Put your code here

}

//Initialize USART
static void UART_config(void)
{
    // Put your code here

}

//Initialize System
void SystemInit(void)
{
    GPIO_config();
    Timer_config();
    UART_config();
    EA = 1;
}
