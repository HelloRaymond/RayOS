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
#define USING_MAILBOX 0
// ʹ�ÿ����̹߳��ӹ���
#define USING_IDLEHOOK 1
// ʹ��CPUռ����ͳ��
#define USING_CPUUSAGE 1

#endif
