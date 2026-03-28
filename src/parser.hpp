#pragma once
#include "types.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Mesh validateObj(const std::string &path) {
  Mesh mesh;
  std::ifstream file(path);
  std::string line;
  int lineNum = 0;
  while (std::getline(file, line)) {
    lineNum++;
    std::istringstream iss(line);
    std::string token;
    iss >> token;
    if (token != "#" && !token.empty() && token != "vn" && token != "fn") {
      if (token == "v") {
        float x, y, z;
        if (!(iss >> x >> y >> z)) {
          std::cerr << "Error baris " << lineNum
                    << "Input obj tidak valid program gagal. Terima kasih! "
                       "Baca lagi spesifikasi ya"
                    << std::endl;
          exit(1);
        }

        mesh.vertices.push_back({x, y, z});
      } else if (token == "f") {
        std::string s1, s2, s3;
        Triangle temp;

        if (!(iss >> s1 >> s2 >> s3)) {
          std::cerr << "Error. gagal membaca.";
          continue;
        }

        size_t pos1 = s1.find('/');
        std::string justNum1;
        if (pos1 != std::string::npos) {
          justNum1 = s1.substr(0, pos1);
        } else {
          justNum1 = s1;
        }

        int index1 = std::stoi(justNum1);

        size_t pos2 = s2.find('/');
        std::string justNum2;
        if (pos2 != std::string::npos) {
          justNum2 = s2.substr(0, pos2);
        } else {
          justNum2 = s2;
        }

        int index2 = std::stoi(justNum2);

        size_t pos3 = s3.find('/');
        std::string justNum3;
        if (pos3 != std::string::npos) {
          justNum3 = s3.substr(0, pos3);
        } else {
          justNum3 = s3;
        }

        int index3 = std::stoi(justNum3);

        temp.v[0] = index1 - 1;
        temp.v[1] = index2 - 1;
        temp.v[2] = index3 - 1;
        mesh.triangles.push_back(temp);
      }
    }
  }
  return mesh;
}

AABB computeAABB(const Mesh &mesh) {
    if (mesh.vertices.empty()) {
        std::cerr << "Error: mesh tidak memiliki vertex." << std::endl;
        exit(1);
    }
  AABB res;
  Vec3 min = mesh.vertices[0];
  Vec3 max = mesh.vertices[0];
  for (int i = 1; i < mesh.vertices.size(); i++) {
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

AABB expandToCube(AABB aabb) {
  float dx = aabb.max.x - aabb.min.x;
  float dy = aabb.max.y - aabb.min.y;
  float dz = aabb.max.z - aabb.min.z;
  float side = std::max({dx, dy, dz});
  Vec3 center = (aabb.min + aabb.max) * 0.5f;
  aabb.min = center - side * 0.5f;
  aabb.max = center + side * 0.5f;
  return aabb;
}