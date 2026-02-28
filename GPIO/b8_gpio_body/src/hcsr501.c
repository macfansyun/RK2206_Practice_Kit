#include "hcsr501.h"

#include "iot_gpio.h"

/* hcsr501 GPIO */
#define HCSR501_GPIO GPIO0_PB5

/**
 * @brief hcsr505 初始化
 * 
 */
void hcsr501_init(void)
{
    IoTGpioInit(HCSR501_GPIO);
    IoTGpioSetDir(HCSR501_GPIO, IOT_GPIO_DIR_IN);
}

/**
 * @brief hcsr501 释放
 * 
 */
void hcsr501_deinit(void)
{
    IoTGpioDeinit(HCSR501_GPIO);
}

/**
 * @brief 获取 hcsr501 值
 * 
 * @return true 
 * @return false 
 */
bool hcsr501_get(void)
{
    IotGpioValue value = IOT_GPIO_VALUE0;

    IoTGpioGetInputVal(HCSR501_GPIO, &value);
    if (value == IOT_GPIO_VALUE0)
    {
        return false;
    }
    else if (value == IOT_GPIO_VALUE1)
    {
        return true;
    }
}
