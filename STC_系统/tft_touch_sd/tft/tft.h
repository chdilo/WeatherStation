#ifndef __TFT_H
#define __TFT_H

//����TFT�ĳߴ�,��30*8���ַ�����20*16���ַ�
#define LCD_W 240
#define LCD_H 320
//������ɫ
#define WHITE 0xFFFF   //��ɫ   1111 1111 1111 1111 (31,63,31) 
#define BLACK 0x0000   //��ɫ   0000 0000 0000 0000 ( 0, 0, 0)
#define BLUE 0x001F    //��ɫ   0000 0000 0001 1111 ( 0, 0,31)
#define GBLUE 0X07FF   //����ɫ 0000 0111 1111 1111 ( 0,63,31)
#define RED 0xF800     //��ɫ   1111 1000 0000 0000 (31, 0, 0)
#define MAGENTA 0xF81F //Ʒ��� 1111 1000 0001 1111 (31, 0,31)
#define GREEN 0x07E0   //��ɫ   0000 0111 1110 0000 ( 0,63, 0)
#define CYAN 0x7FFF    //��ɫ   0111 1111 1111 1111 (15,63,31)
#define YELLOW 0xFFE0  //��ɫ   1111 1111 1110 0000 (31,63, 0)
#define BROWN 0XBC40   //��ɫ   1011 1100 0100 0000 (23,34, 0)
#define BRRED 0XFC07   //�غ�ɫ 1111 1100 0000 0111 (31,32, 7)
#define GRAY 0X8430    //��ɫ   1000 0100 0011 0000 (16,33,16)
//GUI��ɫ
#define DARKBLUE 0X01CF  //����ɫ
#define LIGHTBLUE 0X7D7C //ǳ��ɫ
#define GRAYBLUE 0X5458  //����ɫ
//������ɫΪPANEL����ɫ
#define LIGHTGREEN 0X841F //ǳ��ɫ
#define LGRAY 0XC618      //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE 0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE 0X2B12    //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

//#define BACK_COLOR WHITE
//#define POINT_COLOR RED //����ɫ������ɫ

#define HVSCREEN //����ñ�����ʾ��������ʾ�������Ǻ�����ʾ
void TFT_Writ_Bus(unsigned char VH, unsigned char VL);
void TFT_WR_DATA8(unsigned char VH, unsigned char VL);
void TFT_WR_DATA(unsigned int da);
void TFT_WR_REG(unsigned int da);
void TFT_WR_REG_DATA(unsigned int reg, unsigned int da);
void TFT_AddressSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void TFT_ClearByColor(unsigned int Color);
void TFT_ClearArea(unsigned int start_x, unsigned int start_y, unsigned int x_size, unsigned int y_size, unsigned int Color);
void TFT_ClearONELINE(unsigned int n, unsigned int Color);
void TFT_Init(void);
/*******************************************��ʾ�ַ����ܺ�����*********************************************************************/
//x,yΪ�к��У��������ص�Ӣ��8*16,����16*16
void TFT_ShowWord(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned char *temp, unsigned int PColor, unsigned int BColor);
void TFT_ShowString(unsigned int x, unsigned int y, unsigned char *ptr, unsigned int PenColor, unsigned int BackColor);
/*******************************************��ʾͼƬ���ܺ�����*********************************************************************/
/*******************************************��ʾ��ͼ���ܺ�����*********************************************************************/
void LCD_DrawPoint_big(unsigned int x, unsigned int y, unsigned char n, unsigned int Pen_Color);
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int Pen_Color);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int Pen_Color);
void LCD_DrawBox(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned int Pen_Color);
void Draw_Circle(unsigned int x0, unsigned int y0, unsigned int r, unsigned int Pen_Color);
void TFT_ShowString_w25(unsigned int x, unsigned int y, unsigned char *ptr, unsigned int PenColor, unsigned int BackColor);
#endif
