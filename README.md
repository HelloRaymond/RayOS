# RayOS - 学习操作系统 #

[TOC]

## 简介 ##

​	RayOS是一个非常简单的操作系统，由于太过简单甚至不能称之为操作系统，目前只在8051单片机上实现，而且只实现了线程调度（抢占式时间片轮转）、线程同步（信号量）、线程通信（有限）的基本功能，由于8051系列单片机资源性能实在有限，系统时钟周期太长保证不了实时性，太短又会导致调度器本身占用资源太大，所以本项目的实际应用意义不大，只是为了学习操作系统相关知识而动手实践。



## 目录结构 ##

​	项目由Keil4 C51 IDE开发，安装好keil4后，直接打开根目录下的RayOS.uvproj文件即可。

> **/os 目录**
>
> - RayOSKernel.asm 操作系统核心的汇编文件，实现了上下文切换和上电初始化的相关功能
> - RayOS_Scheduler.c 调度器模块C语言文件，实现了操作系统的线程调度功能
> - RayOS_Semaphore.c 信号量模块C语言文件，实现了操作系统的线程同步功能
> - RayOS_MailBox.c 邮箱模块C语言文件，实现了操作系统的邮箱线程通信功能（开发中，尚未完善）
> - RayOS.h 操作系统公用头文件，包含了一些类型定义、宏定义和API函数声明
> - RayOSConfig.h 操作系统配置头文件，用于裁剪和配置操作系统功能

> **/user 目录**
>
> - main.c 用户代码入口文件，用户需要在main_user()函数中创建并启动线程
> - isr.c 中断服务函数文件，用户的中断服务函数需要在这里定义，当触发中断时系统会自动保存恢复上下文



## API函数说明

### 创建线程

```c
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint16_t ticks, ray_uint8_t priority);
```

- 参数：

|      参数     |         说明          |     类型     |
| :-----------: | :-------------------: | :---------: |
| EntryFunction |      线程入口函数     | void pointer |
| ticks         | 线程一个时间片的周期数 | unsigned int |
| priority      |       线程优先级      | unsigned char |

- 返回值：

|  返回值 |  说明  |     类型      |
| :-----: | :----: | :----------: |
|   tid   | 线程ID | unsigned char |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 暂不支持参数传递
> 2. 高优先级线程必须以阻塞式方式运行
> 3. 当返回值小于0，表明创建线程失败


### 启动线程

```c
ray_err_t ThreadStart(ray_uint8_t tid);
```

- 参数：

| 参数 |     说明      |     类型     |
| :-: | :-----------: | :----------: |
| tid | 要启动的线程ID | unsigned char |

- 返回值：

| 返回值 |       说明        |    类型    |
| :----: | :---------------: | :-------: |
| status | 执行情况及错误代码 | ray_err_t |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 只可以启动创建后尚未运行的线程


### 删除线程

```c
ray_err_t ThreadDelete(ray_uint8_t tid);
```

- 参数：

| 参数 |      说明      |     类型      |
| :--: | :-----------: | :-----------: |
| tid  | 要启动的线程ID | unsigned char |

- 返回值：

| 返回值 |       说明        |    类型    |
| :----: | :---------------: | :-------: |
| status | 执行情况及错误代码 | ray_err_t |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 线程被删除后将不会再被调度器调用，且如果有新的线程被创建，其线程控制块将会被占用


### 线程休眠

```c
ray_err_t ThreadSleep(ray_uint16_t time);
```

- 参数：

| 参数 |         说明          |     类型     |
| :--: | :------------------: | :----------: |
| time | 要休眠的系统心跳周期数 | unsigned int |

- 返回值：

| 返回值 |       说明        |    类型    |
| :----: | :---------------: | :---- --: |
| status | 执行情况及错误代码 | ray_err_t |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 若休眠时间为零或超过65535，将操作失败并返回error


### 线程延时

```c
ray_err_t ThreadDelayMs(ray_uint16_t time);
```

- 参数：

| 参数 |            说明             |    类型      |
| :--: | :------------------------: | :----------: |
| time | 要延时的时间长度，单位：毫秒 | unsigned int |

- 返回值：

| 返回值 |       说明         |   类型    |
| :----: | :---------------: | :-------: |
| status | 执行情况及错误代码 | ray_err_t |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 若延时时间为零或超过系统心跳周期，将操作失败并返回error
> 2. 若延时的时间不是系统心跳周期的整数倍，延时会产生误差，函数返回warning


### 释放信号量

```c
void SemaphoreRealease(ray_sem_t *ThreadSemaphore);
```

- 参数：

|       参数      |        说明        |    类型    |
| :-------------: | :---------------: | :--------: |
| ThreadSemaphore | 要释放的信号量指针 | ray_sem_t* |

- 返回值：

| 返回值 |   说明   |  类型  |
| :----: | :-----: | :----: |
|  void  | 无返回值 |  void  |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 若信号量值大于0，则将进程队列中第一个因等待信号量而阻塞的线程唤醒
> 2. 需要将配置文件中的USING_SEMAPHORE宏定义为1


### 接收信号量

```c
void SemaphoreTake(ray_sem_t *ThreadSemaphore);
```

- 参数：

|       参数      |        说明        |    类型    |
| :-------------: | :---------------: | :--------: |
| ThreadSemaphore | 要接收的信号量指针 | ray_sem_t* |

- 返回值：

| 返回值 |   说明   |  类型  |
| :----: | :-----: | :----: |
|  void  | 无返回值 |  void  |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 若信号量值大于0，则进行接收，否则将当前线程挂起
> 2. 需要将配置文件中的USING_SEMAPHORE宏定义为1

### 发送邮件

```c
void MailSend(ray_mailbox_t *mailbox, ray_uint32_t mail);
```

- 参数：

|  参数   |         说明          |      类型      |
| :-----: | :------------------: | :------------: |
| mailbox |   要发送的邮箱指针    | ray_mailbox_t* |
|  mail   | 要发送的邮件（4字节） | unsigned long  |

- 返回值：

| 返回值 |   说明   |  类型  |
| :----: | :-----: | :----: |
|  void  | 无返回值 |  void  |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 若邮箱为空，则进行发送，否则将当前线程挂起
> 2. 需要将配置文件中的USING_MAILBOX宏定义为1

### 接收邮件

```c
void MailRecieve(ray_mailbox_t *mailbox, ray_uint32_t *mail);
```

- 参数：

|  参数   |         说明          |      类型      |
| :-----: | :------------------: | :------------: |
| mailbox |   要接收的邮箱指针    | ray_mailbox_t* |
|  mail   | 要接收的邮件（4字节） | unsigned long  |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :-----: | :--: |
|  void  | 无返回值 | void |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 若邮箱为满，则进行接收，否则将当前线程挂起
> 2. 需要将配置文件中的USING_MAILBOX宏定义为1

### 设置空闲线程钩子

```c
void IdleHookFunctionSet(void (*hook)(void));
```

- 参数：

| 参数 |     说明     |     类型     |
| :--: | :---------: | :----------: |
| hook | 钩子函数指针 | void pointer |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :-----: | :--: |
|  void  | 无返回值 | void |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 需要将配置文件中的USING_IDLEHOOK宏定义为1

### 取消空闲线程钩子

```c
void IdleHookFunctionSet(void);
```

- 参数：

| 参数 | 说明 |     类型     |
| :--: | :--: | :----------: |
| void |  无  | void pointer |

- 返回值：

| 返回值 |   说明   | 类型 |
| :----: | :------: | :--: |
|  void  | 无返回值 | void |

- 需要包含的头文件：/os/RayOS.h

- 备注：

> 1. 需要将配置文件中的USING_IDLEHOOK宏定义为1