#ifndef _TM1637_H_
#define _TM1637_H_

#define CLK P34 //定义模拟IIC总线的时钟线接口
#define DIO P35 //定义模拟IIC总线的数据线接口
void TM1637_display(ray_uint8_t a, ray_uint8_t b, ray_uint8_t c, ray_uint8_t d, ray_uint8_t dp, ray_uint8_t light);

#endif
