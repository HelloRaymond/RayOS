#ifndef _RAYOS_H_
#define _RAYOS_H_
#include <STC15F2K60S2.H>
#include "RayOSConfig.h"

typedef unsigned char ray_uint8_t;
typedef unsigned int ray_uint16_t;
typedef unsigned long ray_uint32_t;
typedef char ray_int8_t;
typedef int ray_int16_t;
typedef long ray_int32_t;
#if USING_SEMAPHORE
typedef ray_int8_t ray_sem_t;
#endif
typedef ray_uint8_t ray_err_t;
#define RAY_NULL (void *)0
#define RAY_EOK 1
#define RAY_ERROR 0

#define OS_ENTER_CRITICAL() EA = 0
#define OS_EXIT_CRITICAL() EA = 1

#if USING_MAILBOX
typedef struct ray_mailbox_t
{
    ray_uint32_t mail;
    enum
    {
        EMPTY = 0,
        FULL
    } status;
} ray_mailbox_t;
#endif

struct ray_tcb_t
{
    ray_uint8_t ThreadStack[STACK_SIZE];
    enum
    {
        DELETED = 1,
        BLOCKED,
        RUNNING,
        INITIAL,
        READY
    } ThreadStatus;
    enum
    {
        NONE = 0,
        WAIT,
        SIGNSL,
        RECIEVE,
        SEND,
        DELAY
    } BlockEvent;
    ray_uint8_t ThreadID;
    ray_uint8_t Priority;
    ray_uint8_t ThreadStackPointer;
    ray_uint8_t ThreadGPRStack[8];
    ray_uint8_t ThreadSFRStack[5];
    void (*EntryFunction)(void);
    ray_uint16_t DelayTime;
    ray_uint16_t Ticks;
    ray_uint16_t RunTime;
#if USING_SEMAPHORE
    ray_sem_t *ThreadSemaphore;
#endif
#if USING_MAILBOX
    ray_mailbox_t *ThreadMailBox;
#endif
};

typedef struct ray_tcb_t *ray_thread_t;
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority);
ray_err_t ThreadStart(ray_uint8_t tid);
ray_err_t ThreadDelete(ray_uint8_t tid);
ray_err_t ThreadSleep(ray_uint16_t time);
ray_err_t ThreadDelayMs(ray_uint16_t time);

#if USING_SEMAPHORE
void SemaphoreTake(ray_sem_t *ThreadSemaphore);
void SemaphoreRelease(ray_sem_t *ThreadSemaphore);
#endif

#if USING_MAILBOX
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail);
void MailRecieve(ray_mailbox_t *mailbox, ray_uint32_t *mail);
#endif

#if USING_IDLEHOOK
void IdleHookFunctionSet(void (*hook)(void));
void IdleHookFunctionReset(void);
#endif

#if USING_CPUUSAGE
ray_uint8_t GetCPUUsage(void);
#endif

#endif
