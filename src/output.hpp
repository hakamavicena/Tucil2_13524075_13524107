#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include "types.hpp"
#include <string>
#include <filesystem>
#include <cstdio>
#include <thread>
namespace fs = std::filesystem;

VoxelMesh generateVoxelMesh(const std::vector<OctreeNode*>& voxels) {
    int faceIndices[12][3] = {
        {0,2,1}, {0,3,2},
        {4,5,6}, {4,6,7},
        {0,1,5}, {0,5,4},
        {2,3,7}, {2,7,6},
        {0,4,7}, {0,7,3},
        {1,2,6}, {1,6,5}
    };

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    int totalVoxels = (int)voxels.size();
    if ((int)numThreads > totalVoxels) numThreads = totalVoxels;

    std::vector<std::vector<Vec3>> threadVertices(numThreads);
    std::vector<std::vector<Triangle>> threadFaces(numThreads);

    std::vector<int> chunkStart(numThreads);
    std::vector<int> chunkEnd(numThreads);
    int baseSize = totalVoxels / numThreads;
    int remainder = totalVoxels % numThreads;
    int offset = 0;
    for (unsigned int t = 0; t < numThreads; t++) {
        chunkStart[t] = offset;
        int thisSize = baseSize + (t < (unsigned int)remainder ? 1 : 0);
        chunkEnd[t] = offset + thisSize;
        offset += thisSize;
    }

    std::vector<std::thread> threads(numThreads);
    for (unsigned int t = 0; t < numThreads; t++) {
        threads[t] = std::thread([&, t]() {
            for (int n = chunkStart[t]; n < chunkEnd[t]; n++) {
                Vec3 mn = voxels[n]->bounds.min;
                Vec3 mx = voxels[n]->bounds.max;

                threadVertices[t].push_back({mn.x, mn.y, mn.z});
                threadVertices[t].push_back({mx.x, mn.y, mn.z});
                threadVertices[t].push_back({mx.x, mx.y, mn.z});
                threadVertices[t].push_back({mn.x, mx.y, mn.z});
                threadVertices[t].push_back({mn.x, mn.y, mx.z});
                threadVertices[t].push_back({mx.x, mn.y, mx.z});
                threadVertices[t].push_back({mx.x, mx.y, mx.z});
                threadVertices[t].push_back({mn.x, mx.y, mx.z});

                int globalOffset = n * 8 + 1;
                for (int f = 0; f < 12; f++) {
                    Triangle tri;
                    tri.v[0] = globalOffset + faceIndices[f][0];
                    tri.v[1] = globalOffset + faceIndices[f][1];
                    tri.v[2] = globalOffset + faceIndices[f][2];
                    threadFaces[t].push_back(tri);
                }
            }
        });
    }

    for (unsigned int t = 0; t < numThreads; t++) {
        threads[t].join();
    }

    VoxelMesh result;
    for (unsigned int t = 0; t < numThreads; t++) {
        result.vertices.insert(result.vertices.end(), threadVertices[t].begin(), threadVertices[t].end());
        result.faces.insert(result.faces.end(), threadFaces[t].begin(), threadFaces[t].end());
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

    for(int i = 0; i < (int)res.vertices.size(); i++){
        output_file << "v " << res.vertices[i].x << ' ' << res.vertices[i].y << ' ' << res.vertices[i].z << std::endl;
    }

    for (int i = 0; i < (int)res.faces.size(); i++)
    {
        output_file << "f " << res.faces[i].v[0] << ' ' << res.faces[i].v[1] << ' ' << res.faces[i].v[2] << std::endl;
    }

    output_file.close();
    std::cout << "Output disimpan di      : " << output_path << std::endl;
}