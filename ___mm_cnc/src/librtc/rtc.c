/********************************************************************************/
/*!
	@file			rtc_support.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2011.06.12
	@brief          Based on ST Microelectronics's Sample Thanks!

    @section HISTORY
		2011.06.12	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
RTC_InitTypeDef   RTC_InitStructure;
RTC_TimeTypeDef   RTC_TimeStructure;
RTC_DateTypeDef   RTC_DateStructure;
__IO uint32_t AsynchPrediv = 0;
__IO uint32_t SynchPrediv = 0;
//__IO uint32_t TimeDisplay = 0;
static const uint8_t DaysInMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/*! 
    RealTimeClockmodule Initialize
*/
/**************************************************************************/
void RTC_Configuration(void)
{
	/* Enable the PWR APB1 Clock Interface */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
  
	if (RTC_ReadBackupRegister(RTC_BKP_DR2) != 0xA5A5) {

		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
		/* The RTC Clock may varies due to LSI frequency dispersion. */
		/* Enable the LSI OSC */ 
		RCC_LSICmd(ENABLE);

		/* Wait till LSI is ready */  
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

		SynchPrediv = 0xFF;
		AsynchPrediv = 0x7F;

		#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
		/* Enable the LSE OSC */
		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */  
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		SynchPrediv = 0xFF;
		AsynchPrediv = 0x7F;

		#else
		#error Please select the RTC Clock source inside the main.c file
		#endif /* RTC_CLOCK_SOURCE_LSI */

		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();
		
		/* Allow access to BKP Domain */
		PWR_BackupAccessCmd(ENABLE);
	
		/* Write to the first RTC Backup Data Register */
		RTC_WriteBackupRegister(RTC_BKP_DR2,0xA5A5);

		/* Set the Time */
		RTC_TimeStructure.RTC_Hours   = 20;
		RTC_TimeStructure.RTC_Minutes = 14;
		RTC_TimeStructure.RTC_Seconds = 00;

		/* Set the Date */
		RTC_DateStructure.RTC_Month = 10;
		RTC_DateStructure.RTC_Date  = 24;
		RTC_DateStructure.RTC_Year  = 13;
		RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Thursday;

		/* Calendar Configuration */
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv  = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);

		/* Set Current Time and Date */
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);  
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure); 
#if 0
		/* Configure the RTC Wakeup Clock source and Counter (Wakeup event each 1 second) */
		RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
		RTC_SetWakeUpCounter(0x7FF);

		/* Enable the Wakeup Interrupt */
		RTC_ITConfig(RTC_IT_WUT, ENABLE);

		/* Enable Wakeup Counter */
		RTC_WakeUpCmd(ENABLE); 
#endif
		/*  Backup SRAM ***************************************************************/
		/* Enable BKPRAM Clock */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

		/* Enable the Backup SRAM low power Regulator to retain it's content in VBAT mode */
		PWR_BackupRegulatorCmd(ENABLE);

		/* Wait until the Backup SRAM low power Regulator is ready */
		while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
		{
		}

	}
	else{
	
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();
		/* Clear the Wakeup Interrupt */
		RTC_ClearITPendingBit(RTC_IT_WUT);

		/*  Backup SRAM ***************************************************************/
		/* Enable BKPSRAM Clock */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
	}


}

uint32_t RTC_GetCounter(void){
	uint8_t i;
	uint32_t result = 0;
	uint16_t idx, year;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	//ts_mday, ts_mon, ts_year/*-2000*/, ts_hour, ts_min, ts_sec

	year = ts_year;

	/* Calculate days of years before */
	result = (uint32_t)year * 365;
	if (ts_year >= 1) {
		result += (year + 3) / 4;
		result -= (year - 1) / 100;
		result += (year - 1) / 400;
	}

	/* Start with 2000 a.d. */
	//result -= 730485UL;

	/* Make month an array index */
	idx = ts_month - 1;

	/* Loop thru each month, adding the days */
	for (i = 0; i < idx; i++) {
		result += DaysInMonth[i];
	}

	/* Leap year? adjust February */
	if (year%400 == 0 || (year%4 == 0 && year%100 !=0)) {
		;
	} else {
		if (ts_month > 1) {
			result--;
		}
	}

	/* Add remaining days */
	result += ts_mday;

	/* Convert to seconds, add all the other stuff */
	result = (result-1) * 86400L + (uint32_t)ts_hour * 3600 +
		(uint32_t)ts_min * 60 + ts_sec;

	return result;
}

/* End Of File ---------------------------------------------------------------*/
