#include "stm32f4xx_conf.h"
#include "lcd.h"

#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )

unsigned short GUI_Color565(unsigned int RGB)
{
  unsigned char  r, g, b;

  b = ( RGB >> (0+3) ) & 0x1f;
  g = ( RGB >> (8+2) ) & 0x3f;
  r = ( RGB >> (16+3)) & 0x1f;

  return( (r<<11) + (g<<5) + (b<<0) );
}

void GUI_Text(unsigned short x, unsigned short y, const unsigned char *str, unsigned short len,unsigned short Color, unsigned short bkColor)
{
  unsigned char i;

  for (i=0;i<len;i++)
  {
	  LCD_PutChar((x+8*i), y, *str++, Color, bkColor);
  }
}

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
  			LCD_Fast_DrawPoint(x,y0,color);
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
  			LCD_Fast_DrawPoint(x0,y,color);
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
			LCD_Fast_DrawPoint(x,y,color);
  		}
 	}
 	else
 	{
 		LCD_Fast_DrawPoint(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			signed short p = dy * 2 - dx;
    			signed short twoDy = 2 * dy;
    			signed short twoDyMinusDx = 2 * (dy - dx);
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
     				LCD_Fast_DrawPoint(x, y,color);
    			}
   			}
   			else
   			{
    			signed short p = dx * 2 - dy;
    			signed short twoDx = 2 * dx;
    			signed short twoDxMinusDy = 2 * (dx - dy);
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
     				LCD_Fast_DrawPoint(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			signed short p = dy * 2 - dx;
    			signed short twoDy = 2 * dy;
	    		signed short twoDyMinusDx = 2 * (dy - dx);
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
     				LCD_Fast_DrawPoint(x, y,color);
    			}
   			}
	   		else
   			{
    			signed short p = dx * 2 - dy;
    			signed short twoDx = 2 * dx;
	    		signed short twoDxMinusDy = 2 * (dx - dy);
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
     				LCD_Fast_DrawPoint(x, y,color);
    			}
   			}
  		}
 	}
}

void GUI_Circle(unsigned short cx,unsigned short cy,unsigned short r,unsigned short color,unsigned char fill)
{
	unsigned short x,y;
	signed short delta,tmp;
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
			LCD_Fast_DrawPoint(cx+x,cy+y,color);
			LCD_Fast_DrawPoint(cx-x,cy+y,color);
			LCD_Fast_DrawPoint(cx+x,cy-y,color);
			LCD_Fast_DrawPoint(cx-x,cy-y,color);
			LCD_Fast_DrawPoint(cx+y,cy+x,color);
			LCD_Fast_DrawPoint(cx-y,cy+x,color);
			LCD_Fast_DrawPoint(cx+y,cy-x,color);
			LCD_Fast_DrawPoint(cx-y,cy-x,color);
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

void GUI_Rectangle(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1,unsigned short color,unsigned char fill) {
	unsigned short h = y1-y0;

	if(fill)	{
		for(h++;x0 <=x1;x0++) LCD_VLine(x0, y0, h, color);
		return;
	}

	LCD_VLine(x0, y0, h,color);
	GUI_Line(x0,y0,x1,y0,color);
	GUI_Line(x0,y1,x1,y1,color);
	LCD_VLine(x1, y0, h,color);
}

void  GUI_Square(unsigned short x0, unsigned short y0, unsigned short with, unsigned short color,unsigned char fill)
{
	GUI_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}
