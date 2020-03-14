#ifndef _RAYOSCONFIG_H_
#define _RAYOSCONFIG_H_

// ����Ƶ�ʣ���λHz��
#define FOSC 24000000L
// ��ʱ����Ƶϵ��
#define DEVIDER 1
// ϵͳ�������ڳ��ȣ���λms��
#define TICKS 2
// ����߳���
#define THREAD_MAX 5
// ������ȼ�
#define PRIORITY_MAX 5
// ջ���
#define STACK_SIZE 10

// �����ź�������
#define USING_SEMAPHORE 1
// �������书��
#define USING_MAILBOX 0
// ���������̹߳��ӹ���
#define USING_IDLEHOOK 1
// ����CPUռ����ͳ��
#define USING_CPUUSAGE 1

// ����ADC�⺯��
#define USING_ADC 0
// ����EEEPROM�⺯��
#define USING_EEPROM 0
// ����Exti�⺯��
#define USING_Exti 0
// ����GPIOExti
#define USING_GPIO 1
// ����PCAExti
#define USING_PCA 0
// ����soft_uart�⺯��
#define USING_soft_uart 0
// ����timer�⺯��
#define USING_timer 1
// ����USART�⺯��
#define USING_USART 1

#endif
