#include "RayOS.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_MAILBOX // Mailbox function, used for inter-thread communication, is not perfect and has not been tested
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail)
{
    ray_uint8_t i;
    if (mailbox->status == FULL) //The mailbox is full and cannot be sent, the thread is blocked
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadMailBox = mailbox;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        ThreadHandlerIndex[CurrentThreadID]->BlockEvent = SEND;
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //Mailbox is empty, send mail, and wake up a thread waiting to receive mail
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
    if (mailbox->status == EMPTY) //If the mailbox is empty, you cannot collect it, the thread is blocked
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadMailBox = mailbox;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        ThreadHandlerIndex[CurrentThreadID]->BlockEvent = RECIEVE;
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //Mailbox is full, pick up mail, and wake up a thread waiting to send mail
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
