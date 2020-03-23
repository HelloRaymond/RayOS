#include "RayOS.h"
#include "scheduler.h"
#include "board.h"

extern ray_uint8_t idata TaskStack[]; //The actual stack when the thread is running, all threads share this stack
extern ray_thread_t ThreadHandlerIndex[];
extern ray_uint8_t CurrentThreadID;

SFR(SP_REG, 0x81);
ray_uint8_t idata OSStack[10];

//modified 2020.03.23 change asm into c by raymond
//The compiler will automatically save the context when an interrupt occurs, we do not need to do it ourselves
void Timer0ISR(void) interrupt 1
{
    OS_ENTER_CRITICAL();
    ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer = (void *)SP_REG;      //MOV TaskSP, SP
    SP_REG = OSStack - 1;                                                          //MOV SP, #(OSStack - 1)
    Timer_Reload();                                                                //LCALL Timer_Reload
    ThreadScan();                                                                  //LCALL ThreadScan
    ThreadSwitch();                                                                //LCALL ThreadSwitch
    SP_REG = (ray_uint8_t)ThreadHandlerIndex[CurrentThreadID]->ThreadStackPointer; //MOV SP, TaskSP
    OS_EXIT_CRITICAL();
}
