#include "ohos_init.h"
#include "los_task.h"

#include "sht40.h"

#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief sht40 线程
 *
 * @param args
 */
static void i2c_sht40_thread(void *args)
{
    sht40_data_t data = {0};

    sht40_init(); // sht40 初始化

    while (1) {
        sht40_read_data(&data);
        printf("temperature: %.2f, humidity: %.2f\n", data.temperature, data.humidity);
        LOS_Msleep(1000);
    }
}

/**
 * @brief sht40 i2c 使用例程
 *
 */
void sht40_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)i2c_sht40_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "i2c_sht40_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(sht40_example);
