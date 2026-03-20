#include <vector>

struct Vec3{
    float x,y,z;

    Vec3  operator+(Vec3 a){
        Vec3 res;
        res.x = x + a.x; res.y = y + a.y; res.z = z + a.z;
        return (res);
    } 
    Vec3  operator-(Vec3 a){
        Vec3 res;
        res.x = x - a.x; res.y = y - a.y; res.z = z - a.z;
        return (res);
    }
    Vec3 operator+(float a)
    {
        Vec3 res;
        res.x = x + a;
        res.y = y + a;
        res.z = z + a;
        return (res);
    }
    Vec3 operator-(float a)
    {
        Vec3 res;
        res.x = x - a;
        res.y = y - a;
        res.z = z - a;
        return (res);
    }
    Vec3 operator*(float a)
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



