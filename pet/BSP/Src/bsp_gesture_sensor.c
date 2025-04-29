#include "i2c.h"
#include <stdint.h>
#include "bsp_gesture_sensor.h"
#include "gesture_data.h"
#include "stdio.h"
#include "stdbool.h"


uint8_t enable_cmd[2] = {0x72, 0x01};  // Bank1 0x72寄存器写1
uint8_t interrupt_cmd[2] = {0x41, 0xFF};
uint8_t switch_bank0[2] = {0xEF, 0x00};

// 唤醒函数（带状态确认）
static HAL_StatusTypeDef PAJ7620_WakeUp(void) 
{
    uint8_t confirm = 0;
    HAL_StatusTypeDef status;
    uint8_t temp_data = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, PAJ7620_W_ADDR, &temp_data, 1, Timeout);
    HAL_Delay(INIT_DELAY_MS);
    // 尝试读取唤醒确认寄存器
    for(uint8_t retry = 0; retry < WAKEUP_RETRY; retry++) {
        status = HAL_I2C_Mem_Read(&hi2c1, PAJ7620_R_ADDR, WAKEUP_CONFIRM_REG,
                                 I2C_MEMADD_SIZE_8BIT, &confirm, 1, Timeout);
        if(status == HAL_OK && confirm == WAKEUP_CONFIRM_VAL) {
            return HAL_OK;
        }        
    }    
    return HAL_ERROR;  // 唤醒失败
}

// 完整初始化流程
HAL_StatusTypeDef PAJ7620_Init(void)
{
    static uint8_t reg_addr;
    static uint8_t reg_val = 0;
    // 唤醒设备
    if(PAJ7620_WakeUp() != HAL_OK) 
        return HAL_ERROR;
    // 写入初始化寄存器配置
    for(uint8_t i = 0; i < REG_INIT_ARRAY_SIZE; i++) 
    {
        reg_addr = initial_register_array[i][0];
        reg_val = initial_register_array[i][1];
        if(HAL_I2C_Mem_Write(&hi2c1, PAJ7620_W_ADDR, reg_addr,
                            I2C_MEMADD_SIZE_8BIT, &reg_val, 1, Timeout) != HAL_OK) {                   
            return HAL_ERROR;
        }       
    }      
    reg_addr = interrupt_cmd[0];
    reg_val = interrupt_cmd[1];
    if(HAL_I2C_Mem_Write(&hi2c1, PAJ7620_W_ADDR, reg_addr,
                        I2C_MEMADD_SIZE_8BIT, &reg_val, 1, Timeout) != HAL_OK){
        return HAL_ERROR; 
    }//启用全部的姿态中断
    reg_addr = switch_bank0[0];
    reg_val = switch_bank0[1];
    if(HAL_I2C_Mem_Write(&hi2c1, PAJ7620_W_ADDR, reg_addr,
                        I2C_MEMADD_SIZE_8BIT, &reg_val, 1, Timeout) != HAL_OK){
        return HAL_ERROR; 
    }// 切换回bank0        
    if(HAL_I2C_Mem_Write(&hi2c1, PAJ7620_W_ADDR, THRESHOLD_CONFIG_REG, I2C_MEMADD_SIZE_8BIT,
                        threshold_config_array, THRESHOLD_CONFIG_SIZE, Timeout) != HAL_OK){
        return HAL_ERROR; 
        };//配置灵敏度阈值
    reg_addr = enable_cmd[0];
    reg_val = enable_cmd[1];       
    return HAL_I2C_Mem_Write(&hi2c1, PAJ7620_W_ADDR, reg_addr,
        I2C_MEMADD_SIZE_8BIT, &reg_val, 1, Timeout);//启动传感器
}

Gesture_StatusTypeDef Gesture_Detect(void)
{
    uint8_t gesture_data[2] = {0};
    Gesture_StatusTypeDef fused_data;
    HAL_I2C_Mem_Read(&hi2c1, PAJ7620_R_ADDR, GES_DATA_REG, I2C_MEMADD_SIZE_8BIT, gesture_data, 2, Timeout);
    fused_data = (Gesture_StatusTypeDef)((gesture_data[1] << 8) | gesture_data[0]);
    return fused_data;
}
