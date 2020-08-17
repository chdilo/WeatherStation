#include "delay.h"
#include "tft.h"
#include "txt.h"
#include "chuankou.h"
#include "zhuce.h"
#include "touch.h"

void FailZhuce(void)
{
	TFT_ClearONELINE(3, BLACK);
	TFT_ShowString_W25(8, 3, 2, "网络注册失败", WHITE, BLACK);
	delayms(2000);
	TFT_ShowString(0, 8, "------------------------------", WHITE, BLACK);
	TFT_ShowString_W25(0, 10, 2, "    点击屏幕进入系统界面", WHITE, BLACK);
	TFT_ShowString(0, 12, "------------------------------", WHITE, BLACK);
	while (Touch_IRQ())
		;
}

/************************ 开机注册***********************/
void Zhuce(void)
{
	unsigned int i = 30;

	ChuankouIOInit(); //串口初始化
	USART1_Init();
	TFT_ClearArea(0, 0, 240, 320, BLACK);
	while (i--)
	{
		TFT_ClearONELINE(3, BLACK);
		TFT_ShowString_W25(5, 3, 2, "系统启动中，请稍等.", WHITE, BLACK);
		delayms(500);
		TFT_ShowString_W25(5, 3, 2, "系统启动中，请稍等..", WHITE, BLACK);
		delayms(500);
		TFT_ShowString_W25(5, 3, 2, "系统启动中，请稍等...", WHITE, BLACK);
		delayms(500);
		TFT_ShowString_W25(5, 3, 2, "系统启动中，请稍等....", WHITE, BLACK);
		delayms(500);
		if (GetFlag2 == 1)
		{
			GetFlag2 = 0;
			TFT_ClearONELINE(3, BLACK);
			TFT_ShowString_W25(8, 3, 2, "网络注册成功", WHITE, BLACK);
			delayms(2000);
			TFT_ShowString(0, 8, "------------------------------", WHITE, BLACK);
			TFT_ShowString_W25(0, 10, 2, "        进入系统界面", WHITE, BLACK);
			TFT_ShowString(0, 12, "------------------------------", WHITE, BLACK);
			delayms(2000);
			return;
		}
	}
	FailZhuce();
}
