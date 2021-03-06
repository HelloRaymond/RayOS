# RayOS - RTOS内核 #

![](https://img.shields.io/badge/version-1.1.0-brightgreen) ![](https://img.shields.io/github/languages/code-size/HelloRaymond/RayOS) ![](https://img.shields.io/github/commit-activity/m/HelloRaymond/RayOS) ![](https://img.shields.io/github/last-commit/HelloRaymond/RayOS)

[TOC]

**[English Documents](./README.md)**

## 简介 ##

​	RayOS是一个便于移植的非常简单的操作系统内核，目前在STC15系列(8051内核)单片机上实现，实现了线程调度（抢占式时间片轮转）、线程同步（信号量）、线程通信（邮箱）的基本功能。

**已知问题：**

- 多个较为复杂的线程并发运行时会导致系统崩溃，引起异常复位

**原因分析：**

- keil编译环境下，函数默认是不支持重入的，而多个线程并发时函数发生了重入

   > ​	和PC程序不同，keil编译环境下，函数的局部变量不保存在栈中，而是采用了内存覆盖策略，即在编译时直接指定局部变量的地址，对于不存在直接调用关系的不同函数，编译器会将它们的局部变量分配为同一个内存地址，所以当在多个线程间切换时，会导致局部变量的值发生改变，从而引起系统运行异常

**解决方案：**

1. 将线程函数所有的局部变量定义为static，避免内存覆盖
   - 优点：不影响系统实时性
   - 缺点：内存占用会增加，任务较多、较复杂或内存资源短缺时不宜采用

2. 使用"reentrant"关键字将线程函数定义为可重入函数（不推荐）

3. 在任务开始前关闭中断，结束后再开启中断，避免函数重入
   - 优点：不会增加内存占用
   - 缺点：频繁开关中断影响实时性，实时性要求较高时不宜采用



## 目录结构 ##

​	项目文件存放在对应的bsp目录中，目前已经支持iap15w4k61s4最小系统板，其对应的bsp目录为bsp/stc15/iap15w4k61s4

> **/bsp 目录**
>
> ​	板级支持包目录，目前已经支持iap15w4k61s4最小系统板
>
> ​	[了解更多](./bsp/README_zh.md)

> **/arch 目录**
>
> ​	和CPU有关的汇编代码，包含启动文件和上下文切换代码

> **/include 目录**
>
> ​	RayOS的头文件目录

> **/kernel 目录**
>
> ​	RayOS内核代码文件

> **/tools 目录**
>
> ​	配置工具（尚在开发中，敬请期待）



## 注意事项

- 优先级数值越大，优先级越高，0为最低优先级
- 实时性要求较高的线程（如总线通信）必须设置为高优先级
- 高优先级线程必须以阻塞式方式运行，否则会长期占用CPU从而导致低优先级的任务无法运行
- 空闲线程的优先级为0，应尽量避免将用户线程的优先级设置为0，可以使用空闲线程钩子功能来代替
- 线程创建后为INITIAL状态，并不会自动开始运行，需要使用ThreadStart()函数将其启动



## API函数说明

### 创建线程

```c
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint8_t *stack, ray_uint8_t stack_depth, ray_uint16_t ticks, ray_uint8_t priority, ray_bool_t XStack);
```

- 参数：

|     参数      |      说明      |       类型       |
| :-----------: | :------------: | :--------------: |
| EntryFunction |  线程入口函数  | function pointer |
|     stack     |     线程栈     | unsigned char[]  |
|  stack_depth  |  线程栈的深度  |  unsigned char   |
|     ticks     |   线程时间片   |   unsigned int   |
|   priority    |   线程优先级   |  unsigned char   |
|    XStack     | 是否使用模拟栈 |       bool       |

- 返回值：

| 返回值 |  说明  |     类型      |
| :----: | :----: | :-----------: |
|  tid   | 线程ID | unsigned char |

- 备注：

> 1. 线程的栈有两种类型：真实栈和模拟栈，用XStack参数指定，若为True则使用模拟栈类型，若为False使用真实栈类型
>
>    - ​	真实栈线程直接运行在其设置的栈上，模拟栈线程运行在内部内存的缓冲区，当线程切换时，将缓冲区和模拟栈中的数据进行互相拷贝
>
>    - ​	若线程栈在外部内存，则必须使用模拟栈类型
>
> 2. 暂不支持参数传递
>
> 3. 线程栈只能以静态方式分配，你需要定义一个静态的数组用作线程栈
>
> 4. 高优先级线程必须以阻塞式方式运行
>
> 5. 当返回值小于0，表明创建线程失败


### 启动线程

```c
ray_err_t ThreadStart(ray_uint8_t tid);
```

- 参数：

| 参数 |      说明      |     类型      |
| :--: | :------------: | :-----------: |
| tid  | 要启动的线程ID | unsigned char |

- 返回值：

| 返回值 |        说明        |   类型    |
| :----: | :----------------: | :-------: |
| status | 执行情况及错误代码 | ray_err_t |

- 备注：

> 1. 只可以启动创建后尚未运行的线程


### 删除线程

```c
ray_err_t ThreadDelete(ray_uint8_t tid);
```

- 参数：

| 参数 |      说明      |     类型      |
| :--: | :------------: | :-----------: |
| tid  | 要删除的线程ID | unsigned char |

- 返回值：

| 返回值 |        说明        |   类型    |
| :----: | :----------------: | :-------: |
| status | 执行情况及错误代码 | ray_err_t |

- 备注：

> 1. 线程被删除后将不会再被调度器调用，且如果有新的线程被创建，其线程控制块将会被占用


### 线程休眠

```c
ray_err_t ThreadSleep(ray_uint16_t time);
```

- 参数：

| 参数 |          说明          |     类型     |
| :--: | :--------------------: | :----------: |
| time | 要休眠的系统滴答周期数 | unsigned int |

- 返回值：

| 返回值 |   说明   |   类型    |
| :----: | :------: | :-------: |
| status | 错误代码 | ray_err_t |

- 备注：

> 1. 若休眠时间为零或超过65535，将操作失败并返回error


### 线程延时

```c
ray_err_t ThreadDelayMs(ray_uint16_t time);
```

- 参数：

| 参数 |             说明             |     类型     |
| :--: | :--------------------------: | :----------: |
| time | 要延时的时间长度，单位：毫秒 | unsigned int |

- 返回值：

| 返回值 |        说明        |   类型    |
| :----: | :----------------: | :-------: |
| status | 执行情况及错误代码 | ray_err_t |

- 备注：

> 1. 若延时时间为零或超过系统滴答周期，将操作失败并返回error
> 2. 若延时的时间不是系统滴答周期的整数倍，延时会产生误差，函数返回warning


### 释放信号量

```c
void SemaphoreRelease(ray_sem_t *ThreadSemaphore);
```

- 参数：

|      参数       |        说明        |    类型    |
| :-------------: | :----------------: | :--------: |
| ThreadSemaphore | 要释放的信号量指针 | ray_sem_t* |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 备注：

> 1. 若信号量值大于0，则将进程队列中第一个因等待信号量而阻塞的线程唤醒
> 2. 需要将配置文件中的USING_SEMAPHORE宏定义为1


### 接收信号量

```c
void SemaphoreTake(ray_sem_t *ThreadSemaphore);
```

- 参数：

|      参数       |        说明        |    类型    |
| :-------------: | :----------------: | :--------: |
| ThreadSemaphore | 要接收的信号量指针 | ray_sem_t* |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 备注：

> 1. 若信号量值大于0，则进行接收，否则将当前线程挂起
> 2. 需要将配置文件中的USING_SEMAPHORE宏定义为1

### 发送邮件

```c
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail);
```

- 参数：

|  参数   |         说明          |      类型       |
| :-----: | :-------------------: | :-------------: |
| mailbox |   要发送的邮箱指针    | ray_mailbox_t*  |
|  mail   | 要发送的邮件（4字节） | unsigned long * |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 备注：

> 1. 若邮箱为空，则进行发送，否则将当前线程挂起
> 2. 需要将配置文件中的USING_MAILBOX宏定义为1

### 接收邮件

```c
void MailRecieve(ray_mailbox_t *mailbox, ray_uint32_t *mail);
```

- 参数：

|  参数   |         说明          |      类型       |
| :-----: | :-------------------: | :-------------: |
| mailbox |   要接收的邮箱指针    | ray_mailbox_t*  |
|  mail   | 要接收的邮件（4字节） | unsigned long * |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 备注：

> 1. 若邮箱为满，则进行接收，否则将当前线程挂起
> 2. 需要将配置文件中的USING_MAILBOX宏定义为1

### 设置空闲线程钩子

```c
void IdleHookFunctionSet(void (*hook)(void));
```

- 参数：

| 参数 |     说明     |       类型       |
| :--: | :----------: | :--------------: |
| hook | 钩子函数指针 | function pointer |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 备注：

> 1. 需要将配置文件中的USING_IDLEHOOK宏定义为1

### 取消空闲线程钩子

```c
void IdleHookFunctionReset(void);
```

- 参数：

| 参数 | 说明 | 类型 |
| :--: | :--: | :--: |
| void |  无  | void |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 备注：

> 1. 需要将配置文件中的USING_IDLEHOOK宏定义为1

### 获取CPU占用率

```c
ray_uint8_t GetCPUUsage(void);
```

- 参数：

| 参数 | 说明 | 类型 |
| :--: | :--: | :--: |
| void |  无  | void |

- 返回值：

|  返回值  |           说明            |    类型     |
| :------: | :-----------------------: | :---------: |
| CPUUsage | CPU占用率，百分比保留整数 | ray_uint8_t |

- 备注：

> 1. 需要将配置文件中的USING_CPUUSAGE宏定义为1