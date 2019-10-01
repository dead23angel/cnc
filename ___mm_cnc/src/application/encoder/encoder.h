#ifndef ENCODER_H_
#define ENCODER_H_

//#define HAS_ENCODER
extern int8_t isEncoderCorrection;
#ifdef HAS_ENCODER

void encoder_int();
inline int32_t encoderZvalue() { return (0x007FFF-(int32_t)TIM_GetCounter(TIM8))>> 2; }
inline void encoderZreset() {  TIM8->CNT = 0x7FFF; }

#define ENCODER_CORRECTION_MIN_STEPS 150

#define ENCODER_Z_CNT_PER_360 512
#define ENCODER_Z_MIN_MEASURE (MM_PER_360*1500/ENCODER_Z_CNT_PER_360) // 0.014 mm

extern int8_t isEncoderCorrection;
// TODO for debug.. will be removed
extern int32_t encoderCorrectionCntUp, encoderCorrectionCntDown, encoderCorrectionDelta, encoderCorrectionMaxDelta;
#endif

#endif
