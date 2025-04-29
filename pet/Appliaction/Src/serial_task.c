#include "serial_task.h"
#include "move_task.h"
#include "bsp_serial.h"
#include "display_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "usart.h"
#include <stdint.h>

uint8_t rx_data;
QueueHandle_t serial_detect_handle;
TaskHandle_t serial_task_handle;

Movement_TypeDef FromSerialToMove()
{
    switch (rx_data)
    {
    case 0x10:
        return Move_Stop;
    case 0x11:
        return Move_Forward;
    case 0x12:
        return Move_Backward;
    case 0x13:
        return Move_Left;
    case 0x14:
        return Move_Right;
    case 0x15:
        return Move_Squat;
    case 0x16:
        return Move_Standup;
    case 0x17:
        return Move_Wander;
    default:
        break;
    }
    return Move_Stop;
}

Dispaly_TypeDef FromSerialToDisplay()
{
    switch (rx_data)
    {
    case 0x20:
        return Display_Happy;
    case 0x21:
        return Display_Angry;
    case 0x22:
        return Display_Sad;
    case 0x23:
        return Display_Listening;
    case 0x24:
        return Display_Speaking;
    case 0x25:
        return Display_Idle;
    default:
        break;
    }
    return Display_Happy;
}


void Serial_Task_Init(void)
{
    BaseType_t res = 0;
    Serial_Init(&rx_data);
    serial_detect_handle = xSemaphoreCreateBinary();//创建二值信号量
    if(serial_detect_handle == NULL)
    {
        Error_Handler();
        return;
    }
    res = xSemaphoreTake(serial_detect_handle, portMAX_DELAY);//创建后信号量不为空，此时应主动释放一次
    if(res != pdPASS){
        Error_Handler();
    }
}

void Serial_Task(void* pvParameters)
{
    Serial_Task_Init();
    BaseType_t res = 0;
    Movement_TypeDef temp_move_state;
    Dispaly_TypeDef temp_display_state;
    while(true)
    {
        res = xSemaphoreTake(serial_detect_handle, portMAX_DELAY);//等待收到串口信息
        if(res == pdPASS)
        {
            if(rx_data & SERIAL_MOVE_MASK)
            {
                temp_move_state = FromSerialToMove();
                SetMoveState(temp_move_state);
            }
            else if(rx_data & SERIAL_DISPLAY_MASK)
            {
                temp_display_state = FromSerialToDisplay();
                SetDisplayState(temp_display_state);
            }
        }
        else
        {
            Error_Handler();
        }
    }
}
 
void Serial_Task_Start(void)
{
    xTaskCreate((TaskFunction_t)Serial_Task,
                "serial detect", 
                (configSTACK_DEPTH_TYPE)SERIAL_TASK_STACK,
                NULL,
                (UBaseType_t)SERIAL_TASK_PRIORITY,
                (TaskHandle_t*)&serial_task_handle);
}


void Serial_RXIT_Callback(void)
{
    BaseType_t res = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    res = xSemaphoreGiveFromISR(serial_detect_handle, &xHigherPriorityTaskWoken);
    if(res == pdFALSE)
    {
        Error_Handler();
        return;
    }
    if (xHigherPriorityTaskWoken == pdTRUE) 
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // 确保高优先级任务立即运行
    }
    HAL_UART_Receive_IT(&huart1, &rx_data, 1);//重新启用中断
}



