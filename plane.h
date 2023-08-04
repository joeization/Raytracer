#ifndef _PLANE_H_
#define _PLANE_H_

#include <cmath>

#include "color.h"
#include "object.h"
#include "vect.h"

class plane : public object {
    vect normal;
    double distance;
    color c;

   public:
    plane() {
        normal = vect(1, 0, 0);
        distance = 0;
        c = color(0.5, 0.5, 0.5);
    }

    plane(vect n, double d, color _c) : normal(n), distance(d), c(_c) {}

    vect get_normal() { return normal; }
    double get_distance() { return distance; }
    color get_color() { return c; }

    vect get_normal_at(vect p) {
        return normal;
    }

    double find_intersect(ray r) {
        vect ray_direction = r.get_direction();

        double a = ray_direction.dot(normal);

        if (fabs(a) <= 1e-9) {
            return -1;
        } else {
            double b = normal.dot(r.get_origin().add(normal.mul(distance).neg()));
            return -1 * b / a;
        }
    }
};

#endif