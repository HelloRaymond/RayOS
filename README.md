# RayOS - RTOS Kernel #

![](https://img.shields.io/badge/version-1.1.0-brightgreen) ![](https://img.shields.io/github/languages/code-size/HelloRaymond/RayOS) ![](https://img.shields.io/github/commit-activity/m/HelloRaymond/RayOS) ![](https://img.shields.io/github/last-commit/HelloRaymond/RayOS)

[TOC]

**[中文文档](./README_zh.md)**

## Introduction ##

​	RayOS is a very simple operating system kernel that is easy to port. It is currently implemented on STC15 series (8051 core) MCU. It implements the basic functions of thread scheduling (preemptive scheduling & time slice rotation), thread synchronization (semaphore), and thread communication (mailbox).

**Known Issues:**

- Concurrent running of multiple complex threads will cause the system to crash and cause an abnormal reset.

**Cause Analysis:**

- In the keil compilation environment, functions do not support reentrancy by default, and functions reenter when multiple threads are concurrent.
  
    > ​	Unlike PC programs, in the keil compilation environment, local variables of functions are not stored in the stack, but a memory coverage strategy is adopted, that is, the address of the local variable is directly specified at compile stage. The compiler will assign local variables of different functions without direct calling relationships to the same memory address, so when switching between threads, the value of the local variables will change, which will cause the system to run abnormally.

**Solution:**

1. Define all local variables of the thread function as static to avoid memory overwriting
    - Pros: Does not affect system real-time
    - Cons: memory consumption will increase, it should not be used when there are more complex tasks or more shortage of memory resources

2. Use the "reentrant" keyword to define a thread function as a reentrant function (not recommended)

3. Turn off the interrupt before the task starts, and then turn on the interrupt after the end to avoid function reentry
    - Pros: Does not increase memory consumption
    - Cons: frequent switching interruption affects real-time performance, it should not be used when real-time performance requirements are high



## Directory Structure ##

​	The project files are stored in the corresponding bsp directory. Currently, the iap15w4k61s4 minimum system board is supported. The corresponding bsp directory is bsp/stc15/iap15w4k61s4

> **/bsp directory**
>
> ​	Board Support Package directory, iap15w4k61s4 minimum system board is currently supported
>
> ​	[Learn More](./bsp/README.md)

> **/arch directory**
>
> ​	CPU-related assembly code, including startup files and context switch code

> **/include directory**
>
> ​	RayOS header file directory

> **/kernel directory**
>
> ​	RayOS kernel code file

> **/tools directory**
>
> ​	Configuration tools (still in development, stay tuned)



## Precautions

- The higher the value of priority, the higher the priority, 0 is the lowest priority
- Threads with high real-time requirements (such as bus communication) must be set to high priority
- High-priority threads must run in a blocking manner, otherwise the CPU will be occupied for a long time and low-priority tasks cannot run
- The priority of idle threads is 0. Try to avoid setting the priority of user threads to 0. You can use the idle thread hook function instead
- After the thread is created, it is in the INITIAL state and does not automatically start running. You need to use the ThreadStart() function to start it



## API function description

### Create a thread

```c
ray_uint8_t ThreadCreate(void (*EntryFunction)(void), ray_uint8_t *stack, ray_uint8_t stack_depth, ray_uint16_t ticks, ray_uint8_t priority, ray_bool_t XStack);
```

- Parameters:

|  Parameters   |             Description             |       Type       |
| :-----------: | :---------------------------------: | :--------------: |
| EntryFunction |        thread entry function        | function pointer |
|     stack     |            thread stack             | unsigned char[]  |
|  stack_depth  |          thread stack size          |  unsigned char   |
|     ticks     |          thread time slice          |   unsigned int   |
|   priority    |           thread priority           |  unsigned char   |
|    XStack     | Whether to use the simulation stack |       bool       |

- return value:

| Return Value | Description |     Type      |
| :----------: | :---------: | :-----------: |
|     tid      |  thread ID  | unsigned char |

- Remarks:

> 1. There are two types of thread stacks: real stack and simulated stack, which are specified with XStack parameters. If True, the simulated stack type is used. If False, the real stack type is used.
>
>    - ​	The real stack thread runs directly on the set stack, and the simulated stack thread runs in the internal memory buffer. When the thread switches, the buffer and the simulated stack are copied to each other.
>
>    - ​	If the thread stack is in external memory, you must set stack type as simulated stack.
>
> 2. Does not support parameter passing
>
> 3. The thread stack can only be allocated statically, you need to reserve a static array to use as the thread stack
>
> 4. High priority threads must run in a blocking manner
>
> 5. When the return value is less than 0, it indicates that the thread creation fails


### Start the thread

```c
ray_err_t ThreadStart (ray_uint8_t tid);
```

- Parameters:

| Parameters |    Description     |     Type      |
| :--------: | :----------------: | :-----------: |
|    tid     | thread ID to start | unsigned char |

- return value:

| Return Value |           Description           |   Type    |
| :----------: | :-----------------------------: | :-------: |
|    status    | execution status and error code | ray_err_t |

- Remarks:

> 1. Only threads that have not run since creation can be started


### Delete a thread

```c
ray_err_t ThreadDelete (ray_uint8_t tid);
```

- Parameters:

| Parameters |     Description     |     Type      |
| :--------: | :-----------------: | :-----------: |
|    tid     | thread ID to delete | unsigned char |

- return value:

| Return Value |           Description           |   Type    |
| :----------: | :-----------------------------: | :-------: |
|    status    | execution status and error code | ray_err_t |

- Remarks:

> 1. After the thread is deleted, it will not be called by the scheduler, and if a new thread is created, its thread control block will be occupied


### Thread sleep

```c
ray_err_t ThreadSleep (ray_uint16_t time);
```

- Parameters:

| Parameters |         Description         |     Type     |
| :--------: | :-------------------------: | :----------: |
|    time    | system tick cycles to sleep | unsigned int |

- return value:

| Return Value | Description |   Type    |
| :----------: | :---------: | :-------: |
|    status    | error code  | ray_err_t |

- Remarks:

> 1. If the sleep time is zero or exceeds 65535, the operation fails and error is returned


### Thread delay

```c
ray_err_t ThreadDelayMs (ray_uint16_t time);
```

- Parameters:

| Parameters |                 Description                  |     Type     |
| :--------: | :------------------------------------------: | :----------: |
|    time    | the length of time to delay, in milliseconds | unsigned int |

- return value:

| Return Value |           Description           |   Type    |
| :----------: | :-----------------------------: | :-------: |
|    status    | execution status and error code | ray_err_t |

- Remarks:

> 1. If the delay time is zero or exceeds the system tick period, the operation will fail with error
> 2. If the delay time is not an integer multiple of the system tick period, the delay will cause an error and the function returns warning


### Release a semaphore

```c
void SemaphoreRelease (ray_sem_t * ThreadSemaphore);
```

- Parameters:

|   Parameters    |         Description          |    Type     |
| :-------------: | :--------------------------: | :---------: |
| ThreadSemaphore | semaphore pointer to release | ray_sem_t * |

- return value:

| Return Value |   Description   | Type |
| :----------: | :-------------: | :--: |
|     void     | no return value | void |

- Remarks:

> 1. If the semaphore value is greater than 0, wake up the first thread in the process queue that is blocked by waiting for the semaphore
> 2. Need to define USING_SEMAPHORE macro in configuration file as 1


### Receive a semaphore

```c
void SemaphoreTake (ray_sem_t * ThreadSemaphore);
```

- Parameters:

|   Parameters    |         Description          |    Type     |
| :-------------: | :--------------------------: | :---------: |
| ThreadSemaphore | Semaphore pointer to receive | ray_sem_t * |

- return value:

| Return Value |   Description   | Type |
| :----------: | :-------------: | :--: |
|     void     | no return value | void |

- Remarks:

> 1. If the semaphore value is greater than 0, receive it, otherwise suspend the current thread
> 2. Need to define USING_SEMAPHORE macro in configuration file as 1

### Receive a mail

```c
void MailSend (ray_mailbox_t * mailbox, ray_uint32_t mail);
```

- Parameters:

| Parameters |       Description       |      Type       |
| :--------: | :---------------------: | :-------------: |
|  mailbox   | mailbox pointer to send | ray_mailbox_t * |
|    mail    | mail to send (4 bytes)  | unsigned long * |

- return value:

| Return Value |   Description   | Type |
| :----------: | :-------------: | :--: |
|     void     | no return value | void |

- Remarks:

> 1. If the mailbox is empty, send it, otherwise suspend the current thread
> 2. The USING_MAILBOX macro in the configuration file needs to be defined as 1

### Send a mail

```c
void MailRecieve (ray_mailbox_t * mailbox, ray_uint32_t * mail);
```

- Parameters:

| Parameters | Description                |      Type       |
| :--------: | -------------------------- | :-------------: |
|  mailbox   | Mailbox pointer to receive | ray_mailbox_t * |
|    mail    | mail to receive (4 bytes)  | unsigned long * |

- return value:

| Return Value |   Description   | Type |
| :----------: | :-------------: | :--: |
|     void     | no return value | void |

- Remarks:

> 1. If the mailbox is full, receive it, otherwise suspend the current thread
> 2. The USING_MAILBOX macro in the configuration file needs to be defined as 1

### Set idle thread hook

```c
void IdleHookFunctionSet (void (* hook) (void));
```

- Parameters:

| Parameters |      Description      |       Type       |
| :--------: | :-------------------: | :--------------: |
|    hook    | hook function pointer | function pointer |

- return value:

| Return Value |   Description   | Type |
| :----------: | :-------------: | :--: |
|     void     | no return value | void |

- Remarks:

> 1. Need to define the USING_IDLEHOOK macro in the configuration file as 1

### Cancel idle thread hook

```c
void IdleHookFunctionReset (void);
```

- Parameters:

| Parameters | Description | Type |
| :--------: | :---------: | :--: |
|    void    |    None     | void |

- return value:

| Return Value |   Description   | Type |
| :----------: | :-------------: | :--: |
|     void     | no return value | void |

- Remarks:

> 1. Need to define the USING_IDLEHOOK macro in the configuration file as 1

### Get CPU usage

```c
ray_uint8_t GetCPUUsage (void);
```

- Parameters:

| Parameters | Description | Type |
| :--------: | :---------: | :--: |
|    void    |    None     | void |

- return value:

| Return Value |              Description               |    Type     |
| :----------: | :------------------------------------: | :---------: |
|   CPUUsage   | CPU usage, percentage reserved integer | ray_uint8_t |

- Remarks:

> 1. Need to define USING_CPUUSAGE macro in configuration file as 1