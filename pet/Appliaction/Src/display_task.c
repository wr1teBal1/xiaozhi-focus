#include "display_task.h"
#include "move_task.h"
#include "bsp_oled.h"
#include "oled_data.h"
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"

TaskHandle_t display_task_handle;
volatile Dispaly_TypeDef display_state;
QueueHandle_t display_detect_handle;

void Display_Task_Init(void)
{
    BaseType_t res = 0;
    OLED_Init();
    display_detect_handle= xSemaphoreCreateBinary();//创建二值信号量
    if(display_detect_handle == NULL)
    {
        Error_Handler();
        return;
    }
    res = xSemaphoreTake(display_detect_handle, portMAX_DELAY);//创建后信号量不为空，此时应主动释放一次
    if(res != pdPASS){
        Error_Handler();
    }
}

void Display_Task(void* pvParameters)
{
    Display_Task_Init();
    BaseType_t res = 0;
    while(true)
    {
        res = xSemaphoreTake(display_detect_handle, portMAX_DELAY);
        switch (display_state)
        {
            case Display_Happy:
                OLED_Display(HappyImage);
                break;
            case Display_Angry:
                OLED_Display(AngryImage);
                break;
            case Display_Sad:
                OLED_Display(SadImage);
                break;
            case Display_Listening:
                OLED_Display(ListeningImage);
                break;
            case Display_Speaking:
                OLED_Display(SpeakingImage);
                break;
            case Display_Idle:
                OLED_Display(IdleImage);
                break;
            default:
                break;
        }
        vTaskDelay(500);
        res = xSemaphoreTake(display_detect_handle, 0);//消抖
        if(res != pdPASS){
            Error_Handler();
        }
    }
}

void Display_Task_Start(void)
{
    xTaskCreate((TaskFunction_t)Display_Task,
                "display", 
                (configSTACK_DEPTH_TYPE)DISPLAY_TASK_STACK,
                NULL,
                (UBaseType_t)DISPLAY_TASK_PRIORITY,
                (TaskHandle_t*)&display_task_handle);
}

void SetDisplayState(Dispaly_TypeDef new_display_state)
{
    BaseType_t res = 0;
    display_state = new_display_state;
    res = xSemaphoreGive(display_detect_handle);
    if(res == pdFALSE)
    {
        Error_Handler();
        return;
    }
}

Dispaly_TypeDef GetDisplayState(void)
{
    return display_state;
}
