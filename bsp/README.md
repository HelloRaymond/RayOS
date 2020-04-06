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

​	The main function of RayOS BSP is to provide four functions for the kernel to support different hardware, which are:

`void SystemInit(void);` Hardware initialization

`void OS_ENTER_CRITICAL(void);` Disable global interrupt

`void OS_EXIT_CRITICAL(void);` Enable global interrupt

`void Timer_Reload(void);` Timer0 reload function(This function can be left blank when Timer 0 is configured in auto reload mode)

All three functions are located in the board.c file

### How to port

1. ​	Copy the bsp/template folder and name it whatever you want

2. ​	Modify RayOSConfig.h file for system configuration and cropping

3. ​	Modify the board.c file to implement hardware initialization and switch global interrupt functions (Timer0 interrupt is used for the system tick clock. It must be configured to auto-reload mode and enabled when hardware initializing. The initial value of Timer0 must be configured as T0VAL(The macro has been defined by the system, no need to customize). If Timer 0 does not support auto-reload mode, you need to implement Timer_Reload () function)
