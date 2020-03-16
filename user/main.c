#include <STC15Fxxxx.H>
#include <INTRINS.H>
#include <stdio.h>
#include "RayOS.h"
#include "TM1637.h"

extern ray_uint8_t xdata CPUUSage;
extern ray_uint8_t ThreadNumber;

ray_sem_t sem = 1;
ray_mailbox_t led_buf;
ray_uint8_t tid1, tid2, tid3, tid4, tid5;
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

void logs(void)
{
    printf("CPU Usage is %bd%%\r\n", GetCPUUsage());
    printf("%bd threads is running\r\n", ThreadNumber);
    ThreadDelayMs(500);
}

void FeedDog(void)
{
    WDT_reset(D_WDT_SCALE_256); //ι��
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
        MailSend(&led_buf, ClockHour / 10 == 0 ? 21 << 24 : ClockHour / 10 << 24 || ClockHour % 10 << 16 || ClockMinute / 10 << 8 || ClockMinute % 10);
        printf("Time is %bd:%bd.%bd\r\n", ClockHour, ClockMinute, ClockSecond);
        logs();
        ThreadSleep(500);
    }
}

void LEDUpdate(void)
{
    static ray_uint32_t led_buffer = 0;
    while(1)
    {
        MailRecieve(&led_buf, &led_buffer);
        OS_ENTER_CRITICAL();//���º��������������⣬�����
        TM1637_display((ray_uint8_t)(led_buffer >> 24), (ray_uint8_t)(led_buffer >> 16), (ray_uint8_t)(led_buffer >> 8), (ray_uint8_t)(led_buffer), 1, 3);
        OS_EXIT_CRITICAL();
        ThreadDelayMs(1000);
    }
}

void main_user()
{
    WDT_reset(D_WDT_SCALE_256);         //�������Ź�
    IdleHookFunctionSet(FeedDog);        //���ÿ����̹߳��Ӻ���������ʱι��
    tid1 = ThreadCreate(Flow, 10, 1);   //������ˮ��1�߳�
    tid2 = ThreadCreate(Follow, 10, 1); //������ˮ��2�߳�
    tid3 = ThreadCreate(Clock, 10, 2);   //����ʱ���߳�
    tid4 = ThreadCreate(LEDUpdate, 10, 1);   //������ʾˢ���߳�
    ThreadStart(tid1);                   //������ˮ��1�߳�
    ThreadStart(tid2);                   //������ˮ��2�߳�
    ThreadStart(tid3);                   //����ʱ���߳�
    ThreadStart(tid4);                   //������ʾˢ���߳�
}
