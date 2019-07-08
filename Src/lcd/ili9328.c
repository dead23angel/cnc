#include "lcd/ili9328.h"
#include "lcd/ili9328_font.h"

void ili9320_Delay(unsigned int nCount) {
	for (; nCount != 0; nCount--)
		__NOP();
}

static void LCD_WR_REG(unsigned int index) {
	*(__IO uint16_t *) (Bank1_LCD_C) = index;
}
static void LCD_WR_CMD(uint16_t cmd, uint16_t dat) {
	*(__IO uint16_t *) (Bank1_LCD_C) = cmd;
	*(__IO uint16_t *) (Bank1_LCD_D) = dat;
}

unsigned int a = 0;

static unsigned int LCD_RD_data(void) {
	a = (*(__IO uint16_t *) (Bank1_LCD_D)); 	//Dummy
	a = *(__IO uint16_t *) (Bank1_LCD_D); 	    //L
	return (a);
}

static void LCD_WR_Data(unsigned int val) {
	*(__IO uint16_t *) (Bank1_LCD_D) = val;
}
static void Delay(__IO uint32_t nCount) {
	for (; nCount != 0; nCount--)
		__NOP();
}

void ili9328_Initializtion() {
	unsigned int i;

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
	Delay(0xAFFf);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
	Delay(0xAFFf);

	LCD_RD_data();

	//************* Start Initial Sequence **********//
	LCD_WR_CMD(0x0001, 0x0100); // set SS and SM bit
	LCD_WR_CMD(0x0002, 0x0700); // set 1 line inversion
	LCD_WR_CMD(0x0003, 0x1030); // set GRAM write direction and BGR=1.
	LCD_WR_CMD(0x0004, 0x0000); // Resize register
	LCD_WR_CMD(0x0008, 0x0207); // set the back porch and front porch
	LCD_WR_CMD(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	LCD_WR_CMD(0x000A, 0x0000); // FMARK function
	LCD_WR_CMD(0x000C, 0x0000); // RGB interface setting
	LCD_WR_CMD(0x000D, 0x0000); // Frame marker Position
	LCD_WR_CMD(0x000F, 0x0000); // RGB interface polarity
	//*************Power On sequence ****************//
	LCD_WR_CMD(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_CMD(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_WR_CMD(0x0012, 0x0000); // VREG1OUT voltage
	LCD_WR_CMD(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	LCD_WR_CMD(0x0007, 0x0001);
	Delay(12000); // Dis-charge capacitor power voltage
	LCD_WR_CMD(0x0010, 0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_CMD(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	Delay(15500); // Delay 50ms
	LCD_WR_CMD(0x0012, 0x001C); // Internal reference voltage= Vci;
	Delay(15000); // Delay 50ms
	LCD_WR_CMD(0x0013, 0x1A00); // Set VDV[4:0] for VCOM amplitude
	LCD_WR_CMD(0x0029, 0x0025); // Set VCM[5:0] for VCOMH
	LCD_WR_CMD(0x002B, 0x000C); // Set Frame Rate
	Delay(15000); // Delay 50ms
	LCD_WR_CMD(0x0020, 0x0000); // GRAM horizontal Address
	LCD_WR_CMD(0x0021, 0x0000); // GRAM Vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	LCD_WR_CMD(0x0030, 0x0000);
	LCD_WR_CMD(0x0031, 0x0506);
	LCD_WR_CMD(0x0032, 0x0104);
	LCD_WR_CMD(0x0035, 0x0207);
	LCD_WR_CMD(0x0036, 0x000F);
	LCD_WR_CMD(0x0037, 0x0306);
	LCD_WR_CMD(0x0038, 0x0102);
	LCD_WR_CMD(0x0039, 0x0707);
	LCD_WR_CMD(0x003C, 0x0702);
	LCD_WR_CMD(0x003D, 0x1604);
	//------------------ Set GRAM area ---------------//
	LCD_WR_CMD(0x0050, 0x0000); // Horizontal GRAM Start Address
	LCD_WR_CMD(0x0051, 0x00EF); // Horizontal GRAM End Address
	LCD_WR_CMD(0x0052, 0x0000); // Vertical GRAM Start Address
	LCD_WR_CMD(0x0053, 0x013F); // Vertical GRAM Start Address
	LCD_WR_CMD(0x0060, 0xA700); // Gate Scan Line
	LCD_WR_CMD(0x0061, 0x0001); // NDL,VLE, REV

	LCD_WR_CMD(0x006A, 0x0000); // set scrolling line
	//-------------- Partial Display Control ---------//
	LCD_WR_CMD(0x0080, 0x0000);
	LCD_WR_CMD(0x0081, 0x0000);
	LCD_WR_CMD(0x0082, 0x0000);
	LCD_WR_CMD(0x0083, 0x0000);
	LCD_WR_CMD(0x0084, 0x0000);
	LCD_WR_CMD(0x0085, 0x0000);
	//-------------- Panel Control -------------------//
	LCD_WR_CMD(0x0090, 0x0010);
	LCD_WR_CMD(0x0092, 0x0600);
	LCD_WR_CMD(0x0007, 0x0133); // 262K color and display ON

	LCD_WR_CMD(32, 0);
	LCD_WR_CMD(33, 0);
	*(__IO uint16_t *) (Bank1_LCD_C) = 34;
	for (i = 0; i < 76800; i++) {
		LCD_WR_Data(0xffff);
	}
}

__inline void ili9328_SetWindows(uint16_t StartX, uint16_t StartY,
		uint16_t EndX, uint16_t EndY) {
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x50;
	*(__IO uint16_t *) (Bank1_LCD_D) = StartY;

	*(__IO uint16_t *) (Bank1_LCD_C) = 0x51;
	*(__IO uint16_t *) (Bank1_LCD_D) = EndX;

	*(__IO uint16_t *) (Bank1_LCD_C) = 0x52;
	*(__IO uint16_t *) (Bank1_LCD_D) = 399 - StartX;

	*(__IO uint16_t *) (Bank1_LCD_C) = 0x53;
	*(__IO uint16_t *) (Bank1_LCD_D) = EndY;
}

void ili9328_Clear(uint16_t dat) {
	uint32_t i;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x50;
	*(__IO uint16_t *) (Bank1_LCD_D) = 0;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x51;
	*(__IO uint16_t *) (Bank1_LCD_D) = 239;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x52;
	*(__IO uint16_t *) (Bank1_LCD_D) = 0;
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x53;
	*(__IO uint16_t *) (Bank1_LCD_D) = 319;
	*(__IO uint16_t *) (Bank1_LCD_C) = 32;
	*(__IO uint16_t *) (Bank1_LCD_D) = 0;
	*(__IO uint16_t *) (Bank1_LCD_C) = 33;
	*(__IO uint16_t *) (Bank1_LCD_D) = 0;
	*(__IO uint16_t *) (Bank1_LCD_C) = 34;
	for (i = 0; i < 76800; i++) {
		*(__IO uint16_t *) (Bank1_LCD_D) = dat;
	}
}

void ili9328_WriteData(uint16_t dat) {
	*(__IO uint16_t *) (Bank1_LCD_D) = dat;
	//LCD_WR_Data(dat);
}

void ili9328_WriteIndex(uint16_t idx) {
	*(__IO uint16_t *) (Bank1_LCD_C) = idx;
	//LCD_WR_REG(idx);
}

uint16_t ili9328_ReadData(void) {
	uint16_t val = 0;
	val = LCD_RD_data();
	return val;
}

uint16_t ili9328_ReadRegister(uint16_t index) {
	uint16_t tmp;
	tmp = *(volatile unsigned int *) (0x60000000);

	return tmp;
}

void ili9328_WriteRegister(uint16_t index, uint16_t dat) {
	*(__IO uint16_t *) (Bank1_LCD_C) = index;
	*(__IO uint16_t *) (Bank1_LCD_D) = dat;
}

uint16_t ili9328_GetPoint(uint16_t x, uint16_t y) {
	ili9328_SetCursor(x, y);
	*(__IO uint16_t *) (Bank1_LCD_C) = 34;

	return (ili9328_BGR2RGB(ili9328_ReadData()));
}

void ili9328_SetPoint(uint16_t x, uint16_t y, uint16_t point) {
	if ((y >= 240) || (x >= 320))
		return;
	*(__IO uint16_t *) (Bank1_LCD_C) = 32;
	*(__IO uint16_t *) (Bank1_LCD_D) = y;

	*(__IO uint16_t *) (Bank1_LCD_C) = 33;
	*(__IO uint16_t *) (Bank1_LCD_D) = 319 - x;

	*(__IO uint16_t *) (Bank1_LCD_C) = 34;
	*(__IO uint16_t *) (Bank1_LCD_D) = point;
}

void ili9328_DrawPicture(uint16_t StartX, uint16_t StartY, uint16_t EndX,
		uint16_t EndY, uint16_t *pic) {
	uint16_t i, temp;
	ili9328_SetWindows(StartX, StartY, EndX, EndY);
	ili9328_SetCursor(StartX, StartY);

	LCD_WR_REG(34);
	i = 0;
	while (i < 40000) {
		temp = (uint16_t) (pic[i] << 8) + pic[i + 1];
		LCD_WR_Data(temp);
		i = i + 2;
	}
}

void ili9328_VLine(uint16_t x0, uint16_t y0, uint16_t h, uint16_t color) {
	int i;
	ili9328_SetCursor(x0, y0);
	*(__IO uint16_t *) (Bank1_LCD_C) = 0x0022;
	for (i = 0; i < h; i++) {
		*(__IO uint16_t *) (Bank1_LCD_D) = color;
	}
}

void ili9328_PutChar(uint16_t x, uint16_t y, uint8_t c, uint16_t charColor,
		uint16_t bkColor) {
	uint16_t i = 0;
	uint16_t j = 0;

	uint8_t tmp_char = 0;

	for (i = 0; i < 16; i++) {
		tmp_char = ascii_8x16[((c - 0x20) * 16) + i];
		for (j = 0; j < 8; j++) {
			if (((tmp_char >> (7 - j)) & 0x01) == 0x01) {
				ili9328_SetPoint(x + j, y + i, charColor); // �ַ���ɫ
			} else {
				ili9328_SetPoint(x + j, y + i, bkColor); // ������ɫ
			}
		}
	}
}

void ili9328_Test() {
	uint16_t i, j;
	ili9328_SetCursor(0, 0);

	for (i = 0; i < 400; i++)
		for (j = 0; j < 240; j++) {
			if (i > 279)
				LCD_WR_Data(0x0000);
			else if (i > 239)
				LCD_WR_Data(0x001f);
			else if (i > 199)
				LCD_WR_Data(0x07e0);
			else if (i > 159)
				LCD_WR_Data(0x07ff);
			else if (i > 119)
				LCD_WR_Data(0xf800);
			else if (i > 79)
				LCD_WR_Data(0xf81f);
			else if (i > 39)
				LCD_WR_Data(0xffe0);
			else
				LCD_WR_Data(0xffff);
		}

}

uint16_t ili9328_BGR2RGB(uint16_t c) {
	uint16_t r, g, b, rgb;

	b = (c >> 0) & 0x1f;
	g = (c >> 5) & 0x3f;
	r = (c >> 11) & 0x1f;

	rgb = (b << 11) + (g << 5) + (r << 0);

	return (rgb);
}

void ili9328_Reset() {
//  	Set_Rst;;
//    ili9320_Delay(50000);
//  	Clr_Rst;
//    ili9320_Delay(50000);
//  	Set_Rst;
//    ili9320_Delay(50000);
}

void ili9328_BackLight(uint8_t status) {
	if (status >= 1) {
		Lcd_Light_ON;
	} else {
		Lcd_Light_OFF;
	}
}
