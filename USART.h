#include <avr/io.h>

//+++	UART�� ����� �� �߰��ϴ� �������	+++//
//+ ����
//1. init_devices�� init_port�� ȣ���Ͽ� ����� ��
//2. �ʿ��� uart�Լ��� ����Ұ� - ������� �ʴ� �Լ��� �ּ�ó�� �� ��

int Putchar(char c, FILE *stream);
int Getchar(FILE *stream);

void uart0_init(void);
void uart1_init(void);
//void uart2_init(void);
//void uart3_init(void);

void init_devices(void);
void init_port(void);

