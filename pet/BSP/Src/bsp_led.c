#include "bsp_led.h"
#include "gpio.h"


void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(GPIOB, LED_Pin);
}
