#include "STC15F2K60S2.h"
#include "string.h"
#include "chuankou.h"

/**
  chuankou.c功能
  串口1：ZIGBEE,P14(扩展zigbee蓝牙模块)
  串口2：串口wifi-esp8266,max3232,OBD
*/

//串口2
sbit CK_TXD = P1 ^ 0; //输入
sbit CK_RXD = P1 ^ 1;
//串口1
sbit UTXD = P3 ^ 0; //输入
sbit URXD = P3 ^ 1;
//bit cktflg1 = 0, cktflg2 = 0;
//unsigned char CKShowXNum1 = 2, CKShowYNum1 = 6, CKShowXNum2 = 16, CKShowYNum2 = 6; //串口显示区域的初始位置

volatile unsigned char GetFlag1 = 0, GetFlag2 = 0, GetFlag3 = 0;
volatile unsigned char BUSY1 = 0;
unsigned char CK1Num = 0;
volatile unsigned char GetDataBuf1[100] = '\0';

void ChuankouIOInit(void)
{
	P1M0 &= ~(1 << 0); //输入
	P1M1 |= (1 << 0);  //输入

	P3M0 &= ~(1 << 0); //输入
	P3M1 |= (1 << 0);  //输入

	P1M0 |= (1 << 1);
	P1M1 &= ~(1 << 1);

	P3M0 |= (1 << 1);
	P3M1 &= ~(1 << 1);
}

void USART1_Init(void)
{
	SCON = 0X50;						//01010000：01工作方式1（即8位UART，波特率可变），01允许串行接收
	T2L = (65536 - (F_CPU) / BAUD / 4); //设置波特率重装值
	T2H = (65536 - (F_CPU) / BAUD / 4) >> 8;
	AUXR |= 0X15; //定时器T2为波特率发生器，T2为1T模式，启动定时器2
	ES = 1;		  //串口1中断开
	EA = 1;		  //开总中断
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
		if (a < 128) //只接收字符串,避免乱码
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
