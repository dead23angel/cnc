#include "global.h"

void spindle_init(void) {
	// init spindle control pins:
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Pin = SPINDLE_ON_PIN;
	GPIO_Init(SPINDLE_ON_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPINDLE_DIR_PIN;
	GPIO_Init(SPINDLE_DIR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPINDLE_PWM_PIN;
	GPIO_Init(SPINDLE_PWM_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = COOLANT_PIN;
	GPIO_Init(COOLANT_PORT, &GPIO_InitStructure);

	// init spindle pwm timer(TIM2_CH4):
}

void spindle_on(uint16_t spindle_speed, uint8_t spindle_dir) {
	if(spindle_dir == 0){
		SPINDLE_ON_PORT->BSRRL  = SPINDLE_ON_PIN;
		SPINDLE_DIR_PORT->BSRRH = SPINDLE_DIR_PIN;
		SPINDLE_PWM_PORT->BSRRL = SPINDLE_PWM_PIN;
	} else {
		SPINDLE_ON_PORT->BSRRL  = SPINDLE_ON_PIN;
		SPINDLE_DIR_PORT->BSRRL = SPINDLE_DIR_PIN;
		SPINDLE_PWM_PORT->BSRRL = SPINDLE_PWM_PIN;
	}
}

void spindle_off(void) {
	SPINDLE_ON_PORT->BSRRH  = SPINDLE_ON_PIN;
	SPINDLE_DIR_PORT->BSRRH = SPINDLE_DIR_PIN;
	SPINDLE_PWM_PORT->BSRRH = SPINDLE_PWM_PIN;
}

void coolant_on(void) {
	COOLANT_PORT->BSRRL = COOLANT_PIN;
}
void coolant_off(void) {
	COOLANT_PORT->BSRRH = COOLANT_PIN;
}
