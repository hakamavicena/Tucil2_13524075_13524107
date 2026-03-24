#include <iostream>
#include <map>
#include "input.hpp"
#include "parser.hpp"
#include "octree.hpp"
#include <utility>

int main() {
    std::pair<std::string, int> userData = inputObj();

    std::string pathFile = userData.first;
    int depth = userData.second;

    Mesh mesh = validate_obj(pathFile);

    AABB initialSquare = computeAABB(mesh);
    AABB finalSquare = expandToCube(initialSquare);

    std::cout << "Jumlah Vertex : " << mesh.vertices.size() << std::endl;
    std::cout << "Jumlah Triangle : " << mesh.triangles.size() << std::endl;
    std::cout << "Bounding Box Terkecil : " << finalSquare.min.x << ", "
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

    std::cout << "Kedalaman octree: " << depth << std::endl;

    std::vector<OctreeNode*> voxels = collectLeafVoxels(root);
    std::cout << "Jumlah voxel: " << voxels.size() << std::endl;

    return 0;
}