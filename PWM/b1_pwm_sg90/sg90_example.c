#include "ohos_init.h"
#include "los_task.h"

#include "sg90.h"

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief sg90 线程
 *
 * @param args
 */
static void pwm_sg90_thread(void *args)
{
    sg90_init(); // sg90初始化

    while (1) {
        for (uint8_t i = SG90_ANGEL_0; i < SG90_MAX; i++) {
            sg90_set_angel(i);
            LOS_Msleep(1000);
        }
    }
}

/**
 * @brief sg90 pwm 使用例程
 *
 */
void sg90_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)pwm_sg90_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "pwm_sg90_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(sg90_example);
