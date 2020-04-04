#include "RayOS.h"
#include "uart.h"
#include "stdio.h"

ray_uint8_t tid1;
ray_base_t stack1[20];

char putchar(char c)
{
    SendByte(c);
    return c;
}

void print_demo(void)
{
    static cnt = 0;
    printf("running time: %bd seconds\r\n", cnt++);
    ThreadDelayMs(1000);
}

void main_user(void)
{
    printf("OK!\r\n");
    // Put your code here
    tid1 = ThreadCreate(print_demo, stack1, 20, 10, 1, True);
    ThreadStart(tid1);
}
