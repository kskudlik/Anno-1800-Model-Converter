#pragma once
#include "Triangle.h"
#include "VertexComponents.h"

#include <filesystem>
#include <vector>
struct VertexIndice {
    uint32_t v1, v1t, v1n;
    uint32_t v2, v2t, v2n;
    uint32_t v3, v3t, v3n;
    VertexIndice(int a, int b, int c, int d, int e, int f, int g, int h, int i)
        : v1(a)
        , v1t(b)
        , v1n(c)
        , v2(d)
        , v2t(e)
        , v2n(f)
        , v3(g)
        , v3t(h)
        , v3n(i)
    {
    }
};
struct OBJ_Vertex {
    GeometricVertex<float> v;
    Normal<float>          vn;
    TextureVertex<float>   vt;
    Tangent<float>         tang;
    BiTangent<float>       biTang;
    OBJ_Vertex(GeometricVertex<float> v, Normal<float> vn,
                                                 TextureVertex<float> vt)
        : v(v)
        , vn(vn)
        , vt(vt)
        , tang({0,0,0})
        , biTang({0,0,0})
    {
    }
};
class OBJFile
{
  public:
    OBJFile(std::filesystem::path filePath);

    std::vector<GeometricVertex<float>> v;
    std::vector<Normal<float>>          vn;
    std::vector<TextureVertex<float>>   vt;
    std::vector<VertexIndice>           f;
};