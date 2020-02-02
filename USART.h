#include <avr/io.h>

//+++	UART를 사용할 때 추가하는 헤더파일	+++//
//+ 사용법
//1. init_devices와 init_port를 호출하여 사용할 것
//2. 필요한 uart함수만 사용할것 - 사용하지 않는 함수는 주석처리 할 것

int Putchar(char c, FILE *stream);
int Getchar(FILE *stream);

void uart0_init(void);
void uart1_init(void);
//void uart2_init(void);
//void uart3_init(void);

void init_devices(void);
void init_port(void);

