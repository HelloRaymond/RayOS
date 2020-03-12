#include <STC15F2K60S2.H>
#include "RayOS.h"
#include <INTRINS.H>

ray_sem_t sem = 1;
ray_uint8_t tid1, tid2;
ray_uint8_t task1count = 0;
ray_uint8_t task2count = 0;
/*
测试例程：
设置空闲线程钩子喂狗
流水灯1每流动一轮/流水灯2流动一位
流水灯2流动一轮后线程被删除，停止运行
流水灯1继续流动一轮后删除空闲线程钩子，停止喂狗（会重启）
*/
void Flow(void) //流水灯1每流动一轮（8位）发送一个信号量
{
    P1 = 0x01;
    while (1)
    {
        ++task1count;
        P1 = _cror_(P1, 1);
        if (task2count >= 8)//流水灯2流动一轮后线程被删除，停止运行
        {
            ThreadDelete(tid2);
        }
        ThreadDelayMs(100);
        if (P1 == 1)
            SemaphoreRealease(&sem);
        if (task1count>=8*8*2)
            IdleHookFunctionReset();//流动16轮之后删除空闲线程钩子函数，停止喂狗，观察现象
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

void FeedDog(void)
{
    WDT_CONTR = 0x35;//喂狗
}

void main_user()
{
    WDT_CONTR = 0x35;                    //开启看门狗
    IdleHookFunctionSet(FeedDog);        //设置空闲线程钩子函数，空闲时喂狗
    tid1 = ThreadCreate(Flow, 100, 1);   //创建流水灯1线程
    tid2 = ThreadCreate(Follow, 100, 1); //创建流水灯2线程
    ThreadStart(tid1);                   //启动流水灯1线程
    ThreadStart(tid2);                   //启动流水灯2线程
}
