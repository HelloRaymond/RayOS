#include "STC15Fxxxx.H"
#include "USART.h"
#include "stdio.h"
#include "RayOS.h"

ray_sem_t sem = 0;
ray_uint8_t tid1, tid2;

/*
�������̣�
���������̣߳��ֱ���ʱ�Ӽ�ʱ��ָʾ����˸
ʱ��ÿ��һ�룬ָʾ����˸һ��
*/

void Flicker(void)
{
    while (1)
    {
        SemaphoreTake(&sem);
        P35 = 0;
        ThreadDelayMs(50);
        P35 = 1;
    }
}

void Clock(void)
{
    static ray_uint8_t ClockHour, ClockMinute, ClockSecond;
    ClockHour = ClockMinute = ClockSecond = 0;
    while (1)
    {
        ++ClockSecond;
        if (ClockSecond >= 60)
        {
            ClockSecond -= 60;
            ++ClockMinute;
        }
        if (ClockMinute >= 60)
        {
            ClockMinute -= 60;
            ++ClockHour;
        }
        if (ClockHour >= 24)
        {
            ClockHour = 0;
        }
        SemaphoreRelease(&sem);
        printf("Time:%bd:%bd.%bd\r\nCPU Usage:%bd%%\r\n", ClockHour, ClockMinute, ClockSecond, GetCPUUsage());
        ThreadDelayMs(1000);
    }
}

void FeedDog(void)
{
    WDT_reset(D_WDT_SCALE_256); //ι��
}

void main_user(void)
{
    WDT_reset(D_WDT_SCALE_256);          //�������Ź�
    IdleHookFunctionSet(FeedDog);        //���ÿ����̹߳��Ӻ���������ʱι��
    tid1 = ThreadCreate(Flicker, 10, 1); //����ָʾ���߳�
    tid2 = ThreadCreate(Clock, 10, 2);   //����ʱ���߳�
    ThreadStart(tid1);                   //����ָʾ���߳�
    ThreadStart(tid2);                   //����ʱ���߳�
}
