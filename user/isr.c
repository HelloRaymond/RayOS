#include <STC15F2K60S2.H>
#include "RayOS.h"

//�жϷ�����������д�����Զ����沢�л������ģ�Timer0�Ѿ���ϵͳ������ռ��
void INT0_ISR()
{
}

void INT1_ISR()
{
}

void Timer1_ISR()
{
}

void Uart1_ISR()
{
    if (TI)
    {
        TI = 0;
    }
}

void ADC_ISR()
{
}

void LVD_ISR()
{
}

void PCA_ISR()
{
}

void Uart2_ISR()
{
}

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
