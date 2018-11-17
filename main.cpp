#include <bits/stdc++.h>
#include "vect.h"
#include "ray.h"
#include "camera.h"
#include "color.h"
#include "light.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"

using namespace std;


struct RGBType{
    double r;
    double g;
    double b;
    RGBType(){r=g=b=0;}
};

void saveBMP(const string filename, int w, int h, int dpi, RGBType *data){
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int fliesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

    bmpfileheader[2] = (unsigned char)(fliesize);
    bmpfileheader[3] = (unsigned char)(fliesize>>8);
    bmpfileheader[4] = (unsigned char)(fliesize>>16);
    bmpfileheader[5] = (unsigned char)(fliesize>>24);

    bmpinfoheader[4] = (unsigned char)(w);
    bmpinfoheader[5] = (unsigned char)(w>>8);
    bmpinfoheader[6] = (unsigned char)(w>>16);
    bmpinfoheader[7] = (unsigned char)(w>>24);

    bmpinfoheader[8] = (unsigned char)(h);
    bmpinfoheader[9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    const char wb[] = {'w', 'b', '\0'};
    f = fopen(filename.c_str(), wb);
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<k; i++){
        double r = (data[i].r)*255;
        double g = (data[i].g)*255;
        double b = (data[i].b)*255;

        unsigned char color[3] = {(int)floor(b), (int)floor(g), (int)floor(r)};

        fwrite(color,1,3,f);
    }

    fclose(f);
}

int wining_object_of(vector<double> intersections){
    int s = intersections.size();
    if(s == 0)return -1;
    else if(s == 1){
        if(intersections[0] > 0){
            return 0;
        }
        else return -1;
    }
    int minidx = -1;
    double maxv = *max_element(intersections.begin(), intersections.end());
    if(maxv > 0){
        for(int i=0; i<s; i++){
            if(intersections[i] > 0 && intersections[i] <= maxv){
                maxv = intersections[i];
                minidx = i;
            }
        }
    }
    return minidx;
}

color get_color_at(vect intersection_position, vect intersection_ray_direction,
                    vector<object*> scene_objects, vector<light> light_sources,
                    int index_of_winning, double accuracy, double ambientlight){

    if(ambientlight < 1e-6) return color(0, 0, 0);

    color wining_object_color = scene_objects[index_of_winning]->get_color();
    vect wining_object_normal =
            scene_objects[index_of_winning]->get_normal_at(intersection_position);

    if(wining_object_color.get_s() == 2){
        int square = (int)floor(intersection_position.get_x() +
                    (int)floor(intersection_position.get_z()));
        if(square%2 == 0){
            wining_object_color = color(0, 0, 0, 2);
        }
    }

    color final_color = wining_object_color.scalar(ambientlight);

    if(wining_object_color.get_s() > 0 && wining_object_color.get_s() <= 1){
        double dot1 = wining_object_normal.dot(intersection_ray_direction.neg());
        vect scalar1 = wining_object_normal.mul(dot1);
        vect add1 = scalar1.add(intersection_ray_direction);
        vect scalar2 = add1.mul(2);
        vect add2 = intersection_ray_direction.neg().add(scalar2);
        vect reflection_direction = add2.norm();

        ray reflection_ray = ray(intersection_position, reflection_direction);

        vector<double> reflection_intersections;
        for(int j=0; j<scene_objects.size(); j++){
            reflection_intersections.push_back(
                scene_objects[j]->find_intersect(reflection_ray));
        }
        int index_of_winning_with_reflection = wining_object_of(reflection_intersections);

        if(index_of_winning_with_reflection != -1){
            if(reflection_intersections[index_of_winning_with_reflection] > accuracy){
                vect reflection_intersection_position =
                    intersection_position.add(reflection_direction.mul(
                    reflection_intersections[index_of_winning_with_reflection]));
                vect reflection_intersection_ray_direction = reflection_direction;

                color reflection_intersection_color = get_color_at(
                    reflection_intersection_position,
                    reflection_intersection_ray_direction,
                    scene_objects, light_sources, index_of_winning_with_reflection,
                    accuracy, ambientlight-0.05);

                final_color = final_color.add(
                    reflection_intersection_color.scalar(wining_object_color.get_s()));
            }
        }
    }

    for(int i=0; i<light_sources.size(); i++){
        vect light_direction =
            light_sources[i].get_position().add(intersection_position.neg()).norm();
        double cosine_angle = wining_object_normal.dot(light_direction);
        if(cosine_angle > 0){
            bool shadowed = false;

            vect distance_to_light =
                light_sources[i].get_position().add(intersection_position.neg());
            double distance_to_light_magnitude = distance_to_light.magnitude();
            distance_to_light = distance_to_light.norm();

            ray shadow_ray(intersection_position, distance_to_light);
            vector<double> secondary_intersections;
            for(int j=0; j<scene_objects.size() && shadowed == false; j++){
                secondary_intersections.push_back(
                    scene_objects[j]->find_intersect(shadow_ray));
            }

            for(int j=0; j<secondary_intersections.size(); j++){
                if(secondary_intersections[j] > accuracy){
                    if(secondary_intersections[j] <= distance_to_light_magnitude){
                        shadowed = true;
                        break;
                    }
                }
            }
            if (shadowed == false){
                final_color = final_color.add(wining_object_color.mul(
                        light_sources[i].get_color()).scalar(cosine_angle));

                if(wining_object_color.get_s() > 0 && wining_object_color.get_s() <= 1){
                    double dot1 = wining_object_normal.dot(intersection_ray_direction.neg());
                    vect scalar1 = wining_object_normal.mul(dot1);
                    vect add1 = scalar1.add(intersection_ray_direction);
                    vect scalar2 = add1.mul(2);
                    vect add2 = intersection_ray_direction.neg().add(scalar2);
                    vect reflection_direction = add2.norm();

                    double specular = reflection_direction.dot(light_direction);
                    if(specular > 0){
                        specular = pow(specular, 10);
                        final_color = final_color.add(light_sources[i].get_color().scalar(
                            specular*wining_object_color.get_s()));
                    }
                }
            }
        }
    }
    final_color.clip();
    return final_color;
}

int main(){
    cout << "rendering..." << endl;

    clock_t t1, t2;
    t1 = clock();

    const int dpi = 72;
    const int width = 640;
    const int height = 480;
    const double aspectratio  = (double)width/(double)height;

    int n = width*height;
    RGBType *pixels = new RGBType[n];
    int idx, aaidx;

    int aadepth = 4;
    double aathreshhold = 0.1;
    double ambientlight = 0.2;
    double accuracy = 1e-6;

    vect O(0, 0, 0);
    vect X(1, 0, 0);
    vect Y(0, 1, 0);
    vect Z(0, 0, 1);

    vect campos(3, 1.5, -4);

    vect look_at(0, 0, 0);
    vect diff_btw(campos.get_x()-look_at.get_x(),
                campos.get_y()-look_at.get_y(),
                campos.get_z()-look_at.get_z());

    vect camdir = diff_btw.neg().norm();
    vect camright = Y.cross(camdir).norm();
    vect camdown = camright.cross(camdir);
    camera scene_cam(campos, camdir, camright, camdown);

    color white(1, 1, 1);
    color pretty_green(0.5, 1, 0.5, 0.3);
    color maroon(0.5, 0.25, 0.25, 1);
    color violet(0.5, 0, 1, 0.4);
    color tile_floor(1, 1, 1, 2);
    color gray(0.5, 0.5, 0.5);
    color black(0, 0, 0);

    vect lightpos(-7, 10, -10);
    light scene_light(lightpos, white);

    vector<light> light_sources;
    light_sources.push_back(scene_light);

    sphere scene_sphere(O, 1, pretty_green);
    sphere scene_sphere2(vect(1.75, 0, 0), 0.5, maroon);
    sphere scene_sphere3(vect(0.75, -0.75, -1.5), 0.25, violet);
    plane scene_plane(Y, -1, tile_floor);

    vector<object*> scene_objects;
    scene_objects.push_back(dynamic_cast<object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<object*>(&scene_sphere2));
    scene_objects.push_back(dynamic_cast<object*>(&scene_sphere3));
    scene_objects.push_back(dynamic_cast<object*>(&scene_plane));

    double xamnt, yamnt;

    const int sq=aadepth*aadepth;
    double tmpr[sq];
    double tmpg[sq];
    double tmpb[sq];

    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            //render
            idx = y*width + x;

            for(int aax=0; aax<aadepth; aax++){
                for(int aay=0; aay<aadepth; aay++){

                    aaidx = aay*aadepth + aax;

                    if(aadepth == 1){
                        if(width > height){
                            xamnt = ((x + 0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
                            yamnt = ((height - y) + 0.5)/height;
                        }
                        else if(height > width){
                            xamnt = (x + 0.5)/width;
                            yamnt = (((height - y) + 0.5)/height)/aspectratio
                                    - (((height - width)/(double)width)/2);
                        }
                        else{
                            xamnt = (x + 0.5)/width;
                            yamnt = ((height - y) + 0.5)/height;
                        }
                    }
                    else{
                        if(width > height){
                            xamnt = ((x + (double)aax/((double)aadepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
                            yamnt = ((height - y) + (double)aay/((double)aadepth - 1))/height;
                        }
                        else if(height > width){
                            xamnt = (x + (double)aax/((double)aadepth - 1))/width;
                            yamnt = (((height - y) + (double)aay/((double)aadepth - 1))/height)/aspectratio
                                    - (((height - width)/(double)width)/2);
                        }
                        else{
                            xamnt = (x + (double)aax/((double)aadepth - 1))/width;
                            yamnt = ((height - y) + (double)aay/((double)aadepth - 1))/height;
                        }
                    }

                    vect cam_ray_origin = scene_cam.get_position();
                    vect cam_ray_direction
                        = camdir.add(camright.mul(xamnt - 0.5).add(camdown.mul(yamnt - 0.5))).norm();

                    ray cam_ray(cam_ray_origin, cam_ray_direction);

                    vector<double> intersections;
                    for(int i=0;i<scene_objects.size();i++){
                        intersections.push_back(scene_objects[i]->find_intersect(cam_ray));
                    }

                    int index_of_winning = wining_object_of(intersections);

                    if(index_of_winning == -1){
                        tmpr[aaidx] = 0;
                        tmpg[aaidx] = 0;
                        tmpb[aaidx] = 0;
                    }
                    else{
                        if(intersections[index_of_winning] > accuracy){
                            vect intersection_position = cam_ray_origin.add(
                                    cam_ray_direction.mul(intersections[index_of_winning]));
                            vect intersection_ray_direction = cam_ray_direction;

                            color intersect_color = get_color_at(
                                    intersection_position, intersection_ray_direction,
                                    scene_objects, light_sources, index_of_winning,
                                    accuracy, ambientlight);
                            tmpr[aaidx] = intersect_color.get_r();
                            tmpg[aaidx] = intersect_color.get_g();
                            tmpb[aaidx] = intersect_color.get_b();
                        }
                    }
                }
            }
            pixels[idx].r = accumulate(tmpr, tmpr+sq, 0.0)/sq;
            pixels[idx].g = accumulate(tmpg, tmpg+sq, 0.0)/sq;
            pixels[idx].b = accumulate(tmpb, tmpb+sq, 0.0)/sq;
        }
    }

    saveBMP("scene.bmp", width, height, dpi, pixels);

    delete[] pixels;

    t2 = clock();

    double tik = ((double)t2-(double)t1)/1000;

    cout << tik << " seconds" << endl;

    return 0;
}