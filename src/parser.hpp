#include "types.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

Mesh validate_obj(const std::string &path)
{
    Mesh mesh;
    std::ifstream file(path);
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line))
    {
        lineNum++;
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if (token != "#" && !token.empty() && token != "vn" && token != "fn")
        {
            if (token == "v")
            {
                float x, y, z;
                if (!(iss >> x >> y >> z))
                {
                    std::cerr << "Error baris " << lineNum << "Input obj tidak valid program gagal. Terima kasih! Baca lagi spesifikasi ya" << std::endl;
                    exit(1);
                }

                mesh.vertices.push_back({x, y, z});
                std::cout << token << " " << x << " " << y << " " << z << std::endl;
            }
            else if (token == "f")
            {
                int a, b, c;
                Triangle temp;
                if (!(iss >> a >> b >> c))
                {
                    std::cerr << "Error baris " << lineNum << "Input obj tidak valid program gagal. Terima kasih! Baca lagi spesifikasi ya" << std::endl;
                    exit(1);
                }
                temp.v[0] = a - 1;
                temp.v[1] = b - 1;
                temp.v[2] = c - 1;
                mesh.triangles.push_back(temp);
                std::cout << token << " " << a << " " << b << " " << c << std::endl;
            }
            else
            {
                std::cerr << "Error baris " << lineNum << "Input obj tidak valid program gagal. Terima kasih! Baca lagi spesifikasi ya" << std::endl;
                exit(1);
            }
        }
    }
    return mesh;
}

AABB computeAABB(const Mesh &mesh)
{
    AABB res;
    Vec3 min = mesh.vertices[0];
    Vec3 max = mesh.vertices[0];
    for (int i = 1; i < mesh.vertices.size(); i++)
    {
        min.x = std::min(min.x, mesh.vertices[i].x);
        min.y = std::min(min.y, mesh.vertices[i].y);
        min.z = std::min(min.z, mesh.vertices[i].z);
        max.x = std::max(max.x, mesh.vertices[i].x);
        max.y = std::max(max.y, mesh.vertices[i].y);
        max.z = std::max(max.z, mesh.vertices[i].z);
    }
    res.min = min;
    res.max = max;
    return res;
}

AABB expandToCube(AABB aabb)
{
    float dx = aabb.max.x - aabb.min.x;
    float dy = aabb.max.y - aabb.min.y;
    float dz = aabb.max.z - aabb.min.z;
    float side = std::max({dx, dy, dz});
    Vec3 center = (aabb.min + aabb.max) * 0.5f;
    aabb.min = center - side * 0.5f;
    aabb.max = center + side * 0.5f;
    return aabb;
}