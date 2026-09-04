#include "camera.h"
#include "colour.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"
#include "material.h"
#include "vec3.h"
#include "bvh.h"
#include <memory>

int main() {

    // World
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(colour(0.5,0.5,0.5));
    world.add(std::make_shared<sphere>(point3(0,-1000, 0), 1000, ground_material));

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
