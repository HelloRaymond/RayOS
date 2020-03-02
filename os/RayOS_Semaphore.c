#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "RayOS.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_SEMAPHORE
//�����ź��������ź���P����
void SemaphoreTake(ray_sem_t *ThreadSemaphore)
{
    //P������Ҫʵ��ԭ�Ӳ���
    EA = 0;
    //��S����0���ź���ֵ�Լ�����ǰ�̼߳�������
    if ((*ThreadSemaphore) > 0)
    {
        --(*ThreadSemaphore);
        EA = 1;
    }
    //��SС�ڵ���0����ǰ�߳�����
    if ((*ThreadSemaphore) <= 0)
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadSemaphore = ThreadSemaphore;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        EA = 1;
        //�ȴ���ʱ��Ƭ�ľ�
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
}

//�ͷ��ź��������ź���V����
void SemaphoreRealease(ray_sem_t *ThreadSemaphore)
{
    ray_uint8_t i;
    //V������Ҫʵ��ԭ�Ӳ���
    EA = 0;
    ++(*ThreadSemaphore);
    if (*ThreadSemaphore > 0)
    {
        //�����̶����е�һ����ȴ��ź������������̻߳���
        for (i = 0; i <= THREAD_MAX; i++)
        {
            if (ThreadHandlerIndex[i]->ThreadStatus == BLOCKED && ThreadHandlerIndex[i]->ThreadSemaphore == ThreadSemaphore)
            {
                ThreadHandlerIndex[i]->ThreadStatus = READY;
                ThreadHandlerIndex[i]->BlockEvent = NONE;
                ThreadHandlerIndex[i]->ThreadSemaphore = RAY_NULL;
                break;
            }
        }
    }
    EA = 1;
}
#endif
