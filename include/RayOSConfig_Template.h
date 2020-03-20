#ifndef _RAYOSCONFIG_H_
#define _RAYOSCONFIG_H_

// 晶振频率（单位Hz）
#define FOSC 12000000L
// 定时器分频系数
#define DEVIDER 12
// 系统滴答周期长度（单位ms）
#define TICKS 2
// 最大线程数
#define THREAD_MAX 5
// 最大优先级
#define PRIORITY_MAX 5
// 栈深度
#define STACK_SIZE 10

// 使用信号量功能
#define USING_SEMAPHORE 0
// 使用邮箱功能
#define USING_MAILBOX 0
// 使用空闲线程钩子功能
#define USING_IDLEHOOK 0
// 使用CPU占用率统计
#define USING_CPUUSAGE 0
// 使用动态内存
#define USING_MEMHEAP 0

#endif
