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
测试例程：
设置空闲线程钩子喂狗
流水灯1每流动一轮/流水灯2流动一位
流水灯2流动一轮后线程被删除，停止运行
*/
void Flow(void) //流水灯1每流动一轮（8位）发送一个信号量
{
    P1 = 0x01;
    while (1)
    {
        ++task1count;
        P1 = _cror_(P1, 1);
        if (task2count >= 8) //流水灯2流动一轮后线程被删除，停止运行
        {
            ThreadDelete(tid2);
        }
        ThreadDelayMs(100);
        if (P1 == 1)
            SemaphoreRelease(&sem);
    }
}
void Follow(void) //流水灯2每收到一个信号量流动一位
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
    WDT_reset(D_WDT_SCALE_256); //喂狗
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
        OS_ENTER_CRITICAL();//以下函数重入会出现问题，待解决
        TM1637_display((ray_uint8_t)(led_buffer >> 24), (ray_uint8_t)(led_buffer >> 16), (ray_uint8_t)(led_buffer >> 8), (ray_uint8_t)(led_buffer), 1, 3);
        OS_EXIT_CRITICAL();
        ThreadDelayMs(1000);
    }
}

void main_user()
{
    WDT_reset(D_WDT_SCALE_256);         //开启看门狗
    IdleHookFunctionSet(FeedDog);        //设置空闲线程钩子函数，空闲时喂狗
    tid1 = ThreadCreate(Flow, 10, 1);   //创建流水灯1线程
    tid2 = ThreadCreate(Follow, 10, 1); //创建流水灯2线程
    tid3 = ThreadCreate(Clock, 10, 2);   //创建时钟线程
    tid4 = ThreadCreate(LEDUpdate, 10, 1);   //创建显示刷新线程
    ThreadStart(tid1);                   //启动流水灯1线程
    ThreadStart(tid2);                   //启动流水灯2线程
    ThreadStart(tid3);                   //启动时钟线程
    ThreadStart(tid4);                   //启动显示刷新线程
}
