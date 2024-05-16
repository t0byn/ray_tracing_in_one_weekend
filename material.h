#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"

#define MAX_LAMBERTIAN_MATERIAL_NUMBER 500 
#define MAX_METAL_MATERIAL_NUMBER 500 
#define MAX_DIELECTRIC_MATERIAL_NUMBER 500

struct LambertianMaterial
{
    Color3 albedo;
};

struct MetalMaterial
{
    Color3 albedo;
    double fuzz;
};

struct DielectricMaterial
{
    Color3 albedo;
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refractive_index;
};

int lambertian_count = 0;
int metal_count = 0;
int dielectric_count = 0;
LambertianMaterial lambertian_materials[MAX_LAMBERTIAN_MATERIAL_NUMBER];
MetalMaterial metal_materials[MAX_METAL_MATERIAL_NUMBER];
DielectricMaterial dielectric_materials[MAX_DIELECTRIC_MATERIAL_NUMBER];

inline MaterialHandle add_lambertian(const LambertianMaterial& m)
{
    assert(lambertian_count < MAX_LAMBERTIAN_MATERIAL_NUMBER);
    lambertian_materials[lambertian_count] = m;
    return MaterialHandle{
        .type = Lambertian,
        .index = lambertian_count++
    };
}

inline MaterialHandle add_metal(const MetalMaterial& m)
{
    assert(metal_count < MAX_METAL_MATERIAL_NUMBER);
    metal_materials[metal_count] = m;
    return MaterialHandle{
        .type = Metal,
        .index = metal_count++
    };
}

inline MaterialHandle add_dielectric(const DielectricMaterial& m)
{
    assert(dielectric_count < MAX_DIELECTRIC_MATERIAL_NUMBER);
    dielectric_materials[dielectric_count] = m;
    return MaterialHandle{
        .type = Dielectric,
        .index = dielectric_count++
    };
}


inline bool scatter_lambertian(const Ray& incident_ray, const RayHitRecord& rec, 
    Color3& attenuated, Ray& scattered_ray)
{
    assert(rec.mat.type == Lambertian);
    assert(rec.mat.index < lambertian_count);
    LambertianMaterial mat = lambertian_materials[rec.mat.index];
    Vec3 unit_vector = random_unit_vector();
    Vec3 scatter_direction = rec.normal + unit_vector;
    if (scatter_direction.near_zero()) 
        scatter_direction = rec.normal;

    scattered_ray = Ray{
        .origin = rec.p,
        .dir = scatter_direction
    };
    attenuated = mat.albedo;
    return true;
}

inline bool scatter_metal(const Ray& incident_ray, const RayHitRecord& rec, 
    Color3& attenuated, Ray& scattered_ray)
{
    assert(rec.mat.type == Metal);
    assert(rec.mat.index < metal_count);
    MetalMaterial mat = metal_materials[rec.mat.index];
    Vec3 reflect_direction = reflect(incident_ray.dir, rec.normal);
    // fuzz
    reflect_direction = reflect_direction + (mat.fuzz * random_unit_vector());

    scattered_ray = Ray{
        .origin = rec.p,
        .dir = reflect_direction
    };
    attenuated = mat.albedo;
    return (dot(reflect_direction, rec.normal) > 0);
}

inline double reflectance(double cosine, double refraction_index)
{
    // Christophe Schlick's approximation for reflectance
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

inline bool scatter_dielectric(const Ray& incident_ray, const RayHitRecord& rec, 
    Color3& attenuated, Ray& scattered_ray)
{
    assert(rec.mat.type == Dielectric);
    assert(rec.mat.index < dielectric_count);
    DielectricMaterial mat = dielectric_materials[rec.mat.index];
    double refraction_index = rec.front_face ? (1.0 / mat.refractive_index) : mat.refractive_index;
    Vec3 unit_incident = unit_vector(incident_ray.dir);
    double cos_theta = fmin(dot(-unit_incident, rec.normal), 1);
    double sin_theta = sqrt(1 - cos_theta * cos_theta);

    bool cannot_refract = refraction_index * sin_theta > 1;
    Vec3 scatter_direction{};
    if (cannot_refract || reflectance(cos_theta, refraction_index) > random_double())
    {
        scatter_direction = reflect(unit_incident, rec.normal);
    }
    else
    {
        scatter_direction = refract(unit_incident, rec.normal, refraction_index);
    }

    scattered_ray = Ray{
        .origin = rec.p,
        .dir = scatter_direction
    };
    attenuated = mat.albedo;
    return true;
}

bool scatter(const Ray& incident_ray, const RayHitRecord& hit_record, 
    Color3& attenuated, Ray& scattered_ray)
{
    switch(hit_record.mat.type)
    {
        case Lambertian:
            return scatter_lambertian(incident_ray, hit_record, attenuated, scattered_ray);
        case Metal:
            return scatter_metal(incident_ray, hit_record, attenuated, scattered_ray);
        case Dielectric:
            return scatter_dielectric(incident_ray, hit_record, attenuated, scattered_ray);
    }
    return false;
}

#endif