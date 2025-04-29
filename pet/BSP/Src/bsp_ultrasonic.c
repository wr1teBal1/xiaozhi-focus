#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdbool.h"
#include "tim.h"
#include "bsp_ultrasonic.h"

bool ultrasonic_trigger_flag = true;
uint16_t distance;

void Ultrasonic_Measure(void)
{
    portDISABLE_INTERRUPTS();//关中断，防止中断对触发超声测距的影响/* 待优化 */
    HAL_GPIO_WritePin(GPIOB, Trigger_Pin, GPIO_PIN_SET);
    vTaskDelay(ULTRASONIC_TRIG_TIME);//拉高1msTrig触发一次超声测距
    //HAL_Delay(ULTRASONIC_TRIG_TIME);//或许用hal_delay会好一点
    HAL_GPIO_WritePin(GPIOB, Trigger_Pin, GPIO_PIN_RESET);//此时触发外部中断
    portENABLE_INTERRUPTS();//重新打开中断
    vTaskDelay(ULTRASONIC_PERIOD - ULTRASONIC_TRIG_TIME);//60ms触发一次
}

uint16_t Ultrasonic_Distance(void)
{
    static uint16_t temp;
    temp = distance;
    distance = 0;
    return temp;
}

void Ultrasonic_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim3);//开启TIM3，TIM3 1us记一次，最大时间65.535ms，最大测距11m（模块最大2m）
    TIM3->CR1 |= TIM_CR1_CEN;//使能TIM3
}

void Ultrasonic_Start(void)
{
    TIM3->CNT = 0;//设置TIM3为0
    TIM3->CR1 |= TIM_CR1_CEN;//使能TIM3
}

void Ultrasonic_Stop(void)
{
    distance = TIM3->CNT* 170 / 1000;//mm
    TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));//失能TIM3
}
