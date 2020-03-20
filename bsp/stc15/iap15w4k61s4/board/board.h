#ifndef _BOARD_H_
#define _BOARD_H_
#include "RayOSDef.h"

#define T0VAL (65536 - TICKS * FOSC / DEVIDER / 1000)
void SystemInit(void);
void OS_ENTER_CRITICAL(void);
void OS_EXIT_CRITICAL(void);

#endif
