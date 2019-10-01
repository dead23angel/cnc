#include "stm32f4xx_conf.h"
#include "stdlib.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_font.h"

_lcd_dev lcddev;
uint16_t POINT_COLOR = White;
uint16_t BACK_COLOR  = Black;

void LCD_WR_REG(uint16_t regval)
{
	regval=regval;		//Р“Р‰Р’в„–Р“вЂњР“С“-O2Р“вЂњР“вЂ¦Р’В»Р’Р‡Р’ВµР“вЂћР“Р‰Р’В±Р’С”Р“Р†,Р’В±Р“пїЅР“С’Р“В«Р’Р†Р“ТђР“в‚¬Р“В«Р’ВµР“вЂћР“вЂ�Р“вЂњР“Р‰Р’В±
	LCD->LCD_REG=regval;//Р“С’Р’Т‘Р“в‚¬Р“В«Р“вЂ™Р’Р„Р“С’Р’Т‘Р’ВµР“вЂћР’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·Р“С’Р“Р†Р’С”Р“вЂ¦
}

void LCD_WR_DATA(uint16_t data)
{
	data=data;			//Р“Р‰Р’в„–Р“вЂњР“С“-O2Р“вЂњР“вЂ¦Р’В»Р’Р‡Р’ВµР“вЂћР“Р‰Р’В±Р’С”Р“Р†,Р’В±Р“пїЅР“С’Р“В«Р’Р†Р“ТђР“в‚¬Р“В«Р’ВµР“вЂћР“вЂ�Р“вЂњР“Р‰Р’В±
	LCD->LCD_RAM=data;
}

uint16_t LCD_RD_DATA(void)
{
	uint16_t ram;			//Р’В·Р“Р‚Р“вЂ“Р’в„–Р’В±Р’В»Р“вЂњР“вЂ¦Р’В»Р’Р‡
	ram=LCD->LCD_RAM;
	return ram;
}

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD->LCD_REG = LCD_Reg;		//Р“С’Р’Т‘Р“в‚¬Р“В«Р“вЂ™Р’Р„Р“С’Р’Т‘Р’ВµР“вЂћР’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·Р“С’Р“Р†Р’С”Р“вЂ¦
	LCD->LCD_RAM = LCD_RegValue;//Р“С’Р’Т‘Р“в‚¬Р“В«Р“Р‰Р“Р…Р’С•Р“Сњ
}

uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);		//Р“С’Р’Т‘Р“в‚¬Р“В«Р“вЂ™Р’Р„Р’В¶Р“РѓР’ВµР“вЂћР’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·Р“С’Р“Р†Р’С”Р“вЂ¦
	_delay_us(5);
	return LCD_RD_DATA();		//Р’В·Р’ВµР’В»Р“пїЅР’В¶Р“РѓР’ВµР’Р…Р’ВµР“вЂћР“вЂ“Р’Вµ
}

void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;
}

void LCD_WriteRAM(uint16_t RGB_Code)
{
	LCD->LCD_RAM = RGB_Code;//Р“С’Р’Т‘Р“Р‰Р’В®Р“РѓР“в„–Р“Р‹Р’В»GRAM
}

uint16_t LCD_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;
	rgb=(b<<11)+(g<<5)+(r<<0);
	return(rgb);
}

void opt_delay(uint8_t i)
{
	while(i--);
}

uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//Р’С–Р’В¬Р’в„–Р“Р…Р“РѓР“вЂ№Р’В·Р’В¶Р“Р‹Р’В§,Р“вЂ“Р’В±Р’Р…Р“вЂњР’В·Р’ВµР’В»Р“пїЅ
	LCD_SetCursor(x,y);
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X2E);//9341/6804/3510 Р’В·Р’СћР“вЂ№Р“РЊР’В¶Р“РѓGRAMР“вЂ“Р’С‘Р“РѓР“В®
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 Р’В·Р’СћР“вЂ№Р“РЊР’В¶Р“РѓGRAMР“вЂ“Р’С‘Р“РѓР“В®
	else LCD_WR_REG(R34);      		 				//Р“вЂ Р“В¤Р“вЂ№Р“В»ICР’В·Р’СћР“вЂ№Р“РЊР’В¶Р“РѓGRAMР“вЂ“Р’С‘Р“РѓР“В®
	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,Р“вЂ�Р“вЂњР“Р‰Р’В±2us
 	LCD_RD_DATA();									//dummy Read
	opt_delay(2);
 	r=LCD_RD_DATA();  		  						//Р“Р‰Р’ВµР’С�Р“Р‰Р“вЂ”Р“С‘Р’В±Р“Р„Р“вЂ�Р“вЂўР“вЂ°Р’В«
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510Р“вЂ™Р’Р„Р’В·Р“вЂ“2Р’Т‘Р“Р‹Р’В¶Р“РѓР’С–Р“В¶
 	{
		opt_delay(2);
		b=LCD_RD_DATA();
		g=r&0XFF;		//Р’В¶Р“вЂќР“вЂњР“С™9341/5310/5510,Р’ВµР“С™Р“вЂ™Р’В»Р’Т‘Р“Р‹Р’В¶Р“РѓР“в‚¬Р’РЋР’ВµР“вЂћР“Р‰Р“вЂЎRGР’ВµР“вЂћР“вЂ“Р’Вµ,RР“вЂќР“С™Р“вЂЎР’В°,GР“вЂќР“С™Р’С”Р“С–,Р’С‘Р“В·Р“вЂўР’С�8Р“Р‹Р’В»
		g<<=8;
	}
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0XB505||lcddev.id==0XC505)return r;	//Р“вЂўР“СћР’С�Р’С‘Р“вЂ“Р“вЂ“ICР“вЂ“Р’В±Р’Р…Р“вЂњР’В·Р’ВµР’В»Р“пїЅР“вЂ�Р“вЂўР“вЂ°Р’В«Р“вЂ“Р’Вµ
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510Р“С’Р“РЃР“вЂ™Р’Р„Р’в„–Р’В«Р“Р‰Р’Р…Р“вЂ”Р’Р„Р’В»Р’В»Р“вЂ™Р’В»Р“РЏР“вЂљ
	else return LCD_BGR2RGB(r);						//Р“вЂ Р“В¤Р“вЂ№Р“В»IC
}

void LCD_DisplayOn(void)
{
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X29);	//Р’С—Р’Р„Р“вЂ Р“Т‘Р“РЏР“вЂќР“Р‰Р’С•
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//Р’С—Р’Р„Р“вЂ Р“Т‘Р“РЏР“вЂќР“Р‰Р’С•
	else LCD_WriteReg(R7,0x0173); 				 	//Р’С—Р’Р„Р“вЂ Р“Т‘Р“РЏР“вЂќР“Р‰Р’С•
}

void LCD_DisplayOff(void)
{
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X28);	//Р’в„–Р“пїЅР’В±Р“вЂўР“РЏР“вЂќР“Р‰Р’С•
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//Р’в„–Р“пїЅР’В±Р“вЂўР“РЏР“вЂќР“Р‰Р’С•
	else LCD_WriteReg(R7,0x0);//Р’в„–Р“пїЅР’В±Р“вЂўР“РЏР“вЂќР“Р‰Р’С•
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
 	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos>>8);
		LCD_WR_DATA(Xpos&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_DATA(Ypos&0XFF);
	}else if(lcddev.id==0X6804)
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//Р’С”Р“РЋР“вЂ Р“РѓР“Р‰Р’В±Р’Т‘Р’В¦Р“Р‚Р“В­
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos>>8);
		LCD_WR_DATA(Xpos&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_DATA(Ypos&0XFF);
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(Xpos>>8);
		LCD_WR_REG(lcddev.setxcmd+1);
		LCD_WR_DATA(Xpos&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_REG(lcddev.setycmd+1);
		LCD_WR_DATA(Ypos&0XFF);
	}else
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//Р’С”Р“РЋР“вЂ Р“РѓР“вЂ Р“В¤Р“Р‰Р’ВµР’С•Р“РЊР“Р‰Р“вЂЎР’ВµР“В·Р“вЂ”Р’Р„x,yР“вЂ”Р“С‘Р’В±Р“Р„
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}
}

void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;
	uint16_t xsize,ysize;
	if(lcddev.dir==1&&lcddev.id!=0X6804)//Р’С”Р“РЋР“вЂ Р“РѓР“Р‰Р’В±Р’Р€Р’В¬Р’В¶Р“вЂќ6804Р’Р†Р’В»Р’С‘Р“вЂћР’В±Р“В¤Р“вЂ°Р’РЃР“С“Р“РЃР’В·Р’Р…Р“РЏР“Р†Р’Р€Р’РЋ
	{
		switch(dir)//Р’В·Р’Р…Р“РЏР“Р†Р“вЂ”Р’Р„Р’В»Р’В»
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510)//9341/6804/5310/5510,Р’С”Р“СљР“РЉР“пїЅР“Р‰Р“Сћ
	{
		switch(dir)
		{
			case L2R_U2D://Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™,Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ
				regval|=(0<<7)|(0<<6)|(0<<5);
				break;
			case L2R_D2U://Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™,Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ
				regval|=(1<<7)|(0<<6)|(0<<5);
				break;
			case R2L_U2D://Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–,Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ
				regval|=(0<<7)|(1<<6)|(0<<5);
				break;
			case R2L_D2U://Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–,Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ
				regval|=(1<<7)|(1<<6)|(0<<5);
				break;
			case U2D_L2R://Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ,Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™
				regval|=(0<<7)|(0<<6)|(1<<5);
				break;
			case U2D_R2L://Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ,Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–
				regval|=(0<<7)|(1<<6)|(1<<5);
				break;
			case D2U_L2R://Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ,Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™
				regval|=(1<<7)|(0<<6)|(1<<5);
				break;
			case D2U_R2L://Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ,Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–
				regval|=(1<<7)|(1<<6)|(1<<5);
				break;
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510))regval|=0X08;//5310/5510Р’Р†Р’В»Р“С’Р“РЃР“вЂ™Р’Р„BGR
		if(lcddev.id==0X6804)regval|=0x02;//6804Р’ВµР“вЂћBIT6Р’С”Р“РЊ9341Р’ВµР“вЂћР’В·Р’Т‘Р“РѓР“вЂ№
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//Р’Р…Р’В»Р’В»Р’В»X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else
		{
			if(lcddev.width>lcddev.height)//Р’Р…Р’В»Р’В»Р’В»X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}
		if(lcddev.dir==1&&lcddev.id!=0X6804)
		{
			xsize=lcddev.height;
			ysize=lcddev.width;
		}else
		{
			xsize=lcddev.width;
			ysize=lcddev.height;
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((xsize-1)>>8);
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((xsize-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0);
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((ysize-1)>>8);
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((ysize-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd);
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((xsize-1)>>8);LCD_WR_DATA((xsize-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd);
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((ysize-1)>>8);LCD_WR_DATA((ysize-1)&0XFF);
		}
  	}else
	{
		switch(dir)
		{
			case L2R_U2D://Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™,Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ
				regval|=(1<<5)|(1<<4)|(0<<3);
				break;
			case L2R_D2U://Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™,Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ
				regval|=(0<<5)|(1<<4)|(0<<3);
				break;
			case R2L_U2D://Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–,Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–,Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ
				regval|=(0<<5)|(0<<4)|(0<<3);
				break;
			case U2D_L2R://Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ,Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™
				regval|=(1<<5)|(1<<4)|(1<<3);
				break;
			case U2D_R2L://Р’Т‘Р“вЂњР“вЂ°Р“РЏР’ВµР’Р…Р“РЏР“вЂљ,Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–
				regval|=(1<<5)|(0<<4)|(1<<3);
				break;
			case D2U_L2R://Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ,Р’Т‘Р“вЂњР“вЂ”Р“С–Р’ВµР’Р…Р“вЂњР“вЂ™
				regval|=(0<<5)|(1<<4)|(1<<3);
				break;
			case D2U_R2L://Р’Т‘Р“вЂњР“РЏР“вЂљР’ВµР’Р…Р“вЂ°Р“РЏ,Р’Т‘Р“вЂњР“вЂњР“вЂ™Р’ВµР’Р…Р“вЂ”Р“С–
				regval|=(0<<5)|(0<<4)|(1<<3);
				break;
		}
		if(lcddev.id==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K
	 	}else//Р“вЂ Р“В¤Р“вЂ№Р“В»Р“вЂЎР“Р…Р’В¶Р’Р‡IC
		{
			dirreg=0X03;
			regval|=1<<12;
		}
		LCD_WriteReg(dirreg,regval);
	}
}

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);
	LCD_WriteRAM_Prepare();
	LCD->LCD_RAM=POINT_COLOR;
}

void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x>>8);
		LCD_WR_DATA(x&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y>>8);
		LCD_WR_DATA(y&0XFF);
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(x>>8);
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(x&0XFF);
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(y>>8);
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(y&0XFF);
	}else if(lcddev.id==0X6804)
	{
		if(lcddev.dir==1)x=lcddev.width-1-x;//Р’С”Р“РЋР“вЂ Р“РѓР“Р‰Р’В±Р’Т‘Р’В¦Р“Р‚Р“В­
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(x>>8);
		LCD_WR_DATA(x&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(y>>8);
		LCD_WR_DATA(y&0XFF);
	}else
	{
 		if(lcddev.dir==1)x=lcddev.width-1-x;//Р’С”Р“РЋР“вЂ Р“РѓР“вЂ Р“В¤Р“Р‰Р’ВµР’С•Р“РЊР“Р‰Р“вЂЎР’ВµР“В·Р“вЂ”Р’Р„x,yР“вЂ”Р“С‘Р’В±Р“Р„
		LCD_WriteReg(lcddev.setxcmd,x);
		LCD_WriteReg(lcddev.setycmd,y);
	}
	LCD->LCD_REG=lcddev.wramcmd;
	LCD->LCD_RAM=color;
}

void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			//Р“Р‰Р“С”Р“вЂ Р“Рѓ
	{
		lcddev.dir=0;	//Р“Р‰Р“С”Р“вЂ Р“Рѓ
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
			if(lcddev.id==0X6804||lcddev.id==0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00;
			lcddev.width=480;
			lcddev.height=800;
		}else
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=R32;
			lcddev.setycmd=R33;
		}
	}else 				//Р’С”Р“РЋР“вЂ Р“Рѓ
	{
		lcddev.dir=1;	//Р’С”Р“РЋР“вЂ Р“Рѓ
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
		}else if(lcddev.id==0X6804)
		{
 			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2B;
			lcddev.setycmd=0X2A;
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00;
			lcddev.width=800;
			lcddev.height=480;
		}else
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=R33;
			lcddev.setycmd=R32;
		}
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.width=480;
			lcddev.height=320;
		}
	}
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Р“вЂћР’В¬Р“в‚¬Р“РЏР“вЂ°Р’РЃР“С“Р“РЃР’В·Р’Р…Р“РЏР“Р†
}

void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint8_t hsareg,heareg,vsareg,veareg;
	uint16_t hsaval,heaval,vsaval,veaval;
	width=sx+width-1;
	height=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804)//6804Р’С”Р“РЋР“вЂ Р“РѓР’Р†Р’В»Р“вЂ“Р’В§Р’С–Р“вЂ“
	{
		LCD_WR_REG(lcddev.setxcmd);
		LCD_WR_DATA(sx>>8);
		LCD_WR_DATA(sx&0XFF);
		LCD_WR_DATA(width>>8);
		LCD_WR_DATA(width&0XFF);
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(sy>>8);
		LCD_WR_DATA(sy&0XFF);
		LCD_WR_DATA(height>>8);
		LCD_WR_DATA(height&0XFF);
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(sx>>8);
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(sx&0XFF);
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(width>>8);
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(width&0XFF);
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(sy>>8);
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(sy&0XFF);
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(height>>8);
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(height&0XFF);
	}else	//Р“вЂ Р“В¤Р“вЂ№Р“В»Р“вЂЎР“Р…Р’В¶Р’Р‡IC
	{
		if(lcddev.dir==1)//Р’С”Р“РЋР“вЂ Р“Рѓ
		{
			//Р’Т‘Р’В°Р’С—Р“С™Р“вЂ“Р’Вµ
			hsaval=sy;
			heaval=height;
			vsaval=lcddev.width-width-1;
			veaval=lcddev.width-sx-1;
		}else
		{
			hsaval=sx;
			heaval=width;
			vsaval=sy;
			veaval=height;
		}
		hsareg=0X50;heareg=0X51;//Р“вЂ№Р’В®Р“вЂ Р’Р…Р’В·Р’Р…Р“РЏР“Р†Р’Т‘Р’В°Р’С—Р“С™Р’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·
		vsareg=0X52;veareg=0X53;//Р’Т‘Р’в„–Р“вЂ“Р’В±Р’В·Р’Р…Р“РЏР“Р†Р’Т‘Р’В°Р’С—Р“С™Р’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·
		//Р“вЂ°Р“РЃР“вЂ“Р“С“Р’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·Р“вЂ“Р’Вµ
		LCD_WriteReg(hsareg,hsaval);
		LCD_WriteReg(heareg,heaval);
		LCD_WriteReg(vsareg,vsaval);
		LCD_WriteReg(veareg,veaval);
		LCD_SetCursor(sx,sy);	//Р“вЂ°Р“РЃР“вЂ“Р“С“Р’в„–Р“СћР’В±Р“Р„Р“Р‹Р’В»Р“вЂ“Р“С“
	}
}

void LCD_Init(void)
{
	uint32_t i=0;

  GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//Р“Р‰Р’в„–Р“вЂћР“СљIOР“Р‰Р’В±Р“вЂ“Р“вЂњ
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//Р“Р‰Р’в„–Р“вЂћР“СљFSMCР“Р‰Р’В±Р“вЂ“Р“вЂњ


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PB1 Р“РЊР“вЂ Р“РЊР“В¬Р“Р‰Р“В¤Р’С–Р“В¶,Р’С—Р“пїЅР“вЂ“Р“вЂ Р’В±Р’С–Р’в„–Р“Сћ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//Р“вЂ Р“вЂўР“РЊР’РЃР“Р‰Р“В¤Р’С–Р“В¶Р“вЂћР’Р€Р“Р‰Р’Р…
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Р“РЊР“вЂ Р“РЊР“В¬Р“Р‰Р“В¤Р’С–Р“В¶
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//Р“вЂ°Р“РЏР“Р‚Р’В­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//Р’С–Р“ВµР“Р‰Р’С�Р’В»Р’Р‡ //PB15 Р“РЊР“вЂ Р“РЊР“В¬Р“Р‰Р“В¤Р’С–Р“В¶,Р’С—Р“пїЅР“вЂ“Р“вЂ Р’В±Р’С–Р’в„–Р“Сћ

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8
																|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PD0,1,4,5,8,9,10,14,15 AF OUT
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//Р’С‘Р’Т‘Р“вЂњР“С“Р“Р‰Р“В¤Р’С–Р“В¶
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Р“РЊР“вЂ Р“РЊР“В¬Р“Р‰Р“В¤Р’С–Р“В¶
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//Р“вЂ°Р“РЏР“Р‚Р’В­
  GPIO_Init(GPIOD, &GPIO_InitStructure);//Р’С–Р“ВµР“Р‰Р’С�Р’В»Р’Р‡

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12
																|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PE7~15,AF OUT
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//Р’С‘Р’Т‘Р“вЂњР“С“Р“Р‰Р“В¤Р’С–Р“В¶
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Р“РЊР“вЂ Р“РЊР“В¬Р“Р‰Р“В¤Р’С–Р“В¶
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//Р“вЂ°Р“РЏР“Р‚Р’В­
  GPIO_Init(GPIOE, &GPIO_InitStructure);//Р’С–Р“ВµР“Р‰Р’С�Р’В»Р’Р‡

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC); //Р’С”Р“РЊZET6Р“С’Р’С•Р“вЂ Р’В¬Р’Р†Р“В®Р“вЂ™Р“В¬
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);//Р’С”Р“РЊZET6Р“С’Р’С•Р“вЂ Р’В¬Р’Р†Р“В®Р“вЂ™Р“В¬
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//

  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12

  readWriteTiming.FSMC_AddressSetupTime = 0XF;	 //Р’ВµР“пїЅР“вЂ“Р’В·Р’Р…Р’РЃР“РѓР’СћР“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDSETР’Р€Р’В©Р“Р‹Р’Р„16Р’С‘Р“В¶HCLK 1/168M=6ns*16=96ns
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //Р’ВµР“пїЅР“вЂ“Р’В·Р’В±Р’Р€Р’С–Р“вЂ“Р“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDHLDР’Р€Р’В©Р“вЂћР’Р€Р“Р‰Р’Р…AР“Р‹Р’Т‘Р“вЂњР“С“Р’ВµР’Р…
  readWriteTiming.FSMC_DataSetupTime = 60;			//Р“Р‰Р“Р…Р’С•Р“СњР’В±Р’Р€Р’Т‘Р“В¦Р“Р‰Р’В±Р’С�Р“В¤Р“Р‹Р’Р„60Р’С‘Р“В¶HCLK	=6*60=360ns
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //Р“вЂћР’Р€Р“Р‰Р’Р…A


	writeTiming.FSMC_AddressSetupTime =9;	      //Р’ВµР“пїЅР“вЂ“Р’В·Р’Р…Р’РЃР“РѓР’СћР“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDSETР’Р€Р’В©Р“Р‹Р’Р„9Р’С‘Р“В¶HCLK =54ns
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //Р’ВµР“пїЅР“вЂ“Р’В·Р’В±Р’Р€Р’С–Р“вЂ“Р“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃA
  writeTiming.FSMC_DataSetupTime = 8;		 //Р“Р‰Р“Р…Р’С•Р“СњР’В±Р’Р€Р’Т‘Р“В¦Р“Р‰Р’В±Р’С�Р“В¤Р“Р‹Р’Р„6ns*9Р’С‘Р“В¶HCLK=54ns
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //Р“вЂћР’Р€Р“Р‰Р’Р…A


  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  Р“вЂўР“СћР“Р‚Р“Р‡Р“Р‹Р“вЂ™Р“С“Р“вЂЎР“Р‰Р’в„–Р“вЂњР“С“NE1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // Р’Р†Р’В»Р’С‘Р’Т‘Р“вЂњР“С“Р“Р‰Р“Р…Р’С•Р“СњР’ВµР“пїЅР“вЂ“Р’В·
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//Р’Т‘Р“В¦Р’Т‘Р’СћР“вЂ Р“В·Р“Р‰Р“Р…Р’С•Р“СњР’С—Р“В­Р’В¶Р“в‚¬Р“Р‹Р’Р„16bit
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  Р’Т‘Р“В¦Р’Т‘Р’СћР“вЂ Р“В·Р“С’Р’Т‘Р“Р‰Р’в„–Р“вЂћР“Сљ
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // Р’В¶Р“РѓР“С’Р’Т‘Р“Р‰Р’в„–Р“вЂњР“С“Р’Р†Р’В»Р“РЊР’В¬Р’ВµР“вЂћР“Р‰Р’В±Р“С’Р“Р†
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //Р’В¶Р“РѓР“С’Р’Т‘Р“Р‰Р’В±Р“С’Р“Р†
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //Р“С’Р’Т‘Р“Р‰Р’В±Р“С’Р“Р†

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //Р’С–Р“ВµР“Р‰Р’С�Р’В»Р’Р‡FSMCР“вЂ¦Р“В¤Р“вЂ“Р“С“

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // Р“Р‰Р’в„–Р“вЂћР“СљBANK1

  _delay_ms(50); // delay 50 ms
 	LCD_WriteReg(0x0000,0x0001);
 	_delay_ms(50); // delay 50 ms
	lcddev.id = LCD_ReadReg(0x0000);
  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//Р’В¶Р“РѓР’ВµР’Р…IDР’Р†Р’В»Р“вЂўР“Р…Р“в‚¬Р’В·,Р“С’Р“вЂљР“вЂќР“В¶lcddev.id==0X9300Р“вЂ¦Р“С’Р’В¶Р“РЏР’Р€Р’В¬Р“вЂ™Р“Р†Р“Р‹Р’Р„9341Р“вЂќР“С™Р“Р‹Р’Т‘Р’В±Р’В»Р’С‘Р’Т‘Р“Р‹Р’В»Р’ВµР“вЂћР“вЂЎР“В©Р’С—Р“В¶Р“РЏР“вЂљР’В»Р“РЋР’В±Р’В»Р’В¶Р“РѓР’С–Р“вЂ°9300
	{
 		//Р’С–Р’СћР“Р‰Р“вЂќ9341 IDР’ВµР“вЂћР’В¶Р“РѓР“в‚¬Р’РЋ
		LCD_WR_REG(0XD3);
		lcddev.id=LCD_RD_DATA();	//dummy read
 		lcddev.id=LCD_RD_DATA(); 	//Р’В¶Р“РѓР’ВµР’Р…0X00
  		lcddev.id=LCD_RD_DATA();   	//Р’В¶Р“РѓР“в‚¬Р’РЋ93
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//Р’В¶Р“РѓР“в‚¬Р’РЋ41
 		if(lcddev.id!=0X9341)		//Р’В·Р“вЂЎ9341,Р’С–Р’СћР“Р‰Р“вЂќР“Р‰Р“вЂЎР’Р†Р’В»Р“Р‰Р“вЂЎ6804
		{
 			LCD_WR_REG(0XBF);
			lcddev.id=LCD_RD_DATA();//dummy read
	 		lcddev.id=LCD_RD_DATA();//Р’В¶Р“РѓР’В»Р“пїЅ0X01
	 		lcddev.id=LCD_RD_DATA();//Р’В¶Р“РѓР’В»Р“пїЅ0XD0
	  		lcddev.id=LCD_RD_DATA();//Р“вЂўР“СћР“Р‚Р“Р‡Р’В¶Р“РѓР’В»Р“пїЅ0X68
			lcddev.id<<=8;
	  		lcddev.id|=LCD_RD_DATA();//Р“вЂўР“СћР“Р‚Р“Р‡Р’В¶Р“РѓР’В»Р“пїЅ0X04
			if(lcddev.id!=0X6804)	//Р“вЂ™Р’Р†Р’Р†Р’В»Р“Р‰Р“вЂЎ6804,Р’С–Р’СћР“Р‰Р“вЂќР’С—Р’Т‘Р’С—Р’Т‘Р“Р‰Р“вЂЎР’Р†Р’В»Р“Р‰Р“вЂЎNT35310
			{
				LCD_WR_REG(0XD4);
				lcddev.id=LCD_RD_DATA();	//dummy read
				lcddev.id=LCD_RD_DATA();	//Р’В¶Р“РѓР’В»Р“пїЅ0X01
				lcddev.id=LCD_RD_DATA();	//Р’В¶Р“РѓР’В»Р“пїЅ0X53
				lcddev.id<<=8;
				lcddev.id|=LCD_RD_DATA();	//Р“вЂўР“СћР“Р‚Р“Р‡Р’В¶Р“РѓР’В»Р“пїЅ0X10
				if(lcddev.id!=0X5310)		//Р“вЂ™Р’Р†Р’Р†Р’В»Р“Р‰Р“вЂЎNT35310,Р’С–Р’СћР“Р‰Р“вЂќР’С—Р’Т‘Р’С—Р’Т‘Р“Р‰Р“вЂЎР’Р†Р’В»Р“Р‰Р“вЂЎNT35510
				{
					LCD_WR_REG(0XDA00);
					lcddev.id=LCD_RD_DATA();//Р’В¶Р“РѓР’В»Р“пїЅ0X00
					LCD_WR_REG(0XDB00);
					lcddev.id=LCD_RD_DATA();//Р’В¶Р“РѓР’В»Р“пїЅ0X80
					lcddev.id<<=8;
					LCD_WR_REG(0XDC00);
					lcddev.id|=LCD_RD_DATA();//Р’В¶Р“РѓР’В»Р“пїЅ0X00
					if(lcddev.id==0x8000)lcddev.id=0x5510;//NT35510Р’В¶Р“РѓР’В»Р“пїЅР’ВµР“вЂћIDР“Р‰Р“вЂЎ8000H,Р“Р‹Р’Р„Р’В·Р’Р…Р’В±Р“Р€Р“вЂЎР“С‘Р’В·Р“вЂ“,Р“Р‹Р“вЂ™Р“С“Р“вЂЎР“вЂЎР’С—Р“вЂ“Р“вЂ Р“вЂ°Р“РЃР“вЂ“Р“С“Р“Р‹Р’Р„5510
				}
			}
 		}
	}
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)//Р“в‚¬Р“В§Р’в„–Р“В»Р“Р‰Р“вЂЎР“вЂўР“СћР“в‚¬Р“Р…Р’С‘Р“В¶IC,Р“вЂќР“Р†Р“вЂ°Р“РЃР“вЂ“Р“С“WRР“Р‰Р’В±Р“С’Р“Р†Р“Р‹Р’Р„Р“вЂ”Р“В®Р’С—Р“В¬
	{
		//Р“вЂ“Р“пїЅР“С’Р“вЂљР“вЂ¦Р“В¤Р“вЂ“Р“С“Р“С’Р’Т‘Р“Р‰Р’В±Р“С’Р“Р†Р’С—Р“пїЅР“вЂ“Р“вЂ Р’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·Р’ВµР“вЂћР“Р‰Р’В±Р“С’Р“Р†
		FSMC_Bank1E->BWTR[6]&=~(0XF<<0);//Р’ВµР“пїЅР“вЂ“Р’В·Р’Р…Р’РЃР“РѓР’СћР“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDSETР’Р€Р’В©Р“вЂЎР“ТђР“РѓР“Р€
		FSMC_Bank1E->BWTR[6]&=~(0XF<<8);//Р“Р‰Р“Р…Р’С•Р“СњР’В±Р’Р€Р’Т‘Р“В¦Р“Р‰Р’В±Р’С�Р“В¤Р“вЂЎР“ТђР“РѓР“Р€
		FSMC_Bank1E->BWTR[6]|=3<<0;		//Р’ВµР“пїЅР“вЂ“Р’В·Р’Р…Р’РЃР“РѓР’СћР“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDSETР’Р€Р’В©Р“Р‹Р’Р„3Р’С‘Р“В¶HCLK =18ns
		FSMC_Bank1E->BWTR[6]|=2<<8; 	//Р“Р‰Р“Р…Р’С•Р“СњР’В±Р’Р€Р’Т‘Р“В¦Р“Р‰Р’В±Р’С�Р“В¤Р“Р‹Р’Р„6ns*3Р’С‘Р“В¶HCLK=18ns
	}else if(lcddev.id==0X6804||lcddev.id==0XC505)	//6804/C505Р“вЂ№Р“в„ўР’В¶Р“в‚¬Р“вЂ°Р“РЏР’Р†Р’В»Р“в‚¬Р’Тђ,Р’ВµР“С“Р’Р…Р’ВµР’ВµР“РЊ
	{
		//Р“вЂ“Р“пїЅР“С’Р“вЂљР“вЂ¦Р“В¤Р“вЂ“Р“С“Р“С’Р’Т‘Р“Р‰Р’В±Р“С’Р“Р†Р’С—Р“пїЅР“вЂ“Р“вЂ Р’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В·Р’ВµР“вЂћР“Р‰Р’В±Р“С’Р“Р†
		FSMC_Bank1E->BWTR[6]&=~(0XF<<0);//Р’ВµР“пїЅР“вЂ“Р’В·Р’Р…Р’РЃР“РѓР’СћР“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDSETР’Р€Р’В©Р“вЂЎР“ТђР“РѓР“Р€
		FSMC_Bank1E->BWTR[6]&=~(0XF<<8);//Р“Р‰Р“Р…Р’С•Р“СњР’В±Р’Р€Р’Т‘Р“В¦Р“Р‰Р’В±Р’С�Р“В¤Р“вЂЎР“ТђР“РѓР“Р€
		FSMC_Bank1E->BWTR[6]|=10<<0;	//Р’ВµР“пїЅР“вЂ“Р’В·Р’Р…Р’РЃР“РѓР’СћР“Р‰Р’В±Р’С�Р“В¤Р’Р€Р’РЃADDSETР’Р€Р’В©Р“Р‹Р’Р„10Р’С‘Р“В¶HCLK =60ns
		FSMC_Bank1E->BWTR[6]|=12<<8; 	//Р“Р‰Р“Р…Р’С•Р“СњР’В±Р’Р€Р’Т‘Р“В¦Р“Р‰Р’В±Р’С�Р“В¤Р“Р‹Р’Р„6ns*13Р’С‘Р“В¶HCLK=96ns
	}
	if(lcddev.id==0X9341)	//9341Р’С–Р“ВµР“Р‰Р’С�Р’В»Р’Р‡
	{
		LCD_WR_REG(0xCF);
		LCD_WR_DATA(0x00);
		i++;
		LCD_WR_DATA(0xC1);
		LCD_WR_DATA(0X30);
		LCD_WR_REG(0xED);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0X12);
		LCD_WR_DATA(0X81);
		LCD_WR_REG(0xE8);
		LCD_WR_DATA(0x85);
		LCD_WR_DATA(0x10);
		LCD_WR_DATA(0x7A);
		LCD_WR_REG(0xCB);
		LCD_WR_DATA(0x39);
		LCD_WR_DATA(0x2C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x34);
		LCD_WR_DATA(0x02);
		LCD_WR_REG(0xF7);
		LCD_WR_DATA(0x20);
		LCD_WR_REG(0xEA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xC0);    //Power control
		LCD_WR_DATA(0x1B);   //VRH[5:0]
		LCD_WR_REG(0xC1);    //Power control
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0]
		LCD_WR_REG(0xC5);    //VCM control
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2
		LCD_WR_DATA(0XB7);
		LCD_WR_REG(0x36);    // Memory Access Control
		LCD_WR_DATA(0x48);
		LCD_WR_REG(0x3A);
		LCD_WR_DATA(0x55);
		LCD_WR_REG(0xB1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1A);
		LCD_WR_REG(0xB6);    // Display Function Control
		LCD_WR_DATA(0x0A);
		LCD_WR_DATA(0xA2);
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0x26);    //Gamma curve selected
		LCD_WR_DATA(0x01);
		LCD_WR_REG(0xE0);    //Set Gamma
		LCD_WR_DATA(0x0F);
		LCD_WR_DATA(0x2A);
		LCD_WR_DATA(0x28);
		LCD_WR_DATA(0x08);
		LCD_WR_DATA(0x0E);
		LCD_WR_DATA(0x08);
		LCD_WR_DATA(0x54);
		LCD_WR_DATA(0XA9);
		LCD_WR_DATA(0x43);
		LCD_WR_DATA(0x0A);
		LCD_WR_DATA(0x0F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0XE1);    //Set Gamma
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x15);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x07);
		LCD_WR_DATA(0x11);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x2B);
		LCD_WR_DATA(0x56);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x10);
		LCD_WR_DATA(0x0F);
		LCD_WR_DATA(0x3F);
		LCD_WR_DATA(0x3F);
		LCD_WR_DATA(0x0F);
		LCD_WR_REG(0x2B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);
		LCD_WR_REG(0x11); //Exit Sleep
		_delay_us(120);
		LCD_WR_REG(0x29); //display on
	}else if(lcddev.id==0x6804) //6804Р’С–Р“ВµР“Р‰Р’С�Р’В»Р’Р‡
	{
		LCD_WR_REG(0X11);
		_delay_ms(20);
		LCD_WR_REG(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
		LCD_WR_DATA(0X07);
		LCD_WR_DATA(0X42);
		LCD_WR_DATA(0X1D);
		LCD_WR_REG(0XD1);//VCOMH VCOM_AC amplitude setting
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X1a);
		LCD_WR_DATA(0X09);
		LCD_WR_REG(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0X22);
		LCD_WR_REG(0XC0);//REV SM GS
		LCD_WR_DATA(0X10);
		LCD_WR_DATA(0X3B);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X02);
		LCD_WR_DATA(0X11);

		LCD_WR_REG(0XC5);// Frame rate setting = 72HZ  when setting 0x03
		LCD_WR_DATA(0X03);

		LCD_WR_REG(0XC8);//Gamma setting
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X25);
		LCD_WR_DATA(0X21);
		LCD_WR_DATA(0X05);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X0a);
		LCD_WR_DATA(0X65);
		LCD_WR_DATA(0X25);
		LCD_WR_DATA(0X77);
		LCD_WR_DATA(0X50);
		LCD_WR_DATA(0X0f);
		LCD_WR_DATA(0X00);

   		LCD_WR_REG(0XF8);
		LCD_WR_DATA(0X01);

 		LCD_WR_REG(0XFE);
 		LCD_WR_DATA(0X00);
 		LCD_WR_DATA(0X02);

		LCD_WR_REG(0X20);//Exit invert mode

		LCD_WR_REG(0X36);
		LCD_WR_DATA(0X08);//Р“вЂќР’В­Р“Р‚Р’Т‘Р“Р‰Р“вЂЎa

		LCD_WR_REG(0X3A);
		LCD_WR_DATA(0X55);//16Р“Р‹Р’В»Р“вЂћР’Р€Р“Р‰Р’Р…
		LCD_WR_REG(0X2B);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0X3F);

		LCD_WR_REG(0X2A);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0XDF);
		_delay_ms(120);
		LCD_WR_REG(0X29);
 	}else if(lcddev.id==0x5310)
	{
		LCD_WR_REG(0xED);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0xFE);

		LCD_WR_REG(0xEE);
		LCD_WR_DATA(0xDE);
		LCD_WR_DATA(0x21);

		LCD_WR_REG(0xF1);
		LCD_WR_DATA(0x01);
		LCD_WR_REG(0xDF);
		LCD_WR_DATA(0x10);

		//VCOMvoltage//
		LCD_WR_REG(0xC4);
		LCD_WR_DATA(0x8F);	  //5f

		LCD_WR_REG(0xC6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE2);
		LCD_WR_DATA(0xE2);
		LCD_WR_DATA(0xE2);
		LCD_WR_REG(0xBF);
		LCD_WR_DATA(0xAA);

		LCD_WR_REG(0xB0);
		LCD_WR_DATA(0x0D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x0D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x11);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x19);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x21);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB1);
		LCD_WR_DATA(0x80);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x96);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB4);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x96);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA1);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB5);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x04);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5E);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xAC);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDC);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x70);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x90);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xEB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDC);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xBA);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC1);
		LCD_WR_DATA(0x20);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x54);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xFF);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC2);
		LCD_WR_DATA(0x0A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x04);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC3);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x39);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x37);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x26);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x23);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x26);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x23);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC4);
		LCD_WR_DATA(0x62);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x84);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x50);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x0C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x95);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE6);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC5);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x65);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x76);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC6);
		LCD_WR_DATA(0x20);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE0);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x21);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x46);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x52);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x7A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE1);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x22);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x45);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x52);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x7A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE2);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x0B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x34);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x4F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x61);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x79);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x97);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xE3);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x33);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x50);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x62);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x78);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x97);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD5);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE4);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x4B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x74);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x84);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x93);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBE);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xE5);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x4B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x74);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x84);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x93);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBE);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDC);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE6);
		LCD_WR_DATA(0x11);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x34);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x56);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x76);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x77);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x45);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x43);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE7);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x76);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x67);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x67);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x87);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x77);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x56);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x23);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x33);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x45);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x87);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x77);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xAA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE9);
		LCD_WR_DATA(0xAA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0x00);
		LCD_WR_DATA(0xAA);

		LCD_WR_REG(0xCF);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xF0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x50);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xF9);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x10);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0x3A);
		LCD_WR_DATA(0x55);	//66

		LCD_WR_REG(0x11);
		_delay_ms(100);
		LCD_WR_REG(0x29);
		LCD_WR_REG(0x35);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0x51);
		LCD_WR_DATA(0xFF);
		LCD_WR_REG(0x53);
		LCD_WR_DATA(0x2C);
		LCD_WR_REG(0x55);
		LCD_WR_DATA(0x82);
		LCD_WR_REG(0x2c);
	}else if(lcddev.id==0x5510)
	{
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x01);
		//AVDD Set AVDD 5.2V
		LCD_WriteReg(0xB000,0x0D);
		LCD_WriteReg(0xB001,0x0D);
		LCD_WriteReg(0xB002,0x0D);
		//AVDD ratio
		LCD_WriteReg(0xB600,0x34);
		LCD_WriteReg(0xB601,0x34);
		LCD_WriteReg(0xB602,0x34);
		//AVEE -5.2V
		LCD_WriteReg(0xB100,0x0D);
		LCD_WriteReg(0xB101,0x0D);
		LCD_WriteReg(0xB102,0x0D);
		//AVEE ratio
		LCD_WriteReg(0xB700,0x34);
		LCD_WriteReg(0xB701,0x34);
		LCD_WriteReg(0xB702,0x34);
		//VCL -2.5V
		LCD_WriteReg(0xB200,0x00);
		LCD_WriteReg(0xB201,0x00);
		LCD_WriteReg(0xB202,0x00);
		//VCL ratio
		LCD_WriteReg(0xB800,0x24);
		LCD_WriteReg(0xB801,0x24);
		LCD_WriteReg(0xB802,0x24);
		//VGH 15V (Free pump)
		LCD_WriteReg(0xBF00,0x01);
		LCD_WriteReg(0xB300,0x0F);
		LCD_WriteReg(0xB301,0x0F);
		LCD_WriteReg(0xB302,0x0F);
		//VGH ratio
		LCD_WriteReg(0xB900,0x34);
		LCD_WriteReg(0xB901,0x34);
		LCD_WriteReg(0xB902,0x34);
		//VGL_REG -10V
		LCD_WriteReg(0xB500,0x08);
		LCD_WriteReg(0xB501,0x08);
		LCD_WriteReg(0xB502,0x08);
		LCD_WriteReg(0xC200,0x03);
		//VGLX ratio
		LCD_WriteReg(0xBA00,0x24);
		LCD_WriteReg(0xBA01,0x24);
		LCD_WriteReg(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		LCD_WriteReg(0xBC00,0x00);
		LCD_WriteReg(0xBC01,0x78);
		LCD_WriteReg(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		LCD_WriteReg(0xBD00,0x00);
		LCD_WriteReg(0xBD01,0x78);
		LCD_WriteReg(0xBD02,0x00);
		//VCOM
		LCD_WriteReg(0xBE00,0x00);
		LCD_WriteReg(0xBE01,0x64);
		//Gamma Setting
		LCD_WriteReg(0xD100,0x00);
		LCD_WriteReg(0xD101,0x33);
		LCD_WriteReg(0xD102,0x00);
		LCD_WriteReg(0xD103,0x34);
		LCD_WriteReg(0xD104,0x00);
		LCD_WriteReg(0xD105,0x3A);
		LCD_WriteReg(0xD106,0x00);
		LCD_WriteReg(0xD107,0x4A);
		LCD_WriteReg(0xD108,0x00);
		LCD_WriteReg(0xD109,0x5C);
		LCD_WriteReg(0xD10A,0x00);
		LCD_WriteReg(0xD10B,0x81);
		LCD_WriteReg(0xD10C,0x00);
		LCD_WriteReg(0xD10D,0xA6);
		LCD_WriteReg(0xD10E,0x00);
		LCD_WriteReg(0xD10F,0xE5);
		LCD_WriteReg(0xD110,0x01);
		LCD_WriteReg(0xD111,0x13);
		LCD_WriteReg(0xD112,0x01);
		LCD_WriteReg(0xD113,0x54);
		LCD_WriteReg(0xD114,0x01);
		LCD_WriteReg(0xD115,0x82);
		LCD_WriteReg(0xD116,0x01);
		LCD_WriteReg(0xD117,0xCA);
		LCD_WriteReg(0xD118,0x02);
		LCD_WriteReg(0xD119,0x00);
		LCD_WriteReg(0xD11A,0x02);
		LCD_WriteReg(0xD11B,0x01);
		LCD_WriteReg(0xD11C,0x02);
		LCD_WriteReg(0xD11D,0x34);
		LCD_WriteReg(0xD11E,0x02);
		LCD_WriteReg(0xD11F,0x67);
		LCD_WriteReg(0xD120,0x02);
		LCD_WriteReg(0xD121,0x84);
		LCD_WriteReg(0xD122,0x02);
		LCD_WriteReg(0xD123,0xA4);
		LCD_WriteReg(0xD124,0x02);
		LCD_WriteReg(0xD125,0xB7);
		LCD_WriteReg(0xD126,0x02);
		LCD_WriteReg(0xD127,0xCF);
		LCD_WriteReg(0xD128,0x02);
		LCD_WriteReg(0xD129,0xDE);
		LCD_WriteReg(0xD12A,0x02);
		LCD_WriteReg(0xD12B,0xF2);
		LCD_WriteReg(0xD12C,0x02);
		LCD_WriteReg(0xD12D,0xFE);
		LCD_WriteReg(0xD12E,0x03);
		LCD_WriteReg(0xD12F,0x10);
		LCD_WriteReg(0xD130,0x03);
		LCD_WriteReg(0xD131,0x33);
		LCD_WriteReg(0xD132,0x03);
		LCD_WriteReg(0xD133,0x6D);
		LCD_WriteReg(0xD200,0x00);
		LCD_WriteReg(0xD201,0x33);
		LCD_WriteReg(0xD202,0x00);
		LCD_WriteReg(0xD203,0x34);
		LCD_WriteReg(0xD204,0x00);
		LCD_WriteReg(0xD205,0x3A);
		LCD_WriteReg(0xD206,0x00);
		LCD_WriteReg(0xD207,0x4A);
		LCD_WriteReg(0xD208,0x00);
		LCD_WriteReg(0xD209,0x5C);
		LCD_WriteReg(0xD20A,0x00);

		LCD_WriteReg(0xD20B,0x81);
		LCD_WriteReg(0xD20C,0x00);
		LCD_WriteReg(0xD20D,0xA6);
		LCD_WriteReg(0xD20E,0x00);
		LCD_WriteReg(0xD20F,0xE5);
		LCD_WriteReg(0xD210,0x01);
		LCD_WriteReg(0xD211,0x13);
		LCD_WriteReg(0xD212,0x01);
		LCD_WriteReg(0xD213,0x54);
		LCD_WriteReg(0xD214,0x01);
		LCD_WriteReg(0xD215,0x82);
		LCD_WriteReg(0xD216,0x01);
		LCD_WriteReg(0xD217,0xCA);
		LCD_WriteReg(0xD218,0x02);
		LCD_WriteReg(0xD219,0x00);
		LCD_WriteReg(0xD21A,0x02);
		LCD_WriteReg(0xD21B,0x01);
		LCD_WriteReg(0xD21C,0x02);
		LCD_WriteReg(0xD21D,0x34);
		LCD_WriteReg(0xD21E,0x02);
		LCD_WriteReg(0xD21F,0x67);
		LCD_WriteReg(0xD220,0x02);
		LCD_WriteReg(0xD221,0x84);
		LCD_WriteReg(0xD222,0x02);
		LCD_WriteReg(0xD223,0xA4);
		LCD_WriteReg(0xD224,0x02);
		LCD_WriteReg(0xD225,0xB7);
		LCD_WriteReg(0xD226,0x02);
		LCD_WriteReg(0xD227,0xCF);
		LCD_WriteReg(0xD228,0x02);
		LCD_WriteReg(0xD229,0xDE);
		LCD_WriteReg(0xD22A,0x02);
		LCD_WriteReg(0xD22B,0xF2);
		LCD_WriteReg(0xD22C,0x02);
		LCD_WriteReg(0xD22D,0xFE);
		LCD_WriteReg(0xD22E,0x03);
		LCD_WriteReg(0xD22F,0x10);
		LCD_WriteReg(0xD230,0x03);
		LCD_WriteReg(0xD231,0x33);
		LCD_WriteReg(0xD232,0x03);
		LCD_WriteReg(0xD233,0x6D);
		LCD_WriteReg(0xD300,0x00);
		LCD_WriteReg(0xD301,0x33);
		LCD_WriteReg(0xD302,0x00);
		LCD_WriteReg(0xD303,0x34);
		LCD_WriteReg(0xD304,0x00);
		LCD_WriteReg(0xD305,0x3A);
		LCD_WriteReg(0xD306,0x00);
		LCD_WriteReg(0xD307,0x4A);
		LCD_WriteReg(0xD308,0x00);
		LCD_WriteReg(0xD309,0x5C);
		LCD_WriteReg(0xD30A,0x00);

		LCD_WriteReg(0xD30B,0x81);
		LCD_WriteReg(0xD30C,0x00);
		LCD_WriteReg(0xD30D,0xA6);
		LCD_WriteReg(0xD30E,0x00);
		LCD_WriteReg(0xD30F,0xE5);
		LCD_WriteReg(0xD310,0x01);
		LCD_WriteReg(0xD311,0x13);
		LCD_WriteReg(0xD312,0x01);
		LCD_WriteReg(0xD313,0x54);
		LCD_WriteReg(0xD314,0x01);
		LCD_WriteReg(0xD315,0x82);
		LCD_WriteReg(0xD316,0x01);
		LCD_WriteReg(0xD317,0xCA);
		LCD_WriteReg(0xD318,0x02);
		LCD_WriteReg(0xD319,0x00);
		LCD_WriteReg(0xD31A,0x02);
		LCD_WriteReg(0xD31B,0x01);
		LCD_WriteReg(0xD31C,0x02);
		LCD_WriteReg(0xD31D,0x34);
		LCD_WriteReg(0xD31E,0x02);
		LCD_WriteReg(0xD31F,0x67);
		LCD_WriteReg(0xD320,0x02);
		LCD_WriteReg(0xD321,0x84);
		LCD_WriteReg(0xD322,0x02);
		LCD_WriteReg(0xD323,0xA4);
		LCD_WriteReg(0xD324,0x02);
		LCD_WriteReg(0xD325,0xB7);
		LCD_WriteReg(0xD326,0x02);
		LCD_WriteReg(0xD327,0xCF);
		LCD_WriteReg(0xD328,0x02);
		LCD_WriteReg(0xD329,0xDE);
		LCD_WriteReg(0xD32A,0x02);
		LCD_WriteReg(0xD32B,0xF2);
		LCD_WriteReg(0xD32C,0x02);
		LCD_WriteReg(0xD32D,0xFE);
		LCD_WriteReg(0xD32E,0x03);
		LCD_WriteReg(0xD32F,0x10);
		LCD_WriteReg(0xD330,0x03);
		LCD_WriteReg(0xD331,0x33);
		LCD_WriteReg(0xD332,0x03);
		LCD_WriteReg(0xD333,0x6D);
		LCD_WriteReg(0xD400,0x00);
		LCD_WriteReg(0xD401,0x33);
		LCD_WriteReg(0xD402,0x00);
		LCD_WriteReg(0xD403,0x34);
		LCD_WriteReg(0xD404,0x00);
		LCD_WriteReg(0xD405,0x3A);
		LCD_WriteReg(0xD406,0x00);
		LCD_WriteReg(0xD407,0x4A);
		LCD_WriteReg(0xD408,0x00);
		LCD_WriteReg(0xD409,0x5C);
		LCD_WriteReg(0xD40A,0x00);
		LCD_WriteReg(0xD40B,0x81);

		LCD_WriteReg(0xD40C,0x00);
		LCD_WriteReg(0xD40D,0xA6);
		LCD_WriteReg(0xD40E,0x00);
		LCD_WriteReg(0xD40F,0xE5);
		LCD_WriteReg(0xD410,0x01);
		LCD_WriteReg(0xD411,0x13);
		LCD_WriteReg(0xD412,0x01);
		LCD_WriteReg(0xD413,0x54);
		LCD_WriteReg(0xD414,0x01);
		LCD_WriteReg(0xD415,0x82);
		LCD_WriteReg(0xD416,0x01);
		LCD_WriteReg(0xD417,0xCA);
		LCD_WriteReg(0xD418,0x02);
		LCD_WriteReg(0xD419,0x00);
		LCD_WriteReg(0xD41A,0x02);
		LCD_WriteReg(0xD41B,0x01);
		LCD_WriteReg(0xD41C,0x02);
		LCD_WriteReg(0xD41D,0x34);
		LCD_WriteReg(0xD41E,0x02);
		LCD_WriteReg(0xD41F,0x67);
		LCD_WriteReg(0xD420,0x02);
		LCD_WriteReg(0xD421,0x84);
		LCD_WriteReg(0xD422,0x02);
		LCD_WriteReg(0xD423,0xA4);
		LCD_WriteReg(0xD424,0x02);
		LCD_WriteReg(0xD425,0xB7);
		LCD_WriteReg(0xD426,0x02);
		LCD_WriteReg(0xD427,0xCF);
		LCD_WriteReg(0xD428,0x02);
		LCD_WriteReg(0xD429,0xDE);
		LCD_WriteReg(0xD42A,0x02);
		LCD_WriteReg(0xD42B,0xF2);
		LCD_WriteReg(0xD42C,0x02);
		LCD_WriteReg(0xD42D,0xFE);
		LCD_WriteReg(0xD42E,0x03);
		LCD_WriteReg(0xD42F,0x10);
		LCD_WriteReg(0xD430,0x03);
		LCD_WriteReg(0xD431,0x33);
		LCD_WriteReg(0xD432,0x03);
		LCD_WriteReg(0xD433,0x6D);
		LCD_WriteReg(0xD500,0x00);
		LCD_WriteReg(0xD501,0x33);
		LCD_WriteReg(0xD502,0x00);
		LCD_WriteReg(0xD503,0x34);
		LCD_WriteReg(0xD504,0x00);
		LCD_WriteReg(0xD505,0x3A);
		LCD_WriteReg(0xD506,0x00);
		LCD_WriteReg(0xD507,0x4A);
		LCD_WriteReg(0xD508,0x00);
		LCD_WriteReg(0xD509,0x5C);
		LCD_WriteReg(0xD50A,0x00);
		LCD_WriteReg(0xD50B,0x81);

		LCD_WriteReg(0xD50C,0x00);
		LCD_WriteReg(0xD50D,0xA6);
		LCD_WriteReg(0xD50E,0x00);
		LCD_WriteReg(0xD50F,0xE5);
		LCD_WriteReg(0xD510,0x01);
		LCD_WriteReg(0xD511,0x13);
		LCD_WriteReg(0xD512,0x01);
		LCD_WriteReg(0xD513,0x54);
		LCD_WriteReg(0xD514,0x01);
		LCD_WriteReg(0xD515,0x82);
		LCD_WriteReg(0xD516,0x01);
		LCD_WriteReg(0xD517,0xCA);
		LCD_WriteReg(0xD518,0x02);
		LCD_WriteReg(0xD519,0x00);
		LCD_WriteReg(0xD51A,0x02);
		LCD_WriteReg(0xD51B,0x01);
		LCD_WriteReg(0xD51C,0x02);
		LCD_WriteReg(0xD51D,0x34);
		LCD_WriteReg(0xD51E,0x02);
		LCD_WriteReg(0xD51F,0x67);
		LCD_WriteReg(0xD520,0x02);
		LCD_WriteReg(0xD521,0x84);
		LCD_WriteReg(0xD522,0x02);
		LCD_WriteReg(0xD523,0xA4);
		LCD_WriteReg(0xD524,0x02);
		LCD_WriteReg(0xD525,0xB7);
		LCD_WriteReg(0xD526,0x02);
		LCD_WriteReg(0xD527,0xCF);
		LCD_WriteReg(0xD528,0x02);
		LCD_WriteReg(0xD529,0xDE);
		LCD_WriteReg(0xD52A,0x02);
		LCD_WriteReg(0xD52B,0xF2);
		LCD_WriteReg(0xD52C,0x02);
		LCD_WriteReg(0xD52D,0xFE);
		LCD_WriteReg(0xD52E,0x03);
		LCD_WriteReg(0xD52F,0x10);
		LCD_WriteReg(0xD530,0x03);
		LCD_WriteReg(0xD531,0x33);
		LCD_WriteReg(0xD532,0x03);
		LCD_WriteReg(0xD533,0x6D);
		LCD_WriteReg(0xD600,0x00);
		LCD_WriteReg(0xD601,0x33);
		LCD_WriteReg(0xD602,0x00);
		LCD_WriteReg(0xD603,0x34);
		LCD_WriteReg(0xD604,0x00);
		LCD_WriteReg(0xD605,0x3A);
		LCD_WriteReg(0xD606,0x00);
		LCD_WriteReg(0xD607,0x4A);
		LCD_WriteReg(0xD608,0x00);
		LCD_WriteReg(0xD609,0x5C);
		LCD_WriteReg(0xD60A,0x00);
		LCD_WriteReg(0xD60B,0x81);

		LCD_WriteReg(0xD60C,0x00);
		LCD_WriteReg(0xD60D,0xA6);
		LCD_WriteReg(0xD60E,0x00);
		LCD_WriteReg(0xD60F,0xE5);
		LCD_WriteReg(0xD610,0x01);
		LCD_WriteReg(0xD611,0x13);
		LCD_WriteReg(0xD612,0x01);
		LCD_WriteReg(0xD613,0x54);
		LCD_WriteReg(0xD614,0x01);
		LCD_WriteReg(0xD615,0x82);
		LCD_WriteReg(0xD616,0x01);
		LCD_WriteReg(0xD617,0xCA);
		LCD_WriteReg(0xD618,0x02);
		LCD_WriteReg(0xD619,0x00);
		LCD_WriteReg(0xD61A,0x02);
		LCD_WriteReg(0xD61B,0x01);
		LCD_WriteReg(0xD61C,0x02);
		LCD_WriteReg(0xD61D,0x34);
		LCD_WriteReg(0xD61E,0x02);
		LCD_WriteReg(0xD61F,0x67);
		LCD_WriteReg(0xD620,0x02);
		LCD_WriteReg(0xD621,0x84);
		LCD_WriteReg(0xD622,0x02);
		LCD_WriteReg(0xD623,0xA4);
		LCD_WriteReg(0xD624,0x02);
		LCD_WriteReg(0xD625,0xB7);
		LCD_WriteReg(0xD626,0x02);
		LCD_WriteReg(0xD627,0xCF);
		LCD_WriteReg(0xD628,0x02);
		LCD_WriteReg(0xD629,0xDE);
		LCD_WriteReg(0xD62A,0x02);
		LCD_WriteReg(0xD62B,0xF2);
		LCD_WriteReg(0xD62C,0x02);
		LCD_WriteReg(0xD62D,0xFE);
		LCD_WriteReg(0xD62E,0x03);
		LCD_WriteReg(0xD62F,0x10);
		LCD_WriteReg(0xD630,0x03);
		LCD_WriteReg(0xD631,0x33);
		LCD_WriteReg(0xD632,0x03);
		LCD_WriteReg(0xD633,0x6D);
		//LV2 Page 0 enable
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x00);
		//Display control
		LCD_WriteReg(0xB100, 0xCC);
		LCD_WriteReg(0xB101, 0x00);
		//Source hold time
		LCD_WriteReg(0xB600,0x05);
		//Gate EQ control
		LCD_WriteReg(0xB700,0x70);
		LCD_WriteReg(0xB701,0x70);
		//Source EQ control (Mode 2)
		LCD_WriteReg(0xB800,0x01);
		LCD_WriteReg(0xB801,0x03);
		LCD_WriteReg(0xB802,0x03);
		LCD_WriteReg(0xB803,0x03);
		//Inversion mode (2-dot)
		LCD_WriteReg(0xBC00,0x02);
		LCD_WriteReg(0xBC01,0x00);
		LCD_WriteReg(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		LCD_WriteReg(0xC900,0xD0);
		LCD_WriteReg(0xC901,0x02);
		LCD_WriteReg(0xC902,0x50);
		LCD_WriteReg(0xC903,0x50);
		LCD_WriteReg(0xC904,0x50);
		LCD_WriteReg(0x3500,0x00);
		LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
		LCD_WR_REG(0x1100);
		_delay_ms(1);
		LCD_WR_REG(0x2900);
	}else if(lcddev.id==0x9325)//9325
	{
		LCD_WriteReg(0x00E5,0x78F0);
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
		LCD_WriteReg(0x0003,0x1030);
		LCD_WriteReg(0x0004,0x0000);
		LCD_WriteReg(0x0008,0x0202);
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000);
		LCD_WriteReg(0x000C,0x0000);
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		LCD_WriteReg(0x0007,0x0000);
		//vgh
		LCD_WriteReg(0x0010,0x1690);
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100);
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100);
		//vom H
		LCD_WriteReg(0x0029,0x0025);
		LCD_WriteReg(0x002B,0x000D);
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406);
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007);
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008);
		//ram
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013F);
		LCD_WriteReg(0x0060,0xA700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x006A,0x0000);
		//
		LCD_WriteReg(0x0080,0x0000);
		LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0082,0x0000);
		LCD_WriteReg(0x0083,0x0000);
		LCD_WriteReg(0x0084,0x0000);
		LCD_WriteReg(0x0085,0x0000);
		//
		LCD_WriteReg(0x0090,0x0010);
		LCD_WriteReg(0x0092,0x0600);

		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//
	}else if(lcddev.id==0x9328)//ILI9328   OK
	{
  		LCD_WriteReg(0x00EC,0x108F);// internal timeing
 		LCD_WriteReg(0x00EF,0x1234);// ADD
		//LCD_WriteReg(0x00e7,0x0010);
        //LCD_WriteReg(0x0000,0x0001);//Р’С—Р’Р„Р“вЂ Р“Т‘Р“вЂћР“С™Р’Р†Р’С—Р“Р‰Р’В±Р“вЂ“Р“вЂњ
        LCD_WriteReg(0x0001,0x0100);
        LCD_WriteReg(0x0002,0x0700);//Р’ВµР“В§Р“вЂќР’Т‘Р’С—Р’Р„Р“вЂ Р“Т‘
		//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(Р’С�Р“вЂћР’Т‘Р“В¦Р“вЂ Р“В· 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D Р“вЂўР“Р…Р’С–Р’Р€Р’С•Р“РЊР“вЂњР“С“Р“вЂўР“СћР’С‘Р“В¶.
		// 1  1   1	   U->D	L->R
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K
        LCD_WriteReg(0x0004,0x0000);
        LCD_WriteReg(0x0008,0x0202);
        LCD_WriteReg(0x0009,0x0000);
        LCD_WriteReg(0x000a,0x0000);//display setting
        LCD_WriteReg(0x000c,0x0001);//display setting
        LCD_WriteReg(0x000d,0x0000);//0f3c
        LCD_WriteReg(0x000f,0x0000);
		//Р’ВµР“В§Р“вЂќР’Т‘Р“вЂ¦Р“В¤Р“вЂ“Р“С“
        LCD_WriteReg(0x0010,0x0000);
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);
        LCD_WriteReg(0x0013,0x0000);
     	LCD_WriteReg(0x0007,0x0001);
       	_delay_ms(50);
        LCD_WriteReg(0x0010,0x1490);
        LCD_WriteReg(0x0011,0x0227);
        _delay_ms(50);
        LCD_WriteReg(0x0012,0x008A);
        _delay_ms(50);
        LCD_WriteReg(0x0013,0x1a00);
        LCD_WriteReg(0x0029,0x0006);
        LCD_WriteReg(0x002b,0x000d);
        _delay_ms(50);
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x0000);
		_delay_ms(50);
		//Р“в„ўР’В¤Р“вЂљР“В­Р“С’Р’Р€Р“вЂўР“Р…
        LCD_WriteReg(0x0030,0x0000);
        LCD_WriteReg(0x0031,0x0604);
        LCD_WriteReg(0x0032,0x0305);
        LCD_WriteReg(0x0035,0x0000);
        LCD_WriteReg(0x0036,0x0C09);
        LCD_WriteReg(0x0037,0x0204);
        LCD_WriteReg(0x0038,0x0301);
        LCD_WriteReg(0x0039,0x0707);
        LCD_WriteReg(0x003c,0x0000);
        LCD_WriteReg(0x003d,0x0a0a);
        _delay_ms(50);
        LCD_WriteReg(0x0050,0x0000); //Р“вЂ№Р’В®Р“вЂ Р’Р…GRAMР“вЂ Р“В°Р“Р‰Р’С�Р“Р‹Р’В»Р“вЂ“Р“С“
        LCD_WriteReg(0x0051,0x00ef); //Р“вЂ№Р’В®Р“вЂ Р’Р…GRAMР“вЂ“Р“вЂўР“вЂ“Р’в„–Р“Р‹Р’В»Р“вЂ“Р“С“
        LCD_WriteReg(0x0052,0x0000); //Р’Т‘Р’в„–Р“вЂ“Р’В±GRAMР“вЂ Р“В°Р“Р‰Р’С�Р“Р‹Р’В»Р“вЂ“Р“С“
        LCD_WriteReg(0x0053,0x013f); //Р’Т‘Р’в„–Р“вЂ“Р’В±GRAMР“вЂ“Р“вЂўР“вЂ“Р’в„–Р“Р‹Р’В»Р“вЂ“Р“С“

         LCD_WriteReg(0x0060,0xa700);
        LCD_WriteReg(0x0061,0x0001);
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);

        LCD_WriteReg(0x0090,0x0010);
        LCD_WriteReg(0x0092,0x0600);
        //Р’С—Р’Р„Р“вЂ Р“Т‘Р“РЏР“вЂќР“Р‰Р’С•Р“вЂ°Р“РЃР“вЂ“Р“С“
        LCD_WriteReg(0x0007,0x0133);
	}else if(lcddev.id==0x9320)//Р’Р†Р“СћР“Р‰Р“вЂќOK.
	{
		LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
		LCD_WriteReg(0x03,0x1030);//Entry Mode Set.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.

		LCD_WriteReg(0x04,0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.
		_delay_ms(50);
		LCD_WriteReg(0x07,0x0101);	//Display Contral.
		_delay_ms(50);
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.

		LCD_WriteReg(0x2b,(1<<14)|(1<<4));
		LCD_WriteReg(0x50,0);	//Set X Star
		//Р“вЂ№Р’В®Р“вЂ Р’Р…GRAMР“вЂ“Р“вЂўР“вЂ“Р’в„–Р“Р‹Р’В»Р“вЂ“Р“С“Set X End.
		LCD_WriteReg(0x51,239);	//Set Y Star
		LCD_WriteReg(0x52,0);	//Set Y End.t.
		LCD_WriteReg(0x53,319);	//

		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.

		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.

		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.
		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}else if(lcddev.id==0X9331)//OK |/|/|
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(1<<3));//65K
		//LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		_delay_ms(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		_delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		_delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		_delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x013f); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007, 0x0133); // 262K color and display ON
	}else if(lcddev.id==0x5408)
	{
		LCD_WriteReg(0x01,0x0100);
		LCD_WriteReg(0x02,0x0700);//LCD Driving Waveform Contral
		LCD_WriteReg(0x03,0x1030);//Entry ModeР“вЂ°Р“РЃР“вЂ“Р“С“
		//Р“вЂ“Р’С‘Р“вЂўР“В«Р’Т‘Р“вЂњР“вЂ”Р“С–Р“вЂ“Р“РѓР“вЂњР“вЂ™Р“вЂ”Р“вЂќР“вЂ°Р“РЏР’В¶Р“С‘Р“РЏР“вЂљР’ВµР“вЂћР“вЂ”Р“вЂќР’В¶Р’Р‡Р“вЂќР“В¶Р“вЂћР’Р€Р“Р‰Р’Р…
		//Normal Mode(Window Mode disable)
		//RGBР’С‘Р“В±Р“Р‰Р’Р…
		//16Р“Р‹Р’В»Р“Р‰Р“Р…Р’С•Р“Сњ2Р’Т‘Р“Р‹Р’Т‘Р’В«Р“Р‰Р“В¤Р’ВµР“вЂћ8Р“вЂ”Р“СљР“РЏР“СџР“вЂ°Р“РЃР“вЂ“Р“С“
		LCD_WriteReg(0x04,0x0000); //Scalling Control register
		LCD_WriteReg(0x08,0x0207); //Display Control 2
		LCD_WriteReg(0x09,0x0000); //Display Control 3
		LCD_WriteReg(0x0A,0x0000); //Frame Cycle Control
		LCD_WriteReg(0x0C,0x0000); //External Display Interface Control 1
		LCD_WriteReg(0x0D,0x0000); //Frame Maker Position
		LCD_WriteReg(0x0F,0x0000); //External Display Interface Control 2
 		_delay_ms(20);
		//TFT Р“вЂ™Р’С”Р’С•Р’В§Р’Р†Р“Р‰Р“вЂ°Р’В«Р“РЊР’С�Р“РЏР“В±Р“РЏР“вЂќР“Р‰Р’С•Р’В·Р’Р…Р’В·Р’РЃ14
		LCD_WriteReg(0x10,0x16B0); //0x14B0 //Power Control 1
		LCD_WriteReg(0x11,0x0001); //0x0007 //Power Control 2
		LCD_WriteReg(0x17,0x0001); //0x0000 //Power Control 3
		LCD_WriteReg(0x12,0x0138); //0x013B //Power Control 4
		LCD_WriteReg(0x13,0x0800); //0x0800 //Power Control 5
		LCD_WriteReg(0x29,0x0009); //NVM read data 2
		LCD_WriteReg(0x2a,0x0009); //NVM read data 3
		LCD_WriteReg(0xa4,0x0000);
		LCD_WriteReg(0x50,0x0000); //Р“вЂ°Р“РЃР“вЂ“Р“С“Р’Р†Р“в„ўР“вЂ”Р“В·Р’Т‘Р’В°Р’С—Р“С™Р’ВµР“вЂћXР“вЂ“Р“РЋР’С—Р’Р„Р“Р‰Р’С�Р“РѓР“С’
		LCD_WriteReg(0x51,0x00EF); //Р“вЂ°Р“РЃР“вЂ“Р“С“Р’Р†Р“в„ўР“вЂ”Р“В·Р’Т‘Р’В°Р’С—Р“С™Р’ВµР“вЂћXР“вЂ“Р“РЋР’Р…Р“РЋР“Р‰Р“С‘Р“РѓР“С’
		LCD_WriteReg(0x52,0x0000); //Р“вЂ°Р“РЃР“вЂ“Р“С“Р’Р†Р“в„ўР“вЂ”Р“В·Р’Т‘Р’В°Р’С—Р“С™Р’ВµР“вЂћYР“вЂ“Р“РЋР’С—Р’Р„Р“Р‰Р’С�Р“С’Р“С’
		LCD_WriteReg(0x53,0x013F); //Р“вЂ°Р“РЃР“вЂ“Р“С“Р’Р†Р“в„ўР“вЂ”Р“В·Р’Т‘Р’В°Р’С—Р“С™Р’ВµР“вЂћYР“вЂ“Р“РЋР’Р…Р“РЋР“Р‰Р“С‘Р“С’Р“С’
		LCD_WriteReg(0x60,0x2700); //Driver Output Control
		//Р“вЂ°Р“РЃР“вЂ“Р“С“Р“вЂ Р“РѓР“вЂћР’В»Р’ВµР“вЂћР’ВµР“Р€Р“Р‰Р“Р…Р“вЂ™Р“вЂќР’С�Р’В°Р“вЂ°Р’РЃР“С“Р“РЃР’ВµР“вЂћР“вЂ Р“В°Р“Р‰Р’С�Р“С’Р“С’
		LCD_WriteReg(0x61,0x0001); //Driver Output Control
		LCD_WriteReg(0x6A,0x0000); //Vertical Scroll Control
		LCD_WriteReg(0x80,0x0000); //Display Position Р’РЃC Partial Display 1
		LCD_WriteReg(0x81,0x0000); //RAM Address Start Р’РЃC Partial Display 1
		LCD_WriteReg(0x82,0x0000); //RAM address End - Partial Display 1
		LCD_WriteReg(0x83,0x0000); //Display Position Р’РЃC Partial Display 2
		LCD_WriteReg(0x84,0x0000); //RAM Address Start Р’РЃC Partial Display 2
		LCD_WriteReg(0x85,0x0000); //RAM address End Р’РЃC Partail Display2
		LCD_WriteReg(0x90,0x0013); //Frame Cycle Control
		LCD_WriteReg(0x92,0x0000);  //Panel Interface Control 2
		LCD_WriteReg(0x93,0x0003); //Panel Interface control 3
		LCD_WriteReg(0x95,0x0110);  //Frame Cycle Control
		LCD_WriteReg(0x07,0x0173);
		_delay_ms(50);
	}
	else if(lcddev.id==0x1505)//OK
	{
		// second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007,0x0000);
        _delay_ms(50);
        LCD_WriteReg(0x0012,0x011C);//0x011A   why need to set several times?
        LCD_WriteReg(0x00A4,0x0001);//NVM
        LCD_WriteReg(0x0008,0x000F);
        LCD_WriteReg(0x000A,0x0008);
        LCD_WriteReg(0x000D,0x0008);
  		//Р“в„ўР’В¤Р“вЂљР“В­Р“С’Р’Р€Р“вЂўР“Р…
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0007); //0x0707
        LCD_WriteReg(0x0032,0x0603);
        LCD_WriteReg(0x0033,0x0700);
        LCD_WriteReg(0x0034,0x0202);
        LCD_WriteReg(0x0035,0x0002); //?0x0606
        LCD_WriteReg(0x0036,0x1F0F);
        LCD_WriteReg(0x0037,0x0707); //0x0f0f  0x0105
        LCD_WriteReg(0x0038,0x0000);
        LCD_WriteReg(0x0039,0x0000);
        LCD_WriteReg(0x003A,0x0707);
        LCD_WriteReg(0x003B,0x0000); //0x0303
        LCD_WriteReg(0x003C,0x0007); //?0x0707
        LCD_WriteReg(0x003D,0x0000); //0x1313//0x1f08
        _delay_ms(50);
        LCD_WriteReg(0x0007,0x0001);
        LCD_WriteReg(0x0017,0x0001);//Р’С—Р’Р„Р“вЂ Р“Т‘Р’ВµР“В§Р“вЂќР’Т‘
        _delay_ms(50);
  		//Р’ВµР“В§Р“вЂќР’Т‘Р“вЂ¦Р“В¤Р“вЂ“Р“С“
        LCD_WriteReg(0x0010,0x17A0);
        LCD_WriteReg(0x0011,0x0217);//reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013,0x0F00);//VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A,0x0000);
        LCD_WriteReg(0x0029,0x000A);//0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012,0x013E);// 0x013C  power supply on
        //Coordinates Control//
        LCD_WriteReg(0x0050,0x0000);//0x0e00
        LCD_WriteReg(0x0051,0x00EF);
        LCD_WriteReg(0x0052,0x0000);
        LCD_WriteReg(0x0053,0x013F);
    	//Pannel Image Control//
        LCD_WriteReg(0x0060,0x2700);
        LCD_WriteReg(0x0061,0x0001);
        LCD_WriteReg(0x006A,0x0000);
        LCD_WriteReg(0x0080,0x0000);
    	//Partial Image Control//
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
  		//Panel Interface Control//
        LCD_WriteReg(0x0090,0x0013);//0x0010 frenqucy
        LCD_WriteReg(0x0092,0x0300);
        LCD_WriteReg(0x0093,0x0005);
        LCD_WriteReg(0x0095,0x0000);
        LCD_WriteReg(0x0097,0x0000);
        LCD_WriteReg(0x0098,0x0000);

        LCD_WriteReg(0x0001,0x0100);
        LCD_WriteReg(0x0002,0x0700);
        LCD_WriteReg(0x0003,0x1038);//Р“вЂ°Р’РЃР“С“Р“РЃР’В·Р’Р…Р“РЏР“Р† Р“вЂ°Р“РЏ->Р“РЏР“вЂљ  Р“вЂ”Р“С–->Р“вЂњР“вЂ™
        LCD_WriteReg(0x0004,0x0000);
        LCD_WriteReg(0x000C,0x0000);
        LCD_WriteReg(0x000F,0x0000);
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x0000);
        LCD_WriteReg(0x0007,0x0021);
        _delay_ms(20);
        LCD_WriteReg(0x0007,0x0061);
        _delay_ms(20);
        LCD_WriteReg(0x0007,0x0173);
        _delay_ms(20);
	}else if(lcddev.id==0xB505)
	{
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);

		LCD_WriteReg(0x00a4,0x0001);
		_delay_ms(20);
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0008,0x0202);

		LCD_WriteReg(0x0030,0x0214);
		LCD_WriteReg(0x0031,0x3715);
		LCD_WriteReg(0x0032,0x0604);
		LCD_WriteReg(0x0033,0x0e16);
		LCD_WriteReg(0x0034,0x2211);
		LCD_WriteReg(0x0035,0x1500);
		LCD_WriteReg(0x0036,0x8507);
		LCD_WriteReg(0x0037,0x1407);
		LCD_WriteReg(0x0038,0x1403);
		LCD_WriteReg(0x0039,0x0020);

		LCD_WriteReg(0x0090,0x001a);
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		_delay_ms(20);

		LCD_WriteReg(0x0010,0x0730);
		LCD_WriteReg(0x0011,0x0137);
		_delay_ms(20);

		LCD_WriteReg(0x0012,0x01b8);
		_delay_ms(20);

		LCD_WriteReg(0x0013,0x0f00);
		LCD_WriteReg(0x002a,0x0080);
		LCD_WriteReg(0x0029,0x0048);
		_delay_ms(20);

		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
        LCD_WriteReg(0x0003,0x1038);//Р“вЂ°Р’РЃР“С“Р“РЃР’В·Р’Р…Р“РЏР“Р† Р“вЂ°Р“РЏ->Р“РЏР“вЂљ  Р“вЂ”Р“С–->Р“вЂњР“вЂ™
		LCD_WriteReg(0x0008,0x0202);
		LCD_WriteReg(0x000a,0x0000);
		LCD_WriteReg(0x000c,0x0000);
		LCD_WriteReg(0x000d,0x0000);
		LCD_WriteReg(0x000e,0x0030);
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00ef);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013f);
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x006a,0x0000);
		//LCD_WriteReg(0x0080,0x0000);
		//LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0090,0X0011);
		LCD_WriteReg(0x0092,0x0600);
		LCD_WriteReg(0x0093,0x0402);
		LCD_WriteReg(0x0094,0x0002);
		_delay_ms(20);

		LCD_WriteReg(0x0007,0x0001);
		_delay_ms(20);
		LCD_WriteReg(0x0007,0x0061);
		LCD_WriteReg(0x0007,0x0173);

		LCD_WriteReg(0x0020,0x0000);
		LCD_WriteReg(0x0021,0x0000);
		LCD_WriteReg(0x00,0x22);
	}else if(lcddev.id==0xC505)
	{
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		_delay_ms(20);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
 		LCD_WriteReg(0x00a4,0x0001);
		_delay_ms(20);
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0008,0x0806);

		LCD_WriteReg(0x0030,0x0703);//gamma setting
		LCD_WriteReg(0x0031,0x0001);
		LCD_WriteReg(0x0032,0x0004);
		LCD_WriteReg(0x0033,0x0102);
		LCD_WriteReg(0x0034,0x0300);
		LCD_WriteReg(0x0035,0x0103);
		LCD_WriteReg(0x0036,0x001F);
		LCD_WriteReg(0x0037,0x0703);
		LCD_WriteReg(0x0038,0x0001);
		LCD_WriteReg(0x0039,0x0004);



		LCD_WriteReg(0x0090, 0x0015);	//80Hz
		LCD_WriteReg(0x0010, 0X0410);	//BT,AP
		LCD_WriteReg(0x0011,0x0247);	//DC1,DC0,VC
		LCD_WriteReg(0x0012, 0x01BC);
		LCD_WriteReg(0x0013, 0x0e00);
		_delay_ms(120);
		LCD_WriteReg(0x0001, 0x0100);
		LCD_WriteReg(0x0002, 0x0200);
		LCD_WriteReg(0x0003, 0x1030);

		LCD_WriteReg(0x000A, 0x0008);
		LCD_WriteReg(0x000C, 0x0000);

		LCD_WriteReg(0x000E, 0x0020);
		LCD_WriteReg(0x000F, 0x0000);
		LCD_WriteReg(0x0020, 0x0000);	//H Start
		LCD_WriteReg(0x0021, 0x0000);	//V Start
		LCD_WriteReg(0x002A,0x003D);	//vcom2
		_delay_ms(20);
		LCD_WriteReg(0x0029, 0x002d);
		LCD_WriteReg(0x0050, 0x0000);
		LCD_WriteReg(0x0051, 0xD0EF);
		LCD_WriteReg(0x0052, 0x0000);
		LCD_WriteReg(0x0053, 0x013F);
		LCD_WriteReg(0x0061, 0x0000);
		LCD_WriteReg(0x006A, 0x0000);
		LCD_WriteReg(0x0092,0x0300);

 		LCD_WriteReg(0x0093, 0x0005);
		LCD_WriteReg(0x0007, 0x0100);
	}else if(lcddev.id==0x4531)//OK |/|/|
	{
		LCD_WriteReg(0X00,0X0001);
		_delay_ms(10);
		LCD_WriteReg(0X10,0X1628);
		LCD_WriteReg(0X12,0X000e);//0x0006
		LCD_WriteReg(0X13,0X0A39);
		_delay_ms(10);
		LCD_WriteReg(0X11,0X0040);
		LCD_WriteReg(0X15,0X0050);
		_delay_ms(10);
		LCD_WriteReg(0X12,0X001e);//16
		_delay_ms(10);
		LCD_WriteReg(0X10,0X1620);
		LCD_WriteReg(0X13,0X2A39);
		_delay_ms(10);
		LCD_WriteReg(0X01,0X0100);
		LCD_WriteReg(0X02,0X0300);
		LCD_WriteReg(0X03,0X1038);//Р’С‘Р“вЂћР’В±Р“В¤Р’В·Р’Р…Р“РЏР“Р†Р’ВµР“вЂћ
		LCD_WriteReg(0X08,0X0202);
		LCD_WriteReg(0X0A,0X0008);
		LCD_WriteReg(0X30,0X0000);
		LCD_WriteReg(0X31,0X0402);
		LCD_WriteReg(0X32,0X0106);
		LCD_WriteReg(0X33,0X0503);
		LCD_WriteReg(0X34,0X0104);
		LCD_WriteReg(0X35,0X0301);
		LCD_WriteReg(0X36,0X0707);
		LCD_WriteReg(0X37,0X0305);
		LCD_WriteReg(0X38,0X0208);
		LCD_WriteReg(0X39,0X0F0B);
		LCD_WriteReg(0X41,0X0002);
		LCD_WriteReg(0X60,0X2700);
		LCD_WriteReg(0X61,0X0001);
		LCD_WriteReg(0X90,0X0210);
		LCD_WriteReg(0X92,0X010A);
		LCD_WriteReg(0X93,0X0004);
		LCD_WriteReg(0XA0,0X0100);
		LCD_WriteReg(0X07,0X0001);
		LCD_WriteReg(0X07,0X0021);
		LCD_WriteReg(0X07,0X0023);
		LCD_WriteReg(0X07,0X0033);
		LCD_WriteReg(0X07,0X0133);
		LCD_WriteReg(0XA0,0X0000);
	}else if(lcddev.id==0x4535)
	{
		LCD_WriteReg(0X15,0X0030);
		LCD_WriteReg(0X9A,0X0010);
 		LCD_WriteReg(0X11,0X0020);
 		LCD_WriteReg(0X10,0X3428);
		LCD_WriteReg(0X12,0X0002);//16
 		LCD_WriteReg(0X13,0X1038);
		_delay_ms(40);
		LCD_WriteReg(0X12,0X0012);//16
		_delay_ms(40);
  		LCD_WriteReg(0X10,0X3420);
 		LCD_WriteReg(0X13,0X3038);
		_delay_ms(70);
		LCD_WriteReg(0X30,0X0000);
		LCD_WriteReg(0X31,0X0402);
		LCD_WriteReg(0X32,0X0307);
		LCD_WriteReg(0X33,0X0304);
		LCD_WriteReg(0X34,0X0004);
		LCD_WriteReg(0X35,0X0401);
		LCD_WriteReg(0X36,0X0707);
		LCD_WriteReg(0X37,0X0305);
		LCD_WriteReg(0X38,0X0610);
		LCD_WriteReg(0X39,0X0610);

		LCD_WriteReg(0X01,0X0100);
		LCD_WriteReg(0X02,0X0300);
		LCD_WriteReg(0X03,0X1030);//Р’С‘Р“вЂћР’В±Р“В¤Р’В·Р’Р…Р“РЏР“Р†Р’ВµР“вЂћ
		LCD_WriteReg(0X08,0X0808);
		LCD_WriteReg(0X0A,0X0008);
 		LCD_WriteReg(0X60,0X2700);
		LCD_WriteReg(0X61,0X0001);
		LCD_WriteReg(0X90,0X013E);
		LCD_WriteReg(0X92,0X0100);
		LCD_WriteReg(0X93,0X0100);
 		LCD_WriteReg(0XA0,0X3000);
 		LCD_WriteReg(0XA3,0X0010);
		LCD_WriteReg(0X07,0X0001);
		LCD_WriteReg(0X07,0X0021);
		LCD_WriteReg(0X07,0X0023);
		LCD_WriteReg(0X07,0X0033);
		LCD_WriteReg(0X07,0X0133);
	}
	LCD_Display_Dir(0);
	LCD_LED=1;
	LCD_Clear(White);
}

void LCD_Clear(uint16_t color)
{
	uint32_t index=0;
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//Р’ВµР“С“Р’ВµР’Р…Р“вЂ”Р“СљР’ВµР“Р€Р“Р‰Р“Р…
	if((lcddev.id==0X6804)&&(lcddev.dir==1))//6804Р’С”Р“РЋР“вЂ Р“РѓР’ВµР“вЂћР“Р‰Р’В±Р’С”Р“Р†Р“РЉР“пїЅР“Р‰Р“СћР’Т‘Р’В¦Р“Р‚Р“В­
	{
 		lcddev.dir=0;
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		LCD_SetCursor(0x00,0x0000);		//Р“вЂ°Р“РЃР“вЂ“Р“С“Р’в„–Р“СћР’В±Р“Р„Р“Р‹Р’В»Р“вЂ“Р“С“
 		lcddev.dir=1;
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;
 	}else LCD_SetCursor(0x00,0x0000);	//Р“вЂ°Р“РЃР“вЂ“Р“С“Р’в„–Р“СћР’В±Р“Р„Р“Р‹Р’В»Р“вЂ“Р“С“
	LCD_WriteRAM_Prepare();     		//Р’С—Р’Р„Р“Р‰Р’С�Р“С’Р’Т‘Р“в‚¬Р“В«GRAM
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;
	}
}

void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	uint16_t i,j;
	uint16_t xlen=0;
	uint16_t temp;
	if((lcddev.id==0X6804)&&(lcddev.dir==1))	//6804Р’С”Р“РЋР“вЂ Р“РѓР’ВµР“вЂћР“Р‰Р’В±Р’С”Р“Р†Р“РЉР“пїЅР“Р‰Р“СћР’Т‘Р’В¦Р“Р‚Р“В­
	{
		temp=sx;
		sx=sy;
		sy=lcddev.width-ex-1;
		ex=ey;
		ey=lcddev.width-temp-1;
 		lcddev.dir=0;
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		LCD_Fill(sx,sy,ex,ey,color);
 		lcddev.dir=1;
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;
 	}else
	{
		xlen=ex-sx+1;
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//Р“вЂ°Р“РЃР“вЂ“Р“С“Р’в„–Р“СћР’В±Р“Р„Р“Р‹Р’В»Р“вЂ“Р“С“
			LCD_WriteRAM_Prepare();     			//Р’С—Р’Р„Р“Р‰Р’С�Р“С’Р’Т‘Р“в‚¬Р“В«GRAM
			for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//Р“РЏР“вЂќР“Р‰Р’С•Р“вЂ�Р“вЂўР“вЂ°Р’В«
		}
	}
}

void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1;
	height=ey-sy+1;
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);
		LCD_WriteRAM_Prepare();
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];
	}
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1;
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1;
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x;
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )
	{
		LCD_Fast_DrawPoint(uRow, uCol, color);
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

void LCD_VLine(uint16_t x0, uint16_t y0, uint16_t h, uint16_t color)
{
	LCD_DrawLine(x0, y0, x0, y0+h, color);
}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1,POINT_COLOR);
	LCD_DrawLine(x1,y1,x1,y2,POINT_COLOR);
	LCD_DrawLine(x1,y2,x2,y2,POINT_COLOR);
	LCD_DrawLine(x2,y1,x2,y2,POINT_COLOR);
}

void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;
	di=3-(r<<1);
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0
		LCD_DrawPoint(x0+b,y0+a);             //4
		LCD_DrawPoint(x0+a,y0+b);             //6
		LCD_DrawPoint(x0-a,y0+b);             //1
 		LCD_DrawPoint(x0-b,y0+a);
		LCD_DrawPoint(x0-a,y0-b);             //2
  		LCD_DrawPoint(x0-b,y0-a);             //7
		a++;
		if(di<0)di +=4*a+6;
		else
		{
			di+=10+4*(a-b);
			b--;
		}
	}
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//Р’ВµР“С“Р’ВµР’Р…Р“вЂ”Р“вЂ“Р“РЉР“ТђР“вЂ™Р’В»Р’С‘Р“В¶Р“вЂ”Р“вЂ“Р’В·Р“В»Р’В¶Р“вЂќР“вЂњР’В¦Р’ВµР“Р€Р“вЂўР“С–Р’С�Р’Р‡Р“вЂ№Р“в„–Р“вЂўР’С�Р’ВµР“вЂћР“вЂ”Р“вЂ“Р’Р…Р“С™Р“Р‰Р“Р…
	//Р“вЂ°Р“РЃР“вЂ“Р“С“Р’Т‘Р’В°Р’С—Р“С™
	num=num-' ';//Р’ВµР“С“Р’ВµР’Р…Р“вЂ Р’В«Р“вЂ™Р“вЂ Р’С”Р“С–Р’ВµР“вЂћР“вЂ“Р’Вµ
	for(t=0;t<csize;t++)
	{
		if(size==12)temp=asc2_1206[num][t]; 	 	//Р’ВµР“В·Р“вЂњР“С“1206Р“вЂ”Р“вЂ“Р“РЉР“Тђ
		else if(size==16)temp=asc2_1608[num][t];	//Р’ВµР“В·Р“вЂњР“С“1608Р“вЂ”Р“вЂ“Р“РЉР“Тђ
		else if(size==24)temp=asc2_2412[num][t];	//Р’ВµР“В·Р“вЂњР“С“2412Р“вЂ”Р“вЂ“Р“РЉР“Тђ
		else return;								//Р“С“Р’В»Р“вЂњР“С’Р’ВµР“вЂћР“вЂ”Р“вЂ“Р’С—Р“Сћ
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//Р’С–Р’В¬Р“вЂЎР“С‘Р“вЂњР“Р†Р“РѓР“вЂ№
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//Р’С–Р’В¬Р“вЂЎР“С‘Р“вЂњР“Р†Р“РѓР“вЂ№
				break;
			}
		}
	}
}

void LCD_PutChar(uint16_t x, uint16_t y, uint8_t num, unsigned short fontColor, unsigned short fontBkColor)
{
	uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(FONT_SIZE/8+((FONT_SIZE%8)?1:0))*(FONT_SIZE/2);
	num=num-' ';
	for(t=0;t<csize;t++)
	{
		if(FONT_SIZE==12)temp=asc2_1206[num][t];
		else if(FONT_SIZE==16)temp=asc2_1608[num][t];
		else if(FONT_SIZE==24)temp=asc2_2412[num][t];
		else return;

		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x, y, fontColor);
			else LCD_Fast_DrawPoint(x, y, fontBkColor);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;
			if((y-y0)==FONT_SIZE)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;
				break;
			}
		}
	}
}

uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;
	return result;
}

void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
	}
}

void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
 				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
	}
}

void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}
