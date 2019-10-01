#include <stdlib.h>
#include "global.h"

#define M_LED_X 286
#define M_LED_Y 226
#define M_LED_W 6
#define M_LED_H 7

void manualMode(void) {
	static const char axisName[4] = "XYZE";
	static uint8_t limits = 0xff;
	static const double axisK[4] = {SM_X_STEPS_PER_MM,SM_Y_STEPS_PER_MM, SM_Z_STEPS_PER_MM, SM_E_STEPS_PER_MM};
	int i, k = SM_X_STEPS_PER_MM;
	uint32_t frq[4] = {SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ,SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ,
			                 SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ,SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ};
	uint8_t dir[4] = {0,0,0,0};
	uint32_t steps[4] = {0,0,0,0};

	LCD_Clear(0);
	scr_setfullTextWindow();
	scr_gotoxy(12,0);
	scr_fontColor(Green,Black);
	scr_puts("CNC MANUAL MODE");
	scr_fontColor(Yellow,Black);
	scr_puts("\n  key '6' - [X+]  key '4' - [X-] ");
	scr_puts("\n  key '2' - [Y+]  key '8' - [Y-] ");
	scr_puts("\n  key '1' - [Z+]  key '7' - [Z-] ");
	scr_puts("\n  key '3' - [E+]  key '9' - [E-] ");
	scr_puts("\n  'A'k+1 'B'k-1 '*'k=1 '#'k++");
	scr_puts("\n step motors 'D' - full stop");
	scr_puts("\n 0 - zero  5 - goto zero C - exit");
	_delay_ms(700);
	while(kbd_getKey()>=0) {};
	while(TRUE) {
		scr_fontColor(White,Black);
		for(i = 0; i < 3; i++) {
			double n = (double)stepm_getCurGlobalStepsNum(i)/axisK[i];
			scr_gotoxy(1+i*10,TEXT_Y_MAX-4); scr_printf("%c:%f  ", axisName[i], n);
   if(i < 3) {
			 scr_gotoxy(1,TEXT_Y_MAX-3+i); scr_printf("steps %c:%d      ", axisName[i], stepm_getCurGlobalStepsNum(i));
   }
#ifdef HAS_ENCODER
   if(i == 2) {
   	int32_t v = encoderZvalue();
	   scr_gotoxy(25,TEXT_Y_MAX-1); scr_printf("encZ:%d    ", v);
	   scr_gotoxy(1+2*10,TEXT_Y_MAX-5); scr_printf("errZ:%f  ", (double)(v*MM_PER_360)/ENCODER_Z_CNT_PER_360 - n);
   }
#endif
  	dir[i] = 0;
  	steps[i] = 0;
		}
		scr_fontColor(Blue,Black);
	 scr_gotoxy(2,TEXT_Y_MAX-7); scr_printf("step per key press: %f mm ", (double)k/SM_X_STEPS_PER_MM);

		switch(kbd_getKey()) {
		 case KEY_0:
		  while(stepm_inProc() && kbd_getKey() != KEY_C) {}
		  stepm_ZeroGlobalCrd();
#ifdef HAS_ENCODER
		  encoderZreset();
		  encoderCorrectionCntUp = encoderCorrectionCntDown = encoderCorrectionDelta = encoderCorrectionMaxDelta = 0;
#endif
		  break;
		 case KEY_6: steps[0] = k; dir[0] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_4: steps[0] = k; dir[0] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_2: steps[1] = k; dir[1] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_8: steps[1] = k; dir[1] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_1: steps[2] = k; dir[2] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_7: steps[2] = k; dir[2] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_3: steps[3] = k; dir[3] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_9: steps[3] = k; dir[3] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_5:
		  while(stepm_inProc() && kbd_getKey() != KEY_C) {}
		  if(stepm_getCurGlobalStepsNum(0) != 0 || stepm_getCurGlobalStepsNum(1) != 0) {
	 	 	steps[0] = labs(stepm_getCurGlobalStepsNum(0)); dir[0] = stepm_getCurGlobalStepsNum(0) < 0;
	 	 	steps[1] = labs(stepm_getCurGlobalStepsNum(1)); dir[1] = stepm_getCurGlobalStepsNum(1) < 0;
	 	 	steps[2] = 0; dir[2] = 0;
		  } else {
		  	steps[0] = steps[1] = 0; dir[0] = dir[1] = 0;
		  	steps[2] = labs(stepm_getCurGlobalStepsNum(2)); dir[2] = stepm_getCurGlobalStepsNum(2) < 0;
		 	}
	 	 stepm_addMove(steps, frq, dir);
		 	break;
		 case KEY_A:
		 	if(k < SM_X_STEPS_PER_MM) k++;
		 	break;
		 case KEY_B:
		 	if(k > 1) k--;
		 	break;
		 case KEY_STAR:
		 	k = 1;
		 	break;
		 case KEY_DIES:
		 	if(k < SM_X_STEPS_PER_MM) k+=SM_X_STEPS_PER_MM/8;
		 	break;
		 case KEY_C:
		 	stepm_EmergeStop();
		 	return;
		 case KEY_D:
		 	stepm_EmergeStop();
		 	break;
		}
	 if(limits != limits_chk()) {
	 	limits = limits_chk();
		GUI_Rectangle(M_LED_X,                   M_LED_Y,     M_LED_X + M_LED_W * 4 + 4, M_LED_Y + M_LED_H + 2, Grey, TRUE);
		GUI_Rectangle(M_LED_X + M_LED_W * 0 + 1, M_LED_Y + 1, M_LED_X + M_LED_W * 1 + 0, M_LED_Y + M_LED_H + 1, Black, TRUE);
		GUI_Rectangle(M_LED_X + M_LED_W * 1 + 2, M_LED_Y + 1, M_LED_X + M_LED_W * 2 + 1, M_LED_Y + M_LED_H + 1, limitX_chk()? Red:Green, TRUE);
		GUI_Rectangle(M_LED_X + M_LED_W * 2 + 3, M_LED_Y + 1, M_LED_X + M_LED_W * 3 + 2, M_LED_Y + M_LED_H + 1, limitY_chk()? Red:Green, TRUE);
		GUI_Rectangle(M_LED_X + M_LED_W * 3 + 4, M_LED_Y + 1, M_LED_X + M_LED_W * 4 + 3, M_LED_Y + M_LED_H + 1, limitZ_chk()? Red:Green, TRUE);
	 }
	}
}
