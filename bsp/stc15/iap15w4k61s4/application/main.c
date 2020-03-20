#include "STC15Fxxxx.H"
#include "USART.h"
#include "stdio.h"
#include "RayOS.h"

ray_sem_t sem = 0;
ray_uint8_t tid1, tid2;

/*
测试例程：
设置两个线程，分别是时钟计时、指示灯闪烁
时钟每走一秒，指示灯闪烁一次
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
    WDT_reset(D_WDT_SCALE_256); //喂狗
}

void main_user(void)
{
    WDT_reset(D_WDT_SCALE_256);          //开启看门狗
    IdleHookFunctionSet(FeedDog);        //设置空闲线程钩子函数，空闲时喂狗
    tid1 = ThreadCreate(Flicker, 10, 1); //创建指示灯线程
    tid2 = ThreadCreate(Clock, 10, 2);   //创建时钟线程
    ThreadStart(tid1);                   //启动指示灯线程
    ThreadStart(tid2);                   //启动时钟线程
}
