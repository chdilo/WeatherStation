#include "delay.h"
#include "tft.h"
#include "txt.h"
#include "chuankou.h"
#include "zhuce.h"
#include "touch.h"

void FailZhuce(void)
{
	TFT_ClearONELINE(3, BLACK);
	TFT_ShowString_W25(8, 3, 2, "����ע��ʧ��", WHITE, BLACK);
	delayms(2000);
	TFT_ShowString(0, 8, "------------------------------", WHITE, BLACK);
	TFT_ShowString_W25(0, 10, 2, "    �����Ļ����ϵͳ����", WHITE, BLACK);
	TFT_ShowString(0, 12, "------------------------------", WHITE, BLACK);
	while (Touch_IRQ())
		;
}

/************************ ����ע��***********************/
void Zhuce(void)
{
	unsigned int i = 30;

	ChuankouIOInit(); //���ڳ�ʼ��
	USART1_Init();
	TFT_ClearArea(0, 0, 240, 320, BLACK);
	while (i--)
	{
		TFT_ClearONELINE(3, BLACK);
		TFT_ShowString_W25(5, 3, 2, "ϵͳ�����У����Ե�.", WHITE, BLACK);
		delayms(500);
		TFT_ShowString_W25(5, 3, 2, "ϵͳ�����У����Ե�..", WHITE, BLACK);
		delayms(500);
		TFT_ShowString_W25(5, 3, 2, "ϵͳ�����У����Ե�...", WHITE, BLACK);
		delayms(500);
		TFT_ShowString_W25(5, 3, 2, "ϵͳ�����У����Ե�....", WHITE, BLACK);
		delayms(500);
		if (GetFlag2 == 1)
		{
			GetFlag2 = 0;
			TFT_ClearONELINE(3, BLACK);
			TFT_ShowString_W25(8, 3, 2, "����ע��ɹ�", WHITE, BLACK);
			delayms(2000);
			TFT_ShowString(0, 8, "------------------------------", WHITE, BLACK);
			TFT_ShowString_W25(0, 10, 2, "        ����ϵͳ����", WHITE, BLACK);
			TFT_ShowString(0, 12, "------------------------------", WHITE, BLACK);
			delayms(2000);
			return;
		}
	}
	FailZhuce();
}
