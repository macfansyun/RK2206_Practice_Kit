#include "ohos_init.h"
#include "los_task.h"

#include "rgb.h"

#include "iot_gpio.h"

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief rgb 线程
 *
 * @param args
 */
static void gpio_rgb_thread(void *args)
{
    rgb_t rgb = {0};
    rgb.blue_gpio  = GPIO0_PA3;
    rgb.green_gpio = GPIO0_PA2;
    rgb.red_gpio   = GPIO0_PA4;
    rgb_init(&rgb); // rgb 初始化

    while (1) {
        for (uint8_t i = RGB_GREEN_LIGHT; i <= RGB_CLOSE_LIGHT; i++) {
            rgb_lights_set(&rgb, i);
            LOS_Msleep(1000);
        }
    }
}

/**
 * @brief rgb gpio 使用例程
 *
 */
void rgb_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)gpio_rgb_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "pwm_sg90_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(rgb_example);
