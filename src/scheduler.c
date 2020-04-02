#include "RayOS.h"
#include "scheduler.h"

extern ray_thread_t OS_ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t OS_RunningThreadID;
extern idata ray_base_t OS_XStackBuffer[STACK_SIZE]; //The actual stack when the thread is running, all threads share this stack

ray_uint32_t OS_Ticks;     //System runtime
ray_uint32_t OS_idleTicks; //System idle time
#if USING_CPUUSAGE
ray_uint8_t OS_CPUUsage; //CPU Usage
#endif

//Finding the highest priority ready thread
static ray_uint8_t FindHighestPriorityThreadID(void)
{
    ray_uint8_t i, result;
    result = 0;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (OS_ThreadHandlerIndex[i]->ThreadStatus != READY || (OS_ThreadHandlerIndex[i]->ThreadID == OS_RunningThreadID))
            continue;
        else if (OS_ThreadHandlerIndex[i]->Priority > OS_ThreadHandlerIndex[result]->Priority)
            result = i;
    }
    return result;
}

void ThreadSwitch(ray_uint8_t id)
{
    ray_uint8_t i;
    //If the thread's stack is simulated, save it from physical stack
    if (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStackType == XStack)
        for (i = 0; i < OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStackDepth; ++i)
            OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStack[i] = OS_XStackBuffer[i];
    if (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus == RUNNING)
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus = READY;

    //Switching threads
    OS_RunningThreadID = id;
    OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus = RUNNING;
    ++OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime;

    //If the thread's stack is simulated, recovery it to physical stack
    if (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStackType == XStack)
        for (i = 0; i < OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStackDepth; ++i)
            OS_XStackBuffer[i] = OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStack[i];
}

void ThreadScan(void) //Scan and update thread status
{
    ray_uint8_t i, next;
    ++OS_Ticks;
    if (OS_RunningThreadID == 0)
        ++OS_idleTicks;
#if USING_CPUUSAGE
    if (OS_Ticks % 1000 == 0)
    {
        OS_CPUUsage = (1000 - OS_idleTicks) / 10;
        OS_idleTicks = 0;
    }
#endif
    if (OS_ThreadHandlerIndex[0]->ThreadStatus != READY && OS_ThreadHandlerIndex[0]->ThreadStatus != RUNNING) //Idle thread are not allowed to block
        OS_ThreadHandlerIndex[0]->ThreadStatus = READY;
    if (OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime > OS_ThreadHandlerIndex[OS_RunningThreadID]->Ticks) //cleared RunTime after a time slice completed
        OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime = 0;
    for (i = 0; i <= THREAD_MAX; ++i) //Handle blocked threads
    {
        if (OS_ThreadHandlerIndex[i]->ThreadStatus == BLOCKED)
        {
#if USING_SEMAPHORE
            if (OS_ThreadHandlerIndex[i]->BlockEvent == WAIT) //Threads waiting for a semaphore
            {
                if (OS_ThreadHandlerIndex[i]->ThreadSemaphore == RAY_NULL) //Receive a semaphore and wake up the thread
                {
                    OS_ThreadHandlerIndex[i]->ThreadStatus = READY;
                    OS_ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#endif
            if (OS_ThreadHandlerIndex[i]->BlockEvent == DELAY) //Actively delay pending threads
            {
                --OS_ThreadHandlerIndex[i]->DelayTime;        //Delay count--
                if (OS_ThreadHandlerIndex[i]->DelayTime == 0) //When the delay time expires, wake up the thread
                {
                    OS_ThreadHandlerIndex[i]->ThreadStatus = READY;
                    OS_ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#if USING_MAILBOX
            if (OS_ThreadHandlerIndex[i]->BlockEvent == SEND || OS_ThreadHandlerIndex[i]->BlockEvent == RECIEVE) //Pending thread waiting to receive or send mail
            {
                if (OS_ThreadHandlerIndex[i]->ThreadMailBox == RAY_NULL) //Wake thread
                {
                    OS_ThreadHandlerIndex[i]->ThreadStatus = READY;
                    OS_ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#endif
        }
    }
    next = FindHighestPriorityThreadID(); //Looking for the next highest priority ready thread
    if (next == OS_RunningThreadID)       //Skip switching if there are no other ready threads except idle thread
    {
        ++OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime; //Keep running the current thread
    }
    else if (OS_ThreadHandlerIndex[next]->Priority < OS_ThreadHandlerIndex[OS_RunningThreadID]->Priority) //The next thread selected has a lower priority than the current thread
    {
        if (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus == RUNNING) //The current thread is not blocked or actively suspended, do not switch threads to give up CPU usage, continue to run
        {
            ++OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime; //Keep running the current thread
        }
        else
        {
            ThreadSwitch(next);
        }
    }
    else if (OS_ThreadHandlerIndex[next]->Priority < OS_ThreadHandlerIndex[OS_RunningThreadID]->Priority) //The next thread selected has the same priority as the current thread
    {
        if (OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime < OS_ThreadHandlerIndex[OS_RunningThreadID]->Ticks) //Time slice rotation
        {
            ++OS_ThreadHandlerIndex[OS_RunningThreadID]->RunTime; //Continues to run the current thread when time slice is not over
        }
        else
        {
            ThreadSwitch(next);
        }
    }
    else //The next thread selected has a higher priority than the current thread, and immediately switches threads to preempt the CPU
    {
        ThreadSwitch(next);
    }
}

#if USING_CPUUSAGE
ray_uint8_t GetCPUUsage(void)
{
    return OS_CPUUsage;
}
#endif
