# 板级支持包（BSP） #

**[English Documents](./README.md)**

## BSP（Board Support Package)

　  BSP是介于主板硬件和操作系统之间的一层，是属于操作系统的一部分,主要目的是为了支持操作系统，使之能够更好的运行于硬件主板。

## RayOS BSP 简介

### 主要目录结构

- board 板级支持

  ​	RayOSConfig.h

  ​	board.c

  ​	board.h

- application 用户代码

  ​	main.c

### 主要功能

​	RayOS BSP 的最主要功能，是为内核提供了三个函数，用于支持不同的硬件，分别是：

`void SystemInit(void);` 硬件初始化

`void OS_ENTER_CRITICAL(void);` 关全局中断

`void OS_EXIT_CRITICAL(void);` 开全局中断

`void Timer_Reload(void)` 定时器重载（当定时器0配置为自动重载模式时该函数可留空）

这三个函数都位于board.c文件

### 如何移植

1. 拷贝bsp/template文件夹，并命名为你所需要名字

2. 修改RayOSConfig.h文件，进行系统配置和裁剪

3. 修改board.c文件，实现硬件初始化、开关全局中断函数（定时器0中断用于系统滴答时钟，硬件初始化时必须将它配置并开启，Timer0初值必须配置为T0VAL（系统已经定义好该宏，无需自定义），如果定时器0不支持自动重载模式，则需要自己实现Timer_Reload()函数）
