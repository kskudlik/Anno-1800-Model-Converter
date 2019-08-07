#pragma once
#include "Half.h"
#include <string>

template <class type> struct GeometricVertex {
  private:
    type x, y, z;

  public:
    std::string toOBJ();
};

template <class type> struct Normal {
  private:
    type x, y, z;

  public:
    std::string toOBJ();
};

template <class type> struct Tangent {
  private:
    type x, y, z;
};

template <class type> struct BiTangent {
  private:
    type x, y, z;
};

template <class type> struct TextureVertex {
  private:
    type u, v;

  public:
    std::string toOBJ();
};