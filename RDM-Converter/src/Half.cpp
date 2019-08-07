#include "pch.h"
#include "Half.h"

float Half::halfToFloat(uint16_t half)
{
    return *((float*)&_mm_cvtph_ps((*(__m128i*)&half)));
}
uint16_t Half::floatToHalf(float input)
{
    return *(uint16_t*)&_mm_cvtps_ph((*(__m128*)&input), _MM_FROUND_TO_NEAREST_INT);
}