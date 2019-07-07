/*
 * limits.h
 *      Author: Mm
 */

#ifndef LIMITS_H_
#define LIMITS_H_

#ifndef _WINDOWS
void limits_init(void);
inline uint8_t limitX_chk(void) { return !(XPORT->IDR & XPIN); }
inline uint8_t limitY_chk(void) { return !(YPORT->IDR & YPIN); }
inline uint8_t limitZ_chk(void) { return !(ZPORT->IDR & ZPIN); }

inline uint8_t limits_chk(void) { return limitX_chk() || limitY_chk() || limitZ_chk(); }
#endif

#endif /* LIMITS_H_ */
