#ifndef _RAYOSCONFIG_H_
#define _RAYOSCONFIG_H_

// 晶振频率（单位Hz）
#define FOSC 24000000L
// 定时器分频系数
#define DEVIDER 1
// 系统心跳周期长度（单位ms）
#define TICKS 2
// 最大线程数
#define THREAD_MAX 10
// 最大优先级
#define PRIORITY_MAX 10
// 栈深度
#define STACK_SIZE 23

// 开启信号量功能
#define USING_SEMAPHORE 1
// 开启邮箱功能
#define USING_MAILBOX 0
// 开启空闲线程钩子功能
#define USING_IDLEHOOK 1

#endif
