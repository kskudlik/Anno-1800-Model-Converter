#include "pch.h"
#include "Triangle.h"

// let´s just make sure i don´t mess the sizes up anytime soon
static_assert(sizeof(Triangle<uint16_t>) == 6);
static_assert(sizeof(Triangle<uint32_t>) == 12);