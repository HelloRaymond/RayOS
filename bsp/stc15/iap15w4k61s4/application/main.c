#include "RayOS.h"
#include "example.h"

ray_uint8_t tid1, tid2;

idata ray_uint8_t stack1[20];
xdata ray_uint8_t stack2[30];

void main_user(void)
{
    IdleHookFunctionSet(FeedDog);                           //Set the idle thread hook function to feed the dog when idle
    tid1 = ThreadCreate(Flicker, stack1, 10, 10, 1, False); //Create indicator thread
    tid2 = ThreadCreate(Clock, stack2, 20, 10, 2, True);    //Create clock thread
    ThreadStart(tid1);                                      //Start indicator thread
    ThreadStart(tid2);                                      //Start clock thread
}
