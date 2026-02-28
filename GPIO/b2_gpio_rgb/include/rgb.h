#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief rgb 状态
 *
 */
typedef enum rgb_state {
    RGB_GREEN_LIGHT = 0, /* 绿灯 */
    RGB_BLUE_LIGHT,      /* 蓝灯 */
    RGB_RED_LIGHT,       /* 红灯 */
    RGB_WRITE_LIGHT,     /* 白灯 */
    RGB_CLOSE_LIGHT,     /* 关灯 */
} rgb_state_e;

/**
 * @brief rgb 结构体
 *
 */
typedef struct rgb rgb_t, *rgb_p;
struct rgb {
    uint32_t green_gpio; /* 绿色 gpio */
    uint32_t blue_gpio;  /* 蓝色 gpio */
    uint32_t red_gpio;   /* 红色 gpio */
};

void rgb_init(rgb_t *rgb);
void rgb_deinit(rgb_t *rgb);
void rgb_lights_set(rgb_t *rgb, rgb_state_e state);
