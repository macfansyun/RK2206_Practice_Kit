#include "ohos_init.h"
#include "los_task.h"

#include "fire.h"

#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief fire 线程
 *
 * @param args
 */
static void gpio_fire_thread(void *args)
{
    fire_init(); // fire 初始化

    while (1) {
        bool data = get_fire_value();
        printf("fire: %s\r\n", data ? "true" : "false");
        LOS_Msleep(1000);
    }
}

/**
 * @brief fire gpio 使用例程
 *
 */
void fire_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)gpio_fire_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "gpio_fire_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(fire_example);
