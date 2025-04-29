#include "blink_task.h"
#include "bsp_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "stdbool.h"

TaskHandle_t blink_task_handle;

void Blink_Task(void* pvParameters)
{
    while(true){
        LED_Toggle();
        vTaskDelay(500);
    }
    
}

void Blink_Task_Start(void)
{
    xTaskCreate((TaskFunction_t)Blink_Task,
                "blink-blink", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                (configSTACK_DEPTH_TYPE)128,
                NULL,
                (UBaseType_t)BLINK_TASK_PRIORITY,
                (TaskHandle_t*)&blink_task_handle);
}








