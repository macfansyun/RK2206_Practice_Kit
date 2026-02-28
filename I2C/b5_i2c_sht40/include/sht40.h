#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief sht40 结构体
 * 
 */
typedef struct sht40_data {
    double humidity;
    double temperature;
} sht40_data_t, *sht40_data_ptr;

void sht40_init(void);
void sht40_read_data(sht40_data_ptr dat);
