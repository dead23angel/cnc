#include "global.h"


void kbd_init(void) {
 GPIO_InitTypeDef GPIO_InitStructure;
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

 GPIO_InitStructure.GPIO_Pin = ROW0_PIN; GPIO_Init(ROW0_PORT, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = ROW1_PIN; GPIO_Init(ROW1_PORT, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = ROW2_PIN; GPIO_Init(ROW2_PORT, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = ROW3_PIN; GPIO_Init(ROW3_PORT, &GPIO_InitStructure);


 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;

 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

 GPIO_InitStructure.GPIO_Pin = COL0_PIN; GPIO_Init(COL0_PORT, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = COL1_PIN; GPIO_Init(COL1_PORT, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = COL2_PIN; GPIO_Init(COL2_PORT, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = COL3_PIN; GPIO_Init(COL3_PORT, &GPIO_InitStructure);
}

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
	ROW0_PORT->BSRRH = ROW0_PIN;
	ROW1_PORT->BSRRH = ROW1_PIN;
	ROW2_PORT->BSRRH = ROW2_PIN;
	ROW3_PORT->BSRRH = ROW3_PIN;
  switch(row) {
   case 0: ROW0_PORT->BSRRL = ROW0_PIN; break;
   case 1: ROW1_PORT->BSRRL = ROW1_PIN; break;
   case 2: ROW2_PORT->BSRRL = ROW2_PIN; break;
   case 3: ROW3_PORT->BSRRL = ROW3_PIN; break;
  }
 } else {
 	if(COL0_PORT->IDR & COL0_PIN) kbd_putKey(0x10|row);
 	if(COL1_PORT->IDR & COL1_PIN) kbd_putKey(0x20|row);
 	if(COL2_PORT->IDR & COL2_PIN) kbd_putKey(0x30|row);
 	if(COL3_PORT->IDR & COL3_PIN) kbd_putKey(0x40|row);
 }
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == Bit_RESET) kbd_putKey(0x07F); // key on main board
}

int kbd_getKey(void) {
 return _keyPressedCounter == 0? -1:_keyBuffer[--_keyPressedCounter];
}
