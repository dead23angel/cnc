#ifndef GLOBAL_H_
#define GLOBAL_H_


/*----- keyboard ----------------------------------------
*  col: PE2, PE3, PE4, PE5
*  row: PE6, PC1, PC2, PC3
* ------- stepmotor -------------------------------------
*    EN    DIR   STEP
* 0: PA1   PA2   PA3
* 1: PE0   PB9   PB1
* 2: PB11  PC7   PD12
* 3: PB11  PD6   PB10
* ----- extruder ---------------------------------------
* SPI2: PB13-SCK, PB14-MISO, PB15-MOSI  - thermometer
* TIM8_CH1: PC6 - heater PWM,  PB12 - FUN ON/OFF
* ----- position switch ----------------------------------
*  X,Y,Z: PA0,PB8,PD3
* ---- sensor ------------------------
* PA8
*/

//---- sensor ------------
#define SENSOR_PORT GPIOA
#define SENSOR_PIN  GPIO_Pin_8

//----- limit switch ----------------------------------
#define XPORT GPIOA
#define XPIN GPIO_Pin_0
#define ZPORT GPIOB
#define ZPIN GPIO_Pin_8
#define YPORT GPIOD
#define YPIN GPIO_Pin_3

//------- stepmotor -------------------------------------
// 74hc14 - inverter on the step motors board. STEP on falling edge
//  _______        ______
//         x      |
//         |______|
#define M0_EN_PORT GPIOA
#define M0_EN_PIN GPIO_Pin_1
#define M0_DIR_PORT GPIOA
#define M0_DIR_PIN GPIO_Pin_2
#define M0_STEP_PORT GPIOA
#define M0_STEP_PIN GPIO_Pin_3

#define M1_EN_PORT GPIOE
#define M1_EN_PIN GPIO_Pin_0
#define M1_DIR_PORT GPIOB
#define M1_DIR_PIN GPIO_Pin_9
#define M1_STEP_PORT GPIOB
#define M1_STEP_PIN GPIO_Pin_1

#define M2_EN_PORT GPIOB
#define M2_EN_PIN GPIO_Pin_11
#define M2_DIR_PORT GPIOC
#define M2_DIR_PIN GPIO_Pin_7
#define M2_STEP_PORT GPIOD
#define M2_STEP_PIN GPIO_Pin_12

#define M3_EN_PORT GPIOB
#define M3_EN_PIN GPIO_Pin_11
#define M3_DIR_PORT GPIOD
#define M3_DIR_PIN GPIO_Pin_6
#define M3_STEP_PORT GPIOB
#define M3_STEP_PIN GPIO_Pin_10

//----- keyboard ----------------------------------------
///   0: 1: 2: 3:
//0:  1  2  3  A
//1:  4  5  6  B
//2:  7  8  9  C
//3:  *  0  #  D
#define COL0_PORT GPIOE
#define COL0_PIN GPIO_Pin_2
#define COL1_PORT GPIOE
#define COL1_PIN GPIO_Pin_3
#define COL2_PORT GPIOE
#define COL2_PIN GPIO_Pin_4
#define COL3_PORT GPIOE
#define COL3_PIN GPIO_Pin_5

#define ROW0_PORT GPIOE
#define ROW0_PIN GPIO_Pin_6
#define ROW1_PORT GPIOC
#define ROW1_PIN GPIO_Pin_1
#define ROW2_PORT GPIOC
#define ROW2_PIN GPIO_Pin_2
#define ROW3_PORT GPIOC
#define ROW3_PIN GPIO_Pin_3

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_spi.h"
#include "hw_config.h"
#include "scr_io.h"
#include "rtc.h"
#include "ff.h"
#include "sdcard.h"
#include "flash_spi.h"
#include "rs232_interface.h"
#include "keyboard.h"
#include "gcode.h"
#include "stepmotor.h"
#include "extruder_t.h"
#include "limits.h"
#include "scan.h"
//#include "adjust.h"

void delayMs(uint32_t msec);
char *str_trim(char *str);
uint8_t questionYesNo(char *msg, char *param);

void manualMode(void);

/*
typedef struct {
	uint16_t magicValue;
	double startX;
	double startY;
	double startZ;
	double extruder_k;
} FLASH_VALUES;

extern FLASH_VALUES commonValues;
*/

void showCriticalStatus(char *msg, int st);
uint16_t calcColor(uint8_t val);

//#define DEBUG_MODE
#ifdef DEBUG_MODE
 #define DBG(...) { rf_printf(__VA_ARGS__); }
#else
 #define DBG(...) {}
#endif

#endif /* GLOBAL_H_ */
