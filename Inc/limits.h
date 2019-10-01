#include "main.h"

#ifndef LIMITS_H_
#define LIMITS_H_

void limits_init(void);
inline uint8_t limitX_chk(void) __attribute__ ((always_inline));
inline uint8_t limitY_chk(void) __attribute__ ((always_inline));
inline uint8_t limitZ_chk(void) __attribute__ ((always_inline));
inline uint8_t limits_chk(void) __attribute__ ((always_inline));

inline uint8_t limitX_chk(void) { return HAL_GPIO_ReadPin(LIMIT_X_GPIO_Port, LIMIT_X_Pin) == GPIO_PIN_SET; }
inline uint8_t limitY_chk(void) { return HAL_GPIO_ReadPin(LIMIT_Y_GPIO_Port, LIMIT_Y_Pin) == GPIO_PIN_SET; }
inline uint8_t limitZ_chk(void) { return HAL_GPIO_ReadPin(LIMIT_Z_GPIO_Port, LIMIT_Z_Pin) == GPIO_PIN_SET; }

inline uint8_t limits_chk(void) { return limitX_chk() || limitY_chk() || limitZ_chk(); }

#endif /* LIMITS_H_ */
