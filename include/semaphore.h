#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include "RayOSDef.h"
void SemaphoreTake(ray_sem_t *ThreadSemaphore);
void SemaphoreRelease(ray_sem_t *ThreadSemaphore);
#endif
