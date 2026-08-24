#pragma once

#include "colour.h"
#include "hittable.h"
#include "rtweekend.h"
#include "vec3.h"

class camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;

        void render(const hittable& world) {
            initialize();

            cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;
                for (int i = 0; i < image_width; i++) {
                    colour pixel_colour(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_colour += ray_colour(r, world);
                    }
                    write_colour(cout, pixel_samples_scale * pixel_colour);
                }
            }

            clog << "\rDone.                       \n";
        }

    private:
        int image_height; // Come on man...
        double pixel_samples_scale; // Colour scale factor for a sum of pixel samples
        point3 centre; // It's the centre of the camera
        point3 pixel00_loc; // Location of pixel at 0,0
        vec3 pixel_delta_u; // Offset to right pixel
        vec3 pixel_delta_v; // Impulse per unit of mass

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            centre = point3(0,0,0);

            // Viewport dimensions
            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Edge vectors
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // Pixel-to-pixel delta vectors
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Upper-left pixel
            auto viewport_upper_left = centre
                - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        }

        ray get_ray(int i, int j) {
            // Construct a camera ray from the origin directed
            // at a pseudo randomly selected point around pixel i,j

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                + ((i + offset.x()) * pixel_delta_u)
                + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = centre;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // Returns a vector to a random point in the unit square
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        colour ray_colour(const ray& r, const hittable& world) {
            hit_record rec;

            if(world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + colour(1,1,1));
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*colour(1.0,1.0,1.0) + a*colour(0.5, 0.7, 1.0);
        }
};
