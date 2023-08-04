#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "color.h"
#include "vect.h"

class object {
    color c;

   public:
    object() { c = color(0, 0, 0); };

    virtual color get_color() { return c; }

    virtual double find_intersect(ray r) {
        return 0;
    }

    virtual vect get_normal_at(vect p) {
        return vect();
    }
};

#endif