#ifndef RT_H
#define RT_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

// constants

const double infinity = INFINITY;
const double pi = 3.1415926535897932385;

// utility function

inline double degree_to_radian(const double degree)
{
    return degree * pi / 180;
}

// generate a random number in the range [0, 1)
inline double random_double()
{
    return (rand() / (RAND_MAX + 1.0));
}

// generate a random number in the range [min, max)
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

#include "vec3.h"
#include "material_type.h"
#include "ray.h"
#include "interval.h"
#include "color.h"

#endif