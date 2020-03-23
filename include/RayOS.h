#ifndef _RAYOS_H_
#define _RAYOS_H_
#include "RayOSDef.h"
#include "compiler.h"

#include "thread.h"
#include "scheduler.h"

#if USING_SEMAPHORE
#include "semaphore.h"
#endif

#if USING_MAILBOX
#include "mailbox.h"
#endif

#endif
