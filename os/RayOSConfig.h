#ifndef _RAYOSCONFIG_H_
#define _RAYOSCONFIG_H_

// ����Ƶ�ʣ���λHz��
#define FOSC 24000000L
// ��ʱ����Ƶϵ��
#define DEVIDER 1
// ϵͳ�δ����ڳ��ȣ���λms��
#define TICKS 2
// ����߳���
#define THREAD_MAX 10
// ������ȼ�
#define PRIORITY_MAX 5
// ջ���
#define STACK_SIZE 10

// ʹ���ź�������
#define USING_SEMAPHORE 1
// ʹ�����书��
#define USING_MAILBOX 1
// ʹ�ÿ����̹߳��ӹ���
#define USING_IDLEHOOK 1
// ʹ��CPUռ����ͳ��
#define USING_CPUUSAGE 1

// ʹ��ADC�⺯��
#define USING_ADC 0
// ʹ��EEEPROM�⺯��
#define USING_EEPROM 0
// ʹ���ⲿ�жϿ⺯��
#define USING_Exti 0
// ʹ��GPIO�⺯��
#define USING_GPIO 1
// ʹ��PWM�⺯��
#define USING_PCA 0
// ʹ�����ģ�⴮�ڿ⺯��
#define USING_soft_uart 0
// ʹ�ö�ʱ���⺯��
#define USING_timer 1
// ʹ�ô��ڿ⺯��
#define USING_USART 1

#endif
