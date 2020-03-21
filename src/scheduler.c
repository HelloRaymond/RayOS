#include "RayOS.h"

//Array of transit variables during context switching
ray_uint8_t data StackPointer;
ray_uint8_t data SFRStack[5];
ray_uint8_t data GPRStack[8];

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;
extern ray_uint8_t idata TaskStack[STACK_SIZE]; //The actual stack when the thread is running, all threads share this stack

ray_uint32_t CPUTicks;     //System runtime
ray_uint32_t idleCPUTicks; //System idle time
#if USING_CPUUSAGE
ray_uint8_t CPUUsage; //CPU Usage
#endif

//Finding the highest priority ready thread
static ray_uint8_t FindHighestPriorityThreadID(void)
{
    ray_uint8_t i, result;
    result = 0;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (ThreadHandlerIndex[i]->ThreadStatus != READY || (ThreadHandlerIndex[i]->ThreadID == CurrentThreadID))
            continue;
        else if (ThreadHandlerIndex[i]->Priority > ThreadHandlerIndex[result]->Priority)
            result = i;
    }
    return result;
}

void ThreadScan(void) //Scan and update thread status
{
    ray_uint8_t i;
    ++CPUTicks;
    if (CurrentThreadID == 0)
        ++idleCPUTicks;
#if USING_CPUUSAGE
    if (CPUTicks % 1000 == 0)
    {
        CPUUsage = (1000 - idleCPUTicks) / 10;
        idleCPUTicks = 0;
    }
#endif
    if (ThreadHandlerIndex[0]->ThreadStatus != READY && ThreadHandlerIndex[0]->ThreadStatus != RUNNING) //Idle thread are not allowed to block
        ThreadHandlerIndex[0]->ThreadStatus = READY;
    if (ThreadHandlerIndex[CurrentThreadID]->RunTime > ThreadHandlerIndex[CurrentThreadID]->Ticks) //cleared RunTime after a time slice completed
        ThreadHandlerIndex[CurrentThreadID]->RunTime = 0;
    for (i = 0; i <= THREAD_MAX; ++i) //Handle blocked threads
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == BLOCKED)
        {
#if USING_SEMAPHORE
            if (ThreadHandlerIndex[i]->BlockEvent == WAIT) //Threads waiting for a semaphore
            {
                if (ThreadHandlerIndex[i]->ThreadSemaphore == RAY_NULL) //Receive a semaphore and wake up the thread
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#endif
            if (ThreadHandlerIndex[i]->BlockEvent == DELAY) //Actively delay pending threads
            {
                --ThreadHandlerIndex[i]->DelayTime;        //Delay count--
                if (ThreadHandlerIndex[i]->DelayTime == 0) //When the delay time expires, wake up the thread
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#if USING_MAILBOX
            if (ThreadHandlerIndex[i]->BlockEvent == SEND || ThreadHandlerIndex[i]->BlockEvent == RECIEVE) //Pending thread waiting to receive or send mail
            {
                if (ThreadHandlerIndex[i]->ThreadMailBox == RAY_NULL) //Wake thread
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#endif
        }
    }
}

void ThreadSwitch(void)
{
    ray_uint8_t i, next;
    next = FindHighestPriorityThreadID(); //Looking for the next highest priority ready thread
    if (next == CurrentThreadID)          //Skip switching if there are no other ready threads except idle thread
    {
        ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //Keep running the current thread
        return;
    }
    if (ThreadHandlerIndex[next]->Priority < ThreadHandlerIndex[CurrentThreadID]->Priority) //The next thread selected has a lower priority than the current thread
    {
        if (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == RUNNING) //The current thread is not blocked or actively suspended, do not switch threads to give up CPU usage, continue to run
        {
            ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //Keep running the current thread
            return;
        }
    }
    else if (ThreadHandlerIndex[next]->Priority < ThreadHandlerIndex[CurrentThreadID]->Priority) //The next thread selected has the same priority as the current thread
    {
        if (ThreadHandlerIndex[CurrentThreadID]->RunTime < ThreadHandlerIndex[CurrentThreadID]->Ticks) //Time slice rotation
        {
            ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //Continues to run the current thread when time slice is not over
            return;
        }
    } //The next thread selected has a higher priority than the current thread, and immediately switches threads to preempt the CPU

    //Save context data (SP pointer, SFR, GPR, stack) to TCB
    ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer = StackPointer;
    for (i = 0; i < 5; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadSFRStack[i] = SFRStack[i];
    for (i = 0; i < 8; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadGPRStack[i] = GPRStack[i];
    for (i = 0; i < STACK_SIZE; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadStack[i] = TaskStack[i];
    if (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == RUNNING)
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = READY;

    //Switching threads
    CurrentThreadID = next;
    ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = RUNNING;
    ++ThreadHandlerIndex[CurrentThreadID]->RunTime;

    //Recover Context Data in TCB
    StackPointer = ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer;
    for (i = 0; i < 5; ++i)
        SFRStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadSFRStack[i];
    for (i = 0; i < 8; ++i)
        GPRStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadGPRStack[i];
    for (i = 0; i < STACK_SIZE; ++i)
        TaskStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadStack[i];
}

//Sleep function, the unit is Tick, which is a time slice
ray_err_t ThreadSleep(ray_uint16_t time)
{
    if (ThreadHandlerIndex[CurrentThreadID]->DelayTime + time >= 0xffff || time <= 0)
        return RAY_ERROR;
    ThreadHandlerIndex[CurrentThreadID]->DelayTime += time;
    ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
    ThreadHandlerIndex[CurrentThreadID]->BlockEvent = DELAY;
    while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
        ;
    return RAY_EOK;
}

#if USING_CPUUSAGE
ray_uint8_t GetCPUUsage(void)
{
    return CPUUsage;
}
#endif
