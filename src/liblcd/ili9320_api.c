#include "stm32f10x_conf.h"
#include "ili9320.h"


/****************************************************************************
* ��    �ƣ�GUI_CmpColor()
* ��    �ܣ��ж���ɫֵ�Ƿ�һ�¡�
* ��ڲ�����color1		��ɫֵ1
*		        color2		��ɫֵ2
* ���ڲ���������1��ʾ��ͬ������0��ʾ����ͬ��
* ˵    ����
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )


/****************************************************************************
* ��    �ƣ�unsigned short GUI_Color565(u32 RGB)
* ��    �ܣ���RGB��ɫת��Ϊ16λ��ɫ��
* ��ڲ�����RGB  ��ɫֵ
* ���ڲ���������16λRGB��ɫֵ��
* ˵    ����
* ���÷�����i=GUI_Color565(0xafafaf);
****************************************************************************/
unsigned short GUI_Color565(unsigned int RGB)
{
  unsigned char  r, g, b;

  b = ( RGB >> (0+3) ) & 0x1f;		// ȡBɫ�ĸ�5λ
  g = ( RGB >> (8+2) ) & 0x3f;		// ȡGɫ�ĸ�6λ
  r = ( RGB >> (16+3)) & 0x1f;		// ȡRɫ�ĸ�5λ
   
  return( (r<<11) + (g<<5) + (b<<0) );		
}

/****************************************************************************
* ��    �ƣ�void GUI_Text(unsigned short x, unsigned short y, unsigned char *str, unsigned short len,unsigned short Color, unsigned short bkColor)
* ��    �ܣ���ָ��������ʾ�ַ���
* ��ڲ�����x      ������
*           y      ������
*           *str   �ַ���
*           len    �ַ�������
*           Color  �ַ���ɫ
*           bkColor�ַ�������ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Text(0,0,"0123456789",10,0x0000,0xffff);
****************************************************************************/
void GUI_Text(unsigned short x, unsigned short y, const unsigned char *str, unsigned short len,unsigned short Color, unsigned short bkColor)
{
  unsigned char i;
  
  for (i=0;i<len;i++)
  {
    ili9320_PutChar((x+8*i),y,*str++,Color,bkColor);
  }
}

/****************************************************************************
* ��    �ƣ�void GUI_Line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1,unsigned short color)
* ��    �ܣ���ָ�����껭ֱ��
* ��ڲ�����x0     A��������
*           y0     A��������
*           x1     B��������
*           y1     B��������
*           color  ����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Line(0,0,240,320,0x0000);
****************************************************************************/
void GUI_Line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1,unsigned short color)
{
 	unsigned short x,y;
 	unsigned short dx;// = abs(x1 - x0);
 	unsigned short dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			ili9320_SetPoint(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			ili9320_SetPoint(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			ili9320_SetPoint(x,y,color);
  		}
 	}
 	else
 	{
 		ili9320_SetPoint(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				ili9320_SetPoint(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
  		}
 	}
}

/****************************************************************************
* ��    �ƣ�void GUI_Circle(unsigned short cx,unsigned short cy,unsigned short r,unsigned short color,unsigned char fill)
* ��    �ܣ���ָ�����껭Բ�������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void GUI_Circle(unsigned short cx,unsigned short cy,unsigned short r,unsigned short color,unsigned char fill)
{
	unsigned short x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			GUI_Line(cx+x,cy+y,cx-x,cy+y,color);
			GUI_Line(cx+x,cy-y,cx-x,cy-y,color);
			GUI_Line(cx+y,cy+x,cx-y,cy+x,color);
			GUI_Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			ili9320_SetPoint(cx+x,cy+y,color);
			ili9320_SetPoint(cx-x,cy+y,color);
			ili9320_SetPoint(cx+x,cy-y,color);
			ili9320_SetPoint(cx-x,cy-y,color);
			ili9320_SetPoint(cx+y,cy+x,color);
			ili9320_SetPoint(cx-y,cy+x,color);
			ili9320_SetPoint(cx+y,cy-x,color);
			ili9320_SetPoint(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}

/****************************************************************************
* ��    �ƣ�void GUI_Rectangle(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1,unsigned short color,unsigned char fill)
* ��    �ܣ���ָ�����򻭾��Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void GUI_Rectangle(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1,unsigned short color,unsigned char fill) {
	unsigned short h = y1-y0;
	if(fill)	{
		for(h++;x0 <=x1;x0++) ili9320_VLine(x0, y0, h,color);
		return;
	}
	ili9320_VLine(x0, y0, h,color);
	GUI_Line(x0,y0,x1,y0,color);
	GUI_Line(x0,y1,x1,y1,color);
	ili9320_VLine(x1, y0, h,color);
}

/****************************************************************************
* ��    �ƣ�void  GUI_Square(unsigned short x0, unsigned short y0, unsigned short with, unsigned short color,unsigned char fill)
* ��    �ܣ���ָ�����������Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void  GUI_Square(unsigned short x0, unsigned short y0, unsigned short with, unsigned short color,unsigned char fill)
{
	GUI_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}
