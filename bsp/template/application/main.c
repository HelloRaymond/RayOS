#include "RayOS.h"
#include "example.h"

ray_uint8_t tid1, tid2;

void main_user(void)
{
    IdleHookFunctionSet(FeedDog);        //设置空闲线程钩子函数，空闲时喂狗
    tid1 = ThreadCreate(Flicker, 10, 1); //创建指示灯线程
    tid2 = ThreadCreate(Clock, 10, 2);   //创建时钟线程
    ThreadStart(tid1);                   //启动指示灯线程
    ThreadStart(tid2);                   //启动时钟线程
}
