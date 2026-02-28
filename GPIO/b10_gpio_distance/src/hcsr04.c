#include "hcsr04.h"

#include "iot_gpio.h"
#include "los_task.h"

/* ECHO GPIO */
#define ECHO_GPIO GPIO0_PB1
/* TRIG GPIO */
#define TRIG_GPIO GPIO0_PB0
/* 系统时钟定时器主频40MHz,定时器5作为系统时钟源，每秒24MHz，从0一直累加 */
#define ECHO_TIMER_FREQ 40000000UL

/* 定时器5的CURRENT_VALUE_LOW的基地址 */
static uint32_t *m_ptimer5_current_value_low = (uint32_t *)(0x400000A0U + 0x8U);

/* echo 采集动作 */
typedef enum {
    ECHO_FLAG_CAPTURE_RISE = 0, /* 准备采集上升沿，即开始时间 */
    ECHO_FLAG_CAPTURE_FALL,     /* 准备采集下降沿，即结束时间 */
    ECHO_FLAG_CAPTURE_SUCCESS,  /* 采集成功 */
    ECHO_FLAG_MAX
} echo_flag;

/* 定义与中断相关的采集动作和采集时间相关信息 */
typedef struct echo_data {
    echo_flag flag;       /* 中断采集动作 */
    uint32_t time_rise;   /* 采集上升沿，即开始时间 */
    uint32_t time_fall;   /* 采集下降沿，即结束时间 */
    uint32_t pulse_width; /* 脉宽时间 */
} echo_data_e;

/**
 * @brief echo 数据
 *
 */
echo_data_e echo_data = {
    .flag        = ECHO_FLAG_CAPTURE_RISE,
    .time_rise   = 0,
    .time_fall   = 0,
    .pulse_width = 0,
};

/**
 * @brief 发送 trig 信号
 *
 */
static void hcsr04_send_trig(void)
{
    IoTGpioSetOutputVal(TRIG_GPIO, IOT_GPIO_VALUE1);
    HAL_DelayUs(10);
    IoTGpioSetOutputVal(TRIG_GPIO, IOT_GPIO_VALUE0);
}

/**
 * @brief 捕获ECHO中断函数
 *
 */
static void echo_gpio_isr_func(void)
{
    if (echo_data.flag == ECHO_FLAG_CAPTURE_RISE) {
        echo_data.time_rise = *m_ptimer5_current_value_low;
        echo_data.flag      = ECHO_FLAG_CAPTURE_FALL;
        return;
    }

    if (echo_data.flag == ECHO_FLAG_CAPTURE_FALL) {
        echo_data.time_fall = *m_ptimer5_current_value_low;
        echo_data.flag      = ECHO_FLAG_CAPTURE_SUCCESS;
        return;
    }
}

/**
 * @brief hcsr04 初始化
 *
 */
void hcsr04_init(void)
{
    IoTGpioInit(ECHO_GPIO);
    IoTGpioSetDir(ECHO_GPIO, IOT_GPIO_DIR_IN);
    IoTGpioRegisterIsrFunc(ECHO_GPIO, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_BOTH_TYPE, echo_gpio_isr_func, NULL);
    IoTGpioSetIsrMask(ECHO_GPIO, FALSE);

    IoTGpioInit(TRIG_GPIO);
    IoTGpioSetDir(TRIG_GPIO, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(TRIG_GPIO, IOT_GPIO_VALUE0);
}

/**
 * @brief hcsr04 释放
 *
 */
void hcsr04_deinit(void)
{
    IoTGpioDeinit(ECHO_GPIO);
    IoTGpioDeinit(TRIG_GPIO);
}

/**
 * @brief 获取距离
 *
 * @return double 距离
 */
double hcsr04_get(void)
{
    if (echo_data.flag == ECHO_FLAG_CAPTURE_RISE) {
        hcsr04_send_trig();
    }

    /* 等待下降沿 */
    uint64_t start_tick = LOS_TickCountGet();
    while (echo_data.flag != ECHO_FLAG_CAPTURE_SUCCESS) {
        uint64_t end_tick = LOS_TickCountGet();
        if (LOS_Tick2MS(end_tick - start_tick) > 200) {
            echo_data.flag = ECHO_FLAG_CAPTURE_RISE;
            return -1;
        }
    }
    echo_data.pulse_width = echo_data.time_fall - echo_data.time_rise;

    echo_data.flag = ECHO_FLAG_CAPTURE_RISE;
    /* 距离 = 时间差 * 340米/秒 / 2(超时波来回2次) * 100厘米/米 */
    double data = (double)echo_data.pulse_width / (double)ECHO_TIMER_FREQ * 170.0 * 100.0;

    return data;
}
