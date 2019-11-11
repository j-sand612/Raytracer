#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include <float.h>
#include <random>
#include <ctime>

using namespace std;
std::random_device rd2;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen2(rd2()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis2(0.0, 1.0);
int pixels;
int hits;


vec3 color(const ray& r, hitable * world, int depth){
    hit_record rec;
    if(world->hit(r, 0.001, __FLT_MAX__, rec)){
        hits++;
        ray scattered;
        vec3 attenuation;
        // //if(hits<3){
        //         cout<<"hits: "<< hits << endl;
        //         cout<<"pixels: "<< pixels << endl;
        // //}
        // cout<<typeid(rec.mat_ptr).name()<<endl;
        // cout<<rec.mat_ptr<<endl;

        if(depth<50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){     
            return attenuation*color(scattered, world, depth+1);
        }
        else{
            return vec3(0,0,0);
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
    }
}

int main()
{
    ofstream myfile;
    myfile.open ("raytracer.ppm");
    int nx = 200;
    int ny = 100;
    int ns = 100;
    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[4];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8,0.6,0.2),0.3));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8,0.8,0.8),1.0));
    hitable *world = new hitable_list(list,4);
    camera cam;
    for(int j=ny-1; j>=0; j--){
        for(int i=0; i<nx; i++){
            vec3 col(0,0,0);
            pixels++;
            for(int s=0; s<ns; s++){
                float u = float(i+random_double()) / float(nx);
                float v = float(j+random_double())  / float(ny);
                ray r = cam.get_ray(u,v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r,world,0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            myfile<< ir<< " " << ig << " " << ib << "\n";
        }
    }
    myfile.close();
}