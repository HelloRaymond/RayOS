#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "RayOS.h"

extern ray_thread_t ThreadHandlerIndex[THREAD_MAX];
extern ray_uint8_t CurrentThreadID;

#if USING_MAILBOX //���书�ܣ������̼߳�ͨ�ţ��������ƣ���δ���в���
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail)
{
    ray_uint8_t i;
    if (mailbox->status == FULL) //�������������޷����з��ͣ����̱߳�����
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadMailBox = mailbox;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        ThreadHandlerIndex[CurrentThreadID]->BlockEvent = SEND;
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //����գ������ʼ���������һ���ȴ���ȡ�ʼ����߳�
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
    if (mailbox->status == EMPTY) //����գ����޷�������ȡ�����̱߳�����
    {
        ThreadHandlerIndex[CurrentThreadID]->ThreadMailBox = mailbox;
        ThreadHandlerIndex[CurrentThreadID]->ThreadStatus = BLOCKED;
        ThreadHandlerIndex[CurrentThreadID]->BlockEvent = RECIEVE;
        while (ThreadHandlerIndex[CurrentThreadID]->ThreadStatus == BLOCKED)
            _nop_();
    }
    else //������������ȡ�ʼ���������һ���ȴ������ʼ����߳�
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
