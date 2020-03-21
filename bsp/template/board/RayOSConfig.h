#ifndef _RAYOSCONFIG_H_
#define _RAYOSCONFIG_H_

// Main clock frequency (unit: Hz)
#define FOSC 12000000L
// Timer divide factor
#define DEVIDER 12
// System tick cycle (unit: ms)
#define TICKS 2
// Maximum number of threads
#define THREAD_MAX 5
// Maximum priority
#define PRIORITY_MAX 5
// Stack depth
#define STACK_SIZE 10

// Using the semaphore function
#define USING_SEMAPHORE 0
// Use the mailbox function
#define USING_MAILBOX 0
// Use idle thread hook function
#define USING_IDLEHOOK 0
// Use CPU usage statistics
#define USING_CPUUSAGE 0

#endif
