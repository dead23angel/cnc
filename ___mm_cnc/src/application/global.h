#ifndef GLOBAL_H_
#define GLOBAL_H_

//----- E-Stop ---------------------
#define ESTOP_PORT       GPIOB
#define ESOP_PIN         GPIO_Pin_15

//----- Limit Switch ---------------
#define LIMIT_X_PORT     GPIOB
#define LIMIT_X_PIN      GPIO_Pin_12

#define LIMIT_Y_PORT     GPIOB
#define LIMIT_Y_PIN      GPIO_Pin_13

#define LIMIT_Z_PORT     GPIOB
#define LIMIT_Z_PIN      GPIO_Pin_14

//------- Stepmotor ----------------
#define M_EN_PORT        GPIOE
#define M_EN_PIN         GPIO_Pin_1


//M0 STEP TIMER: TIM5_CH2
#define M0_STEP_PORT     GPIOA
#define M0_STEP_PIN      GPIO_Pin_1

#define M0_DIR_PORT      GPIOA
#define M0_DIR_PIN       GPIO_Pin_0


//M1 STEP TIMER: TIM9_CH2
#define M1_STEP_PORT     GPIOA
#define M1_STEP_PIN      GPIO_Pin_3

#define M1_DIR_PORT      GPIOA
#define M1_DIR_PIN       GPIO_Pin_2


//M2 STEP TIMER: TIM14_CH1
#define M2_STEP_PORT     GPIOA
#define M2_STEP_PIN      GPIO_Pin_7

#define M2_DIR_PORT      GPIOA
#define M2_DIR_PIN       GPIO_Pin_6


//M3 STEP TIMER: TIM3_CH3
#define M3_STEP_PORT     GPIOB
#define M3_STEP_PIN      GPIO_Pin_0

#define M3_DIR_PORT      GPIOC
#define M3_DIR_PIN       GPIO_Pin_5

//----- Spindle --------------------
//SPINDLE ON RELAY:
#define SPINDLE_ON_PORT  GPIOA
#define SPINDLE_ON_PIN   GPIO_Pin_5

//SPINDLE DIR RELAY:
#define SPINDLE_DIR_PORT GPIOB
#define SPINDLE_DIR_PIN  GPIO_Pin_10

//SPINDLE PWM TIMER: TIM2_CH4
#define SPINDLE_PWM_PORT GPIOB
#define SPINDLE_PWM_PIN  GPIO_Pin_11

//----- Coolant --------------------
//COOLANT ON RELAY:
#define COOLANT_PORT     GPIOA
#define COOLANT_PIN      GPIO_Pin_4

//----- Keyboard -------------------
//KEYBOARD COLUMNS:
#define COL0_PORT        GPIOC
#define COL0_PIN         GPIO_Pin_13

#define COL1_PORT        GPIOE
#define COL1_PIN         GPIO_Pin_5

#define COL2_PORT        GPIOE
#define COL2_PIN         GPIO_Pin_3

#define COL3_PORT        GPIOE
#define COL3_PIN         GPIO_Pin_2

//KEYBOARD ROWS:
#define ROW0_PORT        GPIOC
#define ROW0_PIN         GPIO_Pin_3

#define ROW1_PORT        GPIOC
#define ROW1_PIN         GPIO_Pin_2

#define ROW2_PORT        GPIOC
#define ROW2_PIN         GPIO_Pin_1

#define ROW3_PORT        GPIOC
#define ROW3_PIN         GPIO_Pin_0

//----------------------------------
//#define SHOW_INIT_SCR
//#define EXT_INPUT_PULLUP
//#define HAS_ENCODER
//----------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "hw_config.h"
#include "delay.h"
#include "scr_io.h"
#include "rtc.h"
#include "ff.h"
#include "sdio_sd_conf.h"
#include "sdio_sd.h"
#include "keyboard.h"
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "gcode.h"
#include "stepmotor.h"
#include "spindle.h"
#include "limits.h"
#include "encoder.h"
//---------------------------------

//void delayMs(uint32_t msec);
char *str_trim(char *str);
uint8_t questionYesNo(char *msg, char *param);
void manualMode(void);
void showCriticalStatus(char *msg, int st);
uint16_t calcColor(uint8_t val);

//#define DEBUG_MODE
#ifdef DEBUG_MODE
 #define DBG(...) { rf_printf(__VA_ARGS__); }
#else
 #define DBG(...) {}
#endif

#endif /* GLOBAL_H_ */
