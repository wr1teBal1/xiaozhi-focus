#include "gesture_task.h"
#include "move_task.h"
#include "bsp_gesture_sensor.h"
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

TaskHandle_t gesture_task_handle;
Gesture_StatusTypeDef gesture_data;
QueueHandle_t gesture_detect_handle;

Movement_TypeDef FromGesToMove(void)
{
    switch (gesture_data)
    {
    case Ges_Forward:
        return Move_Forward;
    case Ges_Backward:
        return Move_Backward;
    case Ges_Left:
        return Move_Left;
    case Ges_Right:
        return Move_Right;
    case Ges_Up:
        return Move_Standup;
    case Ges_Down:
        return Move_Squat;
    case Ges_Clockwise:
        return Move_Stop;//没想好
    case Ges_ClockwiseCounter:
        return Move_Stop;//没想好
    default:
        return Move_Stop;
    }
}

void Gesture_Task_Init(void)
{
    BaseType_t res = 0;
    PAJ7620_Init();
    gesture_detect_handle = xSemaphoreCreateBinary();//创建二值信号量
    if(gesture_detect_handle == NULL)
    {
        Error_Handler();
        return;
    }
    res = xSemaphoreTake(gesture_detect_handle, portMAX_DELAY);//创建后信号量不为空，此时应主动释放一次
    if(res != pdPASS){
        Error_Handler();
    }
}

void Gesture_Task(void* pvParameters)
{
    Gesture_Task_Init();
    BaseType_t res = 0;
    Movement_TypeDef temp_move_state = Move_Stop;
    while(true)
    {
        res = xSemaphoreTake(gesture_detect_handle, portMAX_DELAY);//等待手势测距完成
        if(res == pdPASS)
        {
            temp_move_state = FromGesToMove();
            SetMoveState(temp_move_state);
        }
        else
        {
            Error_Handler();
        }
        vTaskDelay(1000);
        xSemaphoreTake(gesture_detect_handle, 0);//1s内取消手势测量，可以消抖（二值信号量有则消去，没有也不阻塞）
    }
}


void Gesture_Task_Start(void)
{
    xTaskCreate((TaskFunction_t)Gesture_Task,
                "gesture detect", 
                (configSTACK_DEPTH_TYPE)GESTURE_TASK_STACK,
                NULL,
                (UBaseType_t)GESTURE_TASK_PRIORITY,
                (TaskHandle_t*)&gesture_task_handle);
}

void Gesture_EXTI_Callback(void)
{
    gesture_data = Gesture_Detect();
    BaseType_t res = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    res = xSemaphoreGiveFromISR(gesture_detect_handle, &xHigherPriorityTaskWoken);
    if(res == pdFALSE)
    {
        Error_Handler();
        return;
    }
    if (xHigherPriorityTaskWoken == pdTRUE) 
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // 确保高优先级任务立即运行
    }
}

