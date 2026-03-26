#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include "types.hpp"
#include <string>
#include <filesystem>
#include <cstdio>
namespace fs = std::filesystem;

VoxelMesh generateVoxelMesh(const std::vector<OctreeNode*>& voxels) {
    VoxelMesh result;
    int faceIndices[12][3] = {
        {0,2,1}, {0,3,2},
        {4,5,6}, {4,6,7},
        {0,1,5}, {0,5,4},
        {2,3,7}, {2,7,6},
        {0,4,7}, {0,7,3},
        {1,2,6}, {1,6,5}
    };

    for (int n = 0; n < (int)voxels.size(); n++) {
        Vec3 mn = voxels[n]->bounds.min;
        Vec3 mx = voxels[n]->bounds.max;

        result.vertices.push_back({mn.x, mn.y, mn.z});
        result.vertices.push_back({mx.x, mn.y, mn.z});
        result.vertices.push_back({mx.x, mx.y, mn.z});
        result.vertices.push_back({mn.x, mx.y, mn.z});
        result.vertices.push_back({mn.x, mn.y, mx.z});
        result.vertices.push_back({mx.x, mn.y, mx.z});
        result.vertices.push_back({mx.x, mx.y, mx.z});
        result.vertices.push_back({mn.x, mx.y, mx.z});

        int offset = n * 8 + 1;
        for (int f = 0; f < 12; f++) {
            Triangle tri;
            tri.v[0] = offset + faceIndices[f][0];
            tri.v[1] = offset + faceIndices[f][1];
            tri.v[2] = offset + faceIndices[f][2];
            result.faces.push_back(tri);
        }
    }
    return result;
}

void objWriter(VoxelMesh res, const std::string &input_path)
{
    fs::path p(input_path);
    std::string directory = p.parent_path().string();
    std::string stem = p.stem().string();
    std::string output_filename = stem + "_voxelized.obj";
    std::string output_path = directory.empty() ? output_filename : directory + "/" + output_filename;

    std::ofstream output_file(output_path);
    if (!output_file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return; 
    }

    for(int i = 0; i < res.vertices.size(); i++){
        output_file << "v " << res.vertices[i].x << ' ' << res.vertices[i].y << ' ' << res.vertices[i].z << std::endl;
    }

    for (int i = 0; i < res.faces.size(); i++)
    {
        output_file << "f " << res.faces[i].v[0] << ' ' << res.faces[i].v[1] << ' ' << res.faces[i].v[2] << std::endl;
    }

    output_file.close();
    std::cout << "Output disimpan di: " << output_path << std::endl;
}