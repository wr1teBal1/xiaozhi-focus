#ifndef DISPLAY_TASK_H_
#define DISPLAY_TASK_H_

#define DISPLAY_TASK_PRIORITY 2//不能太高
#define DISPLAY_TASK_STACK    4096

typedef enum
{
    Display_Happy       =0x10U,
    Display_Angry       =0x11U,
    Display_Sad         =0x12U,
    Display_Listening   =0x13U,
    Display_Speaking    =0x14U,
    Display_Idle        =0x15U
} Dispaly_TypeDef;

void Display_Task_Start(void);

void SetDisplayState(Dispaly_TypeDef new_display_state);

Dispaly_TypeDef GetDisplayState(void);

#endif
