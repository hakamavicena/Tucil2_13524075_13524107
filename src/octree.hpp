#pragma once
#include "types.hpp"
#include <cmath>
#include <map>
#include <algorithm>
#include <thread>

inline bool triangleIntersectsAABB(Vec3 v0, Vec3 v1, Vec3 v2, const AABB& box) {
    Vec3 center = {(box.min.x + box.max.x) * 0.5f, (box.min.y + box.max.y) * 0.5f, (box.min.z + box.max.z) * 0.5f};
    Vec3 extents = {(box.max.x - box.min.x) * 0.5f, (box.max.y - box.min.y) * 0.5f, (box.max.z - box.min.z) * 0.5f};

    v0 = v0 - center;
    v1 = v1 - center;
    v2 = v2 - center;

    Vec3 e0 = v1 - v0;
    Vec3 e1 = v2 - v1;
    Vec3 e2 = v0 - v2;

    float p0, p1, p2, r;

    float fex = std::fabs(e0.x);
    float fey = std::fabs(e0.y);
    float fez = std::fabs(e0.z);

    p0 = e0.z * v0.y - e0.y * v0.z;
    p2 = e0.z * v2.y - e0.y * v2.z;
    r = fez * extents.y + fey * extents.z;
    if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false;

    p0 = -e0.z * v0.x + e0.x * v0.z;
    p2 = -e0.z * v2.x + e0.x * v2.z;
    r = fez * extents.x + fex * extents.z;
    if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false;

    p0 = e0.y * v0.x - e0.x * v0.y;
    p2 = e0.y * v2.x - e0.x * v2.y;
    r = fey * extents.x + fex * extents.y;
    if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false;

    fex = std::fabs(e1.x);
    fey = std::fabs(e1.y);
    fez = std::fabs(e1.z);

    p0 = e1.z * v0.y - e1.y * v0.z;
    p2 = e1.z * v2.y - e1.y * v2.z;
    r = fez * extents.y + fey * extents.z;
    if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false;

    p0 = -e1.z * v0.x + e1.x * v0.z;
    p2 = -e1.z * v2.x + e1.x * v2.z;
    r = fez * extents.x + fex * extents.z;
    if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false;

    p0 = e1.y * v0.x - e1.x * v0.y;
    p2 = e1.y * v2.x - e1.x * v2.y;
    r = fey * extents.x + fex * extents.y;
    if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false;

    fex = std::fabs(e2.x);
    fey = std::fabs(e2.y);
    fez = std::fabs(e2.z);

    p0 = e2.z * v0.y - e2.y * v0.z;
    p1 = e2.z * v1.y - e2.y * v1.z;
    r = fez * extents.y + fey * extents.z;
    if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false;

    p0 = -e2.z * v0.x + e2.x * v0.z;
    p1 = -e2.z * v1.x + e2.x * v1.z;
    r = fez * extents.x + fex * extents.z;
    if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false;

    p0 = e2.y * v0.x - e2.x * v0.y;
    p1 = e2.y * v1.x - e2.x * v1.y;
    r = fey * extents.x + fex * extents.y;
    if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false;

    if (std::max({v0.x, v1.x, v2.x}) < -extents.x || std::min({v0.x, v1.x, v2.x}) > extents.x) return false;
    if (std::max({v0.y, v1.y, v2.y}) < -extents.y || std::min({v0.y, v1.y, v2.y}) > extents.y) return false;
    if (std::max({v0.z, v1.z, v2.z}) < -extents.z || std::min({v0.z, v1.z, v2.z}) > extents.z) return false;

    Vec3 normal = cross(e0, e1);
    float d = dot(normal, v0);
    float rn = extents.x * std::fabs(normal.x) + extents.y * std::fabs(normal.y) + extents.z * std::fabs(normal.z);
    if (d > rn || d < -rn) return false;

    return true;
}

inline void buildOctreeSequential(OctreeNode* node, const Mesh& mesh, int maxDepth) {
    if (node->depth >= maxDepth || node->triangleIndices.empty()) {
        node->isLeaf = true;
        return;
    }

    Vec3 mid = {
        (node->bounds.min.x + node->bounds.max.x) * 0.5f,
        (node->bounds.min.y + node->bounds.max.y) * 0.5f,
        (node->bounds.min.z + node->bounds.max.z) * 0.5f
    };

    AABB childBounds[8];
    childBounds[0] = {node->bounds.min, mid};
    childBounds[1] = {{mid.x, node->bounds.min.y, node->bounds.min.z}, {node->bounds.max.x, mid.y, mid.z}};
    childBounds[2] = {{node->bounds.min.x, mid.y, node->bounds.min.z}, {mid.x, node->bounds.max.y, mid.z}};
    childBounds[3] = {{mid.x, mid.y, node->bounds.min.z}, {node->bounds.max.x, node->bounds.max.y, mid.z}};
    childBounds[4] = {{node->bounds.min.x, node->bounds.min.y, mid.z}, {mid.x, mid.y, node->bounds.max.z}};
    childBounds[5] = {{mid.x, node->bounds.min.y, mid.z}, {node->bounds.max.x, mid.y, node->bounds.max.z}};
    childBounds[6] = {{node->bounds.min.x, mid.y, mid.z}, {mid.x, node->bounds.max.y, node->bounds.max.z}};
    childBounds[7] = {mid, node->bounds.max};

    for (int i = 0; i < 8; i++) {
        node->children[i] = new OctreeNode();
        node->children[i]->bounds = childBounds[i];
        node->children[i]->depth = node->depth + 1;

        for (int idx : node->triangleIndices) {
            const Triangle& tri = mesh.triangles[idx];
            Vec3 v0 = mesh.vertices[tri.v[0]];
            Vec3 v1 = mesh.vertices[tri.v[1]];
            Vec3 v2 = mesh.vertices[tri.v[2]];
            if (triangleIntersectsAABB(v0, v1, v2, childBounds[i])) {
                node->children[i]->triangleIndices.push_back(idx);
            }
        }

        buildOctreeSequential(node->children[i], mesh, maxDepth);
    }

    node->isLeaf = false;
}

inline void buildOctree(OctreeNode* node, const Mesh& mesh, int maxDepth) {
    if (node->depth >= maxDepth || node->triangleIndices.empty()) {
        node->isLeaf = true;
        return;
    }

    Vec3 mid = {
        (node->bounds.min.x + node->bounds.max.x) * 0.5f,
        (node->bounds.min.y + node->bounds.max.y) * 0.5f,
        (node->bounds.min.z + node->bounds.max.z) * 0.5f
    };

    AABB childBounds[8];
    childBounds[0] = {node->bounds.min, mid};
    childBounds[1] = {{mid.x, node->bounds.min.y, node->bounds.min.z}, {node->bounds.max.x, mid.y, mid.z}};
    childBounds[2] = {{node->bounds.min.x, mid.y, node->bounds.min.z}, {mid.x, node->bounds.max.y, mid.z}};
    childBounds[3] = {{mid.x, mid.y, node->bounds.min.z}, {node->bounds.max.x, node->bounds.max.y, mid.z}};
    childBounds[4] = {{node->bounds.min.x, node->bounds.min.y, mid.z}, {mid.x, mid.y, node->bounds.max.z}};
    childBounds[5] = {{mid.x, node->bounds.min.y, mid.z}, {node->bounds.max.x, mid.y, node->bounds.max.z}};
    childBounds[6] = {{node->bounds.min.x, mid.y, mid.z}, {mid.x, node->bounds.max.y, node->bounds.max.z}};
    childBounds[7] = {mid, node->bounds.max};

    for (int i = 0; i < 8; i++) {
        node->children[i] = new OctreeNode();
        node->children[i]->bounds = childBounds[i];
        node->children[i]->depth = node->depth + 1;

        for (int idx : node->triangleIndices) {
            const Triangle& tri = mesh.triangles[idx];
            Vec3 v0 = mesh.vertices[tri.v[0]];
            Vec3 v1 = mesh.vertices[tri.v[1]];
            Vec3 v2 = mesh.vertices[tri.v[2]];
            if (triangleIntersectsAABB(v0, v1, v2, childBounds[i])) {
                node->children[i]->triangleIndices.push_back(idx);
            }
        }
    }

    std::thread threads[8];
    for (int i = 0; i < 8; i++) {
        threads[i] = std::thread(buildOctreeSequential, node->children[i], std::cref(mesh), maxDepth);
    }
    for (int i = 0; i < 8; i++) {
        threads[i].join();
    }

    node->isLeaf = false;
}

inline void collectStats(OctreeNode* node, std::map<int, int>& nodeCount, std::map<int, int>& prunedCount) {
    if (node == nullptr) return;
    nodeCount[node->depth]++;
    if (node->isLeaf && node->triangleIndices.empty()) {
        prunedCount[node->depth]++;
    }
    for (int i = 0; i < 8; i++) {
        collectStats(node->children[i], nodeCount, prunedCount);
    }
}

inline std::vector<OctreeNode*> collectLeafVoxels(OctreeNode* node) {
    std::vector<OctreeNode*> result;
    if (node == nullptr) return result;
    if (node->isLeaf) {
        if (!node->triangleIndices.empty()) {
            result.push_back(node);
        }
        return result;
    }
    for (int i = 0; i < 8; i++) {
        std::vector<OctreeNode*> childResult = collectLeafVoxels(node->children[i]);
        result.insert(result.end(), childResult.begin(), childResult.end());
    }
    return result;
}
