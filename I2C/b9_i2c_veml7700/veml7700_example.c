#include "ohos_init.h"
#include "los_task.h"

#include "veml7700.h"

#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief veml7700 线程
 *
 * @param args
 */
static void i2c_veml7700_thread(void *args)
{
    double data = 0;

    veml7700_init(); // veml7700 初始化
    veml7700_set_als_conf_0(ALS_GAIN_3, ALS_IT_25MS, ALS_PERS_1, ALS_INT_DISABLE, ALS_SD_POWERON);
    veml7700_set_psm(PSM_MODE_1, PSM_DISABLE);

    while (1) {
        veml7700_read_data(&data);
        printf("light_intensity: %.2f\r\n", data);
        LOS_Msleep(1000);
    }
}

/**
 * @brief veml7700 i2c 使用例程
 *
 */
void veml7700_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)i2c_veml7700_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "i2c_veml7700_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(veml7700_example);
