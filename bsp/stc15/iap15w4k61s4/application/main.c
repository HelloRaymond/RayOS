#include "RayOS.h"
#include "example.h"

ray_uint8_t tid1, tid2;

void main_user(void)
{
    IdleHookFunctionSet(FeedDog);        //Set the idle thread hook function to feed the dog when idle
    tid1 = ThreadCreate(Flicker, 10, 1); //Create indicator thread
    tid2 = ThreadCreate(Clock, 10, 2);   //Create clock thread
    ThreadStart(tid1);                   //Start indicator thread
    ThreadStart(tid2);                   //Start clock thread
}
