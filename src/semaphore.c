#include "RayOS.h"
#include "board.h"

extern ray_thread_t OS_ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t OS_RunningThreadID;

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
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadSemaphore = ThreadSemaphore;
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus = BLOCKED;
        OS_EXIT_CRITICAL();
        //Wait for this time slice to run out
        while (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus == BLOCKED)
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
            if (OS_ThreadHandlerIndex[i]->ThreadStatus == BLOCKED && OS_ThreadHandlerIndex[i]->ThreadSemaphore == ThreadSemaphore)
            {
                OS_ThreadHandlerIndex[i]->ThreadStatus = READY;
                OS_ThreadHandlerIndex[i]->BlockEvent = NONE;
                OS_ThreadHandlerIndex[i]->ThreadSemaphore = RAY_NULL;
                break;
            }
        }
    }
    OS_EXIT_CRITICAL();
}
#endif
