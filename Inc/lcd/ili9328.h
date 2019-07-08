#ifndef _ILI9320_H_
#define _ILI9320_H_

#include "stm32f1xx_hal.h"

// PD13-LIGHT-PWM
#define Lcd_Light_ON   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
#define Lcd_Light_OFF  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

static inline void ili9328_SetCursor(uint16_t x, uint16_t y) {
	*(__IO uint16_t *) (Bank1_LCD_C) = 32;
	*(__IO uint16_t *) (Bank1_LCD_D) = y;

	*(__IO uint16_t *) (Bank1_LCD_C) = 33;
	*(__IO uint16_t *) (Bank1_LCD_D) = 319 - x;
}

static inline void ili9328_WrVcamLine(uint16_t x, uint16_t y, uint16_t h,
		uint16_t color[]) {
	int i;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x20; // 32
	*(__IO uint16_t *) (Bank1_LCD_D) = y;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x21; // 33;
	*(__IO uint16_t *) (Bank1_LCD_D) = 319 - x;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x0022;
	for (i = 0; i < h; i++) {
		*(__IO uint16_t *) (Bank1_LCD_D) = color[i];
	}
}

unsigned short CheckController(void);
void ili9328_Initializtion(void);
void ili9328_Reset(void);
void ili9328_WriteRegister(unsigned short index, unsigned short dat);
void ili9328_SetWindows(unsigned short StartX, unsigned short StartY,
		unsigned short EndX, unsigned short EndY);
void ili9328_DrawPicture(unsigned short StartX, unsigned short StartY,
		unsigned short EndX, unsigned short EndY, unsigned short *pic);
void ili9328_SetPoint(unsigned short x, unsigned short y, unsigned short point);
void ili9328_PutChar(unsigned short x, unsigned short y, unsigned char c,
		unsigned short charColor, unsigned short bkColor);
void ili9328_Clear(unsigned short dat);
void ili9328_Delay(unsigned int nCount);
void ili9328_Test(void);
unsigned short ili9328_GetCode(void);
void ili9328_WriteData(unsigned short dat);
void ili9328_WriteIndex(unsigned short idx);

void ili9328_VLine(unsigned short x0, unsigned short y0, unsigned short h,
		unsigned short color);
void ili9328_BackLight(unsigned char status);

unsigned short ili9328_BGR2RGB(unsigned short c);

unsigned short ili9328_GetPoint(unsigned short x, unsigned short y);
unsigned short ili9328_ReadData(void);
unsigned short ili9328_ReadRegister(unsigned short index);

unsigned short GUI_Color565(unsigned int RGB);

void GUI_Text(unsigned short x, unsigned short y, const unsigned char *str,
		unsigned short len, unsigned short Color, unsigned short bkColor);
void GUI_Line(unsigned short x0, unsigned short y0, unsigned short x1,
		unsigned short y1, unsigned short color);
void GUI_Circle(unsigned short cx, unsigned short cy, unsigned short r,
		unsigned short color, unsigned char fill);
void GUI_Rectangle(unsigned short x0, unsigned short y0, unsigned short x1,
		unsigned short y1, unsigned short color, unsigned char fill);
void GUI_Square(unsigned short x0, unsigned short y0, unsigned short with,
		unsigned short color, unsigned char fill);

unsigned short Touch_MeasurementX(void);
unsigned short Touch_MeasurementY(void);

// INT   (ADC touch)	- PE12
unsigned char isTouch_pen(void);
unsigned short Touch_ScrX(void);
unsigned short Touch_ScrY(void);

#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#endif
