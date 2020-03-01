#include <STC15F2K60S2.H>
#include "RayOS.h"
#include <INTRINS.H>

ray_sem_t sem = 1;
ray_uint8_t tid1, tid2;
ray_uint8_t task2count = 0;

void Flow(void) //流水灯1每流动一轮（8位）发送一个信号量
{
    P1 = 0x01;
    while (1)
    {
        P1 = _cror_(P1, 1);
        if (task2count >= 8)
        {
            ThreadDelete(tid2);
        }
        DelayMs(100);
        if (P1 == 1)
            SemaphoreRealease(&sem);
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

void main_user()
{
    tid1 = ThreadCreate(Flow, 100, 1);   //创建流水灯1线程
    tid2 = ThreadCreate(Follow, 100, 1); //创建流水灯2线程
    ThreadStart(tid1);                   //启动流水灯1线程
    ThreadStart(tid2);                   //启动流水灯2线程
}
