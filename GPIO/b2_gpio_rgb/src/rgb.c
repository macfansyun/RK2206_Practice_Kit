#include "rgb.h"

#include "iot_gpio.h"
#include "iot_errno.h"

/**
 * @brief rgb 初始化
 *
 * @param rgb rgb_t * 指针
 */
void rgb_init(rgb_t *rgb)
{
    IoTGpioInit(rgb->red_gpio);
    IoTGpioSetDir(rgb->red_gpio, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(rgb->red_gpio, IOT_GPIO_VALUE0);

    IoTGpioInit(rgb->blue_gpio);
    IoTGpioSetDir(rgb->blue_gpio, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(rgb->blue_gpio, IOT_GPIO_VALUE0);

    IoTGpioInit(rgb->green_gpio);
    IoTGpioSetDir(rgb->green_gpio, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE0);
}

/**
 * @brief rgb 注销
 *
 * @param rgb rgb_t * 指针
 */
void rgb_deinit(rgb_t *rgb)
{
    IoTGpioDeinit(rgb->red_gpio);
    IoTGpioDeinit(rgb->green_gpio);
    IoTGpioDeinit(rgb->blue_gpio);
}

/**
 * @brief 设置 rgb 显示
 *
 * @param rgb rgb_t * 指针
 * @param state rgb_state_e rgb 状态
 */
void rgb_lights_set(rgb_t *rgb, rgb_state_e state)
{
    switch (state)
    {
    case RGB_GREEN_LIGHT:
        IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(rgb->blue_gpio, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(rgb->red_gpio, IOT_GPIO_VALUE0);
        break;
    case RGB_BLUE_LIGHT:
        IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(rgb->blue_gpio, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(rgb->red_gpio, IOT_GPIO_VALUE0);
        break;
    case RGB_RED_LIGHT:
        IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(rgb->blue_gpio, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(rgb->red_gpio, IOT_GPIO_VALUE1);
        break;
    case RGB_WRITE_LIGHT:
        IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(rgb->blue_gpio, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(rgb->red_gpio, IOT_GPIO_VALUE1);
        break;
    case RGB_CLOSE_LIGHT:
        IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(rgb->blue_gpio, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(rgb->red_gpio, IOT_GPIO_VALUE0);
        break;
    }
}
