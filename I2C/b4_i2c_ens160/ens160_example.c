#include "ohos_init.h"
#include "los_task.h"

#include "ens160.h"

#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief ens160 线程
 *
 * @param args
 */
static void i2c_ens160_thread(void *args)
{
    uint16_t data = 0;

    ens160_init(); // ens160 初始化

    while (1) {
        ens160_get_data(&data);
        printf("co2: %d\r\n", data);
        LOS_Msleep(1000);
    }
}

/**
 * @brief ens160 i2c 使用例程
 *
 */
void ens160_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)i2c_ens160_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "i2c_ens160_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(ens160_example);
