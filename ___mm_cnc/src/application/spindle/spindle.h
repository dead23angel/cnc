#ifndef SPINDLE_H_
#define SPINDLE_H_

void spindle_init(void);
void spindle_on(uint16_t spindle_speed, uint8_t spindle_dir);
void spindle_off(void);
void coolant_on(void);
void coolant_off(void);
#endif /* SPINDLE_H_ */
