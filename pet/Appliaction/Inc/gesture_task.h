#ifndef GESTURE_TASK_H_
#define GESTURE_TASK_H_

#define GESTURE_TASK_PRIORITY 3
#define GESTURE_TASK_STACK    1024

void Gesture_Task_Start(void);

void Gesture_EXTI_Callback(void);

#endif
