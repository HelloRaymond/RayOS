#include "board.h"
#include "RayOS.h"

extern void main_user(void);

ray_uint8_t idata OSStack[STACK_SIZE];   //The stack when the scheduler is running. This stack is exclusive to the OS.
ray_uint8_t idata TaskStack[STACK_SIZE]; //The actual stack when the thread is running, all threads share this stack

//Global variables for communication between the scheduler and system modules
struct ray_tcb_t TCBHeap[THREAD_MAX];
ray_thread_t ThreadHandlerIndex[THREAD_MAX];
ray_uint8_t ThreadNumber = 0;
ray_uint8_t CurrentThreadID;

//Finding free TCB slots
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

//Array clear function
static void StackInit(ray_uint8_t stack[], ray_uint8_t stacksize)
{
    ray_uint8_t i;
    for (i = 0; i < stacksize; ++i)
        stack[i] = 0;
}

//Create a thread
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority)
{
    ray_uint8_t tid = FindAvailableTID();
    if (tid >= THREAD_MAX || priority > PRIORITY_MAX || ticks <= 0)
    { //Thread ID is limited to 0 ~ THREAD_MAX Thread priority is limited to 0 ~ PRIORITY_MAX Time slice is limited to greater than 0
        return 0xff;
    }                                                            //Parameter out of range limit returned error
    ThreadHandlerIndex[tid] = &TCBHeap[tid];                     //Allocate thread control block
    StackInit(ThreadHandlerIndex[tid]->ThreadStack, STACK_SIZE); //TCB stack initialization
    StackInit(ThreadHandlerIndex[tid]->ThreadSFRStack, 5);       //TCB SFR stack initialization
    StackInit(ThreadHandlerIndex[tid]->ThreadGPRStack, 8);       //TCB GPR initialization
    ThreadHandlerIndex[tid]->EntryFunction = EntryFunction;      //Thread entry function
    ThreadHandlerIndex[tid]->ThreadStatus = INITIAL;             //Thread Status: Initialized
    ThreadHandlerIndex[tid]->BlockEvent = NONE;                  //Blocking event: None
    ThreadHandlerIndex[tid]->DelayTime = 0;                      //Delay: None
    ThreadHandlerIndex[tid]->Ticks = ticks;                      //Time slice
    ThreadHandlerIndex[tid]->RunTime = 0;                        //Elapsed time
#if USING_SEMAPHORE
    ThreadHandlerIndex[tid]->ThreadSemaphore = RAY_NULL; //Semaphores waiting to be received or sent
#endif
    ThreadHandlerIndex[tid]->ThreadID = tid;                                               //Thread ID
    ThreadHandlerIndex[tid]->Priority = priority;                                          //Thread priority
    ThreadHandlerIndex[tid]->ThreadStackPointer = (ray_uint8_t)TaskStack + 1;              //SP pointer initialization: point to the top of the stack
    ThreadHandlerIndex[tid]->ThreadStack[0] = (ray_uint16_t)EntryFunction & 0x00ff;        //The top of the stack is initialized to the lower 8 bits of the entry function address
    ThreadHandlerIndex[tid]->ThreadStack[1] = ((ray_uint16_t)EntryFunction >> 8) & 0x00ff; //The top of the stack +1 is initialized to the upper 8 bits of the entry function address
    ++ThreadNumber;
    return ThreadHandlerIndex[tid]->ThreadID;
}

//Start thread
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

//Delete thread
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

//Delay function in milliseconds. If the delay time is not an integer multiple of the system clock period, an error will occur
ray_err_t ThreadDelayMs(ray_uint16_t time)
{
    ray_err_t err;
    err = time % TICKS == 0 ? RAY_EOK : RAY_ERROR;                //If the delay time is not an integer multiple of the system clock cycle, an error will occur
    err = ThreadSleep(time / TICKS) == RAY_EOK ? err : RAY_ERROR; //If the delay time is not an integer multiple of the system clock cycle, but the delay is successful, a warning is returned. If the delay fails, an error is returned. If the delay time is an integer multiple of the system clock cycle and the delay is successful, OK is returned.
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

//Idle thread
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
    //The program cannot run here, otherwise reset
    while (1)
        (*(void (*)())0)();
}
