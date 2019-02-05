#pragma once

#include <stdint.h>

void exportToWav(int32_t slot, const char *filename, uint8_t channels, int32_t freq);
