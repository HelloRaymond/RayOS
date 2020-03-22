#include "reg52.h"
#include "RayOS.h"
#include "uart.h"

#define BAUD 9600

ray_uint8_t busy;

void uart_config(void)
{
    SCON = 0x50;
    TMOD |= 0x20;
    TH1 = TL1 = -(FOSC/12/32/BAUD);
    TR1 = 1;
    ES = 1;
}

void uart_isr() interrupt 4
{
    if (RI)
    {
        RI = 0;
        P0 = SBUF;          //P0 show UART data
    }
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
}

void SendByte(ray_uint8_t dat)
{
    while (busy);
    ACC = dat;
    busy = 1;
    SBUF = ACC;
}
