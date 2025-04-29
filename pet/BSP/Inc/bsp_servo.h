#ifndef BSP_SERVO_H_
#define BSP_SERVO_H_

#include "tim.h"

#define MAX_ANGLE       180.0f
#define MINIMAL_ANGLE   0.0f

typedef enum
{
    Servo_LF = TIM_CHANNEL_1,
    Servo_LB = TIM_CHANNEL_2,
    Servo_RF = TIM_CHANNEL_3,
    Servo_RB = TIM_CHANNEL_4
}Servo_Typedef;

void Servo_Init(void);

void Servo_SetAngle(Servo_Typedef servo, float angle);

#endif

