#pragma once
#include <vector>

struct Vec3{
    float x,y,z;

    Vec3 operator+(Vec3 a) const {
        Vec3 res;
        res.x = x + a.x; res.y = y + a.y; res.z = z + a.z;
        return (res);
    } 
    Vec3 operator-(Vec3 a) const {
        Vec3 res;
        res.x = x - a.x; res.y = y - a.y; res.z = z - a.z;
        return (res);
    }
    Vec3 operator+(float a) const
    {
        Vec3 res;
        res.x = x + a;
        res.y = y + a;
        res.z = z + a;
        return (res);
    }
    Vec3 operator-(float a) const
    {
        Vec3 res;
        res.x = x - a;
        res.y = y - a;
        res.z = z - a;
        return (res);
    }
    Vec3 operator*(float a) const
    {
        Vec3 res;
        res.x = x * a;
        res.y = y * a;
        res.z = z * a;
        return (res);
    }
};

struct Triangle
{
    int v[3];
};

struct Mesh{
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;
};

struct AABB{
    Vec3 min;
    Vec3 max;
};

Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct OctreeNode {
    AABB bounds;
    int depth;
    OctreeNode* children[8];
    std::vector<int> triangleIndices;
    bool isLeaf;

    OctreeNode() : depth(0), isLeaf(false) {
        for (int i = 0; i < 8; i++) children[i] = nullptr;
    }
};
