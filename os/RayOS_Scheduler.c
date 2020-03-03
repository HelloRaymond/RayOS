#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "RayOS.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)
extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //调度器运行时的栈，OS独享此栈
ray_uint8_t idata TaskStack[STACK_SIZE]; //线程实际运行时的栈，所有线程共享此栈

//进行上下文切换时的中转变量数组
ray_uint8_t data StackPointer;
enum sfr_e
{
    psw = 0,
    acc,
    b,
    dpl,
    dph
};
ray_uint8_t data SFRStack[5];
enum gpr_e
{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
};
ray_uint8_t data GPRStack[8];

//用于调度器与系统模块间相互通信的全局变量
struct ray_tcb_t xdata TCBHeap[THREAD_MAX];
ray_thread_t ThreadHandlerIndex[THREAD_MAX];
ray_uint8_t ThreadNumber = 0;
ray_uint8_t CurrentThreadID;

#if USING_IDLEHOOK
void defaultIdleHookFunction(void)
{
    _nop_();
}
void (*idleHookFunction)(void) = defaultIdleHookFunction;
#endif

//寻找优先级最高的非当前就绪线程
ray_uint8_t FindHighestPriorityThreadID(void)
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

//寻找空闲TCB槽
ray_uint8_t FindAvailableTID(void)
{
    ray_uint8_t i, result;
    result = 0;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == DELETED)
            return i;
    }
    return ThreadNumber;
}

//定时器初始化
void SystemInit(void)
{
    //初始化定时器 初始化代码参考STC范例程序
#if DEVIDER == 12
//  AUXR &= 0x7f;                   //定时器0为12T模式
#else
    AUXR |= 0x80; //定时器0为1T模式
#endif
    TMOD = 0x00; //设置定时器为模式0(16位自动重装载)
    TL0 = T0VAL; //初始化计时值
    TH0 = T0VAL >> 8;
    TR0 = 1; //定时器0开始计时
    ET0 = 1; //使能定时器0中断
    EA = 1;
}

//堆栈清零函数
void StackInit(ray_uint8_t stack[], ray_uint8_t stacksize)
{
    ray_uint8_t i;
    for (i = 0; i < stacksize; ++i)
        stack[i] = 0;
}

//创建线程
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority)
{
    ray_uint8_t tid = FindAvailableTID();
    if (tid == THREAD_MAX - 1 || priority > PRIORITY_MAX) //超过线程数量或优先级的范围限制返回错误
    {
        return 0xff;
    }
    ThreadHandlerIndex[tid] = &TCBHeap[tid];                                               //分配线程控制块
    StackInit(ThreadHandlerIndex[tid]->ThreadStack, STACK_SIZE);                           //TCB栈初始化
    StackInit(ThreadHandlerIndex[tid]->ThreadSFRStack, 5);                                 //TCBSFR栈初始化
    StackInit(ThreadHandlerIndex[tid]->ThreadGPRStack, 8);                                 //TCBGPR栈初始化
    ThreadHandlerIndex[tid]->EntryFunction = EntryFunction;                                //TCB入口函数
    ThreadHandlerIndex[tid]->ThreadStatus = INITIAL;                                       //线程状态：初始化
    ThreadHandlerIndex[tid]->BlockEvent = NONE;                                            //阻塞事件：无
    ThreadHandlerIndex[tid]->DelayTime = 0;                                                //延时：无
    ThreadHandlerIndex[tid]->Ticks = ticks;                                                //时间片
    ThreadHandlerIndex[tid]->RunTime = 0;                                                  //已运行时间
    ThreadHandlerIndex[tid]->ThreadSemaphore = RAY_NULL;                                   //等待接收或发送的信号量
    ThreadHandlerIndex[tid]->ThreadID = tid;                                               //线程ID
    ThreadHandlerIndex[tid]->Priority = priority;                                          //线程优先级
    ThreadHandlerIndex[tid]->ThreadStackPointer = (ray_uint8_t)TaskStack + 1;              //SP指针初始化：指向栈顶
    ThreadHandlerIndex[tid]->ThreadStack[0] = (ray_uint16_t)EntryFunction & 0x00ff;        //栈顶初始化为入口函数地址低8位
    ThreadHandlerIndex[tid]->ThreadStack[1] = ((ray_uint16_t)EntryFunction >> 8) & 0x00ff; //栈顶+1初始化为入口函数地址高8位
    ++ThreadNumber;
    return ThreadHandlerIndex[tid]->ThreadID;
}

//启动线程
ray_err_t ThreadStart(ray_uint8_t tid)
{
    if (ThreadHandlerIndex[tid]->ThreadStatus != INITIAL)
    {
        return RAY_ERROR;
    }
    //    else
    //    {
    ThreadHandlerIndex[tid]->ThreadStatus = READY;
    return RAY_EOK;
    //    }
}

//删除线程
ray_err_t ThreadDelete(ray_uint8_t tid)
{
    if (ThreadHandlerIndex[tid]->ThreadStatus != DELETED)
    {
        ThreadHandlerIndex[tid]->ThreadStatus = DELETED;
        --ThreadNumber;
        return RAY_EOK;
    }
    return RAY_ERROR;
}

void ThreadScan(void) //扫描更新线程状态
{
    ray_uint8_t i;
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

//延时函数，单位为毫秒，若延时时间不是系统时钟周期的整数倍，则会产生误差
ray_err_t ThreadDelayMs(ray_uint16_t time)
{
    ray_err_t err;
    err = time % TICKS == 0 ? RAY_EOK : RAY_ERROR;                //若延时时间不是系统时钟周期的整数倍，则会产生误差
    err = ThreadSleep(time / TICKS) == RAY_EOK ? err : RAY_ERROR; //若延时时间不是系统时钟周期的整数倍，但延时成功，返回警告，若延时失败，返回错误，若延时时间是系统时钟周期的整数倍且延时成功，返回OK
    return err;
}

#if USING_IDLEHOOK
void IdleHookFunctionSet(void (*hook)(void))
{
    idleHookFunction = hook;
}

void IdleHookFunctionReset(void)
{
    idleHookFunction = defaultIdleHookFunction;
}
#endif

//空闲线程
void idle(void)
{
    main_user();
    while (1)
#if USING_IDLEHOOK
        idleHookFunction();
#else
        _nop_();
#endif
}

void main()
{
    SystemInit();
    ThreadCreate(idle, 1, 0);
    ThreadHandlerIndex[0]->ThreadStackPointer = (ray_uint8_t)TaskStack - 1;
    ThreadHandlerIndex[0]->ThreadStatus = RUNNING;
    ThreadHandlerIndex[0]->EntryFunction();
    CurrentThreadID = 0;
    //阻塞在这里，防止程序意外跑飞
    while (1)
        _nop_();
}
