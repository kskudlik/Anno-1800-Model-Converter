#pragma once
#include "Half.h"
#include <string>

template <class type> struct GeometricVertex {

    type x, y, z;

    std::string toOBJ();
    GeometricVertex()
        : x(0)
        , y(0)
        , z(0)
    {

    }
    GeometricVertex(type x, type y, type z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
};

template <class type> struct Normal {

    type x, y, z;
    Normal<uint8_t> scale();
    std::string toOBJ();
    Normal()
        : x(0)
        , y(0)
        , z(0)
    {
    }
    Normal(type x, type y, type z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
};

template <class type> struct Tangent {

    type x, y, z;
    Tangent<uint8_t> scale();
};

template <class type> struct BiTangent {

    type x, y, z;
    BiTangent<uint8_t> scale();
};

template <class type> struct TextureVertex {

    type u, v;

    std::string toOBJ();
    TextureVertex()
        : u(0)
        , v(0)
    {
    }
    TextureVertex(type u, type v)
        : u(u)
        , v(v)
    {
    }
};