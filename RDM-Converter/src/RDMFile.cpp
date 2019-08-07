#include "pch.h"
#include "RDMFile.h"

RDMFile::RDMFile(std::filesystem::path inputPath)
{
    std::ifstream RDM(inputPath, std::ifstream::binary);
    if (RDM) {

        RDM.seekg(0, RDM.end);
        uint64_t length = RDM.tellg();
        RDM.seekg(0, RDM.beg);
        file = new char[length];
        RDM.read(file, length);
        RDM.close();

        if (length < 32)
            throw FileError(inputPath, "Smaller than 32 Bytes");

        uint32_t offsetToOffsets = *(uint32_t*)&file[32];

        if (length - 16 < offsetToOffsets)
            throw FileError(inputPath, "Unknown File Structure");

        uint32_t offsetToVertices  = *(uint32_t*)&file[offsetToOffsets + 12];
        uint32_t offsetToTriangles = *(uint32_t*)&file[offsetToOffsets + 16];

        if (length - 8 <= offsetToVertices || length - 4 <= offsetToTriangles)
            throw FileError(inputPath, "Unknown File Structure");

        verticesSize  = *(uint32_t*)&file[offsetToVertices - 4];
        verticesCount = *(uint32_t*)&file[offsetToVertices - 8];

        trianglesCount = *(unsigned long*)&file[offsetToTriangles - 8] / 3;
        trianglesSize  = *(unsigned long*)&file[offsetToTriangles - 4];

        switch (verticesSize) {
            case (P4h):
            case (P4h_T2h_C4c):
            case (P4h_N4b_T2h_I4b):
            case (P4h_N4b_G4b_B4b_T2h):
            case (P4h_N4b_G4b_B4b_T2h_I4b):
            case (P4h_N4b_G4b_B4b_T2h_C4b_C4b):
            case (P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b):
            case (P3f_N3b_37_T2f):
            case (P3f_N3b_41_T2f):
            case (P3f_N3b_45_T2f):
            case (P3f_N3b_49_T2f):
                vertexFormat = (VertexFormat)verticesSize;
                break;
            default:
                throw UnsupportedVertexFormat(verticesSize, trianglesSize);
                break;
        }
        // data format doesn't matter yet.. just "default" template for now
        vertices  = (VertexData<P4h_N4b_G4b_B4b_T2h>*)&file[offsetToVertices];
        triangles = (Triangle<uint16_t>*)&file[offsetToTriangles];

    } else {
        throw FileError(inputPath, "Can't open file");
    }
}
RDMFile::~RDMFile()
{

    spdlog::trace("char* alloc file deleted");
    delete[] file;
}
std::string RDMFile::verticesToOBJ()
{
    std::string out;
    if (vertexFormat == P4h) {
        VertexData<P4h>* vertices = (VertexData<P4h>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
        }
    } else if (vertexFormat == P4h_T2h_C4c) {
        VertexData<P4h_T2h_C4c>* vertices = (VertexData<P4h_T2h_C4c>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P4h_N4b_T2h_I4b) {
        VertexData<P4h_N4b_T2h_I4b>* vertices = (VertexData<P4h_N4b_T2h_I4b>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P4h_N4b_G4b_B4b_T2h) {
        VertexData<P4h_N4b_G4b_B4b_T2h>* vertices =
            (VertexData<P4h_N4b_G4b_B4b_T2h>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P4h_N4b_G4b_B4b_T2h_I4b) {
        VertexData<P4h_N4b_G4b_B4b_T2h_I4b>* vertices =
            (VertexData<P4h_N4b_G4b_B4b_T2h_I4b>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P4h_N4b_G4b_B4b_T2h_C4b_C4b) {
        VertexData<P4h_N4b_G4b_B4b_T2h_C4b_C4b>* vertices =
            (VertexData<P4h_N4b_G4b_B4b_T2h_C4b_C4b>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b) {
        VertexData<P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b>* vertices =
            (VertexData<P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P3f_N3b_37_T2f) {
        VertexData<P3f_N3b_37_T2f>* vertices = (VertexData<P3f_N3b_37_T2f>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P3f_N3b_41_T2f) {
        VertexData<P3f_N3b_41_T2f>* vertices = (VertexData<P3f_N3b_41_T2f>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P3f_N3b_45_T2f) {
        VertexData<P3f_N3b_45_T2f>* vertices = (VertexData<P3f_N3b_45_T2f>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else if (vertexFormat == P3f_N3b_49_T2f) {
        VertexData<P3f_N3b_49_T2f>* vertices = (VertexData<P3f_N3b_49_T2f>*)this->vertices;
        for (uint32_t i = 0; i < verticesCount; i++) {
            out += vertices[i].v.toOBJ();
            out += vertices[i].vn.toOBJ();
            out += vertices[i].vt.toOBJ();
        }
    } else {
        throw std::logic_error("Should not be called when it's a UnsupportedVertexFormat");
    }

    return out;
}
std::string RDMFile::trianglesToOBJ()
{
    std::string out;
    bool        normals  = true;
    bool        textures = true;
    if (vertexFormat == P4h_T2h_C4c || verticesSize == P4h)
        normals = false;
    if (vertexFormat == P4h)
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

    std::ofstream obj;
    obj.open(outputPath);
    obj << this->verticesToOBJ();
    obj << this->trianglesToOBJ();
    obj.close();
	
    return true; //TODO make this useful...
}
