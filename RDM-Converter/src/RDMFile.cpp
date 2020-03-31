#include "RDMFile.h"
#include "Log.h"
#include "VertexData.h"
#include "RDMFrame.h"

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
        const uint32_t offsetToMaterials = *(uint32_t*)&file[offsetToOffsets + 20];

        if (*(uint32_t*)&file[offsetToMaterials - 4] != 28)
            throw FileError(inputPath, "Unknown File Structure: Materialsize != 28");
        if (length - 8 <= offsetToVertices || length - 4 <= offsetToTriangles)
            throw FileError(inputPath, "Unknown File Structure");

        materialsCount = *(uint32_t*)&file[offsetToMaterials - 8];

        for (int i = 0; i < materialsCount; i++) {
            materials.push_back(Material{*(uint32_t*)&file[offsetToMaterials + 28 * i],
                                         *(uint32_t*)&file[offsetToMaterials + 4 + 28 * i],
                                         *(uint32_t*)&file[offsetToMaterials + 8 + 28 * i]});
        }

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
                throw UnsupportedVertexFormat(verticesSize, trianglesSize, inputPath);
                break;
        }

        triangles = (Triangle<uint16_t>*)&file[offsetToTriangles];
        RDM.close();

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
    int materialIndex = 0;
    if (trianglesSize == 4) {
        Triangle<uint32_t>* triangles = (Triangle<uint32_t>*)this->triangles;
        for (uint32_t i = 0; i < trianglesCount; i++) {
            if ((materials[materialIndex].offset / 3) == i) {
                out += "g " + std::to_string(materials[materialIndex].index) + "\n" + "usemtl "
                       + std::to_string(materials[materialIndex].index) + "\n";
                materialIndex++;
            }

            out += triangles[i].toOBJ(normals, textures);
        }
    } else if (trianglesSize == 2) {
        Triangle<uint16_t>* triangles = (Triangle<uint16_t>*)this->triangles;
        for (uint32_t i = 0; i < trianglesCount; i++) {
            if ((materials[materialIndex].offset / 3) == i) {
                out += "g " + std::to_string(materials[materialIndex].index) + "\n" + "usemtl "
                       + std::to_string(materials[materialIndex].index) + "\n";
                materialIndex++;
            }
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
const GeometricVertex<float> operator+(GeometricVertex<float> const& a,
                                       GeometricVertex<float> const& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
const TextureVertex<float> operator+(TextureVertex<float> const& a, TextureVertex<float> const& b)
{
    return {a.u + b.u, a.v + b.v};
}
BiTangent<float> crossTangents(Normal<float>& nor, Tangent<float>& tan)
{

    BiTangent<float> biTang;
    biTang.x = nor.y * tan.z - nor.z * tan.y;
    biTang.y = nor.z * tan.x - nor.x * tan.z;
    biTang.z = nor.x * tan.y - nor.y * tan.x;
    return biTang;
}
void genTangents(Triangle<uint32_t>& tri, std::vector<OBJ_Vertex>& ver)
{

    //GeometricVertex<float> deltaV1  = ver[tri.a].v + ver[tri.b].v;
    GeometricVertex<float> deltaV1 =
        GeometricVertex<float>{ver[tri.a].v.x - ver[tri.b].v.x, ver[tri.a].v.y - ver[tri.b].v.y,
                               ver[tri.a].v.z - ver[tri.b].v.z};
    // GeometricVertex<float> deltaV2  = ver[tri.c].v + ver[tri.b].v;
    GeometricVertex<float> deltaV2 =
        GeometricVertex<float>{ver[tri.c].v.x - ver[tri.b].v.x, ver[tri.c].v.y - ver[tri.b].v.y,
                               ver[tri.c].v.z - ver[tri.b].v.z};
    //TextureVertex<float>   deltaVt1 = ver[tri.a].vt + ver[tri.b].vt;
    TextureVertex<float> deltaVt1 =
        TextureVertex<float>{ver[tri.a].vt.u - ver[tri.b].vt.u, ver[tri.a].vt.u - ver[tri.b].vt.v};
    //TextureVertex<float>   deltaVt2 = ver[tri.c].vt + ver[tri.b].vt;
    TextureVertex<float> deltaVt2 =
        TextureVertex<float>{ver[tri.c].vt.u - ver[tri.b].vt.u, ver[tri.c].vt.v - ver[tri.b].vt.v};

    float r = -1.0F / (deltaVt1.u * deltaVt2.v - deltaVt1.v * deltaVt2.u);

    float x = (deltaV1.x * deltaVt2.v - deltaV2.x * deltaVt1.v) * r;
    float y = (deltaV1.y * deltaVt2.v - deltaV2.y * deltaVt1.v) * r;
    float z = (deltaV1.z * deltaVt2.v - deltaV2.z * deltaVt1.v) * r;

    Tangent<float> normTangent = {x, y, z};
    ver[tri.a].tang            = normTangent;
    ver[tri.b].tang            = normTangent;
    ver[tri.c].tang            = normTangent;
    BiTangent<float> biTang    = crossTangents(ver[tri.b].vn, normTangent);
    ver[tri.a].biTang          = biTang;
    ver[tri.b].biTang          = biTang;
    ver[tri.c].biTang          = biTang;
}
RDMFile::RDMFile(OBJFile& obj)
{
    std::vector<GeometricVertex<float>>& v  = obj.v;
    std::vector<Normal<float>>&          vn = obj.vn;
    std::vector<TextureVertex<float>>&   vt = obj.vt;
    std::vector<VertexIndice>&           f  = obj.f;

    std::map<std::tuple<uint32_t, uint32_t, uint32_t>, uint32_t> fmap;
    uint32_t                                                     trianglesValue[3];
    std::vector<Triangle<uint32_t>>                              rawTriangles;
    rawTriangles.reserve(f.size());
    std::vector<OBJ_Vertex> rawVertices;
    rawVertices.reserve(f.size());
    std::tuple<uint32_t, uint32_t, uint32_t> t;

    for (VertexIndice& vi : f) {

        t = std::make_tuple(vi.v1, vi.v1n, vi.v1t);
        if (fmap.count(t) > 0) {
            trianglesValue[0] = fmap[t];
        } else {
            rawVertices.emplace_back(v[vi.v1], vn[vi.v1n], vt[vi.v1t]);
            trianglesValue[0] = (rawVertices.size() - 1); // TODO bigger than uint16_t!
            fmap[t]           = trianglesValue[0];
        }

        t = std::make_tuple(vi.v2, vi.v2n, vi.v2t);
        if (fmap.count(t) > 0) {
            trianglesValue[1] = fmap[t];
        } else {
            rawVertices.emplace_back(v[vi.v2], vn[vi.v2n], vt[vi.v2t]);
            trianglesValue[1] = (rawVertices.size() - 1); // TODO bigger than uint16_t!
            fmap[t]           = trianglesValue[1];
        }

        t = std::make_tuple(vi.v3, vi.v3n, vi.v3t);
        if (fmap.count(t) > 0) {
            trianglesValue[2] = fmap[t];
        } else {
            rawVertices.emplace_back(v[vi.v3], vn[vi.v3n], vt[vi.v3t]);
            trianglesValue[2] = (rawVertices.size() - 1); // TODO bigger than uint16_t!
            fmap[t]           = trianglesValue[2];
        }

        rawTriangles.emplace_back(trianglesValue);
        genTangents(rawTriangles[rawTriangles.size() - 1], rawVertices);
    }

    if (rawTriangles.size() > 65535) {
        throw std::exception("more than 65535 faces; not yet implemented");
    }
    Triangle<uint16_t>* triangles = new Triangle<uint16_t>[rawTriangles.size()];
    for (int i = 0; i < rawTriangles.size(); i++) {
        triangles[i] = rawTriangles[i].toShort();
    }
    P4h_N4b_G4b_B4b_T2h* vertices = new P4h_N4b_G4b_B4b_T2h[rawVertices.size()];
    for (int i = 0; i < rawVertices.size(); i++) {

        vertices[i] = P4h_N4b_G4b_B4b_T2h(
            GeometricVertex{Half::floatToHalf(rawVertices[i].v.x),
                            Half::floatToHalf(rawVertices[i].v.y),
                            Half::floatToHalf(rawVertices[i].v.z)},
            rawVertices[i].vn.scale(), rawVertices[i].tang.scale(), rawVertices[i].biTang.scale(),
            TextureVertex{Half::floatToHalf(rawVertices[i].vt.u),
                          Half::floatToHalf(1.0F - rawVertices[i].vt.v)});
    } // invert v for rdm

    this->verticesSize   = 24;
    this->verticesCount  = rawVertices.size();
    this->vertices       = vertices;
    this->trianglesSize  = 2;
    this->trianglesCount = rawTriangles.size();
    this->triangles      = triangles;
}
bool RDMFile::toFile(std::filesystem::path outputPath, const char* versionTag)
{
	using namespace RDMFrame;

    char header[sizeof(RDM_HEADER)];
    char footer[sizeof(RDM_FOOTER)];
    memcpy(&header, &RDM_HEADER, sizeof(RDM_HEADER));
    memcpy(&footer, &RDM_FOOTER, sizeof(RDM_FOOTER));

    uint32_t trianglesIndexCount = trianglesCount * 3;
	uint32_t offsetToTriangles = 661 + (verticesCount * verticesSize);
    uint32_t offsetToEnd =
        669 + (verticesCount * verticesSize) + (trianglesSize * trianglesIndexCount);

	memcpy(&header[36], &offsetToEnd, 4);
    memcpy(&header[317], &offsetToTriangles, 4);
    memcpy(&header[621], &trianglesIndexCount, 4);

	uint32_t offsetToEndStrings = offsetToEnd + 36;
    uint32_t offsetToEndString1 = offsetToEndStrings + 56;
    uint32_t offsetToEndString2 = offsetToEndStrings + 78;
    memcpy(&footer[8], &offsetToEndStrings, 4);
    memcpy(&footer[44], &offsetToEndString1, 4);
    memcpy(&footer[48], &offsetToEndString2, 4);
    memcpy(&footer[143], &versionTag[0], 5);

    std::ofstream outfile(outputPath, std::ios::out | std::ios::binary);

	outfile.write(header, sizeof(header));
    outfile.write((const char*)&verticesCount, 4);
    outfile.write((const char*)&verticesSize, 4);
    std::visit(overloaded{[this, &outfile](auto* vertices) {
                   outfile.write((const char*)(&vertices[0]),
                                 (uint64_t)verticesSize * (uint64_t)verticesCount);
               }},
               vertices);
    outfile.write((const char*)&trianglesIndexCount, 4);
    outfile.write((const char*)&trianglesSize, 4);
    outfile.write((const char*)(&triangles[0]),
                  (uint64_t)trianglesSize * (uint64_t)trianglesIndexCount);
    outfile.write(footer, sizeof(footer));
    outfile.close();

    return true; // TODO make this useful
}

