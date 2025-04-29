#include "interrupt_task.h"
#include "bsp_gesture_sensor.h"
#include "bsp_ultrasonic.h"
#include "avoid_task.h"
#include "gesture_task.h"
#include "serial_task.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == INT_Pin)
	{
        Gesture_EXTI_Callback();
    }
    else if(GPIO_Pin == Echo_Pin)
    {
        Avoid_EXTI_Callback();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        Serial_RXIT_Callback();
    }
}
