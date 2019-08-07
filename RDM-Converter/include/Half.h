#pragma once
#include <immintrin.h>
#include <stdint.h>

namespace Half
{
float    halfToFloat(uint16_t half);
uint16_t floatToHalf(float input);
}; // namespace Half
