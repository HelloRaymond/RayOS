#include "RayOS.h"
#include "USART.h"

char putchar(char c)
{
    TX1_write2buff(c);
    return c;
}
