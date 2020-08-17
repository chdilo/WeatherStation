#ifndef __HUANJING_H
#define __HUANJING_H	

extern unsigned char humitureT[3], humitureH[3]; //温度,湿度
extern unsigned char xdata DispBuf[6];           //光照度

void HuanjingIOInit(void);
void Init_BH1750(void);
void BH1750(void);
void DH11GetHumiture(void);


void HuanjJiancTouch(void);

#endif 