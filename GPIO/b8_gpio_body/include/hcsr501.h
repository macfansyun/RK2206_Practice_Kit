#pragma once

#include <stdint.h>
#include <stdbool.h>

void hcsr501_init(void);
void hcsr501_deinit(void);
bool hcsr501_get(void);
