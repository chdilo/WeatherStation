#ifndef __CHUANKOU_H
#define __CHUANKOU_H	

#define F_CPU 33177600UL
#define BAUD 115200

#define S2RI 0X01
#define S2TI 0X02

extern volatile unsigned char GetFlag1;
extern volatile unsigned char GetFlag2;
extern volatile unsigned char GetFlag3;

void ChuankouIOInit(void);
void USART1_Init(void);
//void USART2_Init(void);

void SendData1(unsigned char dat);
//void SendData2(unsigned char dat);
void SendStr1(unsigned char *s1);
//void SendStr2(unsigned char *s2);

//void zhuce(void);

//void UartWirelessTouch(void);

#endif 