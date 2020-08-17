#include "STC15F2K60S2.h"
#include "tft.h"
#include "txt.h"
#include "touch.h"
#include "string.h"
#include "setphone.h"
#include "font.h"
/**
  setphone.c功能
  电话设置
*/
#define BUTTON_COLOR 0x3CDA
#define BUTTON_EDGE_COLOR 0x226E
#define BUTTON_FONT_COLOR WHITE
#define BUTTON_ON_COLOR 0x33D4
#define BACK_COLOR 0x659B

unsigned char inputNumber = 0;
unsigned char PhoneNumber[12] = "12345678901";
unsigned char num = 0;
unsigned char ShowNumber[] = "0";
void SetPhoneMFrame(void)
{
	TFT_ClearArea(0, 16, 240, 320, BACK_COLOR);
	//	TFT_ClearONELINE(19, BLUE);
	//	TFT_ShowString_W25(0, 19,  "主界面", WHITE, RED);
	LCD_DrawRectangle(17, 20, 222, 70, BUTTON_EDGE_COLOR); //显示号码
	LCD_DrawBox(18, 21, 204, 49, WHITE);

	LCD_DrawRectangle(17, 80, 72, 130, BUTTON_EDGE_COLOR);	 //数字一
	LCD_DrawRectangle(92, 80, 147, 130, BUTTON_EDGE_COLOR);	 //数字二
	LCD_DrawRectangle(167, 80, 222, 130, BUTTON_EDGE_COLOR); //数字三

	LCD_DrawRectangle(17, 140, 72, 190, BUTTON_EDGE_COLOR);	  //数字四
	LCD_DrawRectangle(92, 140, 147, 190, BUTTON_EDGE_COLOR);  //数字五
	LCD_DrawRectangle(167, 140, 222, 190, BUTTON_EDGE_COLOR); //数字六

	LCD_DrawRectangle(17, 200, 72, 250, BUTTON_EDGE_COLOR);	  //数字七
	LCD_DrawRectangle(92, 200, 147, 250, BUTTON_EDGE_COLOR);  //数字八
	LCD_DrawRectangle(167, 200, 222, 250, BUTTON_EDGE_COLOR); //数字九

	LCD_DrawRectangle(17, 260, 72, 310, BUTTON_EDGE_COLOR);	  //删除
	LCD_DrawRectangle(92, 260, 147, 310, BUTTON_EDGE_COLOR);  //数字零
	LCD_DrawRectangle(167, 260, 222, 310, BUTTON_EDGE_COLOR); //确定

	LCD_DrawBox(18, 81, 54, 49, BUTTON_COLOR);	 //数字一
	LCD_DrawBox(93, 81, 54, 49, BUTTON_COLOR);	 //数字二
	LCD_DrawBox(168, 81, 54, 49, BUTTON_COLOR); //数字三

	LCD_DrawBox(18, 141, 54, 49, BUTTON_COLOR);	  //数字四
	LCD_DrawBox(93, 141, 54, 49, BUTTON_COLOR);  //数字五
	LCD_DrawBox(168, 141, 54, 49, BUTTON_COLOR); //数字六

	LCD_DrawBox(18, 201, 54, 49, BUTTON_COLOR);	  //数字七
	LCD_DrawBox(93, 201, 54, 49, BUTTON_COLOR);  //数字八
	LCD_DrawBox(168, 201, 54, 49, BUTTON_COLOR); //数字九

	LCD_DrawBox(18, 261, 54, 49, BUTTON_COLOR);	  //删除
	LCD_DrawBox(93, 261, 54, 49, BUTTON_COLOR);  //数字零
	LCD_DrawBox(168, 261, 54, 49, BUTTON_COLOR); //确定


	TFT_ShowString(5, 2, PhoneNumber, BLACK, WHITE);

	TFT_ShowString(5, 6, "1", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(14, 6, "2", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(24, 6, "3", BUTTON_FONT_COLOR, BUTTON_COLOR);

	TFT_ShowString(5, 10, "4", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(14, 10, "5", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(24, 10, "6", BUTTON_FONT_COLOR, BUTTON_COLOR);

	TFT_ShowString(5, 14, "7", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(14, 14, "8", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(24, 14, "9", BUTTON_FONT_COLOR, BUTTON_COLOR);

	TFT_ShowString(4, 17, "Del", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(14, 17, "0", BUTTON_FONT_COLOR, BUTTON_COLOR);
	TFT_ShowString(22, 17, "Enter", BUTTON_FONT_COLOR, BUTTON_COLOR);
}

void SetPhoneTouch(void)
{
	num = 0;
	SetPhoneMFrame();
	while (1)
	{
		TFT_ShowString(5, 2, PhoneNumber, BLACK, WHITE);
		ShowNumber[0] = PhoneNumber[num];
		TFT_ShowString(5 + num, 2, ShowNumber, WHITE, BLUE);
		while (1)
		{
			if (!Touch_IRQ())
			{
				if (Convert_Pos()) //得到坐标值
				{
					inputNumber = 0;
					if (tp_pixlcd.y > 80 && tp_pixlcd.y < 130) //第一排 (80:80+50)
					{
						if (tp_pixlcd.x > 17 && tp_pixlcd.x < 72) //数字一(17:17+55)
						{
							inputNumber = 1;
						}
						else if (tp_pixlcd.x > 92 && tp_pixlcd.x < 147) //数字二(92:92+55)
						{
							inputNumber = 2;
						}
						else if (tp_pixlcd.x > 167 && tp_pixlcd.x < 222) //数字三(167:167+55)
						{
							inputNumber = 3;
						}
					}
					else if (tp_pixlcd.y > 140 && tp_pixlcd.y < 190) //第二排 (140:140+50)
					{
						if (tp_pixlcd.x > 17 && tp_pixlcd.x < 72) //数字四(17:17+55)
						{
							inputNumber = 4;
						}
						else if (tp_pixlcd.x > 92 && tp_pixlcd.x < 147) //数字五(92:92+55)
						{
							inputNumber = 5;
						}
						else if (tp_pixlcd.x > 167 && tp_pixlcd.x < 222) //数字六(167:167+55)
						{
							inputNumber = 6;
						}
					}
					else if (tp_pixlcd.y > 200 && tp_pixlcd.y < 250) //第三排 (200:200+50)
					{
						if (tp_pixlcd.x > 17 && tp_pixlcd.x < 72) //数字四(17:17+55)
						{
							inputNumber = 7;
						}
						else if (tp_pixlcd.x > 92 && tp_pixlcd.x < 147) //数字五(92:92+55)
						{
							inputNumber = 8;
						}
						else if (tp_pixlcd.x > 167 && tp_pixlcd.x < 222) //数字六(167:167+55)
						{
							inputNumber = 9;
						}
					}
					else if (tp_pixlcd.y > 260 && tp_pixlcd.y < 310) //第四排 (260:260+50)
					{
						if (tp_pixlcd.x > 17 && tp_pixlcd.x < 72) //数字四(17:17+55)
						{
							inputNumber = 10;
						}
						else if (tp_pixlcd.x > 92 && tp_pixlcd.x < 147) //数字五(92:92+55)
						{
							inputNumber = 11;
						}
						else if (tp_pixlcd.x > 167 && tp_pixlcd.x < 222) //数字六(167:167+55)
						{
							inputNumber = 12;
						}
					}
					if (inputNumber == 1)
					{
						LCD_DrawBox(18, 81, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(5, 6, "1", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 2)
					{
						LCD_DrawBox(93, 81, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(14, 6, "2", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 3)
					{
						LCD_DrawBox(168, 81, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(24, 6, "3", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 4)
					{
						LCD_DrawBox(18, 141, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(5, 10, "4", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 5)
					{
						LCD_DrawBox(93, 141, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(14, 10, "5", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 6)
					{
						LCD_DrawBox(168, 141, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(24, 10, "6", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 7)
					{
						LCD_DrawBox(18, 201, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(5, 14, "7", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 8)
					{
						LCD_DrawBox(93, 201, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(14, 14, "8", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 9)
					{
						LCD_DrawBox(168, 201, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(24, 14, "9", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 10)
					{
						LCD_DrawBox(18, 261, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(4, 17, "Del", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 11)
					{
						LCD_DrawBox(93, 261, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(14, 17, "0", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
					else if (inputNumber == 12)
					{
						LCD_DrawBox(168, 261, 54, 49, BUTTON_ON_COLOR);
						TFT_ShowString(22, 17, "Enter", BUTTON_FONT_COLOR, BUTTON_ON_COLOR);
						break;
					}
				}
			}
		}
		while (!Touch_IRQ())
			; //等待触摸松开

		switch (inputNumber)
		{
		case 1:
		{
			PhoneNumber[num] = '1';
			//*(PhoneNumber + Number) = '1';
			LCD_DrawBox(18, 81, 54, 49, BUTTON_COLOR);
			TFT_ShowString(5, 6, "1", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 2:
		{
			PhoneNumber[num] = '2';
			//*(PhoneNumber + Number) = '2';
			LCD_DrawBox(93, 81, 54, 49, BUTTON_COLOR);
			TFT_ShowString(14, 6, "2", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 3:
		{
			PhoneNumber[num] = '3';
			//*(PhoneNumber + Number) = '3';
			LCD_DrawBox(168, 81, 54, 49, BUTTON_COLOR);
			TFT_ShowString(24, 6, "3", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 4:
		{
			PhoneNumber[num] = '4';
			//*(PhoneNumber + Number) = '4';
			LCD_DrawBox(18, 141, 54, 49, BUTTON_COLOR);
			TFT_ShowString(5, 10, "4", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 5:
		{
			PhoneNumber[num] = '5';
			//*(PhoneNumber + Number) = '5';
			LCD_DrawBox(93, 141, 54, 49, BUTTON_COLOR);
			TFT_ShowString(14, 10, "5", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 6:
		{
			PhoneNumber[num] = '6';
			//*(PhoneNumber + Number) = '6';
			LCD_DrawBox(168, 141, 54, 49, BUTTON_COLOR);
			TFT_ShowString(24, 10, "6", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 7:
		{
			PhoneNumber[num] = '7';
			//*(PhoneNumber + Number) = '7';
			LCD_DrawBox(18, 201, 54, 49, BUTTON_COLOR);
			TFT_ShowString(5, 14, "7", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 8:
		{
			PhoneNumber[num] = '8';
			//*(PhoneNumber + Number) = '8';
			LCD_DrawBox(93, 201, 54, 49, BUTTON_COLOR);
			TFT_ShowString(14, 14, "8", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 9:
		{
			PhoneNumber[num] = '9';
			//*(PhoneNumber + Number) = '9';
			LCD_DrawBox(168, 201, 54, 49, BUTTON_COLOR);
			TFT_ShowString(24, 14, "9", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 10:
		{
			if (num > 0)
				num--;
				LCD_DrawBox(18, 261, 54, 49, BUTTON_COLOR);
			TFT_ShowString(4, 17, "Del", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 11:
		{
			PhoneNumber[num] = '0';
			//*(PhoneNumber + Number) = '0';
			LCD_DrawBox(93, 261, 54, 49, BUTTON_COLOR);
			TFT_ShowString(14, 17, "0", BUTTON_FONT_COLOR, BUTTON_COLOR);
			break;
		}
		case 12:
		{
			//PhoneNumber[num] = '\0';
			return;
			break;
		}
		}
		if (((0 < inputNumber && inputNumber < 10) || inputNumber == 11) && num < 10)
			num++;
	}
}
