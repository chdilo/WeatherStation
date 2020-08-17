#include "STC15F2K60S2.h"
#include "string.h"
#include "chuankou.h"

/**
  chuankou.c����
  ����1��ZIGBEE,P14(��չzigbee����ģ��)
  ����2������wifi-esp8266,max3232,OBD
*/

//����2
sbit CK_TXD = P1 ^ 0; //����
sbit CK_RXD = P1 ^ 1;
//����1
sbit UTXD = P3 ^ 0; //����
sbit URXD = P3 ^ 1;
//bit cktflg1 = 0, cktflg2 = 0;
//unsigned char CKShowXNum1 = 2, CKShowYNum1 = 6, CKShowXNum2 = 16, CKShowYNum2 = 6; //������ʾ����ĳ�ʼλ��

volatile unsigned char GetFlag1 = 0, GetFlag2 = 0, GetFlag3 = 0;
volatile unsigned char BUSY1 = 0;
unsigned char CK1Num = 0;
volatile unsigned char GetDataBuf1[100] = '\0';

void ChuankouIOInit(void)
{
	P1M0 &= ~(1 << 0); //����
	P1M1 |= (1 << 0);  //����

	P3M0 &= ~(1 << 0); //����
	P3M1 |= (1 << 0);  //����

	P1M0 |= (1 << 1);
	P1M1 &= ~(1 << 1);

	P3M0 |= (1 << 1);
	P3M1 &= ~(1 << 1);
}

void USART1_Init(void)
{
	SCON = 0X50;						//01010000��01������ʽ1����8λUART�������ʿɱ䣩��01�����н���
	T2L = (65536 - (F_CPU) / BAUD / 4); //���ò�������װֵ
	T2H = (65536 - (F_CPU) / BAUD / 4) >> 8;
	AUXR |= 0X15; //��ʱ��T2Ϊ�����ʷ�������T2Ϊ1Tģʽ��������ʱ��2
	ES = 1;		  //����1�жϿ�
	EA = 1;		  //�����ж�
}

void SendData1(unsigned char dat)
{
	while (BUSY1)
		;
	SBUF = dat;
	BUSY1 = 1;
}

void SendStr1(unsigned char *s1)
{
	while (*s1)
	{
		SendData1(*s1++);
	}
}

void Uart1_Isr() interrupt 4 //using 0
{
	unsigned char a;
	if (RI)
	{
		RI = 0;
		a = SBUF;
		if (a < 128) //ֻ�����ַ���,��������
		{
			if (a == '\r')
				return;
			if (a == '\n')
			{
				GetDataBuf1[CK1Num] = '\0';
				CK1Num = 0;
				if (strcmp(GetDataBuf1, "OK") == 0)
				{
					GetFlag1 = 1;
				}
				else if (strcmp(GetDataBuf1, "+CREG: 1") == 0)
				{
					GetFlag2 = 1;
				}
			}
			else
			{
				GetDataBuf1[CK1Num++] = a;
				if (GetDataBuf1[0] == '>' && GetDataBuf1[1] == ' ')
				{
					GetFlag3 = 1;
				}
			}
		}
	}
	if (TI)
	{
		TI = 0;
		BUSY1 = 0;
	}
}
