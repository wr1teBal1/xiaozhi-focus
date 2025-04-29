#include <stdint.h>
#include "move_task.h"
#include "bsp_servo.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

volatile Movement_TypeDef move_state;
volatile uint32_t move_exe_times;
bool first_run;
TaskHandle_t move_task_handle;

static void Forward(void);
static void Backward(void);
static void Right(void);
static void Left(void);
static void Squat(void);
static void StandUp(void);
static void Stop(void);
static void Wander(void);
static void Avoid(void);

static void Move_Task_Init(void)
{
    Servo_Init();
    move_state = Move_Stop;
}

void Move_Task(void* pvParameters)
{
    Move_Task_Init();
    while (true)
    {
        switch (move_state)
        {
            case Move_Stop:
                Stop();
                break;
            case Move_Forward:
                Forward();
                break;
            case Move_Backward:
                Backward();
                break;
            case Move_Left:
                Left();
                break;
            case Move_Right:
                Right();
                break;
            case Move_Squat:
                Squat();
                break;
            case Move_Standup:
                StandUp();
                break;
            case Move_Wander:
                Wander();
                break;
            case Move_Avoid:
                Avoid();
                break;
            default:
                Stop();
                break;
        }
        move_exe_times--;
        if(move_exe_times == 0)
        {
            SetMoveState(Move_Stop);//执行完对应的轮次后自动停下来
        }
    }    
}

void Move_Task_Start(void)
{
    xTaskCreate((TaskFunction_t)Move_Task,
                "movement", 
                (configSTACK_DEPTH_TYPE)MOVE_TASK_STACK,
                NULL,
                (UBaseType_t)MOVE_TASK_PRIORITY,
                (TaskHandle_t*)&move_task_handle);
}

void SetMoveState(Movement_TypeDef new_move_state)
{
    switch (new_move_state)
    {
    case Move_Stop:
        move_exe_times = 0xffffffff;//无限期执行(大约6年，按50ms执行一次程序)
        break;
    case Move_Forward:
        move_exe_times = 100;//暂定执行100次
        break;
    case Move_Backward:
        move_exe_times = 100;//暂定执行100次
        break;
    case Move_Left:
        move_exe_times = 100;//暂定执行100次
        break;
    case Move_Right:
        move_exe_times = 100;//暂定执行100次
        break;
    case Move_Squat:
        move_exe_times = 1;//执行1次
        break;
    case Move_Standup:
        move_exe_times = 1;
        break;
    case Move_Wander:
        move_exe_times = 0xffffffff;//无限期执行(大于6年，按>50ms执行一次程序)
        break;
    case Move_Avoid:
        move_exe_times = 100;//暂定执行100次
        break;   
    default:
        break;        
    }
    move_state = new_move_state;//更新运动状态
    first_run = true;/* 待优化 */
}

Movement_TypeDef GetMoveState(void)
{
    return move_state;
}


static void Forward(void)
{
    static uint8_t dir;
    if(dir == 1)
    {
        Servo_SetAngle(Servo_RF, 45);
        Servo_SetAngle(Servo_LB, 45);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_LF, 135);
        Servo_SetAngle(Servo_RB, 135);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_RF, 90);
        Servo_SetAngle(Servo_LB, 90);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_LF, 90);
        Servo_SetAngle(Servo_RB, 90);
        vTaskDelay(SPEED_DELAY);
        dir = 0;
    }
    else 
    {
        Servo_SetAngle(Servo_LF, 45);
        Servo_SetAngle(Servo_RB, 45);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_RF, 135);
        Servo_SetAngle(Servo_LB, 135);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_LF, 90);
        Servo_SetAngle(Servo_RB, 90);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_RF, 90);
        Servo_SetAngle(Servo_LB, 90);
        vTaskDelay(SPEED_DELAY);
        dir = 1;
    }
}

static void Backward(void)
{
    static uint8_t dir;
    if(dir == 1)
    {
        Servo_SetAngle(Servo_RF, 135);
        Servo_SetAngle(Servo_LB, 135);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_LF, 45);
        Servo_SetAngle(Servo_RB, 45);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_RF, 90);
        Servo_SetAngle(Servo_LB, 90);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_LF, 90);
        Servo_SetAngle(Servo_RB, 90);
        vTaskDelay(SPEED_DELAY);
        dir = 0;
    }
    else
    {
        Servo_SetAngle(Servo_LF, 135);
        Servo_SetAngle(Servo_RB, 135);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_RF, 45);
        Servo_SetAngle(Servo_LB, 45);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_LF, 90);
        Servo_SetAngle(Servo_RB, 90);
        vTaskDelay(SPEED_DELAY);
        Servo_SetAngle(Servo_RF, 90);
        Servo_SetAngle(Servo_LB, 90);
        vTaskDelay(SPEED_DELAY);
        dir = 1;
    }  
}

static void Right(void)
{
    Servo_SetAngle(Servo_LF, 45);
    Servo_SetAngle(Servo_RB, 135);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_RF, 135);
    Servo_SetAngle(Servo_LB, 135);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_LF, 90);
    Servo_SetAngle(Servo_RB, 90);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_RF, 90);
    Servo_SetAngle(Servo_LB, 90);
    vTaskDelay(SPEED_DELAY);
}

static void Left(void)
{
    Servo_SetAngle(Servo_RF, 45);
    Servo_SetAngle(Servo_LB, 135);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_LF, 135);
    Servo_SetAngle(Servo_RB, 135);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_RF, 90);
    Servo_SetAngle(Servo_LB, 90);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_LF, 90);
    Servo_SetAngle(Servo_RB, 90);
    vTaskDelay(SPEED_DELAY);  
}

static void Squat(void)
{
    Servo_SetAngle(Servo_LF, 20);
    Servo_SetAngle(Servo_RF, 20);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_LB, 160);
    Servo_SetAngle(Servo_RB, 160);
    vTaskDelay(SPEED_DELAY);
}

static void StandUp(void)
{
    Servo_SetAngle(Servo_LF, 90);
    Servo_SetAngle(Servo_RF, 90);
    vTaskDelay(SPEED_DELAY);
    Servo_SetAngle(Servo_LB, 90);
    Servo_SetAngle(Servo_RB, 90);
    vTaskDelay(SPEED_DELAY);
}

static void Stop(void)
{
    vTaskDelay(50);
}

static void Wander(void)/*后期需要修改和优化*/
{
    static uint32_t rand;
    static uint16_t exe_per_movement = 100;//每个时间片初步执行100次
    if(exe_per_movement == 100)
    {   
        rand = SysTick->VAL;
        rand %= 8;//0-7范围内取值
    }//只在每个时间片启动时设置随机动作
    if(rand < 3)// 3/8概率前进
    {
        Forward();
    }
    else if(rand >= 3 && rand < 6)// 3/8概率后退
    {
        Backward();        
    }
    else if(rand == 6)// 1/8概率左转
    {
        Left();
    }
    else// 1/8概率右转
    {
        Right();
    }
    exe_per_movement--;
    if(exe_per_movement == 0)
    {
        exe_per_movement = 100;
    }
}

static void Avoid(void)
{
    uint32_t rand = SysTick->VAL;//用系统滴答定时器模拟随机数生成
    if(first_run==true)
    {       
        rand %= 2;//区间为0、1
    }
    /* 随机左转或右转避障 */
    if(rand ==1)
    {
        Left();
    }
    else 
    {
        Right();
    }
}




