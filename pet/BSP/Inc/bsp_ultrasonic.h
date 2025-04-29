#ifndef BSP_ULTRASONIC_H_
#define BSP_ULTRASONIC_H_

#include <stdint.h>
#include <stdbool.h>

#define ULTRASONIC_TRIG_TIME    1
#define ULTRASONIC_PERIOD       60//60ms测量一次（手册建议）

void Ultrasonic_Start(void);

void Ultrasonic_Stop(void);

void Ultrasonic_Measure(void);

void Ultrasonic_Init(void);

uint16_t Ultrasonic_Distance(void);

#endif
