#include <stdio.h>
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "touch_main.h"
#include "sendmessage.h"
#include "setphone.h"
#include "huanjing.h"

#define BACK_COLOR 0x659B
#define ICON_FONT_COLOR BLACK
#define ICON_FONT_BACK_COLOR WHITE
#define ICON_FONT_ON_COLOR BLUE


unsigned char TouchNumber = 0;

/************************ ������ʾ���***********************/
void MainFrame(void)
{
	TFT_ClearONELINE(0, 0x1A0F);
	TFT_ShowString(0, 0, "CPU:STC15W4K48S4", WHITE, 0x1A0F);
	TFT_ShowString_W25(20, 0, 2, "��������վ", WHITE, 0x1A0F);
	TFT_ClearArea(0, 16, 240, 320 - 16, BACK_COLOR);
	//��ʾͼ�� �� ͼ���Ӧ�������
	TFT_ShowString_W25(2, 7, 2, "���ŷ���", ICON_FONT_COLOR, ICON_FONT_BACK_COLOR);
	TFT_ShowString_W25(11, 7, 2, "�������", ICON_FONT_COLOR, ICON_FONT_BACK_COLOR);
	TFT_ShowString_W25(20, 7, 2, "�绰�趨", ICON_FONT_COLOR, ICON_FONT_BACK_COLOR);


	//�򿪺���ʾ������3��ͼƬ

	DISP_BIN_PT_W25(17, 50, 55, 55, 0x615000, 0x617F44);  //(45,50)
	DISP_BIN_PT_W25(92, 50, 55, 55, 0x621000, 0x623F44);  //(140,50)
	DISP_BIN_PT_W25(167, 50, 55, 55, 0x61F000, 0x621F44); //(140,50)


	TFT_ShowString_W25(2, 13, 2, "��ǰ�趨�ĵ绰����Ϊ:", BLACK, BACK_COLOR);
	TFT_ShowString(8, 14, PhoneNumber, BLACK, BACK_COLOR);
}

/************************ ���ع���***********************/ //	USART1_Init();USART2_Init();

void MainTouch(void)
{
	MainFrame();
	while (1)
	{
		if (!Touch_IRQ())
		{
			if (Convert_Pos()) //�õ�����ֵ
			{
				TouchNumber = 0;
				if (tp_pixlcd.y > 50 && tp_pixlcd.y < 105) //��һ�� (30:30+55)
				{
					if (tp_pixlcd.x > 17 && tp_pixlcd.x < 72) //ͼһ(17:17+55)
					{
						TouchNumber = 1;
					}
					else if (tp_pixlcd.x > 92 && tp_pixlcd.x < 147) //ͼ��(92:92+55)
					{
						TouchNumber = 2;
					}
					else if (tp_pixlcd.x > 167 && tp_pixlcd.x < 222) //ͼ��(167:167+55)
					{
						TouchNumber = 3;
					}
				}
				if (TouchNumber == 1)
				{
					TFT_ShowString_W25(2, 7, 2, "���ŷ���", WHITE, ICON_FONT_ON_COLOR);
					break;
				}
				else if (TouchNumber == 2)
				{
					TFT_ShowString_W25(11, 7, 2, "�������", WHITE, ICON_FONT_ON_COLOR);
					break;
				}
				else if (TouchNumber == 3)
				{
					TFT_ShowString_W25(20, 7, 2, "�绰�趨", WHITE, ICON_FONT_ON_COLOR);
					break;
				}
			}
		}
	}

	while (!Touch_IRQ())
		; //�ȴ������ɿ�

	switch (TouchNumber)
	{
	case 1:
	{
		SendMessageTouch();//���ŷ���
		break;
	}
	case 2:
	{
		HuanjJiancTouch();//�������
		break;
	}
	case 3:
	{
		SetPhoneTouch();//�绰�趨
		break;
	}
	}
}
