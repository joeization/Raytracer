#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vect.h"


class camera{
    vect campos, camdir, camright, camdown;

public:
    camera(){campos=vect();camdir=vect(0,0,1);camright=vect();camdown=vect();}
    camera(vect cp, vect cd, vect cr, vect cdo):
        campos(cp),camdir(cd),camright(cr),camdown(cdo){}

    vect get_position(){return campos;}
    vect get_direction(){return camdir;}
    vect get_right(){return camright;}
    vect get_down(){return camdown;}
};

#endif