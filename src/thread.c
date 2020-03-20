#include "board.h"
#include "RayOS.h"

extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //����������ʱ��ջ��OS�����ջ
ray_uint8_t idata TaskStack[STACK_SIZE]; //�߳�ʵ������ʱ��ջ�������̹߳����ջ

//���ڵ�������ϵͳģ����໥ͨ�ŵ�ȫ�ֱ���
struct ray_tcb_t TCBHeap[THREAD_MAX];
ray_thread_t ThreadHandlerIndex[THREAD_MAX];
ray_uint8_t ThreadNumber = 0;
ray_uint8_t CurrentThreadID;

//Ѱ�ҿ���TCB��
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

//��ջ���㺯��
static void StackInit(ray_uint8_t stack[], ray_uint8_t stacksize)
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
    }                                                            //����������Χ���Ʒ��ش���
    ThreadHandlerIndex[tid] = &TCBHeap[tid];                     //�����߳̿��ƿ�
    StackInit(ThreadHandlerIndex[tid]->ThreadStack, STACK_SIZE); //TCBջ��ʼ��
    StackInit(ThreadHandlerIndex[tid]->ThreadSFRStack, 5);       //TCBSFRջ��ʼ��
    StackInit(ThreadHandlerIndex[tid]->ThreadGPRStack, 8);       //TCBGPRջ��ʼ��
    ThreadHandlerIndex[tid]->EntryFunction = EntryFunction;      //TCB��ں���
    ThreadHandlerIndex[tid]->ThreadStatus = INITIAL;             //�߳�״̬����ʼ��
    ThreadHandlerIndex[tid]->BlockEvent = NONE;                  //�����¼�����
    ThreadHandlerIndex[tid]->DelayTime = 0;                      //��ʱ����
    ThreadHandlerIndex[tid]->Ticks = ticks;                      //ʱ��Ƭ
    ThreadHandlerIndex[tid]->RunTime = 0;                        //������ʱ��
#if USING_SEMAPHORE
    ThreadHandlerIndex[tid]->ThreadSemaphore = RAY_NULL; //�ȴ����ջ��͵��ź���
#endif
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

//��ʱ��������λΪ���룬����ʱʱ�䲻��ϵͳʱ�����ڵ������������������
ray_err_t ThreadDelayMs(ray_uint16_t time)
{
    ray_err_t err;
    err = time % TICKS == 0 ? RAY_EOK : RAY_ERROR;                //����ʱʱ�䲻��ϵͳʱ�����ڵ������������������
    err = ThreadSleep(time / TICKS) == RAY_EOK ? err : RAY_ERROR; //����ʱʱ�䲻��ϵͳʱ�����ڵ�������������ʱ�ɹ������ؾ��棬����ʱʧ�ܣ����ش�������ʱʱ����ϵͳʱ�����ڵ�����������ʱ�ɹ�������OK
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

//�����߳�
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
    // �����ѭ�������ϲ���ִ�У������Ӵ˾������������ʱż�����ֳ����ܷ�����
    // ���������е�������������λ
    while (1)
        (*(void (*)())0)();
}
