#include "beep.h"

#include "iot_pwm.h"

/* beep pwm */
#define BEEP_PORT EPWMDEV_PWM7_M0

/**
 * @brief 蜂鸣器初始化
 *
 */
void beep_init(void)
{
    IoTPwmInit(BEEP_PORT);
}

/**
 * @brief 蜂鸣器释放
 *
 */
void beep_deinit(void)
{
    IoTPwmDeinit(BEEP_PORT);
}

/**
 * @brief 蜂鸣器设置状态
 *
 * @param state bool 开关
 */
void beep_set(bool state)
{
    if (state)
    {
        IoTPwmStart(BEEP_PORT, 50, 500);
    }
    else
    {
        IoTPwmStop(BEEP_PORT);
    }
}
