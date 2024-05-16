#ifndef CAMERA_H
#define CAMERA_H

#include "rt.h"
#include "world.h"
#include "material.h"

class Camera
{
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90; // vertical view angle (field of view)
    Point3 lookfrom = Point3(0, 0, 0);
    Point3 lookat = Point3(0, 0, -1);
    Vec3 vup = Vec3(0, 1, 0);

    double defocus_angle = 0;
    double focus_distance = 10;

    void render(const World& world)
    {
        initialize();

        // render
        
        fprintf(stdout, "P3\n%d %d\n255\n", image_width, image_height);

        // row-wise
        for (int h = 0; h < image_height; h++)
        {
            fprintf(stderr, "\rScanlines remaining: %d         ", image_height - h);
            fflush(stderr);
            for (int w = 0; w < image_width; w++)
            {
                Color3 pixel_color(0, 0, 0);
                for (int i = 0; i < samples_per_pixel; i++)
                {
                    Ray ray = get_ray(w, h);
                    pixel_color += raycast(ray, world, max_depth);
                }
                pixel_color *= pixel_samples_scale;
                write_color(stdout, pixel_color);
            }
        }

        fprintf(stderr, "\rDone.                         \n");
    }

private:
    int image_height;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Point3 pixel_00;
    double pixel_samples_scale;
    Vec3 u, v, w;
    Vec3 defocus_disk_u, defocus_disk_v;

    void initialize()
    {
        // image
        image_height = int(image_width / aspect_ratio);
        if (image_height < 1) image_height = 1; // make sure image height at least 1

        // camera
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // viewport
        double theta = degree_to_radian(vfov);
        double h = tan(theta / 2.0) * focus_distance;
        double viewport_height = 2 * h;
        double viewport_width = viewport_height * (double(image_width) / double(image_height));
        Vec3 viewport_u = u * viewport_width;
        Vec3 viewport_v = -v * viewport_height;

        // pixel spacing
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        Point3 viewport_top_left = 
            lookfrom - (focus_distance * w) - viewport_u / 2 - viewport_v / 2;
        pixel_00 = viewport_top_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        pixel_samples_scale = 1.0 / samples_per_pixel;

        double disk_radius = tan(degree_to_radian(defocus_angle / 2)) * focus_distance;
        defocus_disk_u = u * disk_radius;
        defocus_disk_v = v * disk_radius;
    }

    Vec3 sample_in_unit_square() const
    {
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Vec3 sample_in_defocus_disk() const
    {
        Vec3 v = sample_in_unit_square();
        return lookfrom + (v.x * defocus_disk_u) + (v.y * defocus_disk_v);
    }

    Ray get_ray(const int x, const int y) const
    {
        Vec3 offset = sample_in_unit_square();
        Point3 pixel_sample_point = pixel_00
            + ((x + offset.x) * pixel_delta_u)
            + ((y + offset.y) * pixel_delta_v);

        Point3 ray_origin = defocus_angle > 0 ? sample_in_defocus_disk() : lookfrom;
        Vec3 ray_direction = pixel_sample_point - ray_origin;
        return Ray{
            .origin = ray_origin,
            .dir = ray_direction
        };
    }

    Color3 raycast(const Ray& ray, const World& world, int depth)
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
        {
            return Color3(0, 0, 0);
        }

        RayHitRecord rec;
        // don't use 0 as interval's min value to prevent shadow acne
        if (ray_hit_world(ray, Interval(0.001, infinity), world, rec))
        {
            Ray scatterd_ray{};
            Color3 attenuation{};
            if (scatter(ray, rec, attenuation, scatterd_ray))
                return attenuation * raycast(scatterd_ray, world, depth - 1);
            return Color3(0, 0, 0);
        }

        Color3 white = Color3(1.0, 1.0, 1.0);
        Color3 blue = Color3(0.5, 0.7, 1.0);
        Vec3 unit_direction = unit_vector(ray.dir);
        double a = 0.5 * (unit_direction.y + 1.0);
        return (1 - a) * white + a * blue;
    }
};

#endif