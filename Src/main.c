/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "global.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CONF_FILE_NAME "sm.conf"

#define MAX_FILE_LIST_SZ 256
#define MAX_FILE_NAME_SZ 80

#define FILE_LIST_ROWS 8
#define L_LED_X 286
#define L_LED_Y 226
#define L_LED_W 6
#define L_LED_H 7
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static char fileList[MAX_FILE_LIST_SZ][MAX_FILE_NAME_SZ], loadedFileName[MAX_FILE_NAME_SZ];
static int fileListSz = 0, curFile = 0, firstFileInWin = 0;
static FATFS fatfs;
//FLASH_VALUES commonValues;

uint8_t usb_sts = 0; // TODO переделать когда будет usb
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char *str_trim(char *str) {
	for(int n = 0; str[n] != 0; n++) {
		if(str[n] == '\n' || str[n] == '\r') {
			str[n] = 0; break;
		}
	}
	return str;
}

static void initSmParam(int8_t isSaveFile) {
	_smParam.smoothStartF_from0[0] = SM_SMOOTH_START_X*K_FRQ;
	_smParam.smoothStartF_from0[1] = SM_SMOOTH_START_Y*K_FRQ;
	_smParam.smoothStartF_from0[2] =  SM_SMOOTH_START_Z*K_FRQ;
	_smParam.smoothStopF_to0[0] = SM_SMOOTH_STOP_X*K_FRQ;
	_smParam.smoothStopF_to0[1] = SM_SMOOTH_STOP_Y*K_FRQ;
	_smParam.smoothStopF_to0[2] = SM_SMOOTH_STOP_Z*K_FRQ;
	_smParam.smoothAF[0]= SM_SMOOTH_DFEED_X*SM_X_STEPS_PER_MM*SM_SMOOTH_TFEED*K_FRQ/1000;
	_smParam.smoothAF[1]= SM_SMOOTH_DFEED_Y*SM_Y_STEPS_PER_MM*SM_SMOOTH_TFEED*K_FRQ/1000;
	_smParam.smoothAF[2]= SM_SMOOTH_DFEED_Z*SM_Z_STEPS_PER_MM*SM_SMOOTH_TFEED/1000*K_FRQ;
	_smParam.maxFeedRate[0] = SM_X_MAX_STEPS_PER_SEC*K_FRQ;
	_smParam.maxFeedRate[1] = SM_Y_MAX_STEPS_PER_SEC*K_FRQ;
	_smParam.maxFeedRate[2] = SM_Z_MAX_STEPS_PER_SEC*K_FRQ;
	_smParam.maxSpindleTemperature = MAX_SPINDEL_TEMPERATURE;
	FIL fid;
	FRESULT fres = f_open(&fid, CONF_FILE_NAME, FA_READ);
	char str[256], *p;
	if(fres == FR_OK) {
		scr_printf("\n loading %s", CONF_FILE_NAME);
		for(int i = 0; i < 3; i++) {
			scr_puts(".");
			if(f_gets(str, sizeof(str), &fid) == NULL) break;
			DBG("\nc:%d:'%s'", i, str);
			if(f_gets(str, sizeof(str), &fid) == NULL) break;
			DBG("\nd:%d:'%s'", i, str);
			p = str;
			_smParam.smoothStartF_from0[i] = strtod_M(p, &p);
			_smParam.smoothStopF_to0[i] = strtod_M(p, &p);
			_smParam.smoothAF[i] = strtod_M(p, &p);
			_smParam.maxFeedRate[i] = strtod_M(p, &p);
		}
		if(f_gets(str, sizeof(str), &fid) != NULL) {
			DBG("t:'%s'", str);
			if(f_gets(str, sizeof(str), &fid) != NULL) _smParam.maxSpindleTemperature = strtod_M(str, &p);
		}
		scr_puts("*");
		f_close(&fid);
		scr_puts(" OK");
	}
	if(!isSaveFile) return;
	//--------------------------------------
	f_unlink(CONF_FILE_NAME);
	if((fres = f_open(&fid, CONF_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
		win_showErrorWin();
		scr_printf("Error save file:'%s'\n Status:%d", CONF_FILE_NAME, fres);
		scr_puts("\n\n\t PRESS C-KEY");
		while(kbd_getKey() != KEY_C);
		return;
	}

	scr_printf("\n save into %s", CONF_FILE_NAME);
	for(int i = 0; i < 3; i++) {
		f_printf(&fid, "Crd%d (F:steps*%d/sec): 'smoothStartF_from0,smoothStopF_to0,smoothAF,maxFeedRate\n", i, K_FRQ);
		f_printf(&fid, "%d,%d,%d,%d,%d\n",_smParam.smoothStartF_from0[i],_smParam.smoothStopF_to0[i],_smParam.smoothAF[i],_smParam.maxFeedRate[i]);
		scr_puts(".");
	}
	f_printf(&fid, "Spindle switch-off temperature (C.degree)\n");
	f_printf(&fid, "%d\n", _smParam.maxSpindleTemperature);
	scr_puts("*");
	f_close(&fid);
	scr_puts(" OK");
	scr_gotoxy(2,4);
	scr_puts("CURRENT CONFIGURATION SAVED");
}

static void showStatusString(void) {
	static uint8_t sec = 0, limits = 0xff;
	HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);

	if(ts_sec != sec) {
		scr_setfullTextWindow();
		scr_gotoxy(1,TEXT_Y_MAX-1);
		scr_fontColor(White,Black);
		scr_printf("%02d.%02d.%02d %02d:%02d:%02d  %s", ts_mday, ts_month, ts_year, ts_hour, ts_min, ts_sec, usb_sts ? "USB" : "   ");
		sec = ts_sec;
		limits = 0xff;
		//if(!(SD_CD_PORT->IDR & SD_CD_PIN)) COOLANT_PORT->BSRRL = COOLANT_PIN; else COOLANT_PORT->BSRRH = COOLANT_PIN;
		//if((SD_WP_PORT->IDR & SD_WP_PIN)) SPINDLE_PWM_PORT->BSRRL = SPINDLE_PWM_PIN; else SPINDLE_PWM_PORT->BSRRH = SPINDLE_PWM_PIN;
	}
	if(limits != limits_chk()) {
		limits = limits_chk();
		GUI_Rectangle(L_LED_X,                   L_LED_Y,     L_LED_X + L_LED_W * 4 + 4, L_LED_Y + L_LED_H + 2, Grey, TRUE);
		GUI_Rectangle(L_LED_X + L_LED_W * 1 + 2, L_LED_Y + 1, L_LED_X + L_LED_W * 2 + 1, L_LED_Y + L_LED_H + 1, limitX_chk()? Red:Green, TRUE);
		GUI_Rectangle(L_LED_X + L_LED_W * 2 + 3, L_LED_Y + 1, L_LED_X + L_LED_W * 3 + 2, L_LED_Y + L_LED_H + 1, limitY_chk()? Red:Green, TRUE);
		GUI_Rectangle(L_LED_X + L_LED_W * 3 + 4, L_LED_Y + 1, L_LED_X + L_LED_W * 4 + 3, L_LED_Y + L_LED_H + 1, limitZ_chk()? Red:Green, TRUE);
	}
}

void showCriticalStatus(char *msg, int st) {
	win_showErrorWin();
	scr_gotoxy(2, 2);
	scr_printf(msg, st);
	while(1);
}

static void readFileList(void) {
	FRESULT fres;
	FILINFO finfo;
	DIR dirs;

	win_showMsgWin();
	scr_gotoxy(2, 0);
	scr_printf("Open SD dir.. ");
	char path[50]={""};
	if((fres = f_opendir(&dirs, path)) != FR_OK) showCriticalStatus(" f_opendir()\n  error [code:%d]\n  Only RESET possible at now", fres);
	scr_puts("\nRead file list from SD");
	static char lfn[_MAX_LFN + 1];
//	finfo.fname = lfn; TODO
	finfo.fsize = sizeof(lfn);
	for(fileListSz = 0; f_readdir(&dirs, &finfo) == FR_OK && fileListSz < MAX_FILE_LIST_SZ;) {
		scr_gotoxy(0, 3);
		scr_printf("files:[%d]", fileListSz);
		if(!finfo.fname[0]) break;
		if(finfo.fname[0] == '.') continue;
		if(!(finfo.fattrib & AM_DIR) && strcmp(CONF_FILE_NAME, *finfo.fname? finfo.fname:finfo.fname) != 0) {
			strncpy(&fileList[fileListSz++][0], *finfo.fname? finfo.fname:finfo.fname, MAX_FILE_NAME_SZ);
		}
	}
	if(loadedFileName[0] != 0) { // set last loaded file as selected file
		scr_printf("\nselect:'%s'..", loadedFileName);
			for(int i = 0; i < fileListSz; i++) {
				if(!strcmp(&fileList[i][0], &loadedFileName[0])) {
					loadedFileName[0] = 0; // reset for next dir reload
					curFile = i;
					if(curFile < FILE_LIST_ROWS) firstFileInWin = 0;
					else if((fileListSz - curFile) < FILE_LIST_ROWS) firstFileInWin = fileListSz-FILE_LIST_ROWS;
							else firstFileInWin = curFile;
					break;
				}
			}
		scr_printf("\npos in win/cur file:%d/%d", firstFileInWin, curFile);
	}
	scr_puts("\n         ---- OK -----");
}

static void drawFileList(void) {
	if(curFile >= fileListSz) curFile = 0;
	if(curFile < 0) curFile = fileListSz-1;
	if((firstFileInWin+curFile) >= FILE_LIST_ROWS) firstFileInWin=curFile-FILE_LIST_ROWS+1;
	if(firstFileInWin > curFile) firstFileInWin = curFile;
	if(firstFileInWin < 0) firstFileInWin = 0;
	//win_showMenuScroll(16, 0,	36, FILE_LIST_ROWS, firstFileInWin, curFile, fileListSz);
	win_showMenuScroll(8, 0,	36, FILE_LIST_ROWS, firstFileInWin, curFile, fileListSz);
	for(int i = 0; i < fileListSz; i++) {
		if(i == curFile) scr_fontColorInvers();
		else scr_fontColorNormal();
		scr_printf("%s\n", &fileList[i][0]);
	}
}

uint8_t questionYesNo(char *msg, char *param) {
	win_showMsgWin();
	scr_printf(msg, param);
	scr_gotoxy(5,6); scr_puts("[D] - OK,  [C] - Cancel");
	while(TRUE) {
		switch(kbd_getKey()) {
			case KEY_D: return TRUE;
			case KEY_C: return FALSE;
		}
	}
	return FALSE;
}

static void setTime(void) {
	int c = -1, pos = 0, v = 0;
	win_showMsgWin();
	scr_setScrollOn(FALSE);
	HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
	scr_puts("D-ENTER C-CANCEL A-Up B-Down");
	scr_puts("\n'*' -Left '#' -RIGHT");
	scr_printf("\n\nTime: %02d.%02d.%02d %02d:%02d:%02d", ts_mday, ts_month, ts_year/*-2000*/, ts_hour, ts_min, ts_sec);
	do {
		if(c == KEY_STAR) pos = pos <= 0? 5:pos-1;  // fix kolyat
		if(c == KEY_DIES) pos = pos >= 5? 0:pos+1;
		switch(pos) {
			case 0:
				v = ts_mday;
				break;
			case 1:
				v = ts_month;
				break;
			case 2:
				v = ts_year;//-2000;
				break;
			case 3:
				v = ts_hour;
				break;
			case 4:
				v = ts_min;
				break;
			case 5:
				v = ts_sec;
				break;
		}
		//if(c == KEY_STAR) pos = pos <= 0? 5:pos-1;  // fix kolyat
		//if(c == KEY_DIES) pos = pos >= 5? 0:pos+1;
		scr_gotoxy(0,4);
		scr_fontColorNormal();
		scr_printf(" New: %02d.%02d.%02d %02d:%02d:%02d", ts_mday, ts_month, ts_year/*-2000*/, ts_hour, ts_min, ts_sec);
		scr_fontColorInvers();
		scr_gotoxy(pos*3+6,4); //scr_gotoxy(pos*3+6,3); // fix kolyat
		scr_printf("%02d", v);
		while((c = kbd_getKey()) < 0);
		if(c == KEY_A) v++;
		if(c == KEY_B) v--;
		switch(pos) {
			case 0:
				if(v >= 1 && v <= 31)  ts_mday = v;
				break;
			case 1:
				if(v >= 1 && v <= 12)  ts_month = v;
				break;
			case 2:
				if(v >= 12 && v <= 30) ts_year = v;//+2000;
				break;
			case 3:
				if(v >= 0 && v <= 23)  ts_hour = v;
				break;
			case 4:
				if(c >= 0 && v <= 59)  ts_min = v;
				break;
			case 5:
				if(v >= 0 && v <= 59)  ts_sec = v;
				break;
		}
	} while(c != KEY_C && c != KEY_D);
	if(c == KEY_D){
		HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_FSMC_Init();
  MX_SDIO_SD_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_FATFS_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  SystemStartup();
  FRESULT fres;
  fres = f_mount(&fatfs, "", 1);
  if(fres != FR_OK ) showCriticalStatus(" Mount SD error [code:%d]\nSD card used for any CNC process   Only RESET possible at now", fres);
  initSmParam(FALSE);
  scr_puts("\n         ---- OK -----");
  #ifdef SHOW_INIT_SCR
  	  scr_puts("\n\n\n\nPRESS [D] KEY FOR CONTINUE");
  	  scr_waitKey(KEY_D);
  #endif
  uint8_t rereadDir = TRUE, redrawScr = TRUE, redrawDir = TRUE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  char str[150];
	  if (rereadDir) {
		  readFileList();
		  redrawScr = TRUE;
		  rereadDir = FALSE;
	  }
	  // ---------------------
	  if (redrawScr) {
		  redrawScr = FALSE;
		  LCD_Clear(0);
		  win_showMenu(8,144,37,4); //win_showMenu(18,144,36,4);
		  scr_puts(
				  "0 - start gcode\t   1 - manual mode\n"
				  "2 - show gcode\t   3 - delete file\n"
				  "4 - set time\t   5 - file info\n"
				  "6 - refresh dir\t   7 - save config");
		  redrawDir = TRUE;
	  }

	  if (redrawDir) {
		  redrawDir = FALSE;
		  drawFileList();
	  }
	  // ---------------------
	  showStatusString();
	  // ---------------------
	  switch(kbd_getKey()) {
	  			//------------------------------------------
	  			case KEY_A:
	  				curFile--;
	  				redrawDir = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_B:
	  				curFile++;
	  				redrawDir = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_STAR:
	  				curFile+=FILE_LIST_ROWS;
	  				redrawDir = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_DIES:
	  				curFile-=FILE_LIST_ROWS;
	  				redrawDir = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_0:
	  				while(kbd_getKey() != -1);
	  				uint32_t stime = RTC_GetCounter();
	  				cnc_gfile(&fileList[curFile][0], GFILE_MODE_MASK_EXEC);
	  				while(stepm_inProc()) {
	  					scr_fontColor(Yellow,Blue);
	  					scr_gotoxy(0,13);
	  					scr_printf(" remain moves: %d", stepm_getRemainLines());
	  					scr_clrEndl();
	  				}
	  				stepm_EmergeStop();
	  				scr_fontColor(White,Black);
	  				scr_gotoxy(30,12);
	  				scr_clrEndl();
	  				scr_fontColor(Yellow,Blue);
	  				stime = RTC_GetCounter() - stime;
	  				scr_fontColor(Yellow,Blue);
	  				scr_gotoxy(0,13);
	  				scr_printf("      FINISH. Work time: %02d:%02d:%02d", stime/3600, (stime/60)%60, stime%60);
	  				scr_clrEndl();
	  				scr_gotoxy(0,14);
	  				scr_puts("             PRESS [C] KEY");
	  				scr_clrEndl();
	  				scr_gotoxy(26,14);
	  				while(kbd_getKey() != -1);
	  				_delay_ms(300);
	  				scr_waitKey(KEY_C);
	  				redrawScr = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_1:
	  				manualMode();
	  				redrawScr = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_2:
	  				while(kbd_getKey() != -1);
	  				cnc_gfile(&fileList[curFile][0], GFILE_MODE_MASK_SHOW|GFILE_MODE_MASK_CHK);
	  				scr_printf("\n             PRESS [C] KEY");
	  				while(kbd_getKey() != -1);
	  				scr_waitKey(KEY_C);
	  				redrawScr = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_3:
	  				if(HAL_GPIO_ReadPin(SD_WR_GPIO_Port, SD_WR_Pin) == GPIO_PIN_SET) {
	  					if(questionYesNo("Delete file:\n'%s'?", &fileList[curFile][0])) {
	  						rereadDir = TRUE;
	  						f_unlink(&fileList[curFile][0]);
	  					} else redrawScr = TRUE;
	  				} else {
	  					win_showMsgWin();
	  					scr_gotoxy(6,2);
	  					scr_puts("CARD WRITE PROTECTED");
	  					scr_gotoxy(9,5);
	  					scr_puts("PRESS [C] KEY");
	  					scr_waitKey(KEY_C);
	  					redrawScr = TRUE;
	  				}
	  				break;
	  			//------------------------------------------
	  			case KEY_4:
	  				setTime();
	  				redrawScr = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_5: {
	  				FILINFO finf;
	  				memset(&finf, 0, sizeof(finf));
	  				win_showMsgWin();
	  				scr_setScrollOn(FALSE);
	  				scr_printf("file:'%s'", &fileList[curFile][0]);
	  				f_stat(&fileList[curFile][0], &finf);
	  				scr_gotoxy(0, 1);
	  				scr_printf("File size:%d\n", (uint32_t)finf.fsize);
	  				FIL fid;
	  				FRESULT fres = f_open(&fid, &fileList[curFile][0], FA_READ);
	  				if(fres != FR_OK) {
	  					scr_printf("Error open file: '%s'\nStatus:%d", &fileList[curFile][0], fres);
	  				} else {
	  					scr_fontColorInvers();
	  					scr_setScrollOn(FALSE);
	  					for(int n = 2; n < 7 && f_gets(str, sizeof(str), &fid) != NULL; n++) {
	  						scr_gotoxy(0, n);
	  						scr_puts(str_trim(str));
	  					}
	  				}
	  				scr_fontColorNormal();
	  				scr_gotoxy(8, 7);
	  				scr_printf("PRESS C-KEY");
	  				redrawScr = TRUE;
	  				int c;
	  				do {
	  					c = kbd_getKey();
	  					if(c == KEY_B) {
	  						scr_fontColorInvers();
	  						for(int n = 2; n < 7 && f_gets(str, sizeof(str), &fid) != NULL; n++) {
	  							scr_gotoxy(0, n);
	  							scr_puts(str_trim(str));
	  							scr_clrEndl();
	  						}
	  					}
	  				} while(c != KEY_C);
	  				f_close(&fid);
	  				rereadDir = TRUE;
	  			}
	  				break;
	  			//------------------------------------------
	  			case KEY_6:
	  				rereadDir = TRUE;
	  				break;
	  			//------------------------------------------
	  			case KEY_7:
	  				win_showMsgWin();
	  				if(HAL_GPIO_ReadPin(SD_WR_GPIO_Port, SD_WR_Pin) == GPIO_PIN_SET) {
	  					initSmParam(true);
	  				} else {
	  					scr_gotoxy(6,2);
	  					scr_puts("CARD WRITE PROTECTED");
	  				}
	  				scr_gotoxy(9,6);
	  				scr_puts("PRESS [C] KEY");
	  				scr_waitKey(KEY_C);
	  				redrawScr = TRUE;
	  				break;
	  		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
