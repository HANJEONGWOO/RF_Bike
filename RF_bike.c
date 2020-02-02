#include <avr/io.h>			//avr 헤더 설정 
#include <avr/interrupt.h>	//인터럽트 헤더
#include <util/delay.h>		//딜레이 헤더
#include <stdio.h>
#include <stdlib.h>

#include "usart.h"

#define GPS 	3
#define ECT 	5

void init_buffer(int);
void load_buffer(int );
void SendToStation(int);

unsigned char gps_buffer[200];
static char gps_buffer_flag;
static char gps_load_flag;
static unsigned char gps_buffer_cnt;

char data_uart0;

void init_buffer(int select)	//
{
	int i;
	switch(select)
	{
		case GPS:
			for(i=0;i<50;i++)
				gps_buffer[i]=0;
			break;
		case ECT:
			gps_buffer_flag=0;
			gps_load_flag=0;
			gps_buffer_cnt=0;
			break;
	}
}

void load_buffer(int select)
{
	switch(select)
	{
		case GPS:
			UCSR1B=0x90;	//start data_receive
			break;
	}
}

void SendToStation(int select)
{	
	int i;

	switch(select)
	{
		case GPS:	//gps
			printf("AA,");
			printf("%s",gps_buffer);
			init_buffer(GPS);
			gps_buffer_flag=0;
			break;
	}

}


int main()
{
	init_devices();
	init_port();

	init_buffer(GPS);

	printf("Start RF Bicycle\n");

	while(1)	//무한루프
	{
		if(gps_buffer_flag==1)	//gps의 버퍼가 찼으면
			SendToStation(GPS);	//GPS의 값을 지상국으로 전송
		
		_delay_ms(10);

	}	//while

}	//main


//RF - 0
ISR(USART0_RX_vect)
{
	data_uart0 = UDR0;
	switch(data_uart0)
	{
		case 'g':
			load_buffer(GPS);
			break;
	}
}

//GPS - 1
ISR(USART1_RX_vect)	//gps의 수신완료 인터럽트
{
	char buf=UDR1;
	if(buf=='$' && gps_buffer_flag==0)
		gps_load_flag=1;
		
	if(gps_load_flag==1){
		gps_buffer[gps_buffer_cnt]=buf;
		gps_buffer_cnt++;
		if(gps_buffer_cnt==5)	//버퍼 카운트가 5일때
			if(buf!='G'){	//G가 아니면
				gps_buffer_cnt=0;	//버퍼카운트, 플래그 0 초기화
				gps_load_flag=0;
			}
		if(buf=='\n'){	//버퍼가 개행문자일때
			gps_buffer_flag=1;	//버퍼플래그 1
			gps_load_flag=0;	//로드 0
			gps_buffer_cnt=0;	//버퍼카운트 0
			UCSR1B=0x00;
		}
	}
}

	/*
	//홀센서 - 잘 되지 않음
	ADCSRA = 0xFF;	//ADC 변환시작

	ADMUX = 0xC0;	//내부클럭, ADC0번
	
	while( (ADCSRA & 0x10) == 0);
		hall_val = ADCW;
		ADCW = 0;

	ADCSRA = 0x00;	//ADC 변환 끝
	*/
