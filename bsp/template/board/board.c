#include "STC15Fxxxx.H"
#include "RayOS.h"
#include "board.h"
#include "GPIO.h"
#include "USART.h"
#include "timer.h"

void OS_ENTER_CRITICAL(void)
{
    EA = 0;
}
void OS_EXIT_CRITICAL(void)
{
    EA = 1;
}

//GPIO初始化
static void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        //结构定义
    GPIO_InitStructure.Pin = GPIO_Pin_All;      //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); //初始化GPIO_P0
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure); //初始化GPIO_P1
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); //初始化GPIO_P2
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure); //初始化GPIO_P3
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure); //初始化GPIO_P4
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure); //初始化GPIO_P5
}
//定时器初始化
static void Timer_config(void)
{
    TIM_InitTypeDef TIM_InitStructure;                //结构定义
    TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload; //指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_Polity = PolityHigh;        //指定中断优先级, PolityHigh,PolityLow
    TIM_InitStructure.TIM_Interrupt = ENABLE;         //中断是否允许,   ENABLE或DISABLE
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;   //指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut = DISABLE;           //是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value = T0VAL;              //初值,
    TIM_InitStructure.TIM_Run = ENABLE;               //是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);        //初始化Timer0      Timer0,Timer1,Timer2
}
//串口初始化
static void UART_config(void)
{
    COMx_InitDefine COMx_InitStructure;               //结构定义
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;     //使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;      //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;        //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = ENABLE;       //波特率加倍, ENABLE或DISABLE
    COMx_InitStructure.UART_Interrupt = ENABLE;       //中断允许,   ENABLE或DISABLE
    COMx_InitStructure.UART_Polity = PolityLow;       //中断优先级, PolityLow,PolityHigh
    COMx_InitStructure.UART_P_SW = UART1_SW_P30_P31;  //切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
    COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;  //内部短路RXD与TXD, 做中继, ENABLE,DISABLE
    USART_Configuration(USART1, &COMx_InitStructure); //初始化串口1 USART1,USART2
}

//系统初始化
void SystemInit(void)
{
    GPIO_config();
    Timer_config();
    UART_config();
    EA = 1;
    PrintString1("OS Start Running!\r\n"); //SUART1发送一个字符串
}
