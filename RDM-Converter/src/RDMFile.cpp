#include "RDMFile.h"
#include "Log.h"
#include "VertexData.h"

#include <fstream>
#include <variant>

template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

RDMFile::RDMFile(std::filesystem::path inputPath)
{
    std::ifstream RDM(inputPath, std::ifstream::binary);
    if (RDM) {

        RDM.seekg(0, RDM.end);
        const uint64_t length = RDM.tellg();
        RDM.seekg(0, RDM.beg);
        file = std::make_unique<char[]>(length);
        RDM.read(file.get(), length);
        RDM.close();

        if (length < 32)
            throw FileError(inputPath, "Smaller than 32 Bytes");

        const uintptr_t offsetToOffsets = *(uint32_t*)&file[32];

        if (length - 16 < offsetToOffsets)
            throw FileError(inputPath, "Unknown File Structure");

        const uint32_t offsetToVertices  = *(uint32_t*)&file[offsetToOffsets + 12];
        const uint32_t offsetToTriangles = *(uint32_t*)&file[offsetToOffsets + 16];

        if (length - 8 <= offsetToVertices || length - 4 <= offsetToTriangles)
            throw FileError(inputPath, "Unknown File Structure");

        verticesSize  = *(uint32_t*)&file[offsetToVertices - 4];
        verticesCount = *(uint32_t*)&file[offsetToVertices - 8];

        trianglesCount = *(unsigned long*)&file[offsetToTriangles - 8] / 3;
        trianglesSize  = *(unsigned long*)&file[offsetToTriangles - 4];

        switch (verticesSize) {
            case sizeof(P4h): {
                vertices = reinterpret_cast<P4h*>(&file[offsetToVertices]);
            } break;
            case sizeof(P4h_T2h_C4c): {
                vertices = reinterpret_cast<P4h_T2h_C4c*>(&file[offsetToVertices]);
            } break;
            case sizeof(P4h_N4b_T2h_I4b): {
                vertices = reinterpret_cast<P4h_N4b_T2h_I4b*>(&file[offsetToVertices]);
            } break;
            case sizeof(P4h_N4b_G4b_B4b_T2h): {
                vertices = reinterpret_cast<P4h_N4b_G4b_B4b_T2h*>(&file[offsetToVertices]);
            } break;
            case sizeof(P4h_N4b_G4b_B4b_T2h_I4b): {
                vertices = reinterpret_cast<P4h_N4b_G4b_B4b_T2h_I4b*>(&file[offsetToVertices]);
            } break;
            case sizeof(P4h_N4b_G4b_B4b_T2h_C4b_C4b): {
                vertices = reinterpret_cast<P4h_N4b_G4b_B4b_T2h_C4b_C4b*>(&file[offsetToVertices]);
            } break;
            case sizeof(P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b): {
                vertices = reinterpret_cast<P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b*>(
                    &file[offsetToVertices]);
            } break;
            case sizeof(P3f_N3b_37_T2f): {
                vertices = reinterpret_cast<P3f_N3b_37_T2f*>(&file[offsetToVertices]);
            } break;
            case sizeof(P3f_N3b_41_T2f): {
                vertices = reinterpret_cast<P3f_N3b_41_T2f*>(&file[offsetToVertices]);
            } break;
            case sizeof(P3f_N3b_45_T2f): {
                vertices = reinterpret_cast<P3f_N3b_45_T2f*>(&file[offsetToVertices]);
            } break;
            case sizeof(P3f_N3b_49_T2f): {
                vertices = reinterpret_cast<P3f_N3b_49_T2f*>(&file[offsetToVertices]);
            } break;
            default:
                throw UnsupportedVertexFormat(verticesSize, trianglesSize);
                break;
        }

        triangles = (Triangle<uint16_t>*)&file[offsetToTriangles];

    } else {
        throw FileError(inputPath, "Can't open file");
    }
}
std::string RDMFile::verticesToOBJ() noexcept
{
    std::string out;

    std::visit(overloaded{[this, &out](P4h* vertices) {
                              for (uint32_t i = 0; i < verticesCount; i++) {
                                  out += vertices[i].v.toOBJ();
                              }
                          },
                          [this, &out](P4h_T2h_C4c* vertices) {
                              for (uint32_t i = 0; i < verticesCount; i++) {
                                  out += vertices[i].v.toOBJ();
                                  out += vertices[i].vt.toOBJ();
                              }
                          },
                          [this, &out](auto* vertices) {
                              for (uint32_t i = 0; i < verticesCount; i++) {
                                  out += vertices[i].v.toOBJ();
                                  out += vertices[i].vn.toOBJ();
                                  out += vertices[i].vt.toOBJ();
                              }
                          }},
               vertices);

    return out;
}

std::string RDMFile::trianglesToOBJ()
{
    std::string out;
    bool        normals  = true;
    bool        textures = true;
    if (std::holds_alternative<P4h_T2h_C4c*>(vertices) || std::holds_alternative<P4h*>(vertices))
        normals = false;
    if (std::holds_alternative<P4h*>(vertices))
        textures = false;
    if (trianglesSize == 4) {
        Triangle<uint32_t>* triangles = (Triangle<uint32_t>*)this->triangles;
        for (uint32_t i = 0; i < trianglesCount; i++) {
            out += triangles[i].toOBJ(normals, textures);
        }
    } else if (trianglesSize == 2) {
        Triangle<uint16_t>* triangles = (Triangle<uint16_t>*)this->triangles;
        for (uint32_t i = 0; i < trianglesCount; i++) {
            out += triangles[i].toOBJ(normals, textures);
        }
    } else {
        throw std::logic_error("Should not be called when it's a UnsupportedVertexFormat");
    }

    return out;
}
bool RDMFile::toOBJFile(std::filesystem::path outputPath)
{
    std::filesystem::create_directories(outputPath.parent_path());
    std::ofstream obj;
    obj.open(outputPath);
    obj << this->verticesToOBJ();
    obj << this->trianglesToOBJ();
    obj.close();

    return true; // TODO make this useful...
}
void RDMFile::convertDirectoryToOBJ(std::filesystem::path inputDirectory,
                                    std::filesystem::path outputDirectory)
{
    std::filesystem::path outputPathTemp;
    for (auto& p : std::filesystem::recursive_directory_iterator(inputDirectory)) {

        if (p.path().extension() == ".rdm") {
            RDMFile RDM(p.path());
            outputPathTemp = p.path();

            RDM.toOBJFile(outputDirectory
                          / outputPathTemp.replace_extension(".obj").string().replace(
                              0, inputDirectory.string().size(), ""));
        }
    }
}