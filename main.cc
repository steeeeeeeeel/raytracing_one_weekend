#include "camera.h"
#include "colour.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"
#include "material.h"
#include "vec3.h"
#include "bvh.h"
#include "texture.h"
#include <memory>

void bouncing_spheres() {

    // World
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, colour(.2,.3,.1), colour(.9,.9,.9));
    world.add(std::make_shared<sphere>(point3(0,-1000, 0), 1000, std::make_shared<lambertian>(checker)));

    for (int i = -11; i < 11; i++) {
        for (int j = -11; j < 11; j++) {
            auto choose_mat = random_double();
            point3 centre(i + 0.9*random_double(), 0.2, j + 0.9*random_double());

            if((centre - point3(4,0.2,0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = colour::random() * colour::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    auto centre2 = centre + vec3(0, random_double(0,0.5),0);
                    world.add(std::make_shared<sphere>(centre, centre2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = colour::random();
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(centre, 0.2, sphere_material));
                } else {
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(centre, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(colour(0.4,0.2,0.1));
    world.add(std::make_shared<sphere>(point3(-4,1,0), 1.0, material2));

    auto material3 = std::make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4,1,0), 1.0, material3));

    world = hittable_list(std::make_shared<bvh_node>(world));

    // Render
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(12,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, colour(.2,.3,.1), colour(.9,.9,.9));

    world.add(std::make_shared<sphere>(point3(0,-10,0), 10, std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(point3(0,10,0), 10, std::make_shared<lambertian>(checker)));


    // Render
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.render(world);
}

int main() {
    switch (2) {
        case 1: bouncing_spheres(); break;
        case 2: checkered_spheres(); break;
    }
}
