#include <stdlib.h>
#include <string.h>
#include "global.h"

#define STEPS_BUF_SZ 8
typedef struct {
	uint32_t steps[4];
	uint32_t pscValue[4], arrValue[4], f[4];
	uint8_t dir[4];
} LINE_DATA;

static volatile LINE_DATA steps_buf[STEPS_BUF_SZ];
static int32_t steps_buf_sz, steps_buf_p1, steps_buf_p2;

static LINE_DATA cur_steps_buf; // for debug only

volatile struct {
	int32_t globalSteps;
	uint32_t steps;
	uint8_t clk, dir, isInProc;
} step_motors[4];

static void stepm_powerOff(uint8_t id) {
	steps_buf_sz = steps_buf_p1 = steps_buf_p2 = 0;
	switch(id) {
		case 0: HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET); break;
		case 1: HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET); break;
		case 2: HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET); break;
		case 3: HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET); break;
	}
}

void stepm_init(void) {
	for(int i = 0; i < 4; i++) {
		step_motors[i].steps = 0;
		step_motors[i].clk = TRUE;
		step_motors[i].isInProc = FALSE;
		step_motors[i].globalSteps = 0;
	}
	steps_buf_sz = steps_buf_p1 = steps_buf_p2 = 0;

	stepm_powerOff(0);
	stepm_powerOff(1);
	stepm_powerOff(2);
	stepm_powerOff(3);
} // end stepm_init()

void stepm_proc(uint8_t id) {
	if(limits_chk()) {
		stepm_EmergeStop();
		return;
	}

	if(step_motors[id].isInProc) {
		switch(id) {
			case 0:
				HAL_GPIO_WritePin(M0_STEP_GPIO_Port, M0_STEP_Pin, step_motors[id].clk ? GPIO_PIN_RESET : GPIO_PIN_SET);
				break;
			case 1:
				HAL_GPIO_WritePin(M1_STEP_GPIO_Port, M1_STEP_Pin, step_motors[id].clk ? GPIO_PIN_RESET : GPIO_PIN_SET);
				break;
			case 2:
				HAL_GPIO_WritePin(M2_STEP_GPIO_Port, M2_STEP_Pin, step_motors[id].clk ? GPIO_PIN_RESET : GPIO_PIN_SET);
				break;
			case 3:
				HAL_GPIO_WritePin(M3_STEP_GPIO_Port, M3_STEP_Pin, step_motors[id].clk ? GPIO_PIN_RESET : GPIO_PIN_SET);
				break;
		}
		if(!step_motors[id].clk) {
			if(step_motors[id].steps!=0) step_motors[id].steps--;

			if(step_motors[id].dir) step_motors[id].globalSteps++;
			else step_motors[id].globalSteps--;
		} else {
			if(step_motors[id].steps == 0) step_motors[id].isInProc = FALSE;
		}
		step_motors[id].clk = !step_motors[id].clk;

	} else {
		if(steps_buf_sz > 0 && !step_motors[0].isInProc && !step_motors[1].isInProc && !step_motors[2].isInProc && !step_motors[3].isInProc) {
			__disable_irq();
			LINE_DATA *p = (LINE_DATA *)(&steps_buf[steps_buf_p1]);
			memcpy(&cur_steps_buf, p, sizeof(cur_steps_buf)); // for debug

			for(int i = 0; i < 4; i++) {
				step_motors[i].steps = p->steps[i];
				step_motors[i].dir = p->dir[i];
			}

			if(step_motors[0].steps) {
				step_motors[0].isInProc = TRUE;
				HAL_GPIO_WritePin(M0_DIR_GPIO_Port, M0_DIR_Pin, p->dir[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
				HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET);
				TIM2->PSC = p->pscValue[0];
				__HAL_TIM_SET_AUTORELOAD(&htim2, p->arrValue[0]);
			}

			if(step_motors[1].steps) {
				step_motors[1].isInProc = TRUE;
				HAL_GPIO_WritePin(M1_DIR_GPIO_Port, M1_DIR_Pin, p->dir[1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
				HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET);
				TIM3->PSC = p->pscValue[1];
				__HAL_TIM_SET_AUTORELOAD(&htim3, p->arrValue[1]);
			}

			if(step_motors[2].steps) {
#ifdef HAS_ENCODER
	 		if(isEncoderCorrection && step_motors[2].steps > ENCODER_CORRECTION_MIN_STEPS) {
		 		int32_t enVal = encoderZvalue();
		 		int32_t globalSteps = stepm_getCurGlobalStepsNum(2);
	 			int32_t dv = enVal*MM_PER_360*1000L/ENCODER_Z_CNT_PER_360 - globalSteps*MM_PER_360*1000L/SM_Z_STEPS_PER_360; // delta in 1/1000 mm
	 			encoderCorrectionDelta = dv; // TODO for debug.. will be removed
	 			if(labs(encoderCorrectionMaxDelta) < labs(encoderCorrectionDelta)) encoderCorrectionMaxDelta = encoderCorrectionDelta;

	 			if(dv > ENCODER_Z_MIN_MEASURE) {
	 				encoderCorrectionCntDown++; // TODO for debug.. will be removed

	 				step_motors[2].globalSteps++;
	 				if(step_motors[2].dir) step_motors[2].steps--;
	 				else step_motors[2].steps++;
	 			}
	 			if(dv < -ENCODER_Z_MIN_MEASURE) {
	 				encoderCorrectionCntUp++; // TODO for debug.. will be removed

	 				step_motors[2].globalSteps--;
	 				if(step_motors[2].dir) step_motors[2].steps++;
	 				else step_motors[2].steps--;
	 			}
	 		}
#endif
	 			step_motors[2].isInProc = TRUE;
	 			HAL_GPIO_WritePin(M2_DIR_GPIO_Port, M2_DIR_Pin, p->dir[2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
	 			HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET);
	 			TIM4->PSC = p->pscValue[2];
	 			__HAL_TIM_SET_AUTORELOAD(&htim4, p->arrValue[2]);
			}

			if(step_motors[3].steps) {
				step_motors[3].isInProc = TRUE;
				HAL_GPIO_WritePin(M3_DIR_GPIO_Port, M3_DIR_Pin, p->dir[3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
				HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET);
				TIM5->PSC = p->pscValue[3];
				__HAL_TIM_SET_AUTORELOAD(&htim5, p->arrValue[3]);
			}

			steps_buf_p1++;
			if(steps_buf_p1 >= STEPS_BUF_SZ) steps_buf_p1 = 0;
			steps_buf_sz--;
			__enable_irq();

		} // end if(steps_buf_sz...

	} // end else if(step_motors[id].isInProc)

}

void stepm_EmergeStop(void) {
	for(int i = 0; i < 4; i++) {
		stepm_powerOff(i);
		step_motors[i].isInProc = FALSE;
		step_motors[i].steps = 0;
	}
	steps_buf_sz = steps_buf_p1 = steps_buf_p2 = 0;
}

void stepm_addMove(uint32_t steps[4], uint32_t frq[4], uint8_t dir[4]) {
	if(steps[0] == 0 && steps[1] == 0 && steps[2] == 0) return;

	while(steps_buf_sz >= STEPS_BUF_SZ) __NOP();

	LINE_DATA *p = (LINE_DATA *)(&steps_buf[steps_buf_p2]);

	for(int i = 0; i < 3;i++) {
		uint32_t f = frq[i];

		if(f > (15000*K_FRQ)) f = 15000*K_FRQ; //Max freq: 15kHz
		if(f < K_FRQ) f = K_FRQ; //Min freq: 1Hz

		uint32_t pscValue=1;
		uint32_t arrValue=(84000000UL/2*K_FRQ)/f; // (1 falling age on 2 IRQ)

		while((arrValue & 0xffff0000) != 0) {
			pscValue = pscValue << 1;
			arrValue = arrValue >> 1;
		}

		pscValue--;

		p->f[i] = frq[i]; // for debug
		p->arrValue[i] = arrValue;
		p->pscValue[i] = pscValue;
		p->dir[i] = dir[i];
		p->steps[i] = steps[i];
	}

	steps_buf_p2++;
	if(steps_buf_p2 >= STEPS_BUF_SZ) steps_buf_p2 = 0;
	__disable_irq();
	steps_buf_sz++;
	__enable_irq();
}

int32_t stepm_getRemainLines(void) {
	return steps_buf_sz;
}

int32_t stepm_inProc(void) {
	if(steps_buf_sz > 0) return TRUE;
	for(int i = 0; i < 4; i++)
  if(step_motors[i].isInProc) return TRUE;
	return FALSE;
}

uint32_t stepm_LinesBufferIsFull(void) {
	return steps_buf_sz >= STEPS_BUF_SZ;
}

int32_t stepm_getCurGlobalStepsNum(uint8_t id) {
	return step_motors[id].globalSteps;
}

void stepm_ZeroGlobalCrd(void) {
	for(int i = 0; i < 4; i++) {
		step_motors[i].globalSteps = 0;
	}
}

void step_dump() {
	LINE_DATA *p = (LINE_DATA *)(&cur_steps_buf);
  for(int i = 0; i < 4; i++) {
	  scr_gotoxy(1,7+i);	scr_printf("%d,%d,%d,%d [%d]   ",	p->steps[i], p->dir[i], p->pscValue[i], p->arrValue[i], p->f[i]); // TODO
  }
}
