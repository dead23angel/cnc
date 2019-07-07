#ifndef SCAN_H_
#define SCAN_H_


inline uint8_t sensorTouch(void) { return !(SENSOR_PORT->IDR & SENSOR_PIN); }
void initSensor(void);
void scanMode(void);

#endif /* SCAN_H_ */
