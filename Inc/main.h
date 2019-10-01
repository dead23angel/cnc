/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define TRUE true
#define FALSE false
//#define SHOW_INIT_SCR
//#define HAS_ENCODER
//#define DEBUG_MODE
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#ifdef DEBUG_MODE
 #define DBG(...) { rf_printf(__VA_ARGS__); }
#else
 #define DBG(...) {}
#endif
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
char *str_trim(char *str);
uint8_t questionYesNo(char *msg, char *param);
void manualMode(void);
void showCriticalStatus(char *msg, int st);
uint16_t calcColor(uint8_t val);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define COL_3_Pin GPIO_PIN_2
#define COL_3_GPIO_Port GPIOE
#define COL_2_Pin GPIO_PIN_3
#define COL_2_GPIO_Port GPIOE
#define COL_1_Pin GPIO_PIN_5
#define COL_1_GPIO_Port GPIOE
#define COL_0_Pin GPIO_PIN_13
#define COL_0_GPIO_Port GPIOC
#define ROW_3_Pin GPIO_PIN_0
#define ROW_3_GPIO_Port GPIOC
#define ROW_2_Pin GPIO_PIN_1
#define ROW_2_GPIO_Port GPIOC
#define ROW_1_Pin GPIO_PIN_2
#define ROW_1_GPIO_Port GPIOC
#define ROW_0_Pin GPIO_PIN_3
#define ROW_0_GPIO_Port GPIOC
#define LED_0_Pin GPIO_PIN_6
#define LED_0_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_7
#define LED_1_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_1
#define LCD_BL_GPIO_Port GPIOB
#define LIMIT_X_Pin GPIO_PIN_12
#define LIMIT_X_GPIO_Port GPIOB
#define LIMIT_Y_Pin GPIO_PIN_13
#define LIMIT_Y_GPIO_Port GPIOB
#define LIMIT_Z_Pin GPIO_PIN_14
#define LIMIT_Z_GPIO_Port GPIOB
#define SD_CD_Pin GPIO_PIN_3
#define SD_CD_GPIO_Port GPIOD
#define SD_WR_Pin GPIO_PIN_6
#define SD_WR_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
