#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt.h"

struct Sphere
{
    Point3 center;
    double radius;
    MaterialHandle mat;
};

bool ray_hit_sphere(const Sphere& sphere, const Ray& ray, const Interval& ray_t, RayHitRecord& rec)
{
    Vec3 oc = sphere.center - ray.origin;
    double a = ray.dir.length_squared();
    double h = dot(ray.dir, oc);
    double c = oc.length_squared() - (sphere.radius * sphere.radius);
    double discriminant = (h * h) - (a * c);

    if (discriminant < 0)
    {
        return false;
    }

    double sqrt_d = sqrt(discriminant);

    // find the nearest root that lies in the acceptable range
    double root = (h - sqrt_d) / a;
    if (!ray_t.surrounds(root))
    {
        root = (h + sqrt_d) / a;
        if (!ray_t.surrounds(root)) return false;
    }

    rec.t = root;
    rec.p = ray_at(ray, root);
    Vec3 outward_normal = (rec.p - sphere.center) / sphere.radius;
    set_hit_face_normal(rec, ray.dir, outward_normal);
    rec.mat = sphere.mat;

    return true;
}

#endif