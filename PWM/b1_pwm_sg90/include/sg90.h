#pragma once

#include <stdint.h>

/**
 * @brief 舵机角度
 *
 */
typedef enum sg90_angel {
    SG90_ANGEL_0 = 0, /* 0 度 */
    SG90_ANGEL_45,    /* 45 度 */
    SG90_ANGEL_90,    /* 90 度 */
    SG90_ANGEL_135,   /* 135 度 */
    SG90_ANGEL_180,   /* 180 度 */
    SG90_MAX,         /* 最大角度 */
} sg90_angel_e;

void sg90_init(void);
void sg90_deinit(void);
void sg90_set_angel(sg90_angel_e angel);
