#include <STC15Fxxxx.H>
#include <INTRINS.H>
#include "RayOS.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)
extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //����������ʱ��ջ��OS�����ջ
ray_uint8_t idata TaskStack[STACK_SIZE]; //�߳�ʵ������ʱ��ջ�������̹߳����ջ
ray_uint32_t xdata CPUTicks = 0;         //ϵͳ����ʱ��
ray_uint32_t xdata idleCPUTicks = 0;     //ϵͳ����ʱ��
#if USING_CPUUSAGE
ray_uint8_t xdata CPUUsage = 0; //CPUռ����
#endif

//�����������л�ʱ����ת��������
ray_uint8_t data StackPointer;
ray_uint8_t data SFRStack[5];
ray_uint8_t data GPRStack[8];

//���ڵ�������ϵͳģ����໥ͨ�ŵ�ȫ�ֱ���
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

//Ѱ�����ȼ���ߵķǵ�ǰ�����߳�
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

//Ѱ�ҿ���TCB��
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

//GPIO��ʼ��
void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        //�ṹ����
    GPIO_InitStructure.Pin = GPIO_Pin_All;      //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
    GPIO_InitStructure.Mode = GPIO_PullUp;      //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); //��ʼ��GPIO_P0
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure); //��ʼ��GPIO_P1
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); //��ʼ��GPIO_P2
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure); //��ʼ��GPIO_P3
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure); //��ʼ��GPIO_P4
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure); //��ʼ��GPIO_P5
}
//��ʱ����ʼ��
void Timer_config(void)
{
    TIM_InitTypeDef TIM_InitStructure;                //�ṹ����
    TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload; //ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_Polity = PolityHigh;        //ָ���ж����ȼ�, PolityHigh,PolityLow
    TIM_InitStructure.TIM_Interrupt = ENABLE;         //�ж��Ƿ�����,   ENABLE��DISABLE
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;   //ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut = ENABLE;            //�Ƿ������������, ENABLE��DISABLE
    TIM_InitStructure.TIM_Value = T0VAL;              //��ֵ,
    TIM_InitStructure.TIM_Run = ENABLE;               //�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);        //��ʼ��Timer0      Timer0,Timer1,Timer2
}
//���ڳ�ʼ��
void UART_config(void)
{
    COMx_InitDefine COMx_InitStructure;               //�ṹ����
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer2;     //ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;      //������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;        //��������,   ENABLE��DISABLE
    COMx_InitStructure.BaudRateDouble = ENABLE;       //�����ʼӱ�, ENABLE��DISABLE
    COMx_InitStructure.UART_Interrupt = ENABLE;       //�ж�����,   ENABLE��DISABLE
    COMx_InitStructure.UART_Polity = PolityLow;       //�ж����ȼ�, PolityLow,PolityHigh
    COMx_InitStructure.UART_P_SW = UART1_SW_P30_P31;  //�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
    COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;  //�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
    USART_Configuration(USART1, &COMx_InitStructure); //��ʼ������1 USART1,USART2
}

//ϵͳ��ʼ��
void SystemInit(void)
{
    GPIO_config();
    Timer_config();
    UART_config();
    EA = 1;
    PrintString1("OS Start Running!\r\n"); //SUART1����һ���ַ���
}

//��ջ���㺯��
void StackInit(ray_uint8_t stack[], ray_uint8_t stacksize)
{
    ray_uint8_t i;
    for (i = 0; i < stacksize; ++i)
        stack[i] = 0;
}

//�����߳�
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority)
{
    ray_uint8_t tid = FindAvailableTID();
    if (tid >= THREAD_MAX || priority > PRIORITY_MAX || ticks <= 0)
    { // �߳�ID������0~THREAD_MAX֮�� �߳����ȼ�������0~PRIORITY_MAX֮�� ʱ��Ƭ�����ڴ���0
        return 0xff;
    }                                                                                      //����������Χ���Ʒ��ش���
    ThreadHandlerIndex[tid] = &TCBHeap[tid];                                               //�����߳̿��ƿ�
    StackInit(ThreadHandlerIndex[tid]->ThreadStack, STACK_SIZE);                           //TCBջ��ʼ��
    StackInit(ThreadHandlerIndex[tid]->ThreadSFRStack, 5);                                 //TCBSFRջ��ʼ��
    StackInit(ThreadHandlerIndex[tid]->ThreadGPRStack, 8);                                 //TCBGPRջ��ʼ��
    ThreadHandlerIndex[tid]->EntryFunction = EntryFunction;                                //TCB��ں���
    ThreadHandlerIndex[tid]->ThreadStatus = INITIAL;                                       //�߳�״̬����ʼ��
    ThreadHandlerIndex[tid]->BlockEvent = NONE;                                            //�����¼�����
    ThreadHandlerIndex[tid]->DelayTime = 0;                                                //��ʱ����
    ThreadHandlerIndex[tid]->Ticks = ticks;                                                //ʱ��Ƭ
    ThreadHandlerIndex[tid]->RunTime = 0;                                                  //������ʱ��
    ThreadHandlerIndex[tid]->ThreadSemaphore = RAY_NULL;                                   //�ȴ����ջ��͵��ź���
    ThreadHandlerIndex[tid]->ThreadID = tid;                                               //�߳�ID
    ThreadHandlerIndex[tid]->Priority = priority;                                          //�߳����ȼ�
    ThreadHandlerIndex[tid]->ThreadStackPointer = (ray_uint8_t)TaskStack + 1;              //SPָ���ʼ����ָ��ջ��
    ThreadHandlerIndex[tid]->ThreadStack[0] = (ray_uint16_t)EntryFunction & 0x00ff;        //ջ����ʼ��Ϊ��ں�����ַ��8λ
    ThreadHandlerIndex[tid]->ThreadStack[1] = ((ray_uint16_t)EntryFunction >> 8) & 0x00ff; //ջ��+1��ʼ��Ϊ��ں�����ַ��8λ
    ++ThreadNumber;
    return ThreadHandlerIndex[tid]->ThreadID;
}

//�����߳�
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

//ɾ���߳�
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

void ThreadScan(void) //ɨ������߳�״̬
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
    if (ThreadHandlerIndex[0]->ThreadStatus != READY && ThreadHandlerIndex[0]->ThreadStatus != RUNNING) //�����̲߳���������
        ThreadHandlerIndex[0]->ThreadStatus = READY;
    if (ThreadHandlerIndex[CurrentThreadID]->RunTime > ThreadHandlerIndex[CurrentThreadID]->Ticks) //һ��ʱ��Ƭ������RunTime��0
        ThreadHandlerIndex[CurrentThreadID]->RunTime = 0;
    for (i = 0; i <= THREAD_MAX; ++i) //���������߳�
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == BLOCKED)
        {
#if USING_SEMAPHORE
            if (ThreadHandlerIndex[i]->BlockEvent == WAIT) //�ȴ��ź������߳�
            {
                if (ThreadHandlerIndex[i]->ThreadSemaphore == RAY_NULL) //�յ��ź����������߳�
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#endif
            if (ThreadHandlerIndex[i]->BlockEvent == DELAY) //������ʱ������߳�
            {
                --ThreadHandlerIndex[i]->DelayTime;        //��ʱ����--
                if (ThreadHandlerIndex[i]->DelayTime == 0) //��ʱʱ�䵽�������߳�
                {
                    ThreadHandlerIndex[i]->ThreadStatus = READY;
                    ThreadHandlerIndex[i]->BlockEvent = NONE;
                }
            }
#if USING_MAILBOX
            if (ThreadHandlerIndex[i]->BlockEvent == SEND || ThreadHandlerIndex[i]->BlockEvent == RECIEVE) //�ȴ���ȡ�����ʼ�������߳�
            {
                if (ThreadHandlerIndex[i]->ThreadMailBox == RAY_NULL) //�����߳�
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
    next = FindHighestPriorityThreadID(); //Ѱ����һ��������ȼ��ķǵ�ǰ����̬�߳�
    if (next == CurrentThreadID)          //���������߳��������������߳��������л�
    {
        ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //�������е�ǰ�߳�
        return;
    }
    if (ThreadHandlerIndex[next]->Priority < ThreadHandlerIndex[CurrentThreadID]->Priority) //ѡȡ����һ���߳����ȼ��ȵ�ǰ�߳�С
    {
        if (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == RUNNING) //��ǰ�߳�û���������������𣬲��л��߳��ó�CPUʹ��Ȩ����������
        {
            ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //�������е�ǰ�߳�
            return;
        }
    }
    else if (ThreadHandlerIndex[next]->Priority < ThreadHandlerIndex[CurrentThreadID]->Priority) //ѡȡ����һ���߳����ȼ��͵�ǰ�߳���ͬ
    {
        if (ThreadHandlerIndex[CurrentThreadID]->RunTime < ThreadHandlerIndex[CurrentThreadID]->Ticks) //����ʱ��Ƭ��ת
        {
            ++ThreadHandlerIndex[CurrentThreadID]->RunTime; //ʱ��Ƭû�����������е�ǰ�߳�
            return;
        }
    } //ѡȡ����һ���߳����ȼ��ȵ�ǰ�߳����ȼ��������л��߳���ռCPU

    //������������(SPָ�롢SFR��GPR��ջ)���ݵ��Լ���TCB��
    ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer = StackPointer;
    for (i = 0; i < 5; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadSFRStack[i] = SFRStack[i];
    for (i = 0; i < 8; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadGPRStack[i] = GPRStack[i];
    for (i = 0; i < STACK_SIZE; ++i)
        ThreadHandlerIndex[CurrentThreadID]->ThreadStack[i] = TaskStack[i];
    if (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == RUNNING)
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = READY;

    //�л��߳�
    CurrentThreadID = next;
    ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = RUNNING;
    ++ThreadHandlerIndex[CurrentThreadID]->RunTime;

    //���Լ�TCB�е����������ݻָ�
    StackPointer = ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer;
    for (i = 0; i < 5; ++i)
        SFRStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadSFRStack[i];
    for (i = 0; i < 8; ++i)
        GPRStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadGPRStack[i];
    for (i = 0; i < STACK_SIZE; ++i)
        TaskStack[i] = ThreadHandlerIndex[CurrentThreadID]->ThreadStack[i];
}

//��ʱ��������λΪTick����һ��ʱ��Ƭ
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

//��ʱ��������λΪ���룬����ʱʱ�䲻��ϵͳʱ�����ڵ������������������
ray_err_t ThreadDelayMs(ray_uint16_t time)
{
    ray_err_t err;
    err = time % TICKS == 0 ? RAY_EOK : RAY_ERROR;                //����ʱʱ�䲻��ϵͳʱ�����ڵ������������������
    err = ThreadSleep(time / TICKS) == RAY_EOK ? err : RAY_ERROR; //����ʱʱ�䲻��ϵͳʱ�����ڵ�������������ʱ�ɹ������ؾ��棬����ʱʧ�ܣ����ش�������ʱʱ����ϵͳʱ�����ڵ�����������ʱ�ɹ�������OK
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

//�����߳�
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
    // �����ѭ�������ϲ���ִ�У������Ӵ˾������������ʱż�����ֳ����ܷ�����
    // ���������е�������������λ
    while (1)
        (*(void (*)())0)();
}
