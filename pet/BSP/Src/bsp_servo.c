#include "tim.h"
#include "bsp_servo.h"

void Servo_Init(void) 
{
    // 启动定时器并配置PWM通道
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

void Servo_SetAngle(Servo_Typedef servo, float angle)
{
    if(angle > MAX_ANGLE)
        angle = MAX_ANGLE;
    else if(angle < MINIMAL_ANGLE)
        angle = MINIMAL_ANGLE;
    uint16_t pwm = angle / 180 * 2000 + 500;    
    __HAL_TIM_SetCompare(&htim2, servo, pwm);
}
