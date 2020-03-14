#include <STC15Fxxxx.H>
#include "RayOS.h"

//中断服务函数在这里写，会自动保存并切换上下文，Timer0已经被系统调度器占用
void INT0_ISR()
{
}

void INT1_ISR()
{
}

void Timer1_ISR()
{
}

#ifndef __USART_H
void Uart1_ISR()
{
}

void Uart2_ISR()
{
}
#endif

#ifndef __ADC_H
void ADC_ISR()
{
}
#endif

void LVD_ISR()
{
}

#ifndef __PCA_H
void PCA_ISR()
{
}
#endif

void SPI_ISR()
{
}

void INT2_ISR()
{
}

void INT3_ISR()
{
}

void Timer2_ISR()
{
}

void INT4_ISR()
{
}

void Uart3_ISR()
{
}

void Uart4_ISR()
{
}

void Timer3_ISR()
{
}

void Timer4_ISR()
{
}

void Comparator_ISR()
{
}

void PWM_ISR()
{
}

void PWMFD_ISR()
{
}
