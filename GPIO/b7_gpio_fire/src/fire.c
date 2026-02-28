#include "fire.h"

#include "iot_gpio.h"
#include "iot_errno.h"

#include <stdio.h>

/* 火焰传感器 gpio */
#define FIRE_GPIO GPIO0_PB4

/**
 * @brief 火焰传感器初始化
 *
 */
void fire_init(void)
{
    IoTGpioInit(FIRE_GPIO);
}

/**
 * @brief 火焰传感器返初始化
 *
 */
void fire_deinit(void)
{
    IoTGpioDeinit(FIRE_GPIO);
}

/**
 * @brief 获取火焰传感器的值
 *
 * @return true 检测到火焰
 * @return false 未检测到火焰
 */
bool get_fire_value(void)
{
    IotGpioValue gpio_value = IOT_GPIO_VALUE0;
    if (IoTGpioGetInputVal(FIRE_GPIO, &gpio_value) != IOT_SUCCESS) {
        printf("get_fire_value failed");
    }
    return gpio_value == IOT_GPIO_VALUE0;
}