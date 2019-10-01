#include "main.h"

volatile uint8_t _keyBuffer[6], _keyPressedCounter=0;

static void kbd_putKey(uint8_t key) {
 if(_keyPressedCounter >= sizeof(_keyBuffer)) return;
 _keyBuffer[_keyPressedCounter++] = key;
}

void kbd_proc(void) {
 static uint8_t scan_stage = 0;
 if(++scan_stage >= 8) scan_stage = 0;
 uint8_t row = scan_stage >> 1;

 if((scan_stage & 0x01) == 0) {
	HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, GPIO_PIN_SET);
  switch(row) {
   case 0: HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_RESET); break;
   case 1: HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, GPIO_PIN_RESET); break;
   case 2: HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, GPIO_PIN_RESET); break;
   case 3: HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, GPIO_PIN_RESET); break;
  }
 } else {
 	if(HAL_GPIO_ReadPin(COL_0_GPIO_Port, COL_0_Pin) == GPIO_PIN_SET) kbd_putKey(0x10|row);
 	if(HAL_GPIO_ReadPin(COL_1_GPIO_Port, COL_1_Pin) == GPIO_PIN_SET) kbd_putKey(0x20|row);
 	if(HAL_GPIO_ReadPin(COL_2_GPIO_Port, COL_2_Pin) == GPIO_PIN_SET) kbd_putKey(0x30|row);
 	if(HAL_GPIO_ReadPin(COL_3_GPIO_Port, COL_3_Pin) == GPIO_PIN_SET) kbd_putKey(0x40|row);
 }
}

int kbd_getKey(void) {
 return _keyPressedCounter == 0? -1:_keyBuffer[--_keyPressedCounter];
}
