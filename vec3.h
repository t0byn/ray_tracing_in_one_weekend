#ifndef VEC3_H
#define VEC3_H

#include <math.h>

class Vec3
{
public:
    double x, y, z;

    inline Vec3() : x{0}, y{0}, z{0} {};
    inline Vec3(double x, double y, double z) : x{x}, y{y}, z{z} {};

    inline Vec3 operator-() const 
    {
        return Vec3(-x, -y, -z);
    }

    inline Vec3& operator+=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline Vec3& operator*=(const double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    inline Vec3& operator/=(const double t)
    {
        *this *= (1/t);
        return *this;
    }

    inline double length_squared() const
    {
        return (x*x + y*y + z*z);
    }

    inline double length() const
    {
        return sqrt(length_squared());
    }

    inline bool near_zero() const
    {
        double s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    inline static Vec3 random()
    {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3 random(const double min, const double max)
    {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

using Point3 = Vec3;

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(const double t, const Vec3& v)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3& v, const double t)
{
    return t * v;
}

inline Vec3 operator/(const Vec3& v, const double t)
{
    return (1/t) * v;
}

inline double dot(const Vec3& u, const Vec3& v)
{
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
    return Vec3(
        (u.y * v.z) - (u.z * v.y),
        (u.z * v.x) - (u.x * v.z),
        (u.x * v.y) - (u.y * v.x)
    );
}

inline Vec3 unit_vector(const Vec3& v)
{
    return (v / v.length());
}

inline Vec3 random_vector_in_unit_sphere()
{
    while (true)
    {
        Vec3 v = Vec3::random(-1, 1);
        if (v.length_squared() < 1)
        {
            return v;
        }
    }
}

inline Vec3 random_unit_vector()
{
    return unit_vector(random_vector_in_unit_sphere());
}

inline Vec3 random_unit_vector_on_hemisphere(const Vec3& normal)
{
    Vec3 unit_vector = random_unit_vector();
    if (dot(unit_vector, normal) >= 0)
    {
        return unit_vector;
    }
    else
    {
        return -unit_vector;
    }
}

inline Vec3 random_vector_in_unit_disk()
{
    while (true)
    {
        Vec3 v(random_double(-1, 1), random_double(-1, 1), 0);
        if (v.length_squared() < 1) return v;
    }
}

inline Vec3 reflect(const Vec3& incident, const Vec3& normal)
{
    Vec3 reflected = (incident - 2 * dot(incident, normal) * normal);
    return reflected;
}

inline Vec3 refract(const Vec3& unit_incident, const Vec3& normal, const double eta_over_eta_prime)
{
    double cos_theta = fmin(dot(-unit_incident, normal), 1.0);
    Vec3 perpendicular = 
        eta_over_eta_prime * (unit_incident + cos_theta * normal);
    Vec3 parallel = 
        -sqrt(1 - perpendicular.length_squared()) * normal;
    Vec3 refracted = perpendicular + parallel;
    return refracted;
}

#endif