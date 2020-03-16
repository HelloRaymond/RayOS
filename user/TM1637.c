#include <STC15Fxxxx.H>
#include <INTRINS.H>
#include "RayOS.h"
#include "TM1637.h"

code ray_uint8_t tab[] =
    {
        0x3F, /*0*/
        0x06, /*1*/
        0x5B, /*2*/
        0x4F, /*3*/
        0x66, /*4*/
        0x6D, /*5*/
        0x7D, /*6*/
        0x07, /*7*/
        0x7F, /*8*/
        0x6F, /*9*/
        0x77, /*10 A*/
        0x7C, /*11 b*/
        0x58, /*12 c*/
        0x5E, /*13 d*/
        0x79, /*14 E*/
        0x71, /*15 F*/
        0x76, /*16 H*/
        0x38, /*17 L*/
        0x54, /*18 n*/
        0x73, /*19 P*/
        0x3E, /*20 U*/
        0x00, /*21 空白*/
};
static void Delay_us(ray_uint16_t i)
{
    if (i == 0)
        return;
	_nop_();
	_nop_();
	i = 3 + 6 * (i-1);
	while (--i);
}

static void TM1637_start(void)
{
    CLK = 1;
    DIO = 1;
    Delay_us(2);
    DIO = 0;
}

static void TM1637_ack(void)
{
    ray_uint8_t i;
    CLK = 0;
    Delay_us(5);
    //DIO=1;
    while (DIO == 1 && (i < 250))
        i++;
    CLK = 1;
    Delay_us(2);
    CLK = 0;
}

static void TM1637_stop(void)
{
    CLK = 0;
    Delay_us(2);
    DIO = 0;
    Delay_us(2);
    CLK = 1;
    Delay_us(2);
    DIO = 1;
    Delay_us(2);
}

static void TM1637_Write(ray_uint8_t DATA)
{
    ray_uint8_t i;
    for (i = 0; i < 8; i++)
    {
        CLK = 0;
        if (DATA & 0x01)
            DIO = 1;
        else
            DIO = 0;
        Delay_us(3);
        DATA = DATA >> 1;
        CLK = 1;
        Delay_us(3);
    }
}

void TM1637_display(ray_uint8_t a, ray_uint8_t b, ray_uint8_t c, ray_uint8_t d, ray_uint8_t dp, ray_uint8_t light)
{
    if (light > 7 || light < 0)
    {
        light = 3;
    }
    TM1637_start();
    TM1637_Write(0x40); //写数据+自动地址加1+普通模式
    TM1637_ack();
    TM1637_stop();
    TM1637_start();
    TM1637_Write(0xc0); //设置显示首地址即第一个LED
    TM1637_ack();

    TM1637_Write(tab[a]);
    TM1637_ack();
    TM1637_Write(tab[b] | dp << 7); //h为1时显示时钟中间的两点
    TM1637_ack();
    TM1637_Write(tab[c]);
    TM1637_ack();
    TM1637_Write(tab[d]);
    TM1637_ack();

    TM1637_stop();
    TM1637_start();
    TM1637_Write(0x88 + light); //开显示，调节亮度
    TM1637_ack();
    TM1637_stop();
}
