#include "global.h"

//ErrorStatus HSEStartUpStatus ;
RCC_ClocksTypeDef RCC_ClockFreq;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev  __ALIGN_END;

#ifdef  __cplusplus
extern "C" {
#endif
uint32_t GetCpuClock()
  {
    return RCC_ClockFreq.SYSCLK_Frequency ;
  }
#ifdef  __cplusplus
  }
#endif

void SystemStartup(void) {
	/* Unlock the internal flash */
	//FLASH_Unlock();
	/* RCC system reset(for debug purpose) */
	//RCC_DeInit();
	//RCC_GetClocksFreq(&RCC_ClockFreq);
	SystemInit();
	RCC_GetClocksFreq(&RCC_ClockFreq);
	//SysTick_Config(SystemFrequency / 1000);
	SysTick_Config(168000);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//-------------  GPIO  configure ----------
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);
	//rs232_init();
	kbd_init();
	stepm_init();
	spindle_init();
	#ifdef HAS_ENCODER
		encoder_int();
	#endif
	limits_init();
	RTC_Configuration();
	LCD_Init();
	LCD_Clear(0);
	win_showMsgWin();
	//_delay_ms(500);
	scr_puts("        ---- MMCNC ----");
	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb, &USR_cb);
}
