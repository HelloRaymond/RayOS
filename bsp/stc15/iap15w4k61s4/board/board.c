#include "STC15Fxxxx.H"
#include "RayOS.h"
#include "board.h"
#include "GPIO.h"
#include "USART.h"
#include "timer.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)

//Disable gobal interrupt
void OS_ENTER_CRITICAL(void)
{
    EA = 0;
}
//Enable gobal interrupt
void OS_EXIT_CRITICAL(void)
{
    EA = 1;
}

//Reload Timer0
void Timer_Reload(void)
{
    TH0 = (ray_uint8_t)(T0VAL >> 8);
    TL0 = (ray_uint8_t)T0VAL;
}

//Initialize GPIO
static void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_Pin_All;      //Initialize all pins
    GPIO_InitStructure.Mode = GPIO_PullUp;      //all pins pull up
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); //Initialize GPIO_P0
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure); //Initialize GPIO_P1
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); //Initialize GPIO_P2
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure); //Initialize GPIO_P3
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure); //Initialize GPIO_P4
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure); //InitializeGPIO_P5
}
//Initialize timer
static void Timer_config(void)
{
    TIM_InitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Mode = TIM_16Bit;
    TIM_InitStructure.TIM_Polity = PolityHigh;
    TIM_InitStructure.TIM_Interrupt = ENABLE;
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;
    TIM_InitStructure.TIM_ClkOut = DISABLE;
    TIM_InitStructure.TIM_Value = T0VAL;
    TIM_InitStructure.TIM_Run = ENABLE;
    Timer_Inilize(Timer0, &TIM_InitStructure);
}
//Initialize USART
static void UART_config(void)
{
    COMx_InitDefine COMx_InitStructure;
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;
    COMx_InitStructure.UART_BaudRate = 115200ul;
    COMx_InitStructure.UART_RxEnable = ENABLE;
    COMx_InitStructure.BaudRateDouble = ENABLE;
    COMx_InitStructure.UART_Interrupt = ENABLE;
    COMx_InitStructure.UART_Polity = PolityLow;
    COMx_InitStructure.UART_P_SW = UART1_SW_P30_P31;
    COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;
    USART_Configuration(USART1, &COMx_InitStructure); //USART1
}

//Initialize System
void SystemInit(void)
{
    GPIO_config();
    Timer_config();
    UART_config();
    OS_EXIT_CRITICAL();
    PrintString1("OS Start Running!\r\n"); //Send welcome greeting
}
