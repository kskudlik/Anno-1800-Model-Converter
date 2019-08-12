#pragma once
#include <stdint.h>
#include <string>

template <class type> class Triangle
{
  public:
    std::string toOBJ(bool normals, bool textures)
    {
        // + 1 -> (zero-based numbering in RDM, one-based numbering in OBJ)
        if (normals)
            return "f " + std::to_string(a + 1) + "/" + std::to_string(a + 1) + "/"
                   + std::to_string(a + 1) + " " + std::to_string(b + 1) + "/"
                   + std::to_string(b + 1) + "/" + std::to_string(b + 1) + " "
                   + std::to_string(c + 1) + "/" + std::to_string(c + 1) + "/"
                   + std::to_string(c + 1) + "\n";

        if (textures)
            return "f " + std::to_string(a + 1) + "/" + std::to_string(a + 1) + " "
                   + std::to_string(b + 1) + "/" + std::to_string(b + 1) + " "
                   + std::to_string(c + 1) + "/" + std::to_string(c + 1) + "\n";

        return "f " + std::to_string(a + 1) + " " + std::to_string(b + 1) + " "
               + std::to_string(c + 1) + "\n";
    }
    Triangle<uint16_t> toShort()
    {
        return {(uint16_t)a, (uint16_t)b, (uint16_t)c};
    }
    type a, b, c; // (zero-based numbering in RDM, one-based numbering in OBJ)
    Triangle()
        : a(0)
        , b(0)
        , c(0)
    {
    }
    Triangle(type a, type b, type c)
        : a(a)
        , b(b)
        , c(c){};
    Triangle(type tri[3])
        : a(tri[0])
        , b(tri[1])
        , c(tri[2])
    {
    }
};
