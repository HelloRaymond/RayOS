#include "RayOS.h"
#include "scheduler.h"
#include "board.h"
#include "compiler.h"

extern idata ray_uint8_t OS_XStackBuffer[]; //The actual stack when the thread is running, all threads share this stack
extern ray_thread_t OS_ThreadHandlerIndex[];
extern ray_uint8_t OS_RunningThreadID;

SFR(SP_REG, 0x81);
idata ray_uint8_t OSStack[10];

//modified 2020.03.23 change asm into c by raymond
//The compiler will automatically save the context when an interrupt occurs, we do not need to do it ourselves
INTERRUPT(Timer0ISR, 1)
{
    OS_ENTER_CRITICAL();
    OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStackPointer = (void *)SP_REG;      //MOV TaskSP, SP
    SP_REG = (ray_uint8_t)OSStack - 1;                                                   //MOV SP, #(OSStack - 1)
    Timer_Reload();                                                                      //LCALL Timer_Reload
    ThreadScan();                                                                        //LCALL ThreadScan
    SP_REG = (ray_uint8_t)OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStackPointer; //MOV SP, TaskSP
    OS_EXIT_CRITICAL();
}

void ThreadSwitchTo(ray_thread_t thread)
{
    thread->ThreadStatus = RUNNING;
    thread->ThreadStackPointer = (void *)(thread->ThreadStack - 1);
    OS_RunningThreadID = thread->ThreadID;
    SP_REG = (ray_uint8_t)thread->ThreadStack - 1;
    thread->EntryFunction();
}
