#ifndef _THREAD_H_
#define _THREAD_H_
#include "RayOSDef.h"

ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority);
ray_err_t ThreadStart(ray_uint8_t tid);
ray_err_t ThreadDelete(ray_uint8_t tid);
ray_err_t ThreadSleep(ray_uint16_t time);
ray_err_t ThreadDelayMs(ray_uint16_t time);

#endif
