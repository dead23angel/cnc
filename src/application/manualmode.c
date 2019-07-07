#include <stdlib.h>
#include "global.h"

void manualMode(void) {
	static const char axisName[4] = "XYZE";
	static const double axisK[4] = {SM_X_STEPS_PER_MM,SM_Y_STEPS_PER_MM, SM_Z_STEPS_PER_MM, SM_E_STEPS_PER_MM};
	int i, k = SM_X_STEPS_PER_MM;
	uint32_t frq[4] = {SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ,SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ,
			                 SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ,SM_MANUAL_MODE_STEPS_PER_SEC*K_FRQ};
	uint8_t dir[4] = {0,0,0,0};
	uint32_t steps[4] = {0,0,0,0};

	ili9320_Clear(0); scr_setfullTextWindow();
	scr_gotoxy(6,0); scr_fontColor(Green,Black);
	scr_puts("CNC MANUAL MODE");
	scr_fontColor(Yellow,Black);
	scr_puts("\n  key '6' - [X+]  key '4' - [X-] ");
	scr_puts("\n  key '2' - [Y+]  key '8' - [Y-] ");
	scr_puts("\n  key '1' - [Z+]  key '7' - [Z-] ");
	scr_puts("\n  key '3' - [E+]  key '9' - [E-] ");
	scr_puts("\n  'A'k+1 'B'k-1 '*'k=1 '#'k++");
	scr_puts("\n step motors 'D' - full stop");
	scr_puts("\n 0 - set zero     5 - goto zero ");
	scr_puts("\n    C - exit");
	scr_fontColor(White,Black);
	delayMs(700);
	while(kbd_getKey()>=0);
	while(TRUE) {
		for(i = 0; i < 4; i++) {
			double n = (double)stepm_getCurGlobalStepsNum(i)/axisK[i];
			scr_gotoxy(1+i*10,TEXT_Y_MAX-4); scr_printf("%c:%f  ", axisName[i], n);
   if(i < 3) {
			 scr_gotoxy(1,TEXT_Y_MAX-3+i); scr_printf("steps %c:%d      ", axisName[i], stepm_getCurGlobalStepsNum(i));
   }
  	dir[i] = 0;
  	steps[i] = 0;
		}
		scr_gotoxy(2,TEXT_Y_MAX-6); scr_printf("step per key press: %f mm ", (double)k/SM_X_STEPS_PER_MM);

		switch(kbd_getKey()) {
		 case KEY_0:
		  while(stepm_inProc()) {}
		  stepm_ZeroGlobalCrd();
		 	break;
		 case KEY_6: steps[0] = k; dir[0] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_4: steps[0] = k; dir[0] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_2: steps[1] = k; dir[1] = 1; stepm_addMove(steps, frq, dir);  break;
		 case KEY_8: steps[1] = k; dir[1] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_1: steps[2] = k; dir[2] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_7: steps[2] = k; dir[2] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_3: steps[3] = k; dir[3] = 1; stepm_addMove(steps, frq, dir); break;
		 case KEY_9: steps[3] = k; dir[3] = 0; stepm_addMove(steps, frq, dir); break;
		 case KEY_5:
		  while(stepm_inProc()) {}
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
	}
}
