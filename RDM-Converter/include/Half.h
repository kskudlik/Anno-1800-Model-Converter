#pragma once
#include <immintrin.h>
#include <stdint.h>

namespace Half
{
float    halfToFloat(uint16_t half) noexcept;
uint16_t floatToHalf(float input) noexcept;
}; // namespace Half
