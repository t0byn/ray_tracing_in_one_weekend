#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

#include <stdio.h>
#include <math.h>

using Color3 = Vec3;

// gamma correction with gamma = 2
inline double linear_to_gamma(const double linear_component)
{
    if (linear_component > 0)
    {
        return sqrt(linear_component);
    }
    return 0;
}

void write_color(FILE* f, const Color3& pixel_color)
{
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const Interval intensity(0.0, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));
    fprintf(f, "%d %d %d\n", rbyte, gbyte, bbyte);
}

#endif