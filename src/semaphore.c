#include "RayOS.h"
#include "board.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_SEMAPHORE
//接收信号量，即信号量P操作
void SemaphoreTake(ray_sem_t *ThreadSemaphore)
{
    //P操作需要实现原子操作
    OS_ENTER_CRITICAL();
    //若S大于0，信号量值自减，当前线程继续运行
    if ((*ThreadSemaphore) > 0)
    {
        --(*ThreadSemaphore);
        OS_EXIT_CRITICAL();
    }
    //若S小于等于0，则当前线程阻塞
    if ((*ThreadSemaphore) <= 0)
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadSemaphore = ThreadSemaphore;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        OS_EXIT_CRITICAL();
        //等待此时间片耗尽
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            ;
    }
}

//释放信号量，即信号量V操作
void SemaphoreRelease(ray_sem_t *ThreadSemaphore)
{
    ray_uint8_t i;
    //V操作需要实现原子操作
    OS_ENTER_CRITICAL();
    ++(*ThreadSemaphore);
    if (*ThreadSemaphore > 0)
    {
        //将进程队列中第一个因等待信号量而阻塞的线程唤醒
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
