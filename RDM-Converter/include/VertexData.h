#pragma once
#include "Half.h"
#include "VertexComponents.h"

struct P4h
// 8
// e.g
// graphics\buildings\3rd_party\3rd_party_02_ship\rdm\3rd_party_02_ship_cutout_lod0.rdm
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
};

struct P4h_T2h_C4c
// 16
// e.g
// graphics\buildings\ornamental\cultural_colony01_props_system\cultural_props_system_1x1_system\rdm\cultural_props_1x1_03_decal_detail01_lod0.rdm
// P4h_N4b_T2h graphics\ui\placeholder\camera.rdm is different
// but that´s a don´t care bc afaik it´s only the camera and
// normals and texture isn´t important for that one model
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
    TextureVertex<uint16_t>   vt;
    uint8_t                   UNKNOWN[4];
};

struct P4h_N4b_T2h_I4b
// 20
// e.g
// data\graphics\units\wildlife\pigeon\rdm\pigeon_lod2.rdm
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
    Normal<uint8_t>           vn;
    uint8_t                   padding1;
    TextureVertex<uint16_t>   vt;
    uint8_t                   UNKNOWN[4];
};

struct P4h_N4b_G4b_B4b_T2h
// 24
// e.g
// data\graphics\buildings\ornamental\preorder\rdm\preorder_statue_lod0.rdm
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
    Normal<uint8_t>           vn;
    uint8_t                   padding1;
    Tangent<uint8_t>          tang;
    uint8_t                   padding2;
    BiTangent<uint8_t>        biTang;
    uint8_t                   padding3;
    TextureVertex<uint16_t>   vt;
};

struct P4h_N4b_G4b_B4b_T2h_I4b
// 28
// e.g
// graphics\buildings\production\agriculture_03\rdm\agriculture_03_lod2.rdm
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
    Normal<uint8_t>           vn;
    uint8_t                   padding1;
    Tangent<uint8_t>          tang;
    uint8_t                   padding2;
    BiTangent<uint8_t>        biTang;
    uint8_t                   padding3;
    TextureVertex<uint16_t>   vt;
    uint8_t                   UNKNOWN[4];
};

struct P4h_N4b_G4b_B4b_T2h_C4b_C4b
// 32
// e.g
// graphics\buildings\production\agriculture_01\prp\rdm\agriculture01_field_var0_lod0.rdm
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
    Normal<uint8_t>           vn;
    uint8_t                   padding1;
    Tangent<uint8_t>          tang;
    uint8_t                   padding2;
    BiTangent<uint8_t>        biTang;
    uint8_t                   padding3;
    TextureVertex<uint16_t>   vt;
    uint8_t                   UNKNOWN[8];
};

struct P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b
// 56
// e.g
// graphics\portraits\narrative\3rd_party_02\rdm\3rd_party_02.rdm
{
    GeometricVertex<uint16_t> v;
    uint16_t                  padding0;
    Normal<uint8_t>           vn;
    uint8_t                   padding1;
    Tangent<uint8_t>          tang;
    uint8_t                   padding2;
    BiTangent<uint8_t>        biTang;
    uint8_t                   padding3;
    TextureVertex<uint16_t>   vt;
    uint8_t                   UNKNOWN[32];
};

struct P3f_N3b_37_T2f
// 60
// e.g
// graphics\buildings\3rd_party\3rd_party_08\rdm\3rd_party_08_building_01_cloth_lod0.rdm
{
    GeometricVertex<float> v;
    Normal<uint8_t>        vn;
    uint8_t                UNKNOWN[37];
    TextureVertex<float>   vt;
};

struct P3f_N3b_41_T2f
// 64
// e.g
// graphics\props\flags\rdm\flag_pirate_03.rdm
{
    GeometricVertex<float> v;
    Normal<uint8_t>        vn;
    uint8_t                UNKNOWN[41];
    TextureVertex<float>   vt;
};

struct P3f_N3b_45_T2f
// 64
// e.g
// graphics\campaign\vehicle\colonialist_ship\rdm\frigate_sail__bm_01.rdm
{
    GeometricVertex<float> v;
    Normal<uint8_t>        vn;
    uint8_t                UNKNOWN[45];
    TextureVertex<float>   vt;
};

struct P3f_N3b_49_T2f
// 68
// e.g
// graphics\vehicle\command_ship\rdm\command_ship_sail_bk_01.rdm
{
    GeometricVertex<float> v;
    Normal<uint8_t>        vn;
    uint8_t                UNKNOWN[49];
    TextureVertex<float>   vt;
};
