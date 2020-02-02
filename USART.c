#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "usart.h"

int Putchar(char c, FILE *stream)//FILE 사용안함 송신, avr->컴퓨터
{
    if(c == '\n')
        //putchar('\r');
        Putchar('\r', stream);    // mhlee

    while(!(UCSR0A & 0x20)); // UDRE, data register empty        
    UDR0 = c;

    return 0;
}

int Getchar(FILE *stream)//수신, 컴퓨터->avr
{
    while(!(UCSR0A & 0x80));
    return UDR0;
}

//0x08 - 115200bps
//0x19 - 38400bps
//0x67 - 9600bps

//RF - COM
void uart0_init(void) //
{
    UCSR0B = 0x00; //disable while setting baud rate
    UCSR0A = 0x00;
    UCSR0C = 0x06;
//    UBRR0L = 0x08; //set baud rate 115200
    UBRR0L = 0x67; //set baud rate 115200
    UBRR0H = 0x00; 
    UCSR0B = 0b10011000; //송수신
}

//GPS
void uart1_init(void) 
{
 UCSR1B = 0x00; //disable while setting baud rate
 UCSR1A = 0x00;
 UCSR1C = 0x06;
 UBRR1L = 0x67; //set baud rate 9600
 UBRR1H = 0x00; 
 UCSR1B = 0b10011000; //송수신
}

/*
void uart2_init(void) //
{
    UCSR2B = 0x00; //disable while setting baud rate
    UCSR2A = 0x00;
    UCSR2C = 0x06;
    UBRR2L = 0x33; //set baud rate 115200 -> 19200
	//UBRR1L = 0x08;	//set baud rate 115200
    UBRR2H = 0x00; 
    UCSR2B = 0b10011000; //송수신
}


//사용하지 않음
void uart3_init(void) //
{
    UCSR3B = 0x00; //disable while setting baud rate
    UCSR3A = 0x00;
    UCSR3C = 0x06;
    UBRR3L = 0x33; //set baud rate 115200 -> 19200
	//UBRR3L = 0x08;	//set baud rate 115200
    UBRR3H = 0x00; 
    UCSR3B = 0b10011000; //송수신
}
*/

void init_devices(void)
{
    cli();                         //disable all interrupts
    uart0_init();
	uart1_init();
    fdevopen(Putchar, Getchar);    //file stream 0
    sei(); 
}

void init_port(void)
{

}
