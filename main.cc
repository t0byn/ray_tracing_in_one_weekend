#include "rt.h"

#include "camera.h"

int main(void)
{
    World world{};

    MaterialHandle mat;
    mat = add_lambertian(LambertianMaterial{.albedo=Color3(0.5, 0.5, 0.5)});
    world_add_sphere(world, Sphere{Point3(0, -1000, 0), 1000, mat});

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            double choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8) {
                    // diffuse
                    Color3 albedo = Color3(
                        random_double() * random_double(), 
                        random_double() * random_double(), 
                        random_double() * random_double()
                    );
                    mat = add_lambertian(LambertianMaterial{albedo});
                    world_add_sphere(world, Sphere{center, 0.2, mat});
                } else if (choose_mat < 0.95) {
                    // metal
                    Color3 albedo = Color3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    double fuzz = random_double(0, 0.5);
                    mat = add_metal(MetalMaterial{albedo, fuzz});
                    world_add_sphere(world, Sphere{center, 0.2, mat});
                } else {
                    // glass
                    mat = add_dielectric(DielectricMaterial{Color3(1.0, 1.0, 1.0), 1.5});
                    world_add_sphere(world, Sphere{center, 0.2, mat});
                }
            }
        }
    }

    mat = add_dielectric(DielectricMaterial{Color3(1.0, 1.0, 1.0), 1.5});
    world_add_sphere(world, Sphere{Point3(0, 1, 0), 1.0, mat});

    mat = add_lambertian(LambertianMaterial{Color3(0.4, 0.2, 0.1)});
    world_add_sphere(world, Sphere{Point3(-4, 1, 0), 1.0, mat});

    mat = add_metal(MetalMaterial{Color3(0.7, 0.6, 0.5), 0.0});
    world_add_sphere(world, Sphere{Point3(4, 1, 0), 1.0, mat});

    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = Point3(13,2,3);
    cam.lookat = Point3(0,0,0);
    cam.vup = Vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_distance = 10.0;

    cam.render(world);

    return 0;
}