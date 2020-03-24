#include "RayOS.h"

extern ray_thread_t OS_ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t OS_RunningThreadID;

#if USING_MAILBOX // Mailbox function, used for inter-thread communication, is not perfect and has not been tested
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail)
{
    ray_uint8_t i;
    if (mailbox->status == FULL) //The mailbox is full and cannot be sent, the thread is blocked
    {
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadMailBox = mailbox;
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus = BLOCKED;
        OS_ThreadHandlerIndex[OS_RunningThreadID]->BlockEvent = SEND;
        while (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //Mailbox is empty, send mail, and wake up a thread waiting to receive mail
    {
        for (i = 0; i <= THREAD_MAX; ++i)
        {
            if (OS_ThreadHandlerIndex[i]->ThreadStatus == BLOCKED && OS_ThreadHandlerIndex[i]->BlockEvent == RECIEVE && OS_ThreadHandlerIndex[i]->ThreadMailBox == mailbox)
            {
                OS_ThreadHandlerIndex[i]->ThreadStatus = READY;
                OS_ThreadHandlerIndex[i]->BlockEvent = NONE;
                OS_ThreadHandlerIndex[i]->ThreadMailBox = RAY_NULL;
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
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadMailBox = mailbox;
        OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus = BLOCKED;
        OS_ThreadHandlerIndex[OS_RunningThreadID]->BlockEvent = RECIEVE;
        while (OS_ThreadHandlerIndex[OS_RunningThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //Mailbox is full, pick up mail, and wake up a thread waiting to send mail
    {
        *mail = mailbox->mail;
        mailbox->status = EMPTY;
        for (i = 0; i <= THREAD_MAX; ++i)
        {
            if (OS_ThreadHandlerIndex[i]->ThreadStatus == BLOCKED && OS_ThreadHandlerIndex[i]->BlockEvent == SEND && OS_ThreadHandlerIndex[i]->ThreadMailBox == mailbox)
            {
                OS_ThreadHandlerIndex[i]->ThreadStatus = READY;
                OS_ThreadHandlerIndex[i]->BlockEvent = NONE;
                OS_ThreadHandlerIndex[i]->ThreadMailBox = RAY_NULL;
                break;
            }
        }
    }
}
#endif
