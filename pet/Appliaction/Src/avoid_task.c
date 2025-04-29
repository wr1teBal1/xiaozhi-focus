#include "avoid_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gpio.h"
#include "stdbool.h"
#include "bsp_ultrasonic.h"
#include "move_task.h"

TaskHandle_t avoid_task_handle;
QueueHandle_t ultrasonic_detect_handle;
uint16_t current_avoid_distance;
uint16_t last_avoid_distance;
bool if_avoid;

void Avoid_Task_Init()
{
    BaseType_t res = 0;
    Ultrasonic_Init();
    ultrasonic_detect_handle = xSemaphoreCreateBinary();//创建二值信号量
    if(ultrasonic_detect_handle == NULL)
    {
        Error_Handler();
        return;
    }
    res = xSemaphoreTake(ultrasonic_detect_handle, portMAX_DELAY);//创建后信号量不为空，此时应主动释放一次
    if(res != pdPASS){
        Error_Handler();
    }
}

void Avoid_Task(void* pvParameters)
{
    Avoid_Task_Init();
    BaseType_t res = 0;
    static Movement_TypeDef last_move_state;
    while(true)
    {
        Ultrasonic_Measure();//触发一次超声波测量，60ms一次
        res = xSemaphoreTake(ultrasonic_detect_handle, portMAX_DELAY);//等待超声测距完成
        if(res == pdPASS)
        {
            current_avoid_distance = Ultrasonic_Distance();
            last_avoid_distance = current_avoid_distance;
            current_avoid_distance = DISTANCE_FILTER * current_avoid_distance +
                                     (1 - DISTANCE_FILTER) * last_avoid_distance;//测量完后对观测值滤波
            if(current_avoid_distance < AVOID_DISTANCE_MAX)
            {                
                if(GetMoveState() == Move_Forward || GetMoveState() == Move_Wander)
                {
                    last_move_state = GetMoveState();
                    SetMoveState(Move_Avoid);//如果处于前进/自由行走状态则触发避障                  
                }               
            }
            else//距离较长不需要避障
            {               
                if(GetMoveState() == Move_Avoid)
                {
    //                SetMoveState(Move_Forward);
                    SetMoveState(last_move_state);//如果处于避障状态则触发前进/自由行走
                }
            }
        } 
        else{
            Error_Handler();
        }       
    }
    
}

uint16_t Avoid_GetDistance(void)
{
    return current_avoid_distance;
}

void Avoid_Task_Start(void)
{
    xTaskCreate((TaskFunction_t)Avoid_Task,
                "avoid", 
                (configSTACK_DEPTH_TYPE)AVOID_TASK_STACK,
                NULL,
                (UBaseType_t)AVOID_TASK_PRIORITY,
                (TaskHandle_t*)&avoid_task_handle);
}

void Avoid_EXTI_Callback(void)
{
    BaseType_t res = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(HAL_GPIO_ReadPin(GPIOB, Echo_Pin) == GPIO_PIN_SET)//外部中断上升沿，表示触发超声波测量
    {        
        Ultrasonic_Start();
    }
    else if(HAL_GPIO_ReadPin(GPIOB, Echo_Pin) == GPIO_PIN_RESET)//外部中断下降沿，表示结束超声波测量
    {
        Ultrasonic_Stop();
        res = xSemaphoreGiveFromISR(ultrasonic_detect_handle, &xHigherPriorityTaskWoken);
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
}
