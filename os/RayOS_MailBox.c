#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "RayOS.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_MAILBOX //邮箱功能，用于线程间通信，还不完善，尚未进行测试
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail)
{
    ray_uint8_t i;
    if (mailbox->status == FULL) //邮箱已满，则无法进行发送，该线程被阻塞
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadMailBox = mailbox;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        ThreadHandlerIndex[CurrentThreadID]->BlockEvent = SEND;
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //邮箱空，发送邮件，并唤醒一个等待收取邮件的线程
    {
        for (i = 0; i <= THREAD_MAX; ++i)
        {
            if (ThreadHandlerIndex[i]->ThreadStatus == BLOCKED && ThreadHandlerIndex[i]->BlockEvent == RECIEVE && ThreadHandlerIndex[i]->ThreadMailBox == mailbox)
            {
                ThreadHandlerIndex[i]->ThreadStatus = READY;
                ThreadHandlerIndex[i]->BlockEvent = NONE;
                ThreadHandlerIndex[i]->ThreadMailBox = RAY_NULL;
                break;
            }
        }
        mailbox->mail = mail;
        mailbox->status = FULL;
    }
}

void MailRecieve(ray_mailbox_t *mailbox, ray_uint32_t *mail)
{
    ray_uint8_t i;
    if (mailbox->status == EMPTY) //邮箱空，则无法进行收取，该线程被阻塞
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadMailBox = mailbox;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        ThreadHandlerIndex[CurrentThreadID]->BlockEvent = RECIEVE;
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //邮箱已满，收取邮件，并唤醒一个等待发送邮件的线程
    {
        *mail = mailbox->mail;
        mailbox->status = EMPTY;
        for (i = 0; i <= THREAD_MAX; ++i)
        {
            if (ThreadHandlerIndex[i]->ThreadStatus == BLOCKED && ThreadHandlerIndex[i]->BlockEvent == SEND && ThreadHandlerIndex[i]->ThreadMailBox == mailbox)
            {
                ThreadHandlerIndex[i]->ThreadStatus = READY;
                ThreadHandlerIndex[i]->BlockEvent = NONE;
                ThreadHandlerIndex[i]->ThreadMailBox = RAY_NULL;
                break;
            }
        }
    }
}
#endif
