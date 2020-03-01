#include <STC15F2K60S2.H>
#include "RayOS.h"
#include <INTRINS.H>

ray_sem_t sem = 1;
ray_uint8_t tid1, tid2;
ray_uint8_t task2count = 0;

void Flow(void) //��ˮ��1ÿ����һ�֣�8λ������һ���ź���
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

void main_user()
{
    tid1 = ThreadCreate(Flow, 100, 1);   //������ˮ��1�߳�
    tid2 = ThreadCreate(Follow, 100, 1); //������ˮ��2�߳�
    ThreadStart(tid1);                   //������ˮ��1�߳�
    ThreadStart(tid2);                   //������ˮ��2�߳�
}
