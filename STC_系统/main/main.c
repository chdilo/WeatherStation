#include "touch_main.h"
#include "init.h"
void main()
{
	Init();

	while (1)
	{
		MainTouch(); //启动 触摸屏 操作界面
	}
}
