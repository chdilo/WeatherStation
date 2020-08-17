#include <stdio.h>
#include "delay.h"
#include "sendmessage.h"
#include "tft.h"
#include "txt.h"
#include "touch.h"
#include "huanjing.h"
#include "setphone.h"
#include "chuankou.h"

#define BACK_COLOR 0xC638
#define BUTTON_COLOR 0x3CDA
#define BUTTON_EDGE_COLOR 0x226E
#define BUTTON_FONT_COLOR WHITE
#define BUTTON_ON_COLOR 0x33D4
#define SENDING_COLOR 0xED84
#define FAIL_COLOR 0xA0A6
#define OK_COLOR 0x7546
#define INFO_COLOR 0x0397

/*******************************************���ŷ���*******************************************************/
unsigned char touchNum = 0;
unsigned char Message[45];
unsigned char Number[24];
void SendMessageFrame(void)
{
	TFT_ClearArea(0, 16, 240, 320 - 16, BACK_COLOR); //�����ʾ��

	LCD_DrawRectangle(17, 20, 222, 70, BUTTON_EDGE_COLOR);
	LCD_DrawRectangle(17, 75, 222, 125, BUTTON_EDGE_COLOR);
	LCD_DrawBox(18, 21, 204, 49, BUTTON_COLOR);
	LCD_DrawBox(18, 76, 204, 49, BUTTON_COLOR);

	TFT_ShowString(11, 2, "Reflash", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(7, 6, "Send message", BUTTON_FONT_COLOR, BUTTON_COLOR);

	TFT_ShowString_W25(2, 8, 2, "��ǰ��Ϣ���ݣ�", BLACK, BACK_COLOR);
	TFT_ShowString_W25(2, 11, 2, "��ǰ����ǿ�ȣ�", BLACK, BACK_COLOR);
	TFT_ShowString_W25(2, 13, 2, "��ǰ�¶ȣ�", BLACK, BACK_COLOR);
	TFT_ShowString_W25(2, 15, 2, "��ǰʪ�ȣ�", BLACK, BACK_COLOR);
	TFT_ShowString_W25(2, 17, 2, "���ŷ�������", BLACK, BACK_COLOR);
	TFT_ShowString(2, 18, PhoneNumber, INFO_COLOR, BACK_COLOR);
	TFT_ShowString_W25(0, 19, 2, "������", BLACK, WHITE);
}

void Reflash(void)
{
	BH1750();
	DH11GetHumiture();
	sprintf(Message, "%s%s%s%s%s%s", "illuminance:", DispBuf, ",temperature:", humitureT, ",humidity:", humitureH);
	TFT_ShowString(2, 9, Message, INFO_COLOR, BACK_COLOR);
	TFT_ShowString(2, 12, DispBuf, INFO_COLOR, BACK_COLOR);
	TFT_ShowString(2, 14, humitureT, INFO_COLOR, BACK_COLOR);
	TFT_ShowString(2, 16, humitureH, INFO_COLOR, BACK_COLOR);
}

void FailSend(void)
{
	LCD_DrawBox(18, 76, 204, 49, FAIL_COLOR);
	TFT_ShowString_W25(7, 6, 2, "����ʧ��", WHITE, FAIL_COLOR);
	delayms(2000);
}

void SendMessage(void)
{
	unsigned int i = 0;
	unsigned int ms = 300;
	LCD_DrawBox(18, 76, 204, 49, SENDING_COLOR);
	TFT_ShowString(7, 6, "Send message", WHITE, SENDING_COLOR);
	sprintf(Number, "%s%s%s", "AT+CMGS=\"", PhoneNumber, "\"\r\n");

	GetFlag1 = 0;
	SendStr1("AT+CMGF=1\r\n");
	while (GetFlag1 != 1)
	{
		delayms(100);
		if (i++ > ms)
		{
			FailSend();
			return;
		}
	}
	GetFlag1 = 0;
	LCD_DrawBox(18, 76, 204, 49, SENDING_COLOR);
	TFT_ShowString_W25(7, 6, 2, "�����ı�", WHITE, SENDING_COLOR);

	SendStr1("AT+CSCS=\"GSM\"\r\n");
	while (GetFlag1 != 1)
	{
		delayms(100);
		if (i++ > ms)
		{
			FailSend();
			return;
		}
	}
	GetFlag1 = 0;
	LCD_DrawBox(18, 76, 204, 49, SENDING_COLOR);
	TFT_ShowString_W25(7, 6, 2, "�������", WHITE, SENDING_COLOR);

	GetFlag3 = 0;
	SendStr1(Number);
	while (GetFlag3 != 1)
	{
		delayms(100);
		if (i++ > ms)
		{
			FailSend();
			return;
		}
	}
	GetFlag3 = 0;
	LCD_DrawBox(18, 76, 204, 49, SENDING_COLOR);
	TFT_ShowString_W25(7, 6, 2, "������", WHITE, SENDING_COLOR);

	SendStr1(Message);
	SendStr1("\x1A");
	while (GetFlag1 != 1)
	{
		delayms(100);
		if (i++ > ms)
		{
			FailSend();
			return;
		}
	}
	GetFlag1 = 0;
	LCD_DrawBox(18, 76, 204, 49, OK_COLOR);
	TFT_ShowString(7, 6, "OK!", WHITE, OK_COLOR);
	delayms(2000);
}

void SendMessageTouch(void)
{
	HuanjingIOInit();
	Init_BH1750();
	SendMessageFrame();
	Reflash();
	while (1)
	{
		while (1)
		{
			if (!Touch_IRQ())
			{
				if (Convert_Pos()) //�õ�����ֵ
				{
					touchNum = 0;
					if (17 < tp_pixlcd.x && tp_pixlcd.x < 222)
					{
						if (20 < tp_pixlcd.y && tp_pixlcd.y < 70) //Reflash��
						{
							touchNum = 1;
						}
						else if (75 < tp_pixlcd.y && tp_pixlcd.y < 125) //Send message��
						{
							touchNum = 2;
						}
					}
					if (tp_pixlcd.x < 48 && tp_pixlcd.y > 298) //�±�ǩ��"������"
					{
						TFT_ShowString_W25(0, 19, 2, "������", WHITE, BLUE);
						return;
					}
					if (touchNum == 1)
					{
						LCD_DrawBox(18, 21, 204, 49, BUTTON_ON_COLOR);
						TFT_ShowString(11, 2, "Reflash", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (touchNum == 2)
					{
						LCD_DrawBox(18, 76, 204, 49, BUTTON_ON_COLOR);
						TFT_ShowString(7, 6, "Send message", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
				}
			}
		}
		while (!Touch_IRQ())
			; //�ȴ������ɿ�

		switch (touchNum)
		{
		case 1:
		{
			Reflash();
			LCD_DrawBox(18, 21, 204, 49, BUTTON_COLOR);
			TFT_ShowString(11, 2, "Reflash", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 2:
		{
			SendMessage();
			LCD_DrawBox(18, 76, 204, 49, BUTTON_COLOR);
			TFT_ShowString(7, 6, "Send message", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		}
	}
}