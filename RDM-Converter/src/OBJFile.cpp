#include "OBJFile.h"
#include "Log.h"

#include <fstream>
#include <map>
#include <sstream>

OBJFile::OBJFile(std::filesystem::path filePath)
{

    std::string   line, temp, fa, fb, fc;
    char          c;
    float         u, v, x, y, z;
    uint32_t      f1a, f1b, f1c, f2a, f2b, f2c, f3a, f3b, f3c;
    std::ifstream ifstr(filePath);
    if (!ifstr)
        throw std::exception("Can't open file");
    while (getline(ifstr, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream iss(line);
            iss >> temp >> x >> y >> z;
            this->v.emplace_back(x, y, z);
        }
        if (line.substr(0, 3) == "vn ") {
            std::istringstream iss(line);
            iss >> temp >> x >> y >> z;
            this->vn.emplace_back(x, y, z);
        }
        if (line.substr(0, 3) == "vt ") {
            std::istringstream iss(line);
            iss >> temp >> u >> v;
            this->vt.emplace_back(u, v);
        }
        if (line.substr(0, 2) == "f ") {
            std::istringstream iss(line);
            iss >> temp >> fa >> fb >> fc;
            if (std::size_t delimiter = fa.find('/'); delimiter != std::string::npos) {
                std::size_t delimiter2 = fa.find('/', delimiter + 1);
                if (delimiter2 == delimiter + 1)
                    throw std::exception("texture missing"); // TODO
                if (delimiter2 == std::string::npos)
                    throw std::exception("normal missing"); // TODO
            } else {
                throw std::exception("texture, normal missing"); // TODO
            }
            std::istringstream f1(fa);
            f1 >> f1a >> c >> f1b >> c >> f1c;
            std::istringstream f2(fb);
            f2 >> f2a >> c >> f2b >> c >> f2c;
            std::istringstream f3(fc);
            f3 >> f3a >> c >> f3b >> c >> f3c;
            // f**k obj everything is off by one... only toddlers count like that
            f.emplace_back(f1a - 1, f1b - 1, f1c - 1, f2a - 1, f2b - 1, f2c - 1, f3a - 1, f3b - 1,
                           f3c - 1);
        }
    }
    if (!this->f.size())
        throw std::exception("faces missing"); // TODO
    if (!this->v.size())
        throw std::exception("geometric missing"); // TODO
    if (!this->vn.size())
        throw std::exception("normals missing"); // TODO
    if (!this->vt.size())
        throw std::exception("textures missing"); // TODO

    spdlog::info("GeometricVerticesCount: {} - NormalCount: {} - TextureCount: {} - FaceCount: {}",
                 this->v.size(), this->vn.size(), this->vt.size(), this->f.size());
}
