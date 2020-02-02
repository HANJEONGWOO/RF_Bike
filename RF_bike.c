#include <avr/io.h>			//avr ��� ���� 
#include <avr/interrupt.h>	//���ͷ�Ʈ ���
#include <util/delay.h>		//������ ���
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

	while(1)	//���ѷ���
	{
		if(gps_buffer_flag==1)	//gps�� ���۰� á����
			SendToStation(GPS);	//GPS�� ���� �������� ����
		
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
ISR(USART1_RX_vect)	//gps�� ���ſϷ� ���ͷ�Ʈ
{
	char buf=UDR1;
	if(buf=='$' && gps_buffer_flag==0)
		gps_load_flag=1;
		
	if(gps_load_flag==1){
		gps_buffer[gps_buffer_cnt]=buf;
		gps_buffer_cnt++;
		if(gps_buffer_cnt==5)	//���� ī��Ʈ�� 5�϶�
			if(buf!='G'){	//G�� �ƴϸ�
				gps_buffer_cnt=0;	//����ī��Ʈ, �÷��� 0 �ʱ�ȭ
				gps_load_flag=0;
			}
		if(buf=='\n'){	//���۰� ���๮���϶�
			gps_buffer_flag=1;	//�����÷��� 1
			gps_load_flag=0;	//�ε� 0
			gps_buffer_cnt=0;	//����ī��Ʈ 0
			UCSR1B=0x00;
		}
	}
}

	/*
	//Ȧ���� - �� ���� ����
	ADCSRA = 0xFF;	//ADC ��ȯ����

	ADMUX = 0xC0;	//����Ŭ��, ADC0��
	
	while( (ADCSRA & 0x10) == 0);
		hall_val = ADCW;
		ADCW = 0;

	ADCSRA = 0x00;	//ADC ��ȯ ��
	*/
