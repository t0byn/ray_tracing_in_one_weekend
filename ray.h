#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray
{
    Point3 origin;
    Vec3 dir;
};

Point3 ray_at(const Ray& ray, const double t)
{
    return ray.origin + (t * ray.dir);
}

struct RayHitRecord
{
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;
    MaterialHandle mat;
};

void set_hit_face_normal(RayHitRecord& rec, const Vec3& ray_dir, const Vec3& outward_normal)
{
    rec.front_face = dot(ray_dir, outward_normal) <= 0;
    rec.normal = rec.front_face ? outward_normal : -outward_normal;
}

#endif