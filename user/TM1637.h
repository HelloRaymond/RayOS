#ifndef _TM1637_H_
#define _TM1637_H_

#define CLK P34 //����ģ��IIC���ߵ�ʱ���߽ӿ�
#define DIO P35 //����ģ��IIC���ߵ������߽ӿ�
void TM1637_display(ray_uint8_t a, ray_uint8_t b, ray_uint8_t c, ray_uint8_t d, ray_uint8_t dp, ray_uint8_t light);

#endif
