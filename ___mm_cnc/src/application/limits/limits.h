/*
 * limits.h
 *      Author: Mm
 */

#ifndef LIMITS_H_
#define LIMITS_H_

#ifndef _WINDOWS
void limits_init(void);
inline uint8_t limitX_chk(void) __attribute__ ((always_inline));
inline uint8_t limitY_chk(void) __attribute__ ((always_inline));
inline uint8_t limitZ_chk(void) __attribute__ ((always_inline));
inline uint8_t limits_chk(void) __attribute__ ((always_inline));

inline uint8_t limitX_chk(void) { return !(LIMIT_X_PORT->IDR & LIMIT_X_PIN); }
inline uint8_t limitY_chk(void) { return !(LIMIT_Y_PORT->IDR & LIMIT_Y_PIN); }
inline uint8_t limitZ_chk(void) { return !(LIMIT_Z_PORT->IDR & LIMIT_Z_PIN); }

inline uint8_t limits_chk(void) { return limitX_chk() || limitY_chk() || limitZ_chk(); }
#endif

#endif /* LIMITS_H_ */
