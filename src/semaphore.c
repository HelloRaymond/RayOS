#include "RayOS.h"
#include "board.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_SEMAPHORE
//Receive semaphore, that is, semaphore P operation
void SemaphoreTake(ray_sem_t *ThreadSemaphore)
{
    //P operation needs to be an atomic operation
    OS_ENTER_CRITICAL();
    //If S is greater than 0, the semaphore value is decremented and the current thread continues to run
    if ((*ThreadSemaphore) > 0)
    {
        --(*ThreadSemaphore);
        OS_EXIT_CRITICAL();
    }
    //If S is less than or equal to 0, the current thread is blocked
    if ((*ThreadSemaphore) <= 0)
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadSemaphore = ThreadSemaphore;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        OS_EXIT_CRITICAL();
        //Wait for this time slice to run out
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            ;
    }
}

//Release semaphore, ie semaphore V operation
void SemaphoreRelease(ray_sem_t *ThreadSemaphore)
{
    ray_uint8_t i;
    //V operation needs to be an atomic operation
    OS_ENTER_CRITICAL();
    ++(*ThreadSemaphore);
    if (*ThreadSemaphore > 0)
    {
        //Wakes up the first thread in the thread queue blocked by waiting for a semaphore
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
