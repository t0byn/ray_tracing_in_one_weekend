#ifndef MATERIAL_TYPE_H
#define MATERIAL_TYPE_H

enum MaterialType
{
    Lambertian,
    Metal,
    Dielectric
};

struct MaterialHandle
{
    MaterialType type;
    int index;
};

#endif