#ifndef __DELAY_H

#define __DELAY_H

#define F_CPU 168000000



inline void _delay_loops(__IO uint32_t loops) __attribute__ ((always_inline));

inline void _delay_loops(__IO uint32_t loops) {
   __asm volatile (
      "1: \n"
      " SUBS %[loops], %[loops], #1 \n"
      " BNE 1b \n"
         : [loops] "+r"(loops)
   );
}

#define _delay_us( US ) _delay_loops( (uint32_t)((double)US * F_CPU / 3000000.0) )
#define _delay_ms( MS ) _delay_loops( (uint32_t)((double)MS * F_CPU / 3000.0) )
#define _delay_s( S )   _delay_loops( (uint32_t)((double)S  * F_CPU / 3.0) )

#endif /* __DELAY_H */
