#pragma once

#include "rtweekend.h"
#include "hittable.h"
#include <memory>

class sphere : public hittable {
    public:
        // Static
        sphere(const point3& static_centre, double radius, std::shared_ptr<material> mat)
            : centre(static_centre, vec3(0,0,0)), radius(fmax(0,radius)), mat(mat) {}

        // Animated
        sphere(const point3& centre1, const point3& centre2, double radius, std::shared_ptr<material> mat)
            : centre(centre1, centre2 - centre1), radius(fmax(0,radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            point3 current_centre = centre.at(r.time());
            vec3 oc = current_centre - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            if(discriminant < 0) return false;

            auto sqrtd = sqrt(discriminant);

            // Find nearest acceptable root
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root)) return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - current_centre) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        ray centre;
        double radius;
        std::shared_ptr<material> mat;
};
