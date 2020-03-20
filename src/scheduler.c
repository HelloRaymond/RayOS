#include "RayOS.h"

//进行上下文切换时的中转变量数组
ray_uint8_t data StackPointer;
ray_uint8_t data SFRStack[5];
ray_uint8_t data GPRStack[8];

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;
extern ray_uint8_t idata TaskStack[STACK_SIZE]; //线程实际运行时的栈，所有线程共享此栈

ray_uint32_t CPUTicks;     //系统运行时间
ray_uint32_t idleCPUTicks; //系统空闲时间
#if USING_CPUUSAGE
ray_uint8_t CPUUsage; //CPU占用率
#endif

//寻找优先级最高的非当前就绪线程
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

void ThreadScan(void) //扫描更新线程状态
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
    if (ThreadHandlerIndex[0]->ThreadStatus != READY && ThreadHandlerIndex[0]->ThreadStatus != RUNNING) //空闲线程不允许阻塞
        ThreadHandlerIndex[0]->ThreadStatus = READY;
    if (ThreadHandlerIndex[CurrentThreadID]->RunTime > ThreadHandlerIndex[CurrentThreadID]->Ticks) //一个时间片运行完RunTime清0
        ThreadHandlerIndex[CurrentThreadID]->RunTime = 0;
    for (i = 0; i <= THREAD_MAX; ++i) //处理阻塞线程
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == BLOCKED)
        {
#if USING_SEMAPHORE
            if (ThreadHandlerIndex[i]->BlockEvent == WAIT) //等待信号量的线程
            {
                if (ThreadHandlerIndex[i]->ThreadSemaphore == RAY_NULL) //收到信号量，唤醒线程
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#endif
            if (ThreadHandlerIndex[i]->BlockEvent == DELAY) //主动延时挂起的线程
            {
                --ThreadHandlerIndex[i]->DelayTime;        //延时计数--
                if (ThreadHandlerIndex[i]->DelayTime == 0) //延时时间到，唤醒线程
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#if USING_MAILBOX
            if (ThreadHandlerIndex[i]->BlockEvent == SEND || ThreadHandlerIndex[i]->BlockEvent == RECIEVE) //等待收取或发送邮件挂起的线程
            {
                if (ThreadHandlerIndex[i]->ThreadMailBox == RAY_NULL) //唤醒线程
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
    next = FindHighestPriorityThreadID(); //寻找下一个最高优先级的非当前就绪态线程
    if (next == CurrentThreadID)          //若除空闲线程外无其他就绪线程则跳过切换
    {
        ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //继续运行当前线程
        return;
    }
    if (ThreadHandlerIndex[next]->Priority < ThreadHandlerIndex[CurrentThreadID]->Priority) //选取的下一个线程优先级比当前线程小
    {
        if (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == RUNNING) //当前线程没有阻塞或主动挂起，不切换线程让出CPU使用权，继续运行
        {
            ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //继续运行当前线程
            return;
        }
    }
    else if (ThreadHandlerIndex[next]->Priority < ThreadHandlerIndex[CurrentThreadID]->Priority) //选取的下一个线程优先级和当前线程相同
    {
        if (ThreadHandlerIndex[CurrentThreadID]->RunTime < ThreadHandlerIndex[CurrentThreadID]->Ticks) //进行时间片轮转
        {
            ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //时间片没结束继续运行当前线程
            return;
        }
    } //选取的下一个线程优先级比当前线程优先级大，立即切换线程抢占CPU

    //将上下文数据(SP指针、SFR、GPR、栈)备份到自己的TCB中
    ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer = StackPointer;
    for (i = 0; i < 5; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadSFRStack[i] = SFRStack[i];
    for (i = 0; i < 8; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadGPRStack[i] = GPRStack[i];
    for (i = 0; i < STACK_SIZE; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadStack[i] = TaskStack[i];
    if (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == RUNNING)
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = READY;

    //切换线程
    CurrentThreadID = next;
    ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = RUNNING;
    ++ThreadHandlerIndex[CurrentThreadID]->RunTime;

    //将自己TCB中的上下文数据恢复
    StackPointer = ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer;
    for (i = 0; i < 5; ++i)
        SFRStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadSFRStack[i];
    for (i = 0; i < 8; ++i)
        GPRStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadGPRStack[i];
    for (i = 0; i < STACK_SIZE; ++i)
        TaskStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadStack[i];
}

//延时函数，单位为Tick，即一个时间片
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
