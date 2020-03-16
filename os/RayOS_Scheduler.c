#include <STC15Fxxxx.H>
#include <INTRINS.H>
#include "RayOS.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)
extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //调度器运行时的栈，OS独享此栈
ray_uint8_t idata TaskStack[STACK_SIZE]; //线程实际运行时的栈，所有线程共享此栈
ray_uint32_t xdata CPUTicks = 0;         //系统运行时间
ray_uint32_t xdata idleCPUTicks = 0;     //系统空闲时间
#if USING_CPUUSAGE
ray_uint8_t xdata CPUUsage = 0; //CPU占用率
#endif

//进行上下文切换时的中转变量数组
ray_uint8_t data StackPointer;
ray_uint8_t data SFRStack[5];
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
    ray_uint8_t i;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == DELETED)
            return i;
    }
    return ThreadNumber;
}

//GPIO初始化
void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        //结构定义
    GPIO_InitStructure.Pin = GPIO_Pin_All;      //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); //初始化GPIO_P0
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure); //初始化GPIO_P1
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); //初始化GPIO_P2
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure); //初始化GPIO_P3
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure); //初始化GPIO_P4
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure); //初始化GPIO_P5
}
//定时器初始化
void Timer_config(void)
{
    TIM_InitTypeDef TIM_InitStructure;                //结构定义
    TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload; //指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_Polity = PolityHigh;        //指定中断优先级, PolityHigh,PolityLow
    TIM_InitStructure.TIM_Interrupt = ENABLE;         //中断是否允许,   ENABLE或DISABLE
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;   //指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut = ENABLE;            //是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value = T0VAL;              //初值,
    TIM_InitStructure.TIM_Run = ENABLE;               //是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);        //初始化Timer0      Timer0,Timer1,Timer2
}
//串口初始化
void UART_config(void)
{
    COMx_InitDefine COMx_InitStructure;               //结构定义
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer2;     //使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;      //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;        //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = ENABLE;       //波特率加倍, ENABLE或DISABLE
    COMx_InitStructure.UART_Interrupt = ENABLE;       //中断允许,   ENABLE或DISABLE
    COMx_InitStructure.UART_Polity = PolityLow;       //中断优先级, PolityLow,PolityHigh
    COMx_InitStructure.UART_P_SW = UART1_SW_P30_P31;  //切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
    COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;  //内部短路RXD与TXD, 做中继, ENABLE,DISABLE
    USART_Configuration(USART1, &COMx_InitStructure); //初始化串口1 USART1,USART2
}

//系统初始化
void SystemInit(void)
{
    GPIO_config();
    Timer_config();
    UART_config();
    EA = 1;
    PrintString1("OS Start Running!\r\n"); //SUART1发送一个字符串
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
    if (tid >= THREAD_MAX || priority > PRIORITY_MAX || ticks <= 0)
    { // 线程ID限制在0~THREAD_MAX之间 线程优先级限制在0~PRIORITY_MAX之间 时间片限制在大于0
        return 0xff;
    }                                                                                      //参数超出范围限制返回错误
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

#if USING_CPUUSAGE
ray_uint8_t GetCPUUsage(void)
{
    return CPUUsage;
}
#endif

//空闲线程
void idle(void)
{
    main_user();
    while (1)
    {
#if USING_IDLEHOOK
        idleHookFunction();
#else
        _nop_();
#endif
    }
}

void main()
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
