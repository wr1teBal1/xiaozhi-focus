#include "gesture_task.h"
#include "serial_task.h"
#include "move_task.h"
#include "avoid_task.h"
#include "display_task.h"
#include "blink_task.h"
#include "main_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdint.h"

TaskHandle_t main_task_handle;

void Main_Task(void *pvParameters)
{
    /* 进入临界区:保护临界区里的代码不会被打断 */
    taskENTER_CRITICAL();

    /* 启动若干任务 */
    Gesture_Task_Start();
    Avoid_Task_Start();
    Move_Task_Start();
    Display_Task_Start();
    Serial_Task_Start();    
    Blink_Task_Start();

    /* 启动任务只需要执行一次即可，用完就删除自己 */
    vTaskDelete(NULL);

    /* 退出临界区 */
    taskEXIT_CRITICAL();
}

void Main_Task_Start(void)
{
    /* 1.创建一个启动任务 */
    xTaskCreate((TaskFunction_t)Main_Task,               // 任务函数的地址
                (char *)"main_task",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)MAIN_TASK_STACK, // 任务栈大小
                (void *)NULL,                            // 传递给任务的参数
                (UBaseType_t)MAIN_TASK_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&main_task_handle);      // 任务句柄的地址
}
