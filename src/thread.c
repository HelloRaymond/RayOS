#include "RayOS.h"
#include "thread.h"
#include "board.h"

extern void main_user(void);

idata ray_uint8_t OS_XStackBuffer[STACK_SIZE]; //The actual stack when the thread is running, all threads share this stack
idata ray_uint8_t idleStack[STACK_SIZE];       //The idle thread stack

//Global variables for communication between the scheduler and system modules
struct ray_tcb_t TCBHeap[THREAD_MAX];
ray_thread_t OS_ThreadHandlerIndex[THREAD_MAX];
ray_uint8_t OS_ThreadNumber = 0;
ray_uint8_t OS_RunningThreadID;

//Finding free TCB slots
static ray_uint8_t FindAvailableTID(void)
{
    ray_uint8_t i;
    for (i = 1; i < THREAD_MAX; ++i)
    {
        if (OS_ThreadHandlerIndex[i]->ThreadStatus == DELETED)
            return i;
    }
    return OS_ThreadNumber;
}

//Array clear function
static void StackInit(ray_uint8_t stack[], ray_uint8_t stacksize)
{
    ray_uint8_t i;
    for (i = 0; i < stacksize; ++i)
        stack[i] = 0;
}

//Create a thread
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint8_t *stack, ray_uint8_t stack_depth, ray_uint16_t ticks, ray_uint8_t priority, ray_bool_t XStack)
{
    ray_uint8_t tid = FindAvailableTID();
    if (tid >= THREAD_MAX || priority > PRIORITY_MAX || ticks <= 0 || stack_depth > STACK_SIZE)
    { //Thread ID is limited to 0 ~ THREAD_MAX Thread priority is limited to 0 ~ PRIORITY_MAX Thread stack is limited to less than the maximum stack size (STACK_SIZE)
        return 0xff;
    }                                                                                                 //Parameter out of range limit returned error
    OS_ThreadHandlerIndex[tid] = &TCBHeap[tid];                                                       //Allocate thread control block
    OS_ThreadHandlerIndex[tid]->ThreadStack = stack;                                                  //thread stack initialization
    OS_ThreadHandlerIndex[tid]->ThreadStackDepth = stack_depth;                                       //stack size definition
    StackInit(OS_ThreadHandlerIndex[tid]->ThreadStack, OS_ThreadHandlerIndex[tid]->ThreadStackDepth); //TCB stack initialization
    OS_ThreadHandlerIndex[tid]->EntryFunction = EntryFunction;                                        //Thread entry function
    OS_ThreadHandlerIndex[tid]->ThreadStatus = INITIAL;                                               //Thread Status: Initialized
    OS_ThreadHandlerIndex[tid]->BlockEvent = NONE;                                                    //Blocking event: None
    OS_ThreadHandlerIndex[tid]->DelayTime = 0;                                                        //Delay: None
    OS_ThreadHandlerIndex[tid]->Ticks = ticks;                                                        //Time slice
    OS_ThreadHandlerIndex[tid]->RunTime = 0;                                                          //Elapsed time
#if USING_SEMAPHORE
    OS_ThreadHandlerIndex[tid]->ThreadSemaphore = RAY_NULL; //Semaphores waiting to be received or sent
#endif
    OS_ThreadHandlerIndex[tid]->ThreadID = tid;      //Thread ID
    OS_ThreadHandlerIndex[tid]->Priority = priority; //Thread priority
    if (XStack)
    {
        OS_ThreadHandlerIndex[tid]->ThreadStackPointer = (void *)(OS_XStackBuffer + 1 + CONTEXT_SIZE); //SP pointer initialization: point to the top of the stack
        OS_ThreadHandlerIndex[tid]->ThreadStackType = XStack;
    }
    else
    {
        OS_ThreadHandlerIndex[tid]->ThreadStackPointer = (void *)(stack + 1 + CONTEXT_SIZE); //SP pointer initialization: point to the top of the stack
        OS_ThreadHandlerIndex[tid]->ThreadStackType = Stack;
    }
    OS_ThreadHandlerIndex[tid]->ThreadStack[0] = (ray_uint16_t)EntryFunction & 0x00ff;        //The top of the stack is initialized to the lower 8 bits of the entry function address
    OS_ThreadHandlerIndex[tid]->ThreadStack[1] = ((ray_uint16_t)EntryFunction >> 8) & 0x00ff; //The top of the stack +1 is initialized to the upper 8 bits of the entry function address
    ++OS_ThreadNumber;
    return OS_ThreadHandlerIndex[tid]->ThreadID;
}

//Start thread
ray_err_t ThreadStart(ray_uint8_t tid)
{
    if (OS_ThreadHandlerIndex[tid]->ThreadStatus != INITIAL)
    {
        return RAY_ERROR;
    }
    //    else
    //    {
    OS_ThreadHandlerIndex[tid]->ThreadStatus = READY;
    return RAY_EOK;
    //    }
}

//Delete thread
ray_err_t ThreadDelete(ray_uint8_t tid)
{
    if (OS_ThreadHandlerIndex[tid]->ThreadStatus != DELETED)
    {
        OS_ThreadHandlerIndex[tid]->ThreadStatus = DELETED;
        --OS_ThreadNumber;
        return RAY_EOK;
    }
    return RAY_ERROR;
}

//Sleep function, the unit is Tick, which is a time slice
ray_err_t ThreadSleep(ray_uint16_t time)
{
    if (OS_ThreadHandlerIndex[OS_RunningThreadID]->DelayTime + time >= 0xffff || time <= 0)
        return RAY_ERROR;
    OS_ThreadHandlerIndex[OS_RunningThreadID]->DelayTime += time;
    OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus = BLOCKED;
    OS_ThreadHandlerIndex[OS_RunningThreadID]->BlockEvent = DELAY;
    while (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus == BLOCKED)
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
    OS_ENTER_CRITICAL();
    SystemInit();
    ThreadCreate(idle, idleStack, STACK_SIZE, 1, 0, False);
    ThreadSwitchTo(OS_ThreadHandlerIndex[0]);
    //The program cannot run here, otherwise reset
    while (1)
        (*(void (*)())0)();
}
