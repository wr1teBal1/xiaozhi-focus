#ifndef MOVE_TASK_H_
#define MOVE_TASK_H_

#include <stdint.h>

#define MOVE_TASK_PRIORITY 1    //给低！！便于其他任务打断
#define MOVE_TASK_STACK    4096
#define SPEED_DELAY 100

typedef enum
{
    Move_Stop       = 0x00U,
    Move_Forward    = 0x01U,
    Move_Backward   = 0x02U,
    Move_Left       = 0x03U,
    Move_Right      = 0x04U,
    Move_Squat      = 0x05U,
    Move_Standup    = 0x06U,
    Move_Wander     = 0x07U,
    Move_Avoid      = 0x08U
} Movement_TypeDef;

void Move_Task_Start(void);

Movement_TypeDef GetMoveState(void);

void SetMoveState(Movement_TypeDef new_move_state);

#endif
