#include "ohos_init.h"
#include "los_task.h"

#include "beep.h"

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief beep 线程
 *
 * @param args
 */
static void pwm_beep_thread(void *args)
{
    beep_init(); // beep 初始化

    while (1) {
        beep_set(true);
        LOS_Msleep(1000);
        beep_set(false);
        LOS_Msleep(1000);
    }
}

/**
 * @brief beep pwm 使用例程
 *
 */
void beep_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)pwm_beep_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "pwm_beep_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(beep_example);
