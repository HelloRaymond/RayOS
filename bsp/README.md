# Board Support Package (BSP) #

**[中文文档](./README_zh.md)**

　  BSP is a layer between the motherboard hardware and the operating system. It is part of the operating system. The main purpose is to support the operating system so that it can run better on the hardware motherboard.

## RayOS BSP Introduction

### Main directory structure

- board

  ​	RayOSConfig.h

  ​	board.c

  ​	board.h

- application

  ​	main.c

### The main function

​	The main function of RayOS BSP is to provide three functions for the kernel to support different hardware, which are:

`void SystemInit(void);` Hardware initialization

`void OS_ENTER_CRITICAL(void);` Disable global interrupt

`void OS_EXIT_CRITICAL(void);` Enable global interrupt

All three functions are located in the board.c file

### How to port

​	Copy the bsp/template folder and name it whatever you want

​	Modify RayOSConfig.h file for system configuration and cropping

​	Modify the board.c file to implement hardware initialization and switch global interrupt functions (when hardware is initialized, Timer0 must be configured and turned on for the system tick clock)
