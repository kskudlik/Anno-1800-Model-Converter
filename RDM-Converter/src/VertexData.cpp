#include "VertexData.h"

// let´s just make sure i don´t mess the sizes up anytime soon
static_assert(sizeof(VertexData<P4h>) == P4h);                                                 // 8
static_assert(sizeof(VertexData<P4h_T2h_C4c>) == P4h_T2h_C4c);                                 // 16
static_assert(sizeof(VertexData<P4h_N4b_T2h_I4b>) == P4h_N4b_T2h_I4b);                         // 20
static_assert(sizeof(VertexData<P4h_N4b_G4b_B4b_T2h>) == P4h_N4b_G4b_B4b_T2h);                 // 24
static_assert(sizeof(VertexData<P4h_N4b_G4b_B4b_T2h_I4b>) == P4h_N4b_G4b_B4b_T2h_I4b);         // 28
static_assert(sizeof(VertexData<P4h_N4b_G4b_B4b_T2h_C4b_C4b>) == P4h_N4b_G4b_B4b_T2h_C4b_C4b); // 32
static_assert(sizeof(VertexData<P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b>)
              == P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b); // 56
static_assert(sizeof(VertexData<P3f_N3b_37_T2f>) == P3f_N3b_37_T2f);   // 60
static_assert(sizeof(VertexData<P3f_N3b_41_T2f>) == P3f_N3b_41_T2f);   // 64
static_assert(sizeof(VertexData<P3f_N3b_45_T2f>) == P3f_N3b_45_T2f);   // 68
static_assert(sizeof(VertexData<P3f_N3b_49_T2f>) == P3f_N3b_49_T2f);   // 72
