#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "delay.h"
#include "huanjing.h"
#include "iic.h"
#include "interrupt.h"
#include "STC15F2K60S2.h"

#define SlaveAddress 0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改 \
                          //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A
#define BACK_COLOR 0xC638
#define AXIS_COLOR BLACK
#define PLOT_BACK_COLOR WHITE
#define G_COLOR 0x0397
#define T_COLOR 0xD2A3
#define H_COLOR 0x7546



sbit CG_WDO = P4 ^ 4; //单总线
/**
  dongtai.c功能
 光照度 ；（基于IIC协议）
 温湿度 ； P44
*/

unsigned char xdata BH1750_BUF[3];
unsigned char humitureT[3], humitureH[3]; //温度,湿度
unsigned char xdata DispBuf[6];           //光照度
unsigned char T_H, T_L, R_H, R_L;         //温度,湿度 H为整数位，L为小数位
unsigned int GZData;

void HuanjingIOInit(void)
{
    P4M0 |= (1 << 4);
    P4M1 &= ~(1 << 4);
    Init_IIC();
}
/****************************************以下为光照度功能程序***********************************************/
void Single_Write_BH1750(unsigned char REG_Address)
{
    I2CStart();                //起始信号
    I2CSendByte(SlaveAddress); //发送设备地址+写信号
    I2CSendByte(REG_Address);  //内部寄存器地址，请参考中文pdf22页
    I2CStop();                 //发送停止信号
}
//*********************************************************
//连续读出BH1750内部数据
//*********************************************************
void Multiple_read_BH1750(void)
{
    unsigned char i;
    I2CStart();                    //起始信号
    I2CSendByte(SlaveAddress + 1); //发送设备地址+读信号

    for (i = 0; i < 3; i++) //连续读取6个地址数据，存储中BH1750_BUF
    {
        BH1750_BUF[i] = I2CRecvByte(); //BH1750_BUF[0]存储0x32地址中的数据
        if (i == 3)
        {

            I2CSendACK(1); //最后一个数据需要回NOACK
        }
        else
        {
            I2CSendACK(0); //回应ACK
        }
    }

    I2CStop(); //停止信号
    delayms(5);
}

//初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750(void)
{
    Single_Write_BH1750(0x01);
}

//*********************************************************
//主程序********
//*********************************************************
void BH1750(void)
{

    unsigned int TempData; //变量

    Single_Write_BH1750(0x01); // power on
    Single_Write_BH1750(0x10); // H- resolution mode

    delayms(180); //延时180ms

    Multiple_read_BH1750(); //连续读出数据，存储在BH1750_BUF中

    GZData = BH1750_BUF[0];
    GZData = (GZData << 8) + BH1750_BUF[1]; //合成数据

    GZData = (unsigned int)(((float)GZData) / 1.2);

    DispBuf[0] = GZData / 10000 + 0x30;
    TempData = GZData % 10000; //取余运算
    DispBuf[1] = TempData / 1000 + 0x30;
    TempData = TempData % 1000; //取余运算
    DispBuf[2] = TempData / 100 + 0x30;
    TempData = TempData % 100; //取余运算
    DispBuf[3] = TempData / 10 + 0x30;
    TempData = TempData % 10; //取余运算
    DispBuf[4] = TempData + 0x30;
    DispBuf[5] = '\0';

    //TFT_ShowString(7, 3, DispBuf, BLACK, CYAN);
}

/***************************************以下为温湿度功能程序************************************************/

void DHT_IO_OUT(void)
{
    CG_WDO = 1;
    P4M0 |= (1 << 4);
    P4M1 &= ~(1 << 4);
}
void DHT_IO_IN(void)
{
    CG_WDO = 1;
    P4M0 &= ~(1 << 4);
    P4M1 |= (1 << 4);
}

void DH11Start(void) //开始信号
{
    DHT_IO_OUT();
    CG_WDO = 0;
    delayms(20); //至少拉低18ms
    DHT_IO_IN(); //切换至输入
    delay(30);   //20-40us
}

unsigned char DH11ReceiveOneByte(void) //接收一个字节,高位在前
{
    unsigned char i, temp = 0;
    for (i = 0; i < 8; i++)
    {
        temp <<= 1;
        while (CG_WDO)
            ; //等待拉低50us
        delay(40);
        while (!CG_WDO)
            ; //等待拉高26-28us
        delay(35);
        if (CG_WDO == 1) //表示高电平持续时间超过28为数据1 否者为零
        {
            temp |= (1 << 0);
        }
    }
    return temp;
}

void DH11GetHumiture(void) //接收数据 5组:温度整数,小数,湿度整数,小数,校验和
{                          //传感器小数位为0,所以不需要
    unsigned char check, num_check;

    DH11Start(); //开始信号
    while (CG_WDO)
        ; //等待响应信号拉低
    delay(80);
    while (!CG_WDO)
        ; //等待响应信号拉高
    delay(80);

    R_H = DH11ReceiveOneByte();
    R_L = DH11ReceiveOneByte(); //传感器小数位为0
    T_H = DH11ReceiveOneByte();
    T_L = DH11ReceiveOneByte(); //传感器小数位为0
    num_check = DH11ReceiveOneByte();

    while (CG_WDO)
        ; //等待响应信号拉低
    delay(50);

    DHT_IO_OUT(); //释放总线

    check = R_H + R_L + T_H + T_L;
    if (check == num_check)
    {
        humitureT[0] = T_H / 10 + 0x30;
        humitureT[1] = T_H % 10 + 0x30;
        humitureT[2] = '\0';
        humitureH[0] = R_H / 10 + 0x30;
        humitureH[1] = R_H % 10 + 0x30;
        humitureH[2] = '\0';
    }
}

/****************************************以下为触控屏显示程序***********************************************/

void HuanjingMFrame(void)
{
    TFT_ClearArea(0, 16, 240, 320 - 16, BACK_COLOR);
    //TFT_ClearONELINE(19, BLUE);
    TFT_ShowString_W25(0, 19, 2, "主界面", BLACK, WHITE);

    TFT_ShowString_W25(8, 1, 2, "当前环境检测:", BLACK, BACK_COLOR);
    TFT_ShowString_W25(0, 3, 2, "光照度:", BLACK, BACK_COLOR);
    TFT_ShowString_W25(13, 3, 2, "温度:", BLACK, BACK_COLOR);
    TFT_ShowString_W25(21, 3, 2, "湿度:", BLACK, BACK_COLOR);
}
void HumitureFrame(void)
{
    TFT_ClearArea(20, 70, 200, 200, PLOT_BACK_COLOR);
    //*********坐标
    LCD_DrawLine(20, 70, 20, 270, AXIS_COLOR);
    LCD_DrawLine(20, 70, 15, 80, AXIS_COLOR);
    LCD_DrawLine(20, 70, 25, 80, AXIS_COLOR);
    LCD_DrawLine(20, 270, 220, 270, AXIS_COLOR);
    LCD_DrawLine(220, 270, 210, 265, AXIS_COLOR);
    LCD_DrawLine(220, 270, 210, 275, AXIS_COLOR);
    //*********刻度
    LCD_DrawLine(20, 90, 25, 90, AXIS_COLOR); //间隔20,每一个最小刻度值对应2个像素 坐标=270-温度值*2
    LCD_DrawLine(20, 110, 25, 110, AXIS_COLOR);
    LCD_DrawLine(20, 130, 25, 130, AXIS_COLOR);
    LCD_DrawLine(20, 150, 25, 150, AXIS_COLOR);
    LCD_DrawLine(20, 170, 25, 170, AXIS_COLOR);
    LCD_DrawLine(20, 190, 25, 190, AXIS_COLOR);
    LCD_DrawLine(20, 210, 25, 210, AXIS_COLOR);
    LCD_DrawLine(20, 230, 25, 230, AXIS_COLOR);
    LCD_DrawLine(20, 250, 25, 250, AXIS_COLOR);
}

void HuanjJiancTouch(void)
{
    unsigned int OldX1 = 20, OldY1 = 270, NewX1, NewY1; //温度
    unsigned int OldX2 = 20, OldY2 = 270, NewX2, NewY2; //湿度
    unsigned int OldX3 = 20, OldY3 = 270, NewX3, NewY3; //光照度
    unsigned char cidu = 8;                             //画线的尺度比例
    unsigned char THTemp[3] = {0};

    HuanjingMFrame();
    HuanjingIOInit();
    HumitureFrame();

    Init_BH1750();
    Time1_Init20MS(); //定时1s用于更新传感器时间

    TR1 = 1; //开始计数

    while (1)
    {
        if (!Touch_IRQ()) //有触摸发生
        {
            if (Convert_Pos()) //得到坐标值
            {
                if (tp_pixlcd.x < 48 && tp_pixlcd.y > 298) //下标签栏"主界面"
                {
                    TR1 = 0;
                    ET1 = 0;
                    CountNumber = 0;
                    TFT_ShowString_W25(0, 19, 2, "主界面", WHITE, BLUE);
                    return;
                } //关闭定时器
            }
        }
        //光照度传感器
        BH1750();
        TFT_ShowString(7, 3, DispBuf, G_COLOR, BACK_COLOR);
        //绘制温度曲线

        if (CountNumber > 50) //计时时间大于1s,取一次值    20ms* CountNumber
        {
            CountNumber = 0;
            DH11GetHumiture();
            TFT_ShowString(18, 3, humitureT, T_COLOR, BACK_COLOR); //温度数据
            TFT_ShowString(26, 3, humitureH, H_COLOR, BACK_COLOR);  //湿度数据

            if (OldX1 < 100 * 2)
            {
                NewX1 = OldX1 + cidu;
                NewY1 = 270 - T_H * 2;
                LCD_DrawLine(OldX1, OldY1, NewX1, NewY1, T_COLOR);
                OldX1 = NewX1;
                OldY1 = NewY1;

                NewX2 = OldX2 + cidu;
                NewY2 = 270 - R_H * 2;
                LCD_DrawLine(OldX2, OldY2, NewX2, NewY2, H_COLOR);
                OldX2 = NewX2;
                OldY2 = NewY2;

                NewX3 = OldX3 + cidu;
                if (GZData < 1000)
                {
                    NewY3 = 270 - ((GZData + 5) / 10) * 2;
                }
                else
                {
                    NewY3 = 270 - 100 * 2;
                }
                LCD_DrawLine(OldX3, OldY3, NewX3, NewY3, G_COLOR);
                OldX3 = NewX3;
                OldY3 = NewY3;
            }
            else
            {
                OldX1 = 20;
                OldX2 = 20;
                OldX3 = 20;
                HumitureFrame();
            }
        }
    }
}
