#include <iostream>
#include <map>
#include <chrono>
#include "input.hpp"
#include "parser.hpp"
#include "octree.hpp"
#include "output.hpp"
#include <utility>
#include "viewer.hpp"

int main() {
    std::pair<std::string, int> userData = inputObj();

    std::string pathFile = userData.first;
    int depth = userData.second;

    auto startTime = std::chrono::high_resolution_clock::now();

    Mesh mesh = validateObj(pathFile);

    AABB initialSquare = computeAABB(mesh);
    AABB finalSquare = expandToCube(initialSquare);

    std::cout << "Jumlah Vertex (input)   : " << mesh.vertices.size() << std::endl;
    std::cout << "Jumlah Triangle (input) : " << mesh.triangles.size() << std::endl;
    std::cout << "Bounding Box            : " << finalSquare.min.x << ", "
              << finalSquare.min.y << ", " << finalSquare.min.z << ", "
              << finalSquare.max.x << ", " << finalSquare.max.y << ", "
              << finalSquare.max.z << std::endl;

    OctreeNode* root = new OctreeNode();
    root->bounds = finalSquare;
    root->depth = 0;
    for (int i = 0; i < (int)mesh.triangles.size(); i++) {
        root->triangleIndices.push_back(i);
    }

    buildOctree(root, mesh, depth);

    std::map<int, int> nodeCount, prunedCount;
    collectStats(root, nodeCount, prunedCount);

    std::cout << "Statistik node octree:" << std::endl;
    for (auto& p : nodeCount) {
        std::cout << p.first << " : " << p.second << std::endl;
    }

    std::cout << "Statistik node di-prune:" << std::endl;
    for (auto& p : prunedCount) {
        std::cout << p.first << " : " << p.second << std::endl;
    }

    std::cout << "Kedalaman octree        : " << depth << std::endl;

    std::vector<OctreeNode*> voxels = collectLeafVoxels(root);
    std::cout << "Jumlah voxel            : " << voxels.size() << std::endl;

    VoxelMesh voxelMesh = generateVoxelMesh(voxels);

    
    std::cout << "Jumlah vertex output    : " << voxelMesh.vertices.size() << std::endl;
    std::cout << "Jumlah face output      : " << voxelMesh.faces.size() << std::endl;

    objWriter(voxelMesh, pathFile, depth);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Waktu eksekusi          : " << duration.count() << " ms" << std::endl;

    Viewer view;
    if(view.init()){
        view.run(voxelMesh);
        view.close();
    }

    return 0;
}