#include "board.h"
#include "RayOS.h"

extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //调度器运行时的栈，OS独享此栈
ray_uint8_t idata TaskStack[STACK_SIZE]; //线程实际运行时的栈，所有线程共享此栈

//用于调度器与系统模块间相互通信的全局变量
struct ray_tcb_t TCBHeap[THREAD_MAX];
ray_thread_t ThreadHandlerIndex[THREAD_MAX];
ray_uint8_t ThreadNumber = 0;
ray_uint8_t CurrentThreadID;

//寻找空闲TCB槽
static ray_uint8_t FindAvailableTID(void)
{
    ray_uint8_t i;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == DELETED)
            return i;
    }
    return ThreadNumber;
}

//堆栈清零函数
static void StackInit(ray_uint8_t stack[], ray_uint8_t stacksize)
{
    ray_uint8_t i;
    for (i = 0; i < stacksize; ++i)
        stack[i] = 0;
}

//创建线程
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority)
{
    ray_uint8_t tid = FindAvailableTID();
    if (tid >= THREAD_MAX || priority > PRIORITY_MAX || ticks <= 0)
    { // 线程ID限制在0~THREAD_MAX之间 线程优先级限制在0~PRIORITY_MAX之间 时间片限制在大于0
        return 0xff;
    }                                                            //参数超出范围限制返回错误
    ThreadHandlerIndex[tid] = &TCBHeap[tid];                     //分配线程控制块
    StackInit(ThreadHandlerIndex[tid]->ThreadStack, STACK_SIZE); //TCB栈初始化
    StackInit(ThreadHandlerIndex[tid]->ThreadSFRStack, 5);       //TCBSFR栈初始化
    StackInit(ThreadHandlerIndex[tid]->ThreadGPRStack, 8);       //TCBGPR栈初始化
    ThreadHandlerIndex[tid]->EntryFunction = EntryFunction;      //TCB入口函数
    ThreadHandlerIndex[tid]->ThreadStatus = INITIAL;             //线程状态：初始化
    ThreadHandlerIndex[tid]->BlockEvent = NONE;                  //阻塞事件：无
    ThreadHandlerIndex[tid]->DelayTime = 0;                      //延时：无
    ThreadHandlerIndex[tid]->Ticks = ticks;                      //时间片
    ThreadHandlerIndex[tid]->RunTime = 0;                        //已运行时间
#if USING_SEMAPHORE
    ThreadHandlerIndex[tid]->ThreadSemaphore = RAY_NULL; //等待接收或发送的信号量
#endif
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

//延时函数，单位为毫秒，若延时时间不是系统时钟周期的整数倍，则会产生误差
ray_err_t ThreadDelayMs(ray_uint16_t time)
{
    ray_err_t err;
    err = time % TICKS == 0 ? RAY_EOK : RAY_ERROR;                //若延时时间不是系统时钟周期的整数倍，则会产生误差
    err = ThreadSleep(time / TICKS) == RAY_EOK ? err : RAY_ERROR; //若延时时间不是系统时钟周期的整数倍，但延时成功，返回警告，若延时失败，返回错误，若延时时间是系统时钟周期的整数倍且延时成功，返回OK
    return err;
}

#if USING_IDLEHOOK
static void defaultIdleHookFunction(void)
{
}
void (*idleHookFunction)(void) = defaultIdleHookFunction;

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
static void idle(void)
{
    main_user();
    while (1)
    {
#if USING_IDLEHOOK
        idleHookFunction();
#else
        ;
#endif
    }
}

void main(void)
{
    SystemInit();
    ThreadCreate(idle, 1, 0);
    ThreadHandlerIndex[0]->ThreadStackPointer = (ray_uint8_t)TaskStack - 1;
    ThreadHandlerIndex[0]->ThreadStatus = RUNNING;
    CurrentThreadID = 0;
    ThreadHandlerIndex[0]->EntryFunction();
    // 下面的循环理论上不会执行，但不加此句软件仿真运行时偶尔出现程序跑飞现象
    // 若程序运行到这里，进行软件复位
    while (1)
        (*(void (*)())0)();
}
