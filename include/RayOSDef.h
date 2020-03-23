#ifndef _RAYOSDEF_H_
#define _RAYOSDEF_H_
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
    ray_uint8_t *ThreadStack;
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
    void *ThreadStackPointer;
    ray_uint8_t ThreadStackDepth;
    ray_uint8_t ThreadID;
    ray_uint8_t Priority;
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

#endif
