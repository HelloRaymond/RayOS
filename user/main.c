#include <STC15Fxxxx.H>
#include <INTRINS.H>
#include "RayOS.h"
#include "TM1637.h"

ray_sem_t sem = 1;
ray_uint8_t ClockHour, ClockMinute, ClockSecond;
ray_uint8_t tid1, tid2, tid3;
ray_uint8_t task1count = 0;
ray_uint8_t task2count = 0;
/*
�������̣�
���ÿ����̹߳���ι��
��ˮ��1ÿ����һ��/��ˮ��2����һλ
��ˮ��2����һ�ֺ��̱߳�ɾ����ֹͣ����
*/
void Flow(void) //��ˮ��1ÿ����һ�֣�8λ������һ���ź���
{
    P1 = 0x01;
    while (1)
    {
        ++task1count;
        P1 = _cror_(P1, 1);
        if (task2count >= 8) //��ˮ��2����һ�ֺ��̱߳�ɾ����ֹͣ����
        {
            ThreadDelete(tid2);
        }
        ThreadDelayMs(100);
        if (P1 == 1)
            SemaphoreRelease(&sem);
    }
}
void Follow(void) //��ˮ��2ÿ�յ�һ���ź�������һλ
{
    P4 = 0x01;
    while (1)
    {
        SemaphoreTake(&sem);
        ++task2count;
        P4 = _crol_(P4, 1);
    }
}

void FeedDog(void)
{
    WDT_CONTR = 0x35; //ι��
}

void Clock(void)
{
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
        TM1637_display(ClockHour / 10 == 0 ? 21 : ClockHour / 10, ClockHour % 10, ClockMinute / 10, ClockMinute % 10, ClockSecond % 2, 3);
        ThreadDelayMs(1000);
    }
}

void main_user()
{
    WDT_CONTR = 0x35;                    //�������Ź�
    IdleHookFunctionSet(FeedDog);        //���ÿ����̹߳��Ӻ���������ʱι��
    tid1 = ThreadCreate(Flow, 100, 1);   //������ˮ��1�߳�
    tid2 = ThreadCreate(Follow, 100, 1); //������ˮ��2�߳�
    tid3 = ThreadCreate(Clock, 10, 2);   //����ʱ����ʾ�߳�
    ThreadStart(tid1);                   //������ˮ��1�߳�
    ThreadStart(tid2);                   //������ˮ��2�߳�
    ThreadStart(tid3);                   //����ʱ����ʾ�߳�
}
