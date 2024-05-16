#ifndef WORLD_H
#define WORLD_H

#include "rt.h"
#include "primitive.h"

#define MAX_SPHERE_NUMBER 500

struct World
{
    Sphere spheres[MAX_SPHERE_NUMBER];
    int sphere_count;
};

void world_add_sphere(World& world, const Sphere& sphere)
{
    if (world.sphere_count >= MAX_SPHERE_NUMBER)
    {
        assert(0);
        fprintf(stderr, "reach max sphere number!\n");
        return;
    }
    world.spheres[world.sphere_count] = sphere;
    world.sphere_count++;
}

bool ray_hit_world(const Ray& ray, const Interval& ray_t, const World& world, RayHitRecord& rec)
{
    bool hit = false;
    double closest_hit_root = ray_t.max;
    for (int i = 0; i < world.sphere_count; i++)
    {
        const Sphere& sphere = world.spheres[i];
        if (ray_hit_sphere(sphere, ray, Interval(ray_t.min, closest_hit_root), rec))
        {
            hit = true;
            closest_hit_root = rec.t;
        }
    }
    return hit;
}


#endif