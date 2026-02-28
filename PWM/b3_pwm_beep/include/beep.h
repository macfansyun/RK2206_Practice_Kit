#pragma once

#include <stdint.h>
#include <stdbool.h>

void beep_init(void);
void beep_deinit(void);
void beep_set(bool state);
