#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "RayOSDef.h"
#if USING_IDLEHOOK
void IdleHookFunctionSet(void (*hook)(void));
void IdleHookFunctionReset(void);
#endif
#if USING_CPUUSAGE
ray_uint8_t GetCPUUsage(void);
#endif
#endif
