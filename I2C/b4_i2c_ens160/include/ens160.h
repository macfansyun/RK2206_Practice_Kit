#pragma once

#include <stdint.h>
#include <stdbool.h>

void ens160_init(void);
void ens160_set_temperature_humidity(double temperature, double humidity);
void ens160_get_data(uint16_t *data);
