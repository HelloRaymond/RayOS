#include "RayOS.h"
#include "board.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_SEMAPHORE
//�����ź��������ź���P����
void SemaphoreTake(ray_sem_t *ThreadSemaphore)
{
    //P������Ҫʵ��ԭ�Ӳ���
    OS_ENTER_CRITICAL();
    //��S����0���ź���ֵ�Լ�����ǰ�̼߳�������
    if ((*ThreadSemaphore) > 0)
    {
        --(*ThreadSemaphore);
        OS_EXIT_CRITICAL();
    }
    //��SС�ڵ���0����ǰ�߳�����
    if ((*ThreadSemaphore) <= 0)
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadSemaphore = ThreadSemaphore;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        OS_EXIT_CRITICAL();
        //�ȴ���ʱ��Ƭ�ľ�
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            ;
    }
}

//�ͷ��ź��������ź���V����
void SemaphoreRelease(ray_sem_t *ThreadSemaphore)
{
    ray_uint8_t i;
    //V������Ҫʵ��ԭ�Ӳ���
    OS_ENTER_CRITICAL();
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
    OS_EXIT_CRITICAL();
}
#endif
