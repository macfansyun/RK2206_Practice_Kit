#include "ohos_init.h"
#include "los_task.h"

#include "hcsr501.h"

#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief body 线程
 *
 * @param args
 */
static void gpio_body_thread(void *args)
{
    hcsr501_init(); // body 初始化

    while (1) {
        bool data = hcsr501_get();
        printf("body: %s\r\n", data ? "true" : "false");
        LOS_Msleep(1000);
    }
}

/**
 * @brief body gpio 使用例程
 *
 */
void body_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)gpio_body_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "gpio_body_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(body_example);
