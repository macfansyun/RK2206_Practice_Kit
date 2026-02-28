#include "sg90.h"

#include "iot_pwm.h"
#include "iot_errno.h"

#include <stdio.h>

/* sg90 pwm */
#define SG90_PWM_PORT EPWMDEV_PWM5_M0

/**
 * @brief sg90 初始化
 *
 */
void sg90_init(void)
{
    uint8_t ret = IOT_FAILURE;

    ret = IoTPwmInit(SG90_PWM_PORT);
    if (ret != IOT_SUCCESS) {
        printf("PWM Init fail\r\n");
    }
}

/**
 * @brief sg90 释放
 *
 */
void sg90_deinit(void)
{
    IoTPwmDeinit(SG90_PWM_PORT);
}

/**
 * @brief sg90 设置角度
 *
 * @param angel sg90_angel_e 角度
 */
void sg90_set_angel(sg90_angel_e angel)
{
    const uint32_t freq            = 50;
    static sg90_angel_e last_angel = SG90_MAX;
    uint16_t duty;

    if (last_angel != angel) {
        switch (angel) {
            case SG90_ANGEL_0:
                duty = (uint16_t)(0.06 * (double)freq);
                break;
            case SG90_ANGEL_45:
                duty = (uint16_t)(0.10 * (double)freq);
                break;
            case SG90_ANGEL_90:
                duty = (uint16_t)(0.15 * (double)freq);
                break;
            case SG90_ANGEL_135:
                duty = (uint16_t)(0.20 * (double)freq);
                break;
            case SG90_ANGEL_180:
                duty = (uint16_t)(0.25 * (double)freq);
                break;
            default:
                return;
        }

        IoTPwmStart(SG90_PWM_PORT, duty, freq);
        last_angel = angel;
    }
}
