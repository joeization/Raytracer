#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <cmath>
#include "vect.h"
#include "color.h"
#include "object.h"


class sphere: public object{
    vect center;
    double radius;
    color c;

public:
    sphere(){center=vect();radius=1;c=color();}
    sphere(vect cen, double r, color _c):center(cen),radius(r),c(_c){}

    vect get_center(){return center;}
    double get_radius(){return radius;}
    color get_color(){return c;}

    vect get_normal_at(vect p){
        return p.add(center.neg()).norm();
    }

    double find_intersect(ray r){
        vect ray_origin = r.get_origin();
        double ray_origin_x = ray_origin.get_x();
        double ray_origin_y = ray_origin.get_y();
        double ray_origin_z = ray_origin.get_z();

        vect ray_direction = r.get_direction();
        double ray_direction_x = ray_direction.get_x();
        double ray_direction_y = ray_direction.get_y();
        double ray_direction_z = ray_direction.get_z();

        double sphere_center_x = center.get_x();
        double sphere_center_y = center.get_y();
        double sphere_center_z = center.get_z();

        double a = 1;
        double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x)
                    +(2*(ray_origin_y - sphere_center_y)*ray_direction_y)
                    +(2*(ray_origin_z - sphere_center_z)*ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2)
                    +pow(ray_origin_y - sphere_center_y, 2)
                    +pow(ray_origin_z - sphere_center_z, 2)
                    -(radius*radius);

        double discriminant = b*b-4*c;
        if(discriminant > 0){
            double r1 = ((-b-sqrt(discriminant))/2) - 1e-6;
            if(r1 > 0){
                return r1;
            }
            return ((-b+sqrt(discriminant))/2) - 1e-6;
        }
        else return -1;
    }
};

#endif