#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "RayOS.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)
extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //����������ʱ��ջ��OS�����ջ
ray_uint8_t idata TaskStack[STACK_SIZE]; //�߳�ʵ������ʱ��ջ�������̹߳����ջ

//�����������л�ʱ����ת��������
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
    ray_uint8_t i, result;
    result = 0;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (ThreadHandlerIndex[i]->ThreadStatus == DELETED)
            return i;
    }
    return ThreadNumber;
}

//��ʱ����ʼ��
void SystemInit(void)
{
    //��ʼ����ʱ�� ��ʼ������ο�STC��������
#if DEVIDER == 12
//  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ
#else
    AUXR |= 0x80; //��ʱ��0Ϊ1Tģʽ
#endif
    TMOD = 0x00; //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T0VAL; //��ʼ����ʱֵ
    TH0 = T0VAL >> 8;
    TR0 = 1; //��ʱ��0��ʼ��ʱ
    ET0 = 1; //ʹ�ܶ�ʱ��0�ж�
    EA = 1;
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
    if (tid == THREAD_MAX - 1 || priority > PRIORITY_MAX) //�����߳����������ȼ��ķ�Χ���Ʒ��ش���
    {
        return 0xff;
    }
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

//�����߳�
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
    //�����������ֹ���������ܷ�
    while (1)
        _nop_();
}
