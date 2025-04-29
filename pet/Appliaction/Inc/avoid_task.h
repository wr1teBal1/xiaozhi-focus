#ifndef AVOID_TASK_H_
#define AVOID_TASK_H_
#include <stdint.h>

#define AVOID_TASK_PRIORITY 3
#define AVOID_TASK_STACK    1024
#define DISTANCE_FILTER     0.2   //低通滤波器
#define AVOID_DISTANCE_MAX  30    //30mm时开始避障

uint16_t Avoid_GetDistance(void);

void Avoid_EXTI_Callback(void);

void Avoid_Task_Start(void);

#endif
