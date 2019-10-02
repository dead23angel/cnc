#include "global.h"

void SystemStartup(void) {
//	rs232_init();
	stepm_init();
//	spindle_init();
	#ifdef HAS_ENCODER
		encoder_int();
	#endif
	LCD_Init();
	LCD_Clear(0);
	win_showMsgWin();
	scr_puts(" ----- CNC CWTeam V0.0.1 -----");
//	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb, &USR_cb);
}
