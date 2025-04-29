#ifndef SERIAL_TASK_H_
#define SERIAL_TASK_H_


#define SERIAL_TASK_PRIORITY 3
#define SERIAL_TASK_STACK    4096
#define SERIAL_MOVE_MASK     0x10
#define SERIAL_DISPLAY_MASK  0x20

void Serial_RXIT_Callback(void);

void Serial_Task_Start(void);

#endif
