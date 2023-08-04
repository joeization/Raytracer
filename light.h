#ifndef _LIGHT_H_
#define _LIGHT_H_ value

#include "color.h"
#include "vect.h"

class light {
    vect pos;
    color c;

   public:
    light() {
        pos = vect();
        c = color(1, 1, 1);
    }
    light(vect p, color _c) : pos(p), c(_c) {}

    vect get_position() { return pos; }
    color get_color() { return c; }
};

#endif