#ifndef _RAY_H_
#define _RAY_H_

#include "vect.h"


class ray{
    vect origin, dir;

public:
    ray(){origin=vect();dir=vect(1,0,0);}
    ray(vect o, vect d):origin(o),dir(d){}

    vect get_origin(){return origin;}
    vect get_direction(){return dir;}
};

#endif