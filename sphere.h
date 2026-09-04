#pragma once

#include "rtweekend.h"
#include "hittable.h"
#include "aabb.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        // Static
        sphere(const point3& static_centre, double radius, std::shared_ptr<material> mat)
            : centre(static_centre, vec3(0,0,0)), radius(fmax(0,radius)), mat(mat) {
                auto rvec = vec3(radius, radius, radius);
                bbox = aabb(static_centre - rvec, static_centre + rvec);
            }

        // Animated
        sphere(const point3& centre1, const point3& centre2, double radius, std::shared_ptr<material> mat)
            : centre(centre1, centre2 - centre1), radius(fmax(0,radius)), mat(mat) {
                auto rvec = vec3(radius, radius, radius);
                aabb box1(centre.at(0) - rvec, centre.at(0) + rvec);
                aabb box2(centre.at(1) - rvec, centre.at(1) + rvec);
                bbox = aabb(box1, box2);
            }

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

        aabb bounding_box() const override { return bbox; }

    private:
        ray centre;
        double radius;
        std::shared_ptr<material> mat;
        aabb bbox;
};
