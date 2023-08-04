#ifndef _VECT_H_
#define _VECT_H_

#include <cmath>

class vect {
    double x, y, z;

   public:
    vect() { x = y = z = 0; }

    vect(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    double get_x() { return x; };
    double get_y() { return y; };
    double get_z() { return z; };

    double magnitude() {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    vect norm() {
        double mag = magnitude();
        return vect(x / mag, y / mag, z / mag);
    }

    vect neg() {
        return vect(-x, -y, -z);
    }

    double dot(vect v) {
        return x * v.x + y * v.y + z * v.z;
    }

    vect cross(vect v) {
        return vect(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    vect add(vect v) {
        return vect(x + v.x, y + v.y, z + v.z);
    }

    vect mul(double scal) {
        return vect(x * scal, y * scal, z * scal);
    }
};

#endif