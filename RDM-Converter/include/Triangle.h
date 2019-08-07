#pragma once

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
    type        a, b, c; // (zero-based numbering in RDM, one-based numbering in OBJ)
    Triangle(type a, type b, type c)
        : a(a)
        , b(b)
        , c(c){};
};
