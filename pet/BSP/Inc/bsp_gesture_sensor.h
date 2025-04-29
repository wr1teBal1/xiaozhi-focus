#ifndef BSP_GESTURE_H_
#define BSP_GESTURE_H_

#include "main.h"

#define PAJ7620_W_ADDR          (0x73<<1)   // 7-bit I2C地址
#define PAJ7620_R_ADDR          ((0x73<<1)|0x01)   // 7-bit I2C地址
#define REG_BANK_SELECT         0xEF        // 寄存器Bank切换地址
#define WAKEUP_CONFIRM_REG      0x00        // 唤醒确认寄存器
#define WAKEUP_CONFIRM_VAL      0x20        // 唤醒成功返回值
#define INIT_DELAY_MS           5           // 初始化延时
#define WAKEUP_RETRY            5
#define GES_DATA_REG            0x43
#define THRESHOLD_CONFIG_REG    0x93
#define THRESHOLD_CONFIG_SIZE   5
// #define ENABLE_CONFIG_REG   0x72
// #define ENABLE_CONFIG_VAL   0x01
#define Timeout                 10000
#define REG_INIT_ARRAY_SIZE     50 

extern const uint8_t initial_register_array[REG_INIT_ARRAY_SIZE][2];
extern uint8_t threshold_config_array[THRESHOLD_CONFIG_SIZE];

typedef enum{
    Ges_None             = 0x0000U,
    Ges_Up               = 0x0001U,
    Ges_Down             = 0x0002U,
    Ges_Left             = 0x0004U,
    Ges_Right            = 0x0008U,
    Ges_Forward          = 0x0010U,
    Ges_Backward         = 0x0020U,
    Ges_Clockwise        = 0x0040U,
    Ges_ClockwiseCounter = 0x0080U,
    Ges_Wave             = 0x0100U 
}Gesture_StatusTypeDef;

HAL_StatusTypeDef PAJ7620_Init(void);

Gesture_StatusTypeDef Gesture_Detect(void);

#endif
