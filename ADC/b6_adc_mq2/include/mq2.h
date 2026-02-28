#pragma once

#include <stdint.h>
#include <stdbool.h>

void mq2_init(void);
void mq2_deinit(void);
void mq2_ppm_calibration(void);
double get_mq2_ppm(void);
