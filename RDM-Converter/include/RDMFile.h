#pragma once
#include "Triangle.h"
#include "VertexData.h"

#include <filesystem>
#include <variant>

class RDMFile
{
  public:
    RDMFile(std::filesystem::path inputPath);
    ~RDMFile();
    bool        toOBJFile(std::filesystem::path outputPath);
    std::string trianglesToOBJ();
    std::string verticesToOBJ();

  private:
    struct UnsupportedVertexFormat : public std::exception {
        unsigned int verticesSize;
        unsigned int trianglesSize;
        UnsupportedVertexFormat(uint32_t vertexSize, uint32_t triangleSize,
                                const char* msg = "UnsupportedVertexFormat")
            : std::exception(msg)
            , verticesSize(vertexSize)
            , trianglesSize(triangleSize)
        {
        }

      public:
        unsigned int getVertexSize()
        {
            return verticesSize;
        }
        unsigned int getTriangleSize()
        {
            return trianglesSize;
        }
    };
    struct FileError : public std::exception {
        std::filesystem::path path;
        FileError(std::filesystem::path path, const char* msg = "FileError")
            : std::exception(msg)
            , path(path)
        {
        }

      public:
        std::filesystem::path getPath()
        {
            return path;
        }
    };
    std::unique_ptr<char[]> file;
    uint32_t                verticesSize; // byte size
    uint32_t                verticesCount;
    std::variant<P4h*, P4h_T2h_C4c*, P4h_N4b_T2h_I4b*, P4h_N4b_G4b_B4b_T2h*,
                 P4h_N4b_G4b_B4b_T2h_I4b*, P4h_N4b_G4b_B4b_T2h_C4b_C4b*,
                 P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b*, P3f_N3b_37_T2f*,
                 P3f_N3b_41_T2f*, P3f_N3b_45_T2f*, P3f_N3b_49_T2f*>
        vertices;

    uint32_t            trianglesSize;  // byte size
    uint32_t            trianglesCount; // actual amount of triangles (COUNT IN RDM/3)
    Triangle<uint16_t>* triangles;
};
