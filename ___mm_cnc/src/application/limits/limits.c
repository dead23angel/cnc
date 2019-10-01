#include "global.h"

void limits_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	#ifdef EXT_INPUT_PULLUP
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	#else
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	#endif
	GPIO_InitStructure.GPIO_Pin = LIMIT_X_PIN;
	GPIO_Init(LIMIT_X_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	#ifdef EXT_INPUT_PULLUP
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	#else
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	#endif
	GPIO_InitStructure.GPIO_Pin = LIMIT_Y_PIN;
	GPIO_Init(LIMIT_Y_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	#ifdef EXT_INPUT_PULLUP
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	#else
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	#endif
	GPIO_InitStructure.GPIO_Pin = LIMIT_Z_PIN;
	GPIO_Init(LIMIT_Z_PORT, &GPIO_InitStructure);
}

