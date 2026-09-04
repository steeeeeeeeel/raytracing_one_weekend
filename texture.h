#pragma once

#include "colour.h"
#include "vec3.h"
#include <memory>
class texture {
    public:
        virtual ~texture() = default;

        virtual colour value(double u, double v, const point3& p) const = 0;
};

class solid_colour : public texture {
    public:
        solid_colour(const colour& albedo) : albedo(albedo) {}

        solid_colour(double red, double green, double blue) : solid_colour(colour(red,green,blue)) {}

        colour value(double u, double v, const point3& p) const override {
            return albedo;
        }

        private:
        colour albedo;
};

class checker_texture : public texture {
    public:
        checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
            : inv_scale(1.0 / scale), even(even), odd(odd) {}

        checker_texture(double scale, const colour& c1, const colour& c2)
            : checker_texture(scale, std::make_shared<solid_colour>(c1), std::make_shared<solid_colour>(c2)) {}

        colour value(double u, double v, const point3& p) const override {
            auto x_integer = int(std::floor(inv_scale * p.x()));
            auto y_integer = int(std::floor(inv_scale * p.y()));
            auto z_integer = int(std::floor(inv_scale * p.z()));

            bool is_even = (x_integer + y_integer + z_integer) % 2 == 0;

            return is_even ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
};
