#include "ohos_init.h"
#include "los_task.h"

#include "mq2.h"

#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief mq2 线程
 *
 * @param args
 */
static void adc_mq2_thread(void *args)
{
    mq2_init();          // mq2 初始化
    mq2_ppm_calibration(); // mq2 配准

    while (1) {
        double data = get_mq2_ppm();
        printf("mq2: %.2f\r\n", data);
        LOS_Msleep(1000);
    }
}

/**
 * @brief mq2 i2c 使用例程
 *
 */
void mq2_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)adc_mq2_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "adc_mq2_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(mq2_example);
