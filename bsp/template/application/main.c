#include "RayOS.h"
#include "example.h"

ray_uint8_t tid1, tid2;

void main_user(void)
{
    IdleHookFunctionSet(FeedDog);        //���ÿ����̹߳��Ӻ���������ʱι��
    tid1 = ThreadCreate(Flicker, 10, 1); //����ָʾ���߳�
    tid2 = ThreadCreate(Clock, 10, 2);   //����ʱ���߳�
    ThreadStart(tid1);                   //����ָʾ���߳�
    ThreadStart(tid2);                   //����ʱ���߳�
}
